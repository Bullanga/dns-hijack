#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dns_types.h"
#include "dnslib.h"
#include "variables.h"

// Reads a sequence of number and labels where each number represent the size
// of the following label.
// - Note: Both the number and chars are in a uint8_t or char format.
int raw_hostname_to_s(char *dest, char *src) {
	int bytes_parsed = 1;
	int label_len = (int) src[0];
	while( label_len > 0) {
    strncat(dest, &src[bytes_parsed], label_len);
    strcat(dest, ".");
    bytes_parsed += label_len;	
    label_len = (int) src[bytes_parsed++];
	}
	dest[bytes_parsed-2] = '\x00';
  return bytes_parsed;
}

int s_to_raw_hostname(char *dest, char *src) {
  // NEEDS TO MALLOC at dest
  return 0;
}

void RR_build_RDATA(RR *rr) {
}

void RR_populate_missing(RR *rr) {
  printf("haha1\n");
  if (!rr->TYPE)
    rr->TYPE = TYPE_A;
  if (!rr->CLASS)
    rr->CLASS = CLASS_IN;
  rr->TTL = 10;
  if (!rr->RDLENGTH)
    RR_build_RDATA(rr);
  printf("haha1\n");
}

void RR_build_raw_big_endian(RR *rr) {
  char      buffer[500];
  int       bytes_writen;
  uint16_t  *ptr;

  bytes_writen = s_to_raw_hostname(buffer, rr->NAME);
  ptr = (uint16_t *) (buffer + bytes_writen);

  uint16_t  big_endian_TYPE      =  htons(rr->TYPE);
  uint16_t  big_endian_CLASS     =  htons(rr->CLASS);
  uint32_t  big_endian_TTL       =  htonl(rr->TTL);
  uint16_t  big_endian_RDLENGTH  =  htons(rr->RDLENGTH);

  memcpy(ptr++,   &(big_endian_TYPE),      sizeof(uint16_t));
  memcpy(ptr++,   &(big_endian_CLASS),     sizeof(uint16_t));
  memcpy(ptr,     &(big_endian_TTL),       sizeof(uint32_t));
  memcpy(ptr+=2,  &(big_endian_RDLENGTH),  sizeof(uint16_t));
  memcpy(ptr,     rr->RDATA,               rr->RDLENGTH);

  bytes_writen += sizeof(rr->TYPE)
               +  sizeof(rr->CLASS)
               +  sizeof(rr->TTL)
               +  sizeof(rr->RDLENGTH)
               +  rr->RDLENGTH;

  rr->raw_size = bytes_writen;

  rr->raw = malloc(rr->raw_size);
  memcpy(rr->raw, buffer, rr->raw_size);
}

void generate_success_response(Message *request, const char *ip, const char *comment, int master_socket, struct sockaddr client_addr, int client_len){
      
      // tractem el packet; generem el packet de resposta

      uint16_t FLAGS;
      FLAGS = request->header.FLAGS;
      
      // Assumim error i posem el flac de authoritive reply
      SET_HEADER_RCODE(FLAGS, RCODE_SERVER_ERROR);

      SET_HEADER_QR(FLAGS, QR_RESPONSE);

      int replyLen = 12;
      // comprovem si podem tractar la peticio
      if(GET_HEADER_QR(request->header.FLAGS) == 0)
      {

        int len = 0;
        SET_HEADER_RCODE(FLAGS, RCODE_NO_ERROR);

        // Temporalment sobre escrivim el camp de question des del valor QCLASS :)
        len = ((void *)request->question.QCLASS - (void *)request->raw_body) / sizeof(char);
        len += sizeof(uint16_t);

        // set TTL (4 bytes) a 10 s
        request->raw_body[len++] = 0;
        request->raw_body[len++] = 0;
        request->raw_body[len++] = 0;
        request->raw_body[len++] = 10;
  
        // set data length
        request->raw_body[len++] =0;
        request->raw_body[len++] =4;
       

        int ipAddr = ntohl(inet_addr(ip));
        request->raw_body[len++] = (ipAddr >> 24) & 0xff;
        request->raw_body[len++] = (ipAddr >> 16) & 0xff;
        request->raw_body[len++] = (ipAddr >> 8)  & 0xff;
        request->raw_body[len++] = (ipAddr)       & 0xff;

        // Resposta 2: hostname i TXT
        request->raw_body[len++] = 0xc0; // punter
        request->raw_body[len++] = 0x0c; // punter al nom (inici de data)
        
        // set upper byte of TYPE
        request->raw_body[len++] = (TYPE_TXT/256) & 0xff;
        // set lower byte of TYPE
        request->raw_body[len++] = (TYPE_TXT)     & 0xff;

        // set CLASS
        request->raw_body[len++] = (CLASS_IN/256) & 0xff;
        request->raw_body[len++] = (CLASS_IN)     & 0xff;

        // set TTL (4 bytes) a 10 s
        request->raw_body[len++] = 0;
        request->raw_body[len++] = 0;
        request->raw_body[len++] = 0;
        request->raw_body[len++] = 10;

        request->raw_body[len++] = 0;
	      request->raw_body[len++] = strlen(comment)+1;
	      request->raw_body[len++] = strlen(comment);
	      memcpy(request->raw_body+len,comment,strlen(comment));
      	len += strlen(comment);

      	replyLen += len; /* total size oof packet */

      }
      
      request->header.FLAGS = htons(FLAGS);
      request->header.ANCOUNT = htons(2);
      request->header.QDCOUNT = htons(0);

      if( 0 > sendto(master_socket, request, replyLen, 0, &client_addr, client_len))
      {
        perror("sendto() -> Error");
        exit(EXIT_FAILURE);
      }

}

void generate_failure_response(Message *request,  int master_socket, struct sockaddr client_addr, int client_len){

//      Message reply;
//      // tractem el packet; generem el packet de resposta
//      memset(&reply,0,sizeof(reply));
//      
//      reply.header.ID = request->header.ID;
//      // Assumim error i posem el flac de authoritive reply
//      reply.header.RCODE = RCODE_SERVER_ERROR;
//      reply.header.Flags = HEADER_QR ;//| FLAG_AA; // si li poso authorithive el ping no vol funcionar
//
//      int replyLen = 12;
//      // comprovem si podem tractar la peticio
//      if( (request->header.Flags & HEADER_QR) == 0) 
//      {
//
//        reply.header.RCODE = RCODE_NXDOMAIN;
//        reply.header.ANCOUNT = htons(0); // 0 respostes  
//
//			}
//
//      if( 0 > sendto(master_socket, &reply, replyLen, 0, &client_addr, client_len))
//      {
//        perror("sendto() -> Error");
//        exit(EXIT_FAILURE);
//      }

}

void parse_message_raw_body(Message *message) {
  char *QNAME = message->question.QNAME;
  char *raw_data = message->raw_body;

	memset(QNAME,0, sizeof(message->question.QNAME));

  int bytes_parsed = raw_hostname_to_s(QNAME, raw_data);
  
  message->question.QTYPE   =  (void  *)  (raw_data                  +  bytes_parsed);
  message->question.QCLASS  =  (void  *)  (message->question.QTYPE)  +  sizeof(uint16_t);
  message->answer.rr        =  (void  *)  (message->question.QCLASS) +  sizeof(uint16_t);
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
		if(0 == strcmp(message->question.QNAME, rr->NAME))
		{
      message->answer.rr = rr;
      ++message->header.ANCOUNT;
		}
    rr++;
	}
}
