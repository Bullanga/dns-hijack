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


const char comment[] = "More info: https://github.com/dhap0/dns-hijack";


const char dhcp_ip_range[2][16] = {"127.0.0.0", "127.0.0.2"};
#define IP_INI dhcp_ip_range[0]

const char private_ip[16]= "0.0.0.0";
const RR records[] = {
/*  domain            type  private  ip           */
{   "duniakato.dks",  1,    0,       "127.0.0.1"  },
{   "wikipedia.dks",  1,    1,       "127.0.0.1"  },

};

#endif
