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
#include "dns_RR_t.h"



#define IP "127.0.0.1"
#define COMMENT "More info: <url>"
#define MAX_FORKS 0


int num_forks = 0;

void handler(int sig) {
	wait(NULL);
	--num_forks;

}



int main(int argc, char *argv[])
{

	printf("MAX_FORKS=%d\n",MAX_FORKS);
	signal(SIGCHLD, handler);
  int opt = 1; // TRUE
  int master_socket;
  
  struct sockaddr_in address; // address del servidor
  struct sockaddr client_addr;
  
  DNS_RR Request, Reply;  
  int RequestLen, ReplyLen; 

  char buffer[1024];

  // creem el master socket
  if( (master_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
  {
    perror("socket() -> Error");
    exit(EXIT_FAILURE);

  }

  // configurem el master socket per tractar multiples peticions
  if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
  {
    perror("setsockopt() -> Error");
    exit(EXIT_FAILURE);

  }

  // tipus de socket creat
  address.sin_family = AF_INET; // IPv4
  address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  address.sin_port = htons( 53 );

  // binding del socket
  if( bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
  { 
    perror("bind() -> Error");
    exit(EXIT_FAILURE);
  }
  puts("Socket listening on 0.0.0.0:53");



  while(1) 
  {
    int client_len = sizeof(client_addr); 
    RequestLen = recvfrom(master_socket, &Request, sizeof(Request), 0, &client_addr, &client_len);
    // considerem tamany minim del paacket 12 bytes
    
    

//    sprintf(buffer, "Requested: %s\n", Request.Data); 
//    write(1, buffer, strlen(buffer));
				//printf("FORKS: %d\n", num_forks);
    if(RequestLen >= 12) 
    {
			if(num_forks < MAX_FORKS) {
				num_forks++;
	      int p = fork();
				if(p < 0){
			  	perror("fork() -> Error");       
					exit(EXIT_FAILURE); 
				}
				if(p == 0) 
				{
	 			     
	     		generate_success_response(&Request, IP, COMMENT, master_socket, client_addr, client_len);
					exit(0);
				}
			} else {
	     	generate_success_response(&Request, IP, COMMENT, master_socket, client_addr, client_len);
			}

   	}
  
  }
  
  

}
