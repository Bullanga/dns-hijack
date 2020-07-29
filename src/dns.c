#define _GNU_SOURCE

#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include <libpq-fe.h>

#include "dns_types.h"
#include "dns_RR_t.h"
#include "config.h"
#include "process.h"
#include "guardaIP.h"

// Captive portal definition
#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

int num_forks = 0;

// Captive portal 
// Return, in string human readable format, the most resent ip registered to a postgresql database
char* getLastIPRegistered() {
  char *ret_val;
  char credentials[strlen("user= password= dbname=") 
                  + strlen(db_name) 
                  + strlen(db_password) 
                  + strlen(db_name)
                  + 1];

  // Creates a postgresql connection with credentials from config.h
  sprintf(credentials, "user=%s password=%s dbname=%s", db_user, db_password, db_name);
  PGconn *conn = PQconnectdb(credentials);

  // Checks db connection
  if (PQstatus(conn) == CONNECTION_BAD) {
      
    fprintf(stderr, "Connection to database failed: %s\n",
        PQerrorMessage(conn));
        
    PQfinish(conn);
    exit(1);
  }

  // Execute query
  PGresult *res = PQexec(conn, "select ip from portal_registre order by registrat desc limit 1");
  
  // Check content of response and stores it in return value
  if (PQresultStatus(res) == PGRES_TUPLES_OK) {
    ret_val = PQgetvalue(res, 0, 0);
  }

  // Clean and finish db connection
  PQclear(res);
  PQfinish(conn);

  return ret_val;
}

// Captive portal
// Returns 1 if ip updated sucessfully, 0 otherwise
int ipListUpdate() {
  // Connects to db and gathers the latest ip registered
  char* ip_to_add = getLastIPRegistered();
  
  // Adds latest ip registered with guardaIP utilitis
  if ( ip_to_add != NULL) 
    r_add(ip_to_add);

  return (ip_to_add == NULL);
}

void handler(int sig) {
  if (sig == SIGCHLD){
      wait(NULL);
      --num_forks;
  } 
  // Captive portal
  // SIGUSR1 -> New ip registered and needs to be added
  else if (sig == SIGUSR1){
    int   pid;
    char  *stack;
    char  *stackTop;
    stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);

    if (stack == MAP_FAILED)
      exit(EXIT_FAILURE);

    stackTop = stack + STACK_SIZE;

    pid = clone(ipListUpdate, stackTop, CLONE_VM, NULL);
    if (pid < 0) 
      exit(EXIT_FAILURE);
  } 
  // Captive portal
  // SIGUSR2 -> Ip list must be cleared
  else if (sig == SIGUSR2){
    r_clear(); 
  }
}

// Captive portal setup
void captive_portal_init() {
  // SIGUSR1 -> New ip registered and needs to be added
  // SIGUSR2 -> Ip list must be cleared
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);

  // Writing pid to /run/fakeDNS.pid
  FILE *fp;
  fp = fopen("/run/fakeDNS.pid", "w+");
  fprintf(fp, "%d", getpid());
  fclose(fp);
}

int main(int argc, char * argv[]) {
  //CAL AQUI INICIAR LES ESTRUCTURES GUARDAIP AMB LES IPS CORRECTES
	init_guardaIP(dhcp_ip_range[0],dhcp_ip_range[1]); 

  printf("max_forks=%d\n", max_forks);
  signal(SIGCHLD, handler);

  // Captive portal setup. 
  captive_portal_init();

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
      if (num_forks < max_forks) {
        num_forks++;
        int p = fork();
        if (p < 0) {
          perror("fork() -> Error");
          exit(EXIT_FAILURE);
        }
        if (p == 0) {
	// #### ATENCIO PERQUE CAL CANVIAR AIXÒ ####
          generate_success_response( Request, public_ip, comment, master_socket, client_addr, client_len);
          exit(0);
        }
      } else {
	  if (!process(client_ip, req_domain, Request, public_ip, comment, master_socket, client_addr, client_len))   //processing ip request
	      perror("Error at processing DNS response");
      }

    }

  }

}
