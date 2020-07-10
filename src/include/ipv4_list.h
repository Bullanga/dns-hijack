#ifndef IPV4_LIST_H
	#define IPV4_LIST_H

	/* exported functions */

    // Returns a pointer to ipv4_list object
	void *ipv4_list_create(char *ip_first, char *ip_last);

    // Remove all elements from the list container
	void  ipv4_list_clear(void *ipv4_list);

    // Returns 1 if the ip was successfully added, 0 otherwise
	int   ipv4_list_add(void *ipv4_list, char *ip);

    // Returns 1 if the list contains the given ip, 0 otherwise
	int   ipv4_list_contains(void  *ipv4_list, char *ip);

    // Prints to stdout information about the list object
    void ipv4_list_debug(void *ipv4_list);

#endif
