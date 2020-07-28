#ifndef __WRAPP_H
#define __WRAPP_H
// Configutation file and variables

#include "dns_types.h"


extern const unsigned int use_inite;

extern const char *public_domains[]; 
extern const char *private_domains[] ;

extern int public_domains_num;
extern int private_domains_num;


extern const char public_ip[16];
extern const char private_ip[16];
extern const char comment[];

extern const unsigned int max_forks;

extern const char dhcp_ip_range[2][16];

#define IP_INI dhcp_ip_range[0]

extern const RR *records;

#endif
