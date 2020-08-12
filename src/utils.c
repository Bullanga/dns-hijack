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
void exec_inite (Packet *packet)
{
		char client_ip[16];
    Message *message = &(packet->msg);


    parse_client_ip(client_ip, &(packet->client_addr));
  	if (use_inite) {
      // Si no esta registrat pots fer dues coses:
      //  - Resoldre al host d'inite 
      //  - Bloquejar la peticio si es privada.
      if (!registered(client_ip)) {
        false_RR.domain = message->question.QNAME;
        if (message->answer.rr != NULL && message->answer.rr->privat) 
          false_RR.ip = BLOCK_TARGET;
        else
          false_RR.ip = inite_host;
        message->answer.rr = &false_RR;
      }
    }


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
void resolve_query(Message *message)
{
  RR *rr;
  rr = (RR *) &records;
  message->answer.rr = NULL;
	for(int i = 0; i < RECORDS_SIZE; i++)
	{
		if(0 == strcmp(message->question.QNAME, rr->domain))
		{
      message->answer.rr = rr;
		}
    rr++;
	}
}
