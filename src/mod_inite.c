#define _GNU_SOURCE

#include <sys/socket.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <libpq-fe.h>

#include "dns_types.h"
#include "variables.h"
#include "dnslib.h"
#include "guardaIP.h"
#include "mod_inite.h"

#include <stdio.h>

#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

// Internal definitions
char  *getLastIPRegistered();
int   ipListUpdate();
void  cloneUpdatesIpList();
int   registered(char *ip);

// Captive portal 
// Return, in string human readable format, the most resent ip registered to a postgresql database
char* getLastIPRegistered() {
  char *ret_val;
  char credentials[strlen("user= password= dbname=") 
                  + strlen(db_user) 
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
int ipListUpdate() {
  char* ip_to_add = getLastIPRegistered();
  
  if (ip_to_add != NULL) 
    r_add(ip_to_add);

  return (ip_to_add == NULL);
}

void cloneUpdatesIpList() {
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

void clearIpList() {
    r_clear(); 
}

//check si la ip està registrada
int registered(char ip[16])
{
    return r_findValue(ip);
}

// Captive portal setup
void init_inite() {
  //CAL AQUI INICIAR LES ESTRUCTURES GUARDAIP AMB LES IPS CORRECTES
	init_guardaIP(dhcp_ip_range[0],dhcp_ip_range[1]); 

  // SIGUSR1 -> New ip registered and needs to be added
  // SIGUSR2 -> Ip list must be cleared
  signal(SIGUSR1, cloneUpdatesIpList);
  signal(SIGUSR2, clearIpList);

  // Writing pid to /run/fakeDNS.pid
  FILE *fp;
  fp = fopen("/run/fakeDNS.pid", "w+");
  fprintf(fp, "%d", getpid());
  fclose(fp);
}

void exec_inite (Packet *packet)
{
		char client_ip[16];
    Message *message = &(packet->msg);


    parse_client_ip(client_ip, &(packet->client_addr));
  	if (use_inite) {
      // Si no esta registrat pots fer dues coses:
      //  - Resoldre al host d'inite 
      //  - Bloquejar la peticio si es privada.
      if (!registered(client_ip)) {
        false_RR.domain = message->question.QNAME;
        if (message->answer.rr != NULL && message->answer.rr->privat) 
          false_RR.ip = BLOCK_TARGET;
        else
          false_RR.ip = inite_host;
        message->answer.rr = &false_RR;
      }
    }
}
