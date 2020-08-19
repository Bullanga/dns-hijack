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


// Input:  '6google3com0'
// Return: 12
//
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
  char     *ptr          =  strchr(src,  '.');
  int      bytes_writen  =  0;
  uint8_t  label_len;

  strcpy(dest + 1, src);
  bytes_writen += strlen(src) + 1;

  while ((ptr = strchr(src, '.')) != NULL) {
    label_len = (ptr - src) / sizeof(char);
    *dest = label_len;
    dest  += label_len + 1;
    src = dest + 1;
  }

  *dest = strlen(src);
  ++bytes_writen;
  return bytes_writen;
}

uint16_t RDATA_TYPE_A_build(char *RDATA){
  uint16_t  RDLENGTH  =  4;
  int       ipAddr    =  ntohl(inet_addr(RDATA));

  RDATA =  malloc(RDLENGTH);

  RDATA[0] = (uint8_t) (ipAddr >> 24) & 0xff;
  RDATA[1] = (uint8_t) (ipAddr >> 16) & 0xff;
  RDATA[2] = (uint8_t) (ipAddr >> 8)  & 0xff;
  RDATA[3] = (uint8_t) (ipAddr)       & 0xff;
  
  printf("pre :%x\n", (unsigned char) RDATA[0]);
  printf("pre :%x\n", (unsigned char) RDATA[1]);
  printf("pre :%x\n", (unsigned char) RDATA[2]);
  printf("pre :%x\n", (unsigned char) RDATA[3]);
  printf("----------\n");

  return RDLENGTH;
}

uint16_t (*RDATA_build_array[2])() = { NULL, RDATA_TYPE_A_build };

void RR_populate_missing(RR *rr) {
  if (!rr->TYPE)
    rr->TYPE  = TYPE_A;

  if (!rr->CLASS)
    rr->CLASS = CLASS_IN;

  rr->TTL = 10;

  if (!rr->RDLENGTH) 
    rr->RDLENGTH = RDATA_build_array[rr->TYPE](rr->RDATA);
  printf("post :%x\n", (unsigned char) rr->RDATA[0]);
  printf("post :%x\n", (unsigned char) rr->RDATA[1]);
  printf("post :%x\n", (unsigned char) rr->RDATA[2]);
  printf("post :%x\n\n", (unsigned char) rr->RDATA[3]);
}

void RR_raw_big_endian_build(RR *rr) {
  char      buffer[500];
  int       bytes_writen;
  uint16_t  *ptr;

  bytes_writen = s_to_raw_hostname(buffer, rr->NAME);

  ptr = (uint16_t *) (buffer + bytes_writen);

  uint16_t  big_endian_TYPE      =  htons(rr->TYPE);
  uint16_t  big_endian_CLASS     =  htons(rr->CLASS);
  uint32_t  big_endian_TTL       =  htonl(rr->TTL);
  uint16_t  big_endian_RDLENGTH  =  htons(rr->RDLENGTH);

  memcpy(ptr++,  &(big_endian_TYPE),      sizeof(uint16_t));
  memcpy(ptr++,  &(big_endian_CLASS),     sizeof(uint16_t));
  memcpy(ptr++,  &(big_endian_TTL),       sizeof(uint32_t));
  memcpy(++ptr,  &(big_endian_RDLENGTH),  sizeof(uint16_t));
  memcpy(++ptr,  rr->RDATA,               rr->RDLENGTH);

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
//
//
//        // Temporalment sobre escrivim el camp de question des del valor QCLASS :)
//        len = ((void *)request->question.QCLASS - (void *)request->raw_body) / sizeof(char);
//        len += sizeof(uint16_t);
//
//        // set TTL (4 bytes) a 10 s
//        request->raw_body[len++] = 0;
//        request->raw_body[len++] = 0;
//        request->raw_body[len++] = 0;
//        request->raw_body[len++] = 10;
//  
//        // set data length
//        request->raw_body[len++] =0;
//        request->raw_body[len++] =4;
//       
//
//        int ipAddr = ntohl(inet_addr(ip));
//        request->raw_body[len++] = (ipAddr >> 24) & 0xff;
//        request->raw_body[len++] = (ipAddr >> 16) & 0xff;
//        request->raw_body[len++] = (ipAddr >> 8)  & 0xff;
//        request->raw_body[len++] = (ipAddr)       & 0xff;

//        // Resposta 2: hostname i TXT
//        request->raw_body[len++] = 0xc0; // punter
//        request->raw_body[len++] = 0x0c; // punter al nom (inici de data)
//        
//        // set upper byte of TYPE
//        request->raw_body[len++] = (TYPE_TXT/256) & 0xff;
//        // set lower byte of TYPE
//        request->raw_body[len++] = (TYPE_TXT)     & 0xff;
//
//        // set CLASS
//        request->raw_body[len++] = (CLASS_IN/256) & 0xff;
//        request->raw_body[len++] = (CLASS_IN)     & 0xff;
//
//        // set TTL (4 bytes) a 10 s
//        request->raw_body[len++] = 0;
//        request->raw_body[len++] = 0;
//        request->raw_body[len++] = 0;
//        request->raw_body[len++] = 10;
//
//        request->raw_body[len++] = 0;
//	      request->raw_body[len++] = strlen(comment)+1;
//	      request->raw_body[len++] = strlen(comment);
//	      memcpy(request->raw_body+len,comment,strlen(comment));
//      	len += strlen(comment);
//
//      	replyLen += len; /* total size oof packet */

      }
      
      request->header.FLAGS        =  htons(FLAGS);
      request->header.ANCOUNT      =  htons(1);
      //request->header.ANCOUNT      =  htons(2);
      request->header.QDCOUNT      =  htons(1);
      *(request->question.QTYPE)   =  htons(*(request->question.QTYPE));
      *(request->question.QCLASS)  =  htons(*(request->question.QCLASS));
      replyLen = request->raw_size + 5;

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

void message_big_endian_parse(Message *message) {
  message->header.FLAGS   = ntohs(message->header.FLAGS);
  message->header.QDCOUNT = ntohs(message->header.QDCOUNT);
  message->header.ANCOUNT = ntohs(message->header.ANCOUNT);
  message->header.NSCOUNT = ntohs(message->header.NSCOUNT);
  message->header.ARCOUNT = ntohs(message->header.ARCOUNT);
  message->raw_size       = 12;


  if (GET_HEADER_QR(message->header.FLAGS) == QR_QUERY) {
    int bytes_parsed;
    char  *QNAME     =  message->question.QNAME;
    char  *raw_body  =  message->raw_body;

    memset(QNAME,0, sizeof(message->question.QNAME));
    bytes_parsed = raw_hostname_to_s(QNAME, raw_body);
    
    message->question.QTYPE   =  (void  *)  (raw_body                  +  bytes_parsed);
    message->question.QCLASS  =  (void  *)  (message->question.QTYPE)  +  sizeof(uint16_t);
    message->answer.raw_begin =  (void  *)  (message->question.QCLASS) +  sizeof(uint16_t);

    *(message->question.QTYPE)   =  ntohs(*(message->question.QTYPE));
    *(message->question.QCLASS)  =  ntohs(*(message->question.QCLASS));

    message->raw_size += bytes_parsed 
                      + sizeof(uint16_t) 
                      + sizeof(uint16_t);
  }
}

void parse_client_ip(char *target, const struct sockaddr *client) {

	memset(target,0,16);
	char *ip = inet_ntoa(((struct sockaddr_in *)client)->sin_addr);
	memcpy(target,ip,strlen(ip));

}

void message_query_resolve(Message *message, const RR *records, int records_size)
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
      memcpy(message->answer.raw_begin, rr->raw, rr->raw_size);
      message->raw_size += rr->raw_size;
		}
    rr++;
	}
}
