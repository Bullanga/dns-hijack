// THIS PROGRAM CONTAINS THE CODE TO UPDATE LIST OF IPS
//
// --- TO ADD IN dns.c: ---
// ip_list_update       -> add without modifications
// handler              -> add content into handler function of dns.c
// captive_portal_init  -> add without modifications
// main                 -> add to dns.c the call to captive_portal_init

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "guardaIP.h"

#define READ  0
#define WRITE 1

int ip_list_update() {
    char ip[16];
    FILE *consulta = popen("/usr/bin/echo 255.255.255.255", "r");
    if (!fgets(ip,16, consulta)){
        perror("Cannot read from popen!");
        return 0;
    }
    printf("%s\n",ip);

    // ADD IP TO THE LIST
    // r_add(ip);
}

void handler(int sig) {
    if (sig == SIGUSR1){
        printf("SIGUSR1 captured\n");
        int pid;
        pid = clone();
        if (pid == 0) {
            ip_list_update();
            kill(getpid(),SIGINT);
        }
    } else if (sig == SIGUSR2){
        //r_clear(); //borro l'estructura
        printf("Struct must be cleared now!!");
    }
}

void captive_portal_init() {
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);
}

int main() {
    printf("tot okay, amb pid: %d\n", getpid());
    captive_portal_init();
    while (1) {
        printf("going to sleep for a sec\n");
        sleep(1);
    }
}
