#include <arpa/inet.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ipv4_list.h"


typedef struct{
	uint32_t ip_n_first;
	uint32_t list_size;
	uint8_t *list;
} ipv4_list;

void *ipv4_list_create(const char *ip_first, const char *ip_last) {
    struct in_addr addr_n_first;
    struct in_addr addr_n_last;

    // Allocate memory for the struct
	ipv4_list *list = malloc(sizeof(ipv4_list));

    // Get first and last address in network byte order
    if (inet_aton(ip_first, &addr_n_first) == 0) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }
    if (inet_aton(ip_last, &addr_n_last) == 0) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }

    // Fill struct
    list->ip_n_first = ntohl(addr_n_first.s_addr);
    list->list_size = ntohl(addr_n_last.s_addr) - list->ip_n_first + 1;

    list->list = malloc(list->list_size*sizeof(uint8_t));
    memset(list->list, 0, list->list_size*sizeof(uint8_t));

	return list;
}
