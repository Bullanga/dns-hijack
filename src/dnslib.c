#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "dns_types.h"
#include "dnslib.h"

void generate_success_response(Message *request, const char *ip, const char *comment, int master_socket, struct sockaddr client_addr, int client_len){
      
      Message reply;
      // tractem el packet; generem el packet de resposta
      memset(&reply,0,sizeof(reply));
      
      reply.header.ID = request->header.ID;
      // Assumim error i posem el flac de authoritive reply
      reply.header.RCODE = RCODE_SERVER_ERROR;
      reply.header.Flags = FLAG_REPLY ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar

      int replyLen = 12;
      // comprovem si podem tractar la peticio
      if( (request->header.Flags & FLAG_REPLY) == 0) 
      {

//        write(1,"ENTRO!\n",7);
        int len = 0;
        reply.header.RCODE  = RCODE_NO_ERROR;
        reply.header.ANCOUNT = htons(2); // 2 respostes: hostname and TXT 

        // RESPOSTA 1: hostname
        

        // store hostname
        reply.raw_body[len] = (int) request->raw_body[len];
        ++len;
        memcpy(reply.raw_body+len,&request->raw_body[len],strlen(&request->raw_body[len]));
        len += strlen(&request->raw_body[len]);
        len++;
        // set upper byte of TYPE
        reply.raw_body[len++] = (TYPE_A/256)&0xff;
        // set lower byte of TYPE
        reply.raw_body[len++] = (TYPE_A)&0xff;

        // set CLASS
        reply.raw_body[len++] = (CLASS_IN/256)&0xff;
        reply.raw_body[len++] = (CLASS_IN)&0xff;
    
        // set TTL (4 bytes) a 10 s
        reply.raw_body[len++] = 0;
        reply.raw_body[len++] = 0;
        reply.raw_body[len++] = 0;
        reply.raw_body[len++] = 10;
  
        // set data length
        reply.raw_body[len++] =0;
        reply.raw_body[len++] =4;
       

        int ipAddr = ntohl(inet_addr(ip));
        reply.raw_body[len++] = (ipAddr >> 24) & 0xff;
        reply.raw_body[len++] = (ipAddr >> 16) & 0xff;
        reply.raw_body[len++] = (ipAddr >> 8)  & 0xff;
        reply.raw_body[len++] = (ipAddr)       & 0xff;
      

        // Resposta 2: hostname i TXT
        reply.raw_body[len++] = 0xc0; // punter
        reply.raw_body[len++] = 0x0c; // punter al nom (inici de data)
        
        // set upper byte of TYPE
        reply.raw_body[len++] = (TYPE_TXT/256) & 0xff;
        // set lower byte of TYPE
        reply.raw_body[len++] = (TYPE_TXT)     & 0xff;

        // set CLASS
        reply.raw_body[len++] = (CLASS_IN/256) & 0xff;
        reply.raw_body[len++] = (CLASS_IN)     & 0xff;

        // set TTL (4 bytes) a 10 s
        reply.raw_body[len++] = 0;
        reply.raw_body[len++] = 0;
        reply.raw_body[len++] = 0;
        reply.raw_body[len++] = 10;

        reply.raw_body[len++] = 0;
	      reply.raw_body[len++] = strlen(comment)+1;
	      reply.raw_body[len++] = strlen(comment);
	      memcpy(reply.raw_body+len,comment,strlen(comment));
      	len += strlen(comment);

      	replyLen += len; /* total size oof packet */


      }
      
      printf("This is ANCOUNT of response = %u\n", reply.header.ANCOUNT);

      if( 0 > sendto(master_socket, &reply, replyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }

}

void generate_failure_response(Message *request,  int master_socket, struct sockaddr client_addr, int client_len){

      Message reply;
      // tractem el packet; generem el packet de resposta
      memset(&reply,0,sizeof(reply));
      
      reply.header.ID = request->header.ID;
      // Assumim error i posem el flac de authoritive reply
      reply.header.RCODE = RCODE_SERVER_ERROR;
      reply.header.Flags = FLAG_REPLY ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar

      int replyLen = 12;
      // comprovem si podem tractar la peticio
      if( (request->header.Flags & FLAG_REPLY) == 0) 
      {

        reply.header.RCODE = RCODE_NXDOMAIN;
        reply.header.ANCOUNT = htons(0); // 0 respostes  

			}

      if( 0 > sendto(master_socket, &reply, replyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }

}

void parse_message_raw_body(Message *message) {
	
  char *QNAME = message->question.QNAME;
  char *raw_data = message->raw_body;
	memset(QNAME,0, sizeof(message->question.QNAME));

  //Parse QNAME
	int i = 1;
	int dot = (int) raw_data[0];
	while( dot > 0) {
    strncat(QNAME, &raw_data[i], dot);
    strcat(QNAME, ".");
    i += dot;	
    dot = (int) raw_data[i++];
	}
	QNAME[i-2] = '\x00';

  message->question.QTYPE   =  (uint16_t  *)  &raw_data[++i];
  message->question.QCLASS  =  (uint16_t  *)  &raw_data[++i];
  message->answer.rr        =  (RR *)         &raw_data[++i];
}

void parse_client_ip(char *target, const struct sockaddr *client) {

	memset(target,0,16);
	char *ip = inet_ntoa(((struct sockaddr_in *)client)->sin_addr);
	memcpy(target,ip,strlen(ip));

}

void resolve_query(Message *message, const RR *records, int records_size)
{
  RR *rr;
  rr = (RR *) records;
  message->answer.rr = NULL;
	for(int i = 0; i < records_size; i++)
	{
		if(0 == strcmp(message->question.QNAME, rr->domain))
		{
      message->answer.rr = rr;
		}
    rr++;
	}
}
