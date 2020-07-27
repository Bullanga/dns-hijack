// THIS PROGRAM CONTAINS THE CODE TO UPDATE LIST OF IPS
//
// --- TO ADD IN dns.c: ---
// ip_list_update       -> add without modifications
// handler              -> add content into handler function of dns.c
// captive_portal_init  -> add without modifications
// main                 -> add to dns.c the call to captive_portal_init
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

#define READ  0
#define WRITE 1
#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

void do_exit(PGconn *conn, PGresult *res) {

    fprintf(stderr, "Error: %s\n", PQerrorMessage(conn));

    PQclear(res);
    PQfinish(conn);

    exit(1);
}

int ip_list_update() {
    PGconn *conn = PQconnectdb("user=u_dks password=YjBkNDcwNDdmZDE0YzM3MzEyMTY2YmUz dbname=db_dks");

    if (PQstatus(conn) == CONNECTION_BAD) {
        
        fprintf(stderr, "Connection to database failed: %s\n",
            PQerrorMessage(conn));
            
        PQfinish(conn);
        exit(1);
    }

    PGresult *res = PQexec(conn, "select ip from portal_registre order by registrat desc limit 1");
    
    //if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
	    printf("is this usefull...? %s\n",PQgetvalue(res, 0, 0));
    }
    else {
        do_exit(conn, res);
    }

    return 1;
}

void handler(int sig) {
    if (sig == SIGUSR1){
        printf("SIGUSR1 captured\n");
        int pid;
	char *stack;
	char *stackTop;
	stack = stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
           if (stack == MAP_FAILED)
               printf("mmap");

           stackTop = stack + STACK_SIZE;

        pid = clone(ip_list_update, stackTop, CLONE_VM, NULL);
        if (pid < 0) {
		printf("clone failed %s\n",strerror(errno));
        }

    } else if (sig == SIGUSR2){
        //r_clear(); //borro l'estructura
        printf("Struct must be cleared now!!");
    }
}

void captive_portal_init() {
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);

  FILE *fp;
  fp = fopen("/run/fakeDNS.pid", "w+");
  fprintf(fp, "%d", getpid());
  fclose(fp);
}

int main() {
    printf("tot okay, amb pid: %d\n", getpid());
    captive_portal_init();
    while (1) {
        printf("going to sleep for a sec\n");
        sleep(1);
    }
}
