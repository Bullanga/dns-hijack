#include <arpa/inet.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "ipv4_list.h"


typedef struct{
	uint32_t ip_n_first;
	uint32_t list_size;
	uint8_t *list;
} ipv4_list;

void *ipv4_list_create(const char *ip_first, const char *ip_last) {
	ipv4_list *list = malloc(sizeof(ipv4_list));	
	printf("hello\n");
	return list;
}
