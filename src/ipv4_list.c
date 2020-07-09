#include <arpa/inet.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ipv4_list.h"


typedef struct{
	uint32_t ip_first_n;
	uint32_t list_size;
	uint8_t *presence;
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
    list->ip_first_n = ntohl(addr_n_first.s_addr);
    list->list_size = ntohl(addr_n_last.s_addr) - list->ip_first_n + 1;

    list->presence = malloc(list->list_size*sizeof(uint8_t));
    memset(list->presence, 0, list->list_size*sizeof(uint8_t));

	return list;
}

void ipv4_list_clear(void *_list) {
    ipv4_list * list = _list;
    memset(list->presence, 0, list->list_size*sizeof(uint8_t));
}

int ipv4_list_add(void *_list, char *ip) {
    uint32_t index;
    struct in_addr addr_n;
    ipv4_list *list = _list;
    int return_value = 0;

    // Get value of ip and calculate its index in the precence vector
    if (inet_aton(ip, &addr_n) == 0) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }
    index = ntohl(addr_n.s_addr) - list->ip_first_n;

    // Check if its a valid index and add it if so
    if (index >= 0 && index < list->list_size) {
        list->presence[index]  =  1;
        return_value      =  1;
    }

    return return_value;
}

void ipv4_list_debug(void *_list) {
    int i;
    ipv4_list *list = _list;
    printf("-- LIST INFO --\n");
    printf("list size: %d\n",list->list_size);
    printf("list start: %d\n",list->ip_first_n);
    for (i = 0; i < list->list_size; ++i) {
        printf("%d: %d\n", i, list->presence[i]);
    }
}
