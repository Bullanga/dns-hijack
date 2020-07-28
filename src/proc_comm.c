// THIS PROGRAM CONTAINS THE CODE TO UPDATE LIST OF IPS
//
// To compile this program use the following gcc command from the src directory
//   gcc -std=c99 -I include/ -I /usr/include/postgresql/ guardaIP.c proc_comm.c -lpq
//
// To test the functionality use the following commands from the same machine:
//   sudo kill -SIGUSR1 `cat /run/fakeDNS.pid`
//   sudo kill -SIGUSR2 `cat /run/fakeDNS.pid`
//
// --- TO ADD IN dns.c: ---
// getLastIPRegistered  -> add without modifications
// ip_list_update       -> add without modifications
// handler              -> add content into handler function of dns.c
// captive_portal_init  -> add without modifications
// main                 -> add to main of dns.c the call to captive_portal_init
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sched.h>
#include <sys/mman.h>
#include <libpq-fe.h>
#include <stdlib.h>

#include "guardaIP.h"
#include "config.h"

#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

char* getLastIPRegistered() {
    char *ret_val;
    // Creates a postgresql connection with hardcoded credentials
    PGconn *conn = PQconnectdb("user=u_dks password=YjBkNDcwNDdmZDE0YzM3MzEyMTY2YmUz dbname=db_dks");

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
	    printf("is this usefull...? %s\n",PQgetvalue(res, 0, 0));
	    ret_val = PQgetvalue(res, 0, 0);
    }

    // Clean and finish db connection
    PQclear(res);
    PQfinish(conn);

    return ret_val;
}

int ip_list_update() {
    // Connects to db and gathers the latest ip registered
    char* ip_to_add = getLastIPRegistered();
    
    // Adds latest ip registered with guardaIP utilitis
    if ( ip_to_add != NULL) 
	    r_add(ip_to_add);
    return (ip_to_add == NULL);
}

void handler(int sig) {
    // SIGUSR1 -> New ip registered and needs to be added
    if (sig == SIGUSR1){
        int pid;
	char *stack;
	char *stackTop;
	stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
        if (stack == MAP_FAILED)
            exit(EXIT_FAILURE);

        stackTop = stack + STACK_SIZE;

        pid = clone(ip_list_update, stackTop, CLONE_VM, NULL);
        if (pid < 0) {
		exit(EXIT_FAILURE);
        }

    }
    // SIGUSR2 -> Ip list must be cleared
    else if (sig == SIGUSR2){
        r_clear(); 
    }
}

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

int main() {
    printf("My pid is writen in /run/fakeDNS.pid\n");

    // Signal setup and pid writen in /run/fakeDNS.pid
    captive_portal_init();

    // Testing purpose. Start a registration list to update with a child
    init_guardaIP("192.168.1.1","192.168.1.200");
    
    // Check if 192.168.1.41 is added and sleep for 2 seconds
    while (1) {
	printf("is 41 in the list? %d\n",r_findValue("192.168.1.41"));
        printf("going to sleep for two sec\n");
        sleep(2);
    }
}
