#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "dns_RR_t.h"
#include "config.h"
#include "guardaIP.h"


int num_forks = 0;

void handler(int sig) {
    if (sig == SIGCHLD){
	wait(NULL);
	--num_forks;
    } else if (sig == SIGUSR1){
	//Ve un nou, cal afegir-lo a l'estructura de dades
	int fd;
	char ip[16];
	fd = open("vivachavez", O_RDONLY);
	if (!read(fd,ip,16)){
	    perror("Cannot read from named PIPE to update registered IPs. It exists?");
	}
	close(fd);
	if (!r_add(ip)){
	    perror("Could not add new ip to registered IPs");
	}
    } else if (sig == SIGUSR2){
	r_clear(); //borro l'estructura
    }
}

//esta aqui pel més bon enteniment del codi
int registrat(char* ip){
    return r_findValue(ip);
}

int main(int argc, char * argv[]) {
  //CAL AQUI INICIAR LES ESTRUCTURES GUARDAIP AMB LES IPS CORRECTES
  printf("MAX_FORKS=%d\n", MAX_FORKS);
  signal(SIGCHLD, handler);
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);
  int opt = 1; // TRUE
  int master_socket;

  struct sockaddr_in address; // address del servidor
  struct sockaddr client_addr;

  DNS_RR Request, Reply;
  int RequestLen, ReplyLen;

  char req_domain[256];
  char client_ip[20];

  // creem el master socket
  if ((master_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket() -> Error");
    exit(EXIT_FAILURE);

  }

  // configurem el master socket per tractar multiples peticions
  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char * ) & opt, sizeof(opt)) < 0) {
    perror("setsockopt() -> Error");
    exit(EXIT_FAILURE);

  }

  // tipus de socket creat
  address.sin_family = AF_INET; // IPv4
  address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  address.sin_port = htons(53);

  // binding del socket
  if (bind(master_socket, (struct sockaddr * ) & address, sizeof(address)) < 0) {
    perror("bind() -> Error");
    exit(EXIT_FAILURE);
  }
  puts("Socket listening on 0.0.0.0:53");

  while (1) {
    int client_len = sizeof(client_addr);
    RequestLen = recvfrom(master_socket, & Request, sizeof(Request), 0, & client_addr, & client_len);
    // considerem tamany minim del paacket 12 bytes

    parse_requested_domain(req_domain, Request.Data);
    // write(1, req_domain, strlen(req_domain));
    parse_client_ip(client_ip, & client_addr);
    //write(1, client_ip, strlen(client_ip));

    if (RequestLen >= 12) {
      if (num_forks < MAX_FORKS) {
        num_forks++;
        int p = fork();
        if (p < 0) {
          perror("fork() -> Error");
          exit(EXIT_FAILURE);
        }
        if (p == 0) {

          generate_success_response( & Request, IP, COMMENT, master_socket, client_addr, client_len);
          exit(0);
        }
      } else {
	if (!registrat(client_ip))
	    if (0 == strcmp(req_domain, DOMAIN)) {
	      generate_success_response( & Request, IP, COMMENT, master_socket, client_addr, client_len);
	    } else {
	      generate_failure_response( & Request, master_socket, client_addr, client_len);
	    }
      }

    }

  }

}
