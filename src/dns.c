#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "dns_types.h"
#include "config.h"
#include "guardaIP.h"
#include "mod_inite.h"
#include "dnslib.h"


int num_forks = 0;

void modules_initialization() {
  #if USE_INITE
    inite_initialization();
  #endif
}

void modules_execute(Packet *packet) {
  #if USE_INITE
    inite_execute(packet);
  #endif
}

void handler(int sig) {
  if (sig == SIGCHLD){
      wait(NULL);
      --num_forks;
  } 
}

void get_multiclient_single_thread_socket(int *master_socket, int opt) {
  // creem el master socket
  if ((*master_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket() -> Error");
    exit(EXIT_FAILURE);
  }

  // configurem el master socket per tractar multiples peticions
  if (setsockopt(*master_socket, SOL_SOCKET, SO_REUSEADDR, (char * ) & opt, sizeof(opt)) < 0) {
    perror("setsockopt() -> Error");
    exit(EXIT_FAILURE);
  }
}

int RR_initialize(RR *records) {
  int i;
  RR *rr = records;
  for (i = 0; i < RECORDS_SIZE; ++i) {
    printf("one\n");
    RR_populate_missing(rr);
    RR_build_raw_big_endian(rr);
    ++rr;
  }
  return 1;
}

int main(int argc, char * argv[]) {

  Packet  packet;
  int     messageLen;
  int     master_socket;
  int     master_socket_opt = 1;            
  struct  sockaddr_in address; // address del servidor

  RR_initialize(records);

  get_multiclient_single_thread_socket(& master_socket, master_socket_opt);
  signal(SIGCHLD, handler);

  // Tipus de socket creat
  address.sin_family       =  AF_INET;     //  IPv4
  address.sin_addr.s_addr  =  INADDR_ANY;  //  0.0.0.0
  address.sin_port         =  htons(53);

  // Binding del socket
  if (bind(master_socket, (struct sockaddr * ) & address, sizeof(address)) < 0) {
    perror("bind() -> Error");
    exit(EXIT_FAILURE);
  }
  puts("Socket listening on 0.0.0.0:53");

  modules_initialization();

  while (1) {

    socklen_t client_len = sizeof(packet.client_addr);
    messageLen = recvfrom(master_socket, & (packet.message), sizeof(packet.message), 0, &(packet.client_addr), &client_len);
    // considerem tamany minim del paacket 12 bytes

    if (messageLen >= 12) {
      parse_message_raw_body(& (packet.message));

      resolve_query(& (packet.message), records, RECORDS_SIZE);

      modules_execute(&packet);

      if (packet.message.header.ANCOUNT) 
        generate_success_response(&(packet.message), packet.message.answer.rr->RDATA, comment, master_socket, packet.client_addr, client_len);
      else 
        generate_failure_response(&(packet.message), master_socket, packet.client_addr, client_len);
    }
  }
}
