#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "dns_types.h"
#include "dnslib.h"

void generate_success_response(Message request, const char *ip, const char *comment, int master_socket, struct sockaddr client_addr, int client_len){
      
      Message reply;
      // tractem el packet; generem el packet de resposta
      memset(&reply,0,sizeof(reply));
      
      reply.header.ID = request.header.ID;
      // Assumim error i posem el flac de authoritive reply
      reply.header.Rcode = RCODE_SERVER_ERROR;
      reply.header.Flags = FLAG_REPLY ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar

      int replyLen = 12;
      // comprovem si podem tractar la peticio
      if( (request.header.Flags & FLAG_REPLY) == 0) 
      {

//        write(1,"ENTRO!\n",7);
        int len = 0;
        reply.header.Rcode  = RCODE_NO_ERROR;
        reply.header.Acount = htons(2); // 2 respostes: hostname and TXT 

        // RESPOSTA 1: hostname
        

        // store hostname
        reply.header.Data[len] = (int) request.header.Data[len];
        ++len;
        memcpy(reply.header.Data+len,&request.header.Data[len],strlen(&request.header.Data[len]));
        len += strlen(&request.header.Data[len]);
        len++;
        // set upper byte of TYPE
        reply.header.Data[len++] = (TYPE_A/256)&0xff;
        // set lower byte of TYPE
        reply.header.Data[len++] = (TYPE_A)&0xff;

        // set CLASS
        reply.header.Data[len++] = (CLASS_IN/256)&0xff;
        reply.header.Data[len++] = (CLASS_IN)&0xff;
    
        // set TTL (4 bytes) a 10 s
        reply.header.Data[len++] = 0;
        reply.header.Data[len++] = 0;
        reply.header.Data[len++] = 0;
        reply.header.Data[len++] = 10;
  
        // set data length
        reply.header.Data[len++] =0;
        reply.header.Data[len++] =4;
       

        int ipAddr = ntohl(inet_addr(ip));
        reply.header.Data[len++] = (ipAddr >> 24) & 0xff;
        reply.header.Data[len++] = (ipAddr >> 16) & 0xff;
        reply.header.Data[len++] = (ipAddr >> 8)  & 0xff;
        reply.header.Data[len++] = (ipAddr)       & 0xff;
      

        // Resposta 2: hostname i TXT
        reply.header.Data[len++] = 0xc0; // punter
        reply.header.Data[len++] = 0x0c; // punter al nom (inici de data)
        
        // set upper byte of TYPE
        reply.header.Data[len++] = (TYPE_TXT/256) & 0xff;
        // set lower byte of TYPE
        reply.header.Data[len++] = (TYPE_TXT)     & 0xff;

        // set CLASS
        reply.header.Data[len++] = (CLASS_IN/256) & 0xff;
        reply.header.Data[len++] = (CLASS_IN)     & 0xff;

        // set TTL (4 bytes) a 10 s
        reply.header.Data[len++] = 0;
        reply.header.Data[len++] = 0;
        reply.header.Data[len++] = 0;
        reply.header.Data[len++] = 10;

        reply.header.Data[len++] = 0;
	      reply.header.Data[len++] = strlen(comment)+1;
	      reply.header.Data[len++] = strlen(comment);
	      memcpy(reply.header.Data+len,comment,strlen(comment));
      	len += strlen(comment);

      	replyLen += len; /* total size oof packet */


      }
      
      if( 0 > sendto(master_socket, &reply, replyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }

}

void generate_failure_response(Message request,  int master_socket, struct sockaddr client_addr, int client_len){

      Message reply;
      // tractem el packet; generem el packet de resposta
      memset(&reply,0,sizeof(reply));
      
      reply.header.ID = request.header.ID;
      // Assumim error i posem el flac de authoritive reply
      reply.header.Rcode = RCODE_SERVER_ERROR;
      reply.header.Flags = FLAG_REPLY ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar

      int replyLen = 12;
      // comprovem si podem tractar la peticio
      if( (request.header.Flags & FLAG_REPLY) == 0) 
      {

        reply.header.Rcode = RCODE_NXDOMAIN;
        reply.header.Acount = htons(0); // 0 respostes  

			}

      if( 0 > sendto(master_socket, &reply, replyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }

}

unsigned int parse_requested_domain(char *target, char *data) {
	
	memset(target,0,16);
	int i = 1;
	int dot = (int) data[0];
	while( dot > 0) {

	strncat(target, &data[i], dot);
	strcat(target, ".");
	i += dot;	
	dot = (int) data[i++];

	}
	target[i-2] = '\x00';
	
	return (uint16_t)data[++i];

}

void parse_client_ip(char *target, const struct sockaddr *client) {

	memset(target,0,16);
	char *ip = inet_ntoa(((struct sockaddr_in *)client)->sin_addr);
	memcpy(target,ip,strlen(ip));

}
