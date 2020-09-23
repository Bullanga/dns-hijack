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
#define BLOCK_TARGET "0.0.0.0"

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
  sprintf(credentials, "user=%s password=%s dbname=%s", db_user, 
                                                        db_password,
                                                        db_name);
  PGconn *conn = PQconnectdb(credentials);

  // Checks db connection
  if (PQstatus(conn) == CONNECTION_BAD) {
    fprintf(stderr, "Connection to database failed: %s\n",
    PQerrorMessage(conn));
    PQfinish(conn);
    exit(EXIT_FAILURE);
  }

  // Execute query
  char *query = "select ip from portal_registre order by registrat desc limit 1";
  PGresult *res = PQexec(conn, query);
  
  // Check content of response and stores it in return value
  if (PQresultStatus(res) == PGRES_TUPLES_OK) 
    ret_val = PQgetvalue(res, 0, 0);
  

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
void inite_initialization() {
  // SIGUSR1 -> New ip registered and needs to be added
  // SIGUSR2 -> Ip list must be cleared
  signal(SIGUSR1, cloneUpdatesIpList);
  signal(SIGUSR2, clearIpList);

  // Writing pid to /run/fakeDNS.pid
  FILE *fp;
  fp = fopen("/run/fakeDNS.pid", "w+");
  fprintf(fp, "%d", getpid());
  fclose(fp);
  
  //CAL AQUI INICIAR LES ESTRUCTURES GUARDAIP AMB LES IPS CORRECTES
	init_guardaIP(dhcp_ip_range[0],dhcp_ip_range[1]); 

}
void message_hijack(Message *message) {
  int  i;             
  RR   *rr_i; 
  RR   *rr_response;
  int  RR_private = 0;

  SET_HEADER_QR(message->header.FLAGS, QR_RESPONSE);

  rr_i = message->answer.rr;
  for (i = 0; i < message->header.ANCOUNT; ++i) {
    if (rr_i->privat) {
      RR_private = 1;
      break;
    }
    ++rr_i;
  }

  if (RR_private) 
    rr_response = &RR_false_block;
  else 
    rr_response = &RR_false_inite;

  RR_populate_missing(rr_response);
  strcpy(rr_response->NAME, message->question.QNAME);
  RR_raw_big_endian_build(rr_response);

  message->raw_size        -=  message->answer.raw_end - message->answer.raw_begin;
  message->header.ANCOUNT  =   0;
  message->answer.raw_end  =   message->answer.raw_begin;

  message_answer_RR_add(message, rr_response);
}

void inite_execute (Packet *packet){
		char client_ip[16];
    Message *message = &(packet->message);

    parse_client_ip(client_ip, &(packet->client_addr));

    if (!registered(client_ip)) 
      message_hijack(message);
}
