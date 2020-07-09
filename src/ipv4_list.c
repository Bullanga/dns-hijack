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

uint32_t ip_to_n(uint32_t offset, char *ip) {
    struct in_addr addr_n;

    // Get value of ip
    if (inet_aton(ip, &addr_n) == 0) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }

    // Return index relative to offset
    return ntohl(addr_n.s_addr) - offset;
}

void *ipv4_list_create(char *ip_first, char *ip_last) {
    // Allocate memory for the struct
	ipv4_list *list = malloc(sizeof(ipv4_list));

    // Fill struct
    list->ip_first_n = ip_to_n(0, ip_first);
    list->list_size = ip_to_n(0, ip_last) - list->ip_first_n + 1;

    list->presence = malloc(list->list_size*sizeof(uint8_t));
    memset(list->presence, 0, list->list_size*sizeof(uint8_t));

	return list;
}

void ipv4_list_clear(void *_list) {
    ipv4_list * list = _list;

    // Set to 0 all positions from presence vector.
    memset(list->presence, 0, list->list_size*sizeof(uint8_t));
}

int ipv4_list_add(void *_list, char *ip) {
    uint32_t index;
    ipv4_list *list = _list;
    int return_value = 0;

    index = ip_to_n(list->ip_first_n, ip);

    // Check if its a valid index and add it if so
    if (index >= 0 && index < list->list_size) {
        list->presence[index] = 1;
        return_value = 1;
    }

    return return_value;
}

int ipv4_list_contains(void *_list, char *ip) {
    uint32_t index;
    int return_value = 0;
    ipv4_list *list = _list;

    index = ip_to_n(list->ip_first_n, ip);

    // Check if its a valid index and add it if so
    if (index >= 0 && index < list->list_size && list->presence[index])
        return_value = 1;

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
