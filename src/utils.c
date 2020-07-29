#include <sys/socket.h>
#include "dns_types.h"
#include "wrapp.h"
#include "dns_RR_t.h"
#include "guardaIP.h"
#include "utils.h"

void process (Packet request, int master_socket, const struct sockaddr client_addr, socklen_t client_len )
{
		char req_domain[256];
		char client_ip[16];
		char ip[16];		
		int privat;

    parse_requested_domain(req_domain, request.Data);
    parse_client_ip(client_ip, &client_addr);



		privat = resolve_query(req_domain, ip);

		// If resource is marked as private and inite is up, ip must be registered
		if (privat) 
		{ 
    	if (use_inite && registered(client_ip))
			{ 
				// Hook resolved ip to private_ip
	    	generate_success_response(request, private_ip, comment, master_socket, client_addr, client_len);
				return;
			}
    }

		if( ip == NULL) 
			generate_failure_response(request, master_socket, client_addr, client_len);
		else 
	   	generate_success_response(request, ip, comment, master_socket, client_addr, client_len);
		return;
    
}

//check si la ip està registrada
int registered(char ip[16])
{
    return r_findValue(ip);
}

/*
 *  	ip -> buffer to store the ip corresponding req_domain, NULL if not in resource records
 * 		@return -> 1 if its a private domain, 0 if not
 */
int resolve_query(char ip[16], char *req_domain)
{
	int size = sizeof(records)/sizeof(RR);
	const RR *rr = records;
	memset(ip,'\x00',16);
	for(int i = 0; i < size; i++)
	{
		if(0 == strcmp(req_domain, rr->domain))
		{
			memcpy(ip, rr->ip, 15);
			return rr->privat;			
		}
	}
	return 0;
}
