#ifndef IPV4_LIST_H
	#define IPV4_LIST_H

	/* exported functions */
	void *ipv4_list_create(const char *ip_first, const char *ip_last);
	void  ipv4_list_clear(void *ipv4_list);
	int   ipv4_list_add(void *ipv4_list, char *ip);
	int   ipv4_list_contains(void  *ipv4_list, char *ip);


    void ipv4_list_debug(void *ipv4_list);

#endif
