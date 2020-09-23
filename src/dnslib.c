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


// src:  '6google3com0'
// dest: 'google.com'
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

// src:  'google.com'
// dest: '6google3com0'
// Return: 12
//
// - Note: Both the number and chars are in a uint8_t or char format.
int s_to_raw_hostname(char *dest, char *src) {
  char     *ptr          =  strchr(src,  '.');
  int      bytes_writen  =  0;
  uint8_t  label_len;

  strcpy(dest + 1, src);
  bytes_writen += strlen(src) + 1;

  while ((ptr = strchr(src, '.')) != NULL) {
    label_len  =   (ptr - src)/sizeof(char);
    *dest      =   label_len;              
    dest       +=  label_len + 1;       
    src        =   dest + 1;       
  }

  *dest = strlen(src);
  ++bytes_writen;
  return bytes_writen;
}

uint16_t RDATA_TYPE_A_build(char **RDATA){
  uint16_t  RDLENGTH  =  4;
  int       ipAddr    =  ntohl(inet_addr(*RDATA));

  *RDATA = malloc(RDLENGTH);

  (*RDATA)[0] = (uint8_t) (ipAddr >> 24) & 0xff;
  (*RDATA)[1] = (uint8_t) (ipAddr >> 16) & 0xff;
  (*RDATA)[2] = (uint8_t) (ipAddr >> 8)  & 0xff;
  (*RDATA)[3] = (uint8_t) (ipAddr)       & 0xff;

  return RDLENGTH;
}

uint16_t RDATA_TYPE_TXT_build(char **RDATA){
  uint16_t RDLENGTH = strlen(*RDATA) + 1;
  char *buff = malloc(RDLENGTH);
  buff[0] = RDLENGTH - 1;
  memcpy(&buff[1], *RDATA, RDLENGTH -1);
  *RDATA = buff;
  return RDLENGTH;
}

uint16_t (*RDATA_build_array[255])() = { NULL,
                                         RDATA_TYPE_A_build,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         RDATA_TYPE_TXT_build,
                                         };


void RR_populate_missing(RR *rr) {
  if(rr->NAME == NULL)
    rr->NAME = malloc(255);

  if (!rr->TYPE)
    rr->TYPE  = TYPE_A;

  if (!rr->CLASS)
    rr->CLASS = CLASS_IN;

  rr->TTL = 10;

  if (!rr->RDLENGTH) 
    rr->RDLENGTH = RDATA_build_array[rr->TYPE](&(rr->RDATA));
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

void message_answer_RR_add(Message *message, RR *rr) {
  message->answer.rr[message->header.ANCOUNT] = *rr;
  ++message->header.ANCOUNT;

  memcpy(message->answer.raw_end, rr->raw, rr->raw_size);

  message->answer.raw_end  +=  rr->raw_size;
  message->raw_size        +=  rr->raw_size;
}

void message_big_endian_build(Message *message){
  message->header.FLAGS        =  htons(message->header.FLAGS);
  message->header.QDCOUNT = htons(message->header.QDCOUNT);
  message->header.ANCOUNT = htons(message->header.ANCOUNT);
  message->header.NSCOUNT = htons(message->header.NSCOUNT);
  message->header.ARCOUNT = htons(message->header.ARCOUNT);

  if (message->header.QDCOUNT) {
    *(message->question.QTYPE)   =  htons(*(message->question.QTYPE));
    *(message->question.QCLASS)  =  htons(*(message->question.QCLASS));
  }
}

void message_big_endian_parse(Message *message) {
  message->header.FLAGS   = ntohs(message->header.FLAGS);
  message->header.QDCOUNT = ntohs(message->header.QDCOUNT);
  message->header.ANCOUNT = ntohs(message->header.ANCOUNT);
  message->header.NSCOUNT = ntohs(message->header.NSCOUNT);
  message->header.ARCOUNT = ntohs(message->header.ARCOUNT);
  message->raw_size       = 12;


  if (message->header.QDCOUNT) {
    int bytes_parsed;
    char  *QNAME     =  message->question.QNAME;
    char  *raw_body  =  message->raw_body;

    memset(QNAME,0, sizeof(message->question.QNAME));
    bytes_parsed = raw_hostname_to_s(QNAME, raw_body);
    
    message->question.QTYPE   =  (void  *)  (raw_body                  +  bytes_parsed);
    message->question.QCLASS  =  (void  *)  (message->question.QTYPE)  +  sizeof(uint16_t);
    message->answer.raw_begin =  (void  *)  (message->question.QCLASS) +  sizeof(uint16_t);
    message->answer.raw_end   =  message->answer.raw_begin;

    *(message->question.QTYPE)   =  ntohs(*(message->question.QTYPE));
    *(message->question.QCLASS)  =  ntohs(*(message->question.QCLASS));

    message->raw_size += bytes_parsed 
                      + sizeof(uint16_t) 
                      + sizeof(uint16_t);
  }
}

void message_query_resolve(Message *message, const RR *records, int records_size)
{
  RR *rr;
  rr = (RR *) records;

  SET_HEADER_QR(message->header.FLAGS, QR_RESPONSE);

	for(int i = 0; i < records_size; i++)
	{
		if(0 == strcmp(message->question.QNAME, rr->NAME))
      message_answer_RR_add(message, rr);
    rr++;
	}
}

void parse_client_ip(char *target, const struct sockaddr *client) {
	memset(target,0,16);
	char *ip = inet_ntoa(((struct sockaddr_in *)client)->sin_addr);
	memcpy(target,ip,strlen(ip));
}
