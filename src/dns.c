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
#include "guardaIP.h"
#include "mod_inite.h"
#include "dnslib.h"


// Configuration
// ----------------------
#include "config.h"


// Program options
// ---------------------
const char *argp_program_version = "dns-hijack 1.0.0";
static char doc[] = "Dump dns that hijacks users who are not present in a db";
const char *argp_program_bug_address = "<your e-mail address>";
// port
// trap ip


int num_forks = 0;

void  modules_initialization();
void  modules_execute(Packet *packet);
void  handler(int sig);
void  get_multiclient_single_thread_socket(int *master_socket, int opt);
int   RR_initialize(RR *records, int records_size);

int main(int argc, char * argv[]) {

  Packet  packet;
  int     messageLen;
  int     master_socket;
  int     master_socket_opt = 1;            
  struct  sockaddr_in address; // address del servidor
  RR rr_banner = { .NAME = "BANNER", 
                   .TYPE = TYPE_TXT, 
                   .RDATA = comment, 
                   .privat = 0 };

  RR_initialize(records, RECORDS_SIZE);
  RR_initialize(&rr_banner, 1);

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
    messageLen = recvfrom(master_socket, 
                          &(packet.message),
                          sizeof(packet.message),
                          0,
                          &(packet.client_addr),
                          &client_len);
    
    // considerem tamany minim del paacket 12 bytes
    if (messageLen >= 12) {
      message_big_endian_parse(& (packet.message));

      message_query_resolve(& (packet.message), records, RECORDS_SIZE);
      
      modules_execute(&packet);

      if (!packet.message.header.ANCOUNT)
        SET_HEADER_RCODE(packet.message.header.FLAGS, RCODE_SERVER_ERROR);

      message_answer_RR_add(&(packet.message), &rr_banner);

      message_big_endian_build(&(packet.message));


      if( 0 > sendto(master_socket, 
                     &(packet.message), 
                     packet.message.raw_size, 
                     0, 
                     &packet.client_addr, 
                     client_len)) {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }
    }
  }
}

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

int RR_initialize(RR *records, int records_size) {
  int i;
  RR *rr = records;
  for (i = 0; i < records_size; ++i) {
    RR_populate_missing(rr);
    RR_raw_big_endian_build(rr);
    ++rr;
  }
  return 1;
}
