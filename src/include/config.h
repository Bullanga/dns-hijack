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

/*
 *  Configure db_user, db_password and db_name with the postgresql data
 *
 */
const  char*  db_user      =  "u_dks";
const  char*  db_password  =  "YjBkNDcwNDdmZDE0YzM3MzEyMTY2YmUz";
const  char*  db_name      =  "db_dks";

/*
*	`public_domains is a list of domains (should be ended with '.') that the dns will respond the ip indicated in 
* the `public_ip` variable.
*/
const char *public_domains[] = { "duniakato.dks.", };
const int public_domains_num = 1;
/*
*	`private_domains is a list of domains (should be ended with '.') that the dns will respond the ip indicated in 
* the `private_ip` variable. The `private_domain` variable only have sense if `use_inite` equals 1 and the values 
* should correspond with the domain names you would prevent user access without registering in the INITE portal first.
*/
const char *private_domains[] = { "wikipedia.dks.", };
const int private_domains_num = 1;

const char public_ip[16]= "192.168.1.42"  ;
const char private_ip[16]= "192.168.1.42"  ;

const char comment[] = "More info: https://github.com/dhap0/dns-hijack";

const unsigned int max_forks =  0;

const char dhcp_ip_range[2][16] = {"192.168.1.1", "192.168.1.200"};

#define IP_INI dhcp_ip_range[0]
#endif
