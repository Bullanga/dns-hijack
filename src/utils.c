#include <sys/socket.h>
#include <string.h>
#include "dns_types.h"
#include "variables.h"
#include "dnslib.h"
#include "guardaIP.h"
#include "utils.h"

#include <stdio.h>

// Takes almost everithing as a parameter...
// 1. Parses from the raw packed
void process (Message *message, int master_socket, struct sockaddr client_addr, socklen_t client_len )
{
		char client_ip[16];
		char ip[16];		
		int privat;

    ip[0] = '\x00';


    parse_requested_domain(message);
    parse_client_ip(client_ip, &client_addr);

		privat = resolve_query(ip, message);

  	if (use_inite) {
      // Si no esta registrat pots fer dues coses:
      //  - Resoldre al host d'inite 
      //  - Bloquejar la peticio si es privada.
      if (!registered(client_ip)) {
        if (privat)
          strcpy(ip, BLOCK_TARGET);
        else
          strcpy(ip, inite_host);
      }
    }


		if (ip[0] == '\x00') {
			generate_failure_response(message, master_socket, client_addr, client_len);
    }
    else
      generate_success_response(message, ip, comment, master_socket, client_addr, client_len);
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
int resolve_query(char ip[16], Message *message)
{
  RR *rr;
  rr = (RR *) &records;
	for(int i = 0; i < RECORDS_SIZE; i++)
	{
		if(0 == strcmp(message->question.QNAME, rr->domain))
		{
			strcpy(ip, rr->ip);
			return rr->privat;			
		}
    rr++;
	}
	return 0;
}
