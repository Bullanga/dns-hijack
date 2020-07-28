#include <sys/socket.h>
#include "dns_types.h"
#include "process.h"
#include "wrapp.h"
#include "checks.h"

//	void make_response(char* client_ip, char* req_domain, DNS_RR Request, const char *ip, const char *comment, int master_socket, const struct sockaddr client_addr, int client_len ){
//	    if (check_if_private_domain(req_domain)){
//				generate_success_response( Request, public_ip, comment, master_socket, client_addr, client_len);
//	    } else {
//				generate_failure_response( Request, master_socket, client_addr, client_len);
//		
//	    }
//	}

int process (char* client_ip, char* req_domain, DNS_RR Request, const char *comment, int master_socket, const struct sockaddr client_addr, int client_len ){

		char ip[16];		
		int privat = resolve_query(req_domain, ip);

    if (!check_if_use_inite() && ip != NULL){ //if inite not used, no more checks
	    generate_success_response(Request, ip, comment, master_socket, client_addr, client_len);
			return 1;
    }
    if (!check_if_registered(client_ip)){ //if not registered
			if (privat)  //and going to private ip return 0.0.0.0
	    	generate_success_response(Request, "0.0.0.0", comment, master_socket, client_addr, client_len);
				return 1;
    }

	  generate_success_response(Request, ip, comment, master_socket, client_addr, client_len);
		return 1;
    
}
