#ifndef __CONFIG_H
#define __CONFIG_H

// Configutation file and variables
/*
*	`use_inite` can have only two differents values:
*
* 		0	: The web portal INITE is NOT in use
* 		1	: The web portal INITE is in use
*/
const unsigned int use_inite = 1;
const unsigned int max_forks =  0;

/*
 *  Configure db_user, db_password and db_name with the postgresql data
 *
 */
const  char*  db_user      =  "u_dks";
const  char*  db_password  =  "YjBkNDcwNDdmZDE0YzM3MzEyMTY2YmUz";
const  char*  db_name      =  "db_dks";

const char comment[] = "More info: https://github.com/dhap0/dns-hijack";

const  char  dhcp_ip_range[2][16]  =  {"127.0.0.0",  "127.0.0.200"};

#define IP_INI dhcp_ip_range[0]

const char inite_host[16]= "0.0.0.0";

const RR records[] = {
  /*  domain            type  private  ip           */
  {   "cpdvl-blau.vida",  TYPE_A,    0,       "192.168.1.42"  },
  {   "wikipedia.dks",  TYPE_A,    1,       "127.0.0.1"  },
};

int r_length;
int *registry;
#endif
