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
#include <stdlib.h>

#include "dns_types.h"

// Configutation file and variables
const unsigned int max_forks =  0;

/*
 *  To enable INITE set the USE_INITE 
*	  `USE_INITE` can have only two differents values:
*
* 		0	: The web portal INITE is NOT in use
* 		1	: The web portal INITE is in use
 */
#define USE_INITE 1
char  inite_host[16] = "192.168.1.35";
const  char  dhcp_ip_range[2][16] = {"127.0.0.0",  "192.168.2.100"};

const  char*  db_user      =  "u_dks";
const  char*  db_password  =  "YjBkNDcwNDdmZDE0YzM3MzEyMTY2YmUz";
const  char*  db_name      =  "db_dks";

RR RR_false_inite = {.TYPE = TYPE_A, .privat = 0, .RDATA = inite_host};
RR RR_false_block = {.TYPE = TYPE_A, .privat = 0, .RDATA = "0.0.0.0"};

const char comment[] = "More info: https://github.com/dhap0/dns-hijack";


#define IP_INI dhcp_ip_range[0]


const int RECORDS_SIZE = 2;

RR records[] = {
  /*  domain            type  private  ip           */
	{  .NAME  =  "cpdvl-blau.vida",  .TYPE  =  TYPE_A,  .privat  =  0,  .RDATA  =  "192.168.1.42"  },
	{  .NAME  =  "wikipedia.dks",    .TYPE  =  TYPE_A,  .privat  =  1,  .RDATA  =  "127.0.0.1"     },
	{  .NAME  =  "moodle.dks",       .TYPE  =  TYPE_A,  .privat  =  1,  .RDATA  =  "127.0.0.1"     },
	{  .NAME  =  "khanacademy.dks",  .TYPE  =  TYPE_A,  .privat  =  1,  .RDATA  =  "127.0.0.1"     },
};

#endif
