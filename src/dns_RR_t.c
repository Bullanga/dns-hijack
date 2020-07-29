#include <stdlib.h> 
#include <sys/socket.h>
#include <errno.h>
#include "dns_types.h"
#include "dns_RR_t.h"



void generate_success_response(Packet request, const char *ip, const char *comment, int master_socket, const struct sockaddr client_addr, int client_len){
      
      Packet reply;
      // tractem el packet; generem el packet de resposta
      memset(&reply,0,sizeof(reply));
      
      reply.ID = request.ID;
      // Assumim error i posem el flac de authoritive reply
      reply.Rcode = RCODE_SERVER_ERROR;
      reply.Flags = FLAG_REPLY ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar

      int replyLen = 12;
      // comprovem si podem tractar la peticio
      if( (request.Flags & FLAG_REPLY) == 0) 
      {

//        write(1,"ENTRO!\n",7);
        int len = 0;
        reply.Rcode = RCODE_NO_ERROR;
        reply.Acount = htons(2); // 2 respostes: hostname and TXT 

        // RESPOSTA 1: hostname
        

        // store hostname
        reply.Data[len] = (int) request.Data[len];
        ++len;
        memcpy(reply.Data+len,&request.Data[len],strlen(&request.Data[len]));
        len += strlen(&request.Data[len]);
        reply.Data[len++];
        // set upper byte of TYPE
        reply.Data[len++] = (TYPE_A/256)&0xff;
        // set lower byte of TYPE
        reply.Data[len++] = (TYPE_A)&0xff;

        // set CLASS
        reply.Data[len++] = (CLASS_IN/256)&0xff;
        reply.Data[len++] = (CLASS_IN)&0xff;
    
        // set TTL (4 bytes) a 10 s
        reply.Data[len++] = 0;
        reply.Data[len++] = 0;
        reply.Data[len++] = 0;
        reply.Data[len++] = 10;
  
        // set data length
        reply.Data[len++] =0;
        reply.Data[len++] =4;
       

        int ipAddr = ntohl(inet_addr(ip));
        reply.Data[len++] = (ipAddr >> 24) & 0xff;
        reply.Data[len++] = (ipAddr >> 16) & 0xff;
        reply.Data[len++] = (ipAddr >> 8) & 0xff;
        reply.Data[len++] = (ipAddr) & 0xff;
      

        // Resposta 2: hostname i TXT
        reply.Data[len++] = 0xc0; // punter
        reply.Data[len++] = 0x0c; // punter al nom (inici de data)
        
        // set upper byte of TYPE
        reply.Data[len++] = (TYPE_TXT/256)&0xff;
        // set lower byte of TYPE
        reply.Data[len++] = (TYPE_TXT)&0xff;

        // set CLASS
        reply.Data[len++] = (CLASS_IN/256)&0xff;
        reply.Data[len++] = (CLASS_IN)&0xff;

        // set TTL (4 bytes) a 10 s
        reply.Data[len++] = 0;
        reply.Data[len++] = 0;
        reply.Data[len++] = 0;
        reply.Data[len++] = 10;

        reply.Data[len++] = 0;
	      reply.Data[len++] = strlen(comment)+1;
	      reply.Data[len++] = strlen(comment);
	      memcpy(reply.Data+len,comment,strlen(comment));
      	len += strlen(comment);

      	replyLen += len; /* total size oof packet */


      }
      
      if( 0 > sendto(master_socket, &reply, replyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }

}




void generate_failure_response(Packet request,  int master_socket, const struct sockaddr client_addr, int client_len){


      Packet reply;
      // tractem el packet; generem el packet de resposta
      memset(&reply,0,sizeof(reply));
      
      reply.ID = request.ID;
      // Assumim error i posem el flac de authoritive reply
      reply.Rcode = RCODE_SERVER_ERROR;
      reply.Flags = FLAG_REPLY ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar

      int replyLen = 12;
      // comprovem si podem tractar la peticio
      if( (request.Flags & FLAG_REPLY) == 0) 
      {

        int len = 0;
        reply.Rcode = RCODE_NXDOMAIN;
        reply.Acount = htons(0); // 0 respostes  

			}

      if( 0 > sendto(master_socket, &reply, replyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }


}
void parse_requested_domain(char *target, char *data) {
	
	memset(target,0,16);
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

	
	memset(target,0,16);
	char *ip = inet_ntoa(((struct sockaddr_in *)client)->sin_addr);
	memcpy(target,ip,strlen(ip));


}


