#ifndef _TYPES_H
#define _TYPES_H
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
/*
                        HEADER QUERY / RESPONSE DNS
      
             0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |                      ID                       |    2 bytes per linea 
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |QR|   Opcode  |AA|TC|RD|RA| Z|AD|CD|   RCODE   |    
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |                QDCOUNT/ZOCOUNT                |     No hem de tractar
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |                ANCOUNT/PRCOUNT                |     paquets inferiors a
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |                NSCOUNT/UPCOUNT                |     12 bytes ja que no
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            |                    ARCOUNT                    |     sera cap query dns
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+


            
     APUNTS:
      ------------------------------------------------
       OpCode Name                

        0     Query 
        1     IQuery  (Inverse Query) 
        2     Status                  
        3     available for assignment
        4     Notify                  
        5     Update                 
       6-15   available for assignment
      ------------------------------------------------
       RCODE   Name    Description                       

        0    NoError   No Error                           
        1    FormErr   Format Error                       
        2    ServFail  Server Failure                     
        3    NXDomain  Non-Existent Domain                
        4    NotImp    Not Implemented                    
        5    Refused   Query Refused                      
        6    YXDomain  Name Exists when it should not     
        7    YXRRSet   RR Set Exists when it should not   
        8    NXRRSet   RR Set that should exist does not  
        9    NotAuth   Server Not Authoritative for zone  
       10    NotZone   Name not contained in zone         
       11-15           available for assignment
       16    BADVERS   Bad OPT Version                    
       16    BADSIG    TSIG Signature Failure             
       17    BADKEY    Key not recognized                 
       18    BADTIME   Signature out of time window       
       19    BADMODE   Bad TKEY Mode                      
       20    BADNAME   Duplicate key name                 
       21    BADALG    Algorithm not supported            
      ------------------------------------------------
*/

enum TYPE
  {
  TYPE_A=1, /* host address */
  TYPE_NS, /* authoritative name server */
  TYPE_MD, /* mail destination (Obsolete - use MX) */
  TYPE_MF, /* mail forwarder (Obsolete - use MX) */
  TYPE_CNAME, /* canonical name for an alias */
  TYPE_SOA, /* start of a zone of authority */
  TYPE_MB, /* mailbox domain name (EXPERIMENTAL) */
  TYPE_MG, /* mail group member (EXPERIMENTAL) */
  TYPE_MR, /* mail rename domain name (EXPERIMENTAL) */
  TYPE_NULL, /* a NULL RR (EXPERIMENTAL) */
  TYPE_WKS, /* well known service description */
  TYPE_PTR, /* domain name pointer */
  TYPE_HINFO, /* host information */
  TYPE_MINFO, /* mailbox or mail list information */
  TYPE_MX, /* mail exchange */
  TYPE_TXT, /* text strings */
  TYPE_AAAA   = 0x1c, /* IPv6 address request - RFC1886 */
  /** QTYPE are a superset of TYPE **/
  QTYPE_AXFR  = 252, /* request for a transfer of an entire zone */
  QTYPE_MAILB = 253, /* request for mailbox records (MB/MG/MR) */
  QTYPE_MAILA = 254, /* request for mail agents (Obsolete by MX) */
  QTYPE_ALL   = 255 /* request for all records (symbol "*") */
  };
typedef enum TYPE TYPE;

typedef struct {
  uint16_t  ID;       /*  session serial number  */
  uint8_t   Flags;    /*  see FLAGs */
  uint8_t   Rcode;    /*  see RCODE */
  uint16_t  Qcount;   /*  # entries in the question section  */
  uint16_t  Acount;   /*  # entries in the answer section  */
  uint16_t  NScount;  /*  # name server records in authority section */
  uint16_t  ARcount; 	/* # resource records in additional records section */
  /* NOTE: MTU for UDP is 512 bytes.
     512 bytes - header = 500 data bytes */
  char      Data[500]; 	/* data */
} Header;

typedef struct {
  char      *QNAME;
  uint16_t  *QTYPE;
  uint16_t  *QCLASS;

  char      raw[500-16*6];
} Question;

//struct Answer {
//  RR *RRs;
//};

typedef struct {
  Header header;
  //Question question;
  //Answer answer;
} Message;

typedef struct {
  struct sockaddr client_addr;
  Message msg;
} Packet;

/*** Flags for DNS header.  OR these together. ***/
#define FLAG_REPLY 0x80     /* is this a query or reply?
			       0=query, 1=reply */
#define FLAG_OPCODE_MASK   0x30 	/* query mask */
#define FLAG_OPCODE_QUERY  0x00 	/* standard query */
#define FLAG_OPCODE_IQUERY 0x10 /* inverse query */
#define FLAG_OPCODE_STATUS 0x20 /* server status request */
/* other opcode values bits reserved */
#define FLAG_AA 0x04 	/* authoritative answer */
#define FLAG_TC 0x02 	/* message truncated */
#define FLAG_RD 0x01 	/* recursion denied */

/* Flags added to the rcode byte */
#define FLAG_RA    0x80 	/* recursion available */
#define FLAG_AAA   0x20 	/* answer authenticated */
#define RCODE_MASK 0x0f
enum RCODE
  {
  RCODE_NO_ERROR=0, 	/* no error condition */
  RCODE_FORMAT_ERROR, 	/* format error */
  RCODE_SERVER_ERROR, 	/* server error */
  RCODE_NXDOMAIN, 	/* no existent domain */
  RCODE_NA, 		/* not implemented (not available) */
  RCODE_REFUSED, 	/* refused */
  };
typedef enum RCODE RCODE;

#define CLASS_IN 0x01


typedef struct {
  const  char  *domain;
  const  int   type;
  const  int   privat;
  const  char  *ip;
} RR;

#endif
