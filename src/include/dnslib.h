#ifndef __DNSLIB_H_
#define __DNSLIN_H_
#include <sys/types.h>
#include <sys/socket.h>
#include "dns_types.h"


int raw_hostname_to_s(char *dest, char *src);
int s_to_raw_hostname(char *dest, char *src);


void message_big_endian_parse(Message *message);
void message_big_endian_build(Message *message);

void message_query_resolve(Message *message, const RR *records, int records_size);
void message_answer_RR_add(Message *message, RR *rr);

void RR_populate_missing(RR *rr);
void RR_raw_big_endian_build(RR *rr);


// DESTRUIR
void parse_client_ip(char *target, const struct sockaddr *client);

#endif
