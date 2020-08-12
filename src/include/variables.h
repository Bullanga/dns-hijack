#ifndef __VARIABLES_H
#define __VARIABLES_H
// Configutation file and variables

#include "dns_types.h"

extern  const  int          RECORDS_SIZE;
extern  const  RR           records[];
extern  const  char         comment[];
extern  const  unsigned int max_forks;
extern  const  char         dhcp_ip_range[2][16];

// Inite stuff
#define BLOCK_TARGET "0.0.0.0"

extern const unsigned int use_inite;
extern const char inite_host[16];
extern RR false_RR;

#define IP_INI dhcp_ip_range[0]
#endif
