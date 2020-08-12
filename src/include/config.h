#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sched.h>
#include <sys/mman.h>
//#include <postgresql/libpq-fe.h>
#include <stdlib.h>

#include "dns_types.h"

// Configutation file and variables
const unsigned int max_forks =  0;

/*
 *  INITE config
*	  `use_inite` can have only two differents values:
*
* 		0	: The web portal INITE is NOT in use
* 		1	: The web portal INITE is in use
 */
const unsigned int use_inite = 1;

const  char  dhcp_ip_range[2][16]  =  {"192.168.2.1",  "192.168.2.200"};
const  char  inite_host[16]        =  "192.168.1.35";

const  char*  db_user      =  "u_dks";
const  char*  db_password  =  "YjBkNDcwNDdmZDE0YzM3MzEyMTY2YmUz";
const  char*  db_name      =  "db_dks";
RR false_RR = {.type = TYPE_A, .privat = 0, };

const char comment[] = "More info: https://github.com/dhap0/dns-hijack";


#define IP_INI dhcp_ip_range[0]


const int RECORDS_SIZE = 2;

const RR records[] = {
  /*  domain            type  private  ip           */
  {   "cpdvl-blau.vida",  TYPE_A,    0,       "192.168.1.42"  },
  {   "wikipedia.dks",  TYPE_A,    1,       "127.0.0.1"  },
};

#endif
