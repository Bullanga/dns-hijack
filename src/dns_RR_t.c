#include <stdlib.h> 
#include <sys/socket.h>
#include <errno.h>
#include "dns_types.h"
#include "dns_RR_t.h"



void generate_success_response(DNS_RR Request, const char *ip, const char *comment, int master_socket, const struct sockaddr client_addr, int client_len){
      
      DNS_RR Reply;
      // tractem el packet; generem el packet de resposta
      memset(&Reply,0,sizeof(Reply));
      
      Reply.ID = Request.ID;
      // Assumim error i posem el flac de authoritive reply
      Reply.Rcode = RCODE_SERVER_ERROR;
      Reply.Flags = FLAG_REPLY ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar

      int ReplyLen = 12;
      // comprovem si podem tractar la peticio
      if( (Request.Flags & FLAG_REPLY) == 0) 
      {

//        write(1,"ENTRO!\n",7);
        int len = 0;
        Reply.Rcode = RCODE_NO_ERROR;
        Reply.Acount = htons(2); // 2 respostes: hostname and TXT 

        // RESPOSTA 1: hostname
        

        // store hostname
        Reply.Data[len] = (int) Request.Data[len];
        ++len;
        memcpy(Reply.Data+len,&Request.Data[len],strlen(&Request.Data[len]));
        len += strlen(&Request.Data[len]);
        Reply.Data[len++];
        // set upper byte of TYPE
        Reply.Data[len++] = (TYPE_A/256)&0xff;
        // set lower byte of TYPE
        Reply.Data[len++] = (TYPE_A)&0xff;

        // set CLASS
        Reply.Data[len++] = (CLASS_IN/256)&0xff;
        Reply.Data[len++] = (CLASS_IN)&0xff;
    
        // set TTL (4 bytes) a 10 s
        Reply.Data[len++] = 0;
        Reply.Data[len++] = 0;
        Reply.Data[len++] = 0;
        Reply.Data[len++] = 10;
  
        // set data length
        Reply.Data[len++] =0;
        Reply.Data[len++] =4;
       

        int ipAddr = ntohl(inet_addr(ip));
        Reply.Data[len++] = (ipAddr >> 24) & 0xff;
        Reply.Data[len++] = (ipAddr >> 16) & 0xff;
        Reply.Data[len++] = (ipAddr >> 8) & 0xff;
        Reply.Data[len++] = (ipAddr) & 0xff;
      

        // Resposta 2: hostname i TXT
        Reply.Data[len++] = 0xc0; // punter
        Reply.Data[len++] = 0x0c; // punter al nom (inici de data)
        
        // set upper byte of TYPE
        Reply.Data[len++] = (TYPE_TXT/256)&0xff;
        // set lower byte of TYPE
        Reply.Data[len++] = (TYPE_TXT)&0xff;

        // set CLASS
        Reply.Data[len++] = (CLASS_IN/256)&0xff;
        Reply.Data[len++] = (CLASS_IN)&0xff;

        // set TTL (4 bytes) a 10 s
        Reply.Data[len++] = 0;
        Reply.Data[len++] = 0;
        Reply.Data[len++] = 0;
        Reply.Data[len++] = 10;

        Reply.Data[len++] = 0;
	      Reply.Data[len++] = strlen(comment)+1;
	      Reply.Data[len++] = strlen(comment);
	      memcpy(Reply.Data+len,comment,strlen(comment));
      	len += strlen(comment);

      	ReplyLen += len; /* total size oof packet */


      }
      
      if( 0 > sendto(master_socket, &Reply, ReplyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }

}




void generate_failure_response(DNS_RR Request,  int master_socket, const struct sockaddr client_addr, int client_len){


      DNS_RR Reply;
      // tractem el packet; generem el packet de resposta
      memset(&Reply,0,sizeof(Reply));
      
      Reply.ID = Request.ID;
      // Assumim error i posem el flac de authoritive reply
      Reply.Rcode = RCODE_SERVER_ERROR;
      Reply.Flags = FLAG_REPLY ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar

      int ReplyLen = 12;
      // comprovem si podem tractar la peticio
      if( (Request.Flags & FLAG_REPLY) == 0) 
      {

        int len = 0;
        Reply.Rcode = RCODE_NXDOMAIN;
        Reply.Acount = htons(0); // 0 respostes  

			}

      if( 0 > sendto(master_socket, &Reply, ReplyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }


}
void parse_requested_domain(char *target, char *data) {
	
	memset(target,0,(strlen(target)));
	int i = 1;
	int dot = (int) data[0];
	while( dot > 0) {

	strncat(target, &data[i], dot);
	strcat(target, ".");
	i += dot;	
	dot = (int) data[i++];

	}

}

void parse_client_ip(char *target, struct sockaddr *client) {

	
	memset(target,0,(strlen(target)));
	char *ip = inet_ntoa(((struct sockaddr_in *)client)->sin_addr);
	memcpy(target,ip,strlen(ip));


}


