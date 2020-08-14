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

int main(int argc, char * argv[]) {

  Packet  packet;
  int     msgLen;
  int     master_socket;
  int     opt = 1;            //  master_socket  option
  struct sockaddr_in address; // address del servidor

  get_multiclient_single_thread_socket(& master_socket, opt);
  signal(SIGCHLD, handler);

  // Tipus de socket creat
  address.sin_family       =  AF_INET;     //  IPv4
  address.sin_addr.s_addr  =  INADDR_ANY;  //  0.0.0.0
  address.sin_port         =  htons(53);

  printf("max_forks=%d\n", max_forks);

  // Binding del socket
  if (bind(master_socket, (struct sockaddr * ) & address, sizeof(address)) < 0) {
    perror("bind() -> Error");
    exit(EXIT_FAILURE);
  }
  puts("Socket listening on 0.0.0.0:53");

  modules_initialization();

  while (1) {

    socklen_t client_len = sizeof(packet.client_addr);
    msgLen = recvfrom(master_socket, & (packet.msg), sizeof(packet.msg), 0, &(packet.client_addr), &client_len);
    // considerem tamany minim del paacket 12 bytes

    if (msgLen >= 12) {
      if (num_forks < max_forks) {
        num_forks++;
        int p = fork();
        if (p < 0) {
          perror("fork() -> Error");
          exit(EXIT_FAILURE);
        }
        if (p == 0) {
					// #### ATENCIO PERQUE CAL CANVIAR AIXÒ ####
          exit(0);
        }
      } else {

        parse_message_raw_body(& (packet.msg));

        resolve_query(& (packet.msg), records, RECORDS_SIZE);

	  		modules_execute(&packet);

        if (packet.msg.answer.rr == NULL) 
          generate_failure_response(&(packet.msg), master_socket, packet.client_addr, client_len);
        else 
          generate_success_response(&(packet.msg), packet.msg.answer.rr->ip, comment, master_socket, packet.client_addr, client_len);
        
			}
    }
  }
}
