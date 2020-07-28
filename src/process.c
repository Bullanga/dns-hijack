#include <sys/socket.h>
#include "dns_types.h"
#include "process.h"
#include "wrapp.h"
#include "checks.h"
#include "dns_RR_t.h"

int process (char* client_ip, char* req_domain, DNS_RR Request, const char *comment, int master_socket, const struct sockaddr client_addr, int client_len ){

		char ip[16];		
		int privat = resolve_query(req_domain, ip);

		// If resource is marked as private and inite is up, ip must be registered
		if (privat) 
		{ 
    	if (check_if_use_inite() && !check_if_registered(client_ip))
			{ 
				// Hook resolved ip to private_ip
	    	generate_success_response(Request, private_ip, comment, master_socket, client_addr, client_len);
				return 1;
			}
    }

		if( ip == NULL) 
			generate_failure_response( Request, master_socket, client_addr, client_len);
		else 
	   	generate_success_response(Request, ip, comment, master_socket, client_addr, client_len);
		return 1;
    
}
