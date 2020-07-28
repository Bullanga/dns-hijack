#include "checks.h"
#include "wrapp.h"

//check si la ip està registrada
int check_if_registered(char* ip){
    return r_findValue(ip);
}

int check_if_use_inite(){
    return use_inite;
}


/*
 *  	ip -> buffer to store the ip corresponding req_domain, NULL if not in resource records
 * 		@return -> 1 if its a private domain, 0 if not
 */
int resolve_query(char* req_domain, char ip[16])
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
