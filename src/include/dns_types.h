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
#define  HEADER_QR_OFFSET      15
#define  HEADER_Opcode_OFFSET  11
#define  HEADER_AA_OFFSET      10
#define  HEADER_TC_OFFSET      9
#define  HEADER_RD_OFFSET      8
#define  HEADER_RA_OFFSET      7
#define  HEADER_Z_OFFSET       4
#define  HEADER_RCODE_OFFSET   0

#define  HEADER_QR_MASK      0x8000
#define  HEADER_Opcode_MASK  0x7800
#define  HEADER_AA_MASK      0x0400
#define  HEADER_TC_MASK      0x0200
#define  HEADER_RD_MASK      0x0100
#define  HEADER_RA_MASK      0x0080
#define  HEADER_Z_MASK       0x0070
#define  HEADER_RCODE_MASK   0x000f

#define  GET_HEADER_QR(FLAGS)      ((FLAGS  &  HEADER_QR_MASK)      >>  HEADER_QR_OFFSET)
#define  GET_HEADER_Opcode(FLAGS)  ((FLAGS  &  HEADER_Opcode_MASK)  >>  HEADER_Opcode_OFFSET)
#define  GET_HEADER_AA(FLAGS)      ((FLAGS  &  HEADER_AA_MASK)      >>  HEADER_AA_OFFSET)
#define  GET_HEADER_TC(FLAGS)      ((FLAGS  &  HEADER_TC_MASK)      >>  HEADER_TC_OFFSET)
#define  GET_HEADER_RD(FLAGS)      ((FLAGS  &  HEADER_RD_MASK)      >>  HEADER_RD_OFFSET)
#define  GET_HEADER_RA(FLAGS)      ((FLAGS  &  HEADER_RA_MASK)      >>  HEADER_RA_OFFSET)
#define  GET_HEADER_Z(FLAGS)       ((FLAGS  &  HEADER_Z_MASK)       >>  HEADER_Z_OFFSET)
#define  GET_HEADER_RCODE(FLAGS)   ((FLAGS  &  HEADER_RCODE_MASK)   >>  HEADER_RCODE_OFFSET)

#define  SET_HEADER_QR(FLAGS,QR)          FLAGS  =  (FLAGS  &  ~HEADER_QR_MASK)      |  (QR      <<  HEADER_QR_OFFSET)
#define  SET_HEADER_Opcode(FLAGS,Opcode)  FLAGS  =  (FLAGS  &  ~HEADER_Opcode_MASK)  |  (Opcode  <<  HEADER_Opcode_OFFSET)
#define  SET_HEADER_AA(FLAGS,AA)          FLAGS  =  (FLAGS  &  ~HEADER_AA_MASK)      |  (AA      <<  HEADER_AA_OFFSET)
#define  SET_HEADER_TC(FLAGS,TC)          FLAGS  =  (FLAGS  &  ~HEADER_TC_MASK)      |  (TC      <<  HEADER_TC_OFFSET)
#define  SET_HEADER_RD(FLAGS,RD)          FLAGS  =  (FLAGS  &  ~HEADER_RD_MASK)      |  (RD      <<  HEADER_RD_OFFSET)
#define  SET_HEADER_RA(FLAGS,RA)          FLAGS  =  (FLAGS  &  ~HEADER_RA_MASK)      |  (RA      <<  HEADER_RA_OFFSET)
#define  SET_HEADER_Z(FLAGS,Z)            FLAGS  =  (FLAGS  &  ~HEADER_Z_MASK)       |  (Z       <<  HEADER_Z_OFFSET)
#define  SET_HEADER_RCODE(FLAGS,RCODE)    FLAGS  =  (FLAGS  &  ~HEADER_RCODE_MASK)   |  (RCODE   <<  HEADER_RCODE_OFFSET)

enum TYPE
  {
TYPE_A=1,         /* host address */
TYPE_NS,          /* authoritative  name         server     */
TYPE_MD,          /* mail destination Obsolete - use) */
TYPE_MF,          /* mail forwarder (Obsolete  -               use MX)*/
TYPE_CNAME,       /* canonical name for an alias */
TYPE_SOA,         /* start of a zone of authority  */
TYPE_MB,          /* mailbox domain name (EXPERIMENTAL)  */
TYPE_MG,          /* mail group member (EXPERIMENTAL)*/
TYPE_MR,          /* mail rename domain name (EXPERIMENTAL)  */
TYPE_NULL,        /* a              NULL RR (EXPERIMENTAL)  */
TYPE_WKS,         /* well known service description     */
TYPE_PTR,         /* domain name pointer    */
TYPE_HINFO,       /* host information  */
TYPE_MINFO,       /* mailbox or mail list information     */
TYPE_MX,          /* mail exchange     */
TYPE_TXT,         /* text strings      */
TYPE_AAAA=0x1c,   /* IPv6 address request - RFC1886 */
          /** QTYPE are a superset of TYPE **/
QTYPE_AXFR=252,   /* request for a          transfer of an entire zone  */
QTYPE_MAILB=253,  /* request for mailbox records (MB/MG/MR)      */
QTYPE_MAILA=254,  /* request for mail agents (Obsolete by MX)     */
QTYPE_ALL=255     /* request for all records (symbol "*") */
  };
typedef enum TYPE TYPE;

enum header_QR {
  QR_QUERY,
  QR_RESPONSE
};
typedef enum header_QR header_QR; 

enum header_Opcode {
  Opcode_QUERY,
  Opcode_IQUERY,
  Opcode_STATUS
};
typedef enum header_Opcode header_Opcode;

enum header_AA {
  AA_NOT_AUTHORITY,
  AA_AUTHORITY
};
typedef enum header_AA header_AA;

enum header_TC {
  TC_NOT_TRUNCATED,
  TC_TRUNCATED
};
typedef enum header_TC header_TC;

enum header_RD {
  RD_NOT_DESIRED,
  RD_DESIRED
};
typedef enum header_RD header_RD;
  
enum header_RA {
  RA_NOT_AVAILABLE,
  RA_AVAILABLE
};
typedef enum header_RA header_RA;

enum header_RCODE {
  RCODE_NO_ERROR, 	/* no error condition */
  RCODE_FORMAT_ERROR, 	/* format error */
  RCODE_SERVER_ERROR, 	/* server error */
  RCODE_NXDOMAIN, 	/* no existent domain */
  RCODE_NA, 		/* not implemented (not available) */
  RCODE_REFUSED, 	/* refused */
};
typedef enum header_RCODE header_RCODE;

enum RR_CLASS {
  CLASS_IN = 1,
  CLASS_CS,
  CLASS_CH,
  CLASS_HS,
};
typedef enum RR_CLASS RR_CLASS;

/*** Flags for DNS header.  OR these together. ***/
#define HEADER_QR 0x80     /* is this a query or reply?
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

typedef struct {
  char      *NAME;
  uint16_t  TYPE;
  uint16_t  CLASS;
  uint32_t  TTL;
  uint16_t  RDLENGTH;
  char      *RDATA;
  int       privat;

  uint16_t  raw_size;
  char      *raw;
} RR;

typedef struct {
  uint16_t  ID;       /*  session serial number  */
  //uint8_t Flags;
  //uint8_t RCODE;
  uint16_t  FLAGS;    /*  see FLAGs */
  uint16_t  QDCOUNT;  /*  # entries in the question section  */
  uint16_t  ANCOUNT;  /*  # entries in the answer section  */
  uint16_t  NSCOUNT;  /*  # name server records in authority section */
  uint16_t  ARCOUNT; 	/* # resource records in additional records section */
  /* NOTE: MTU for UDP is 512 bytes.
     512 bytes - header = 500 data bytes */
} Header;

typedef struct {
  char      QNAME[256];
  uint16_t  *QTYPE;
  uint16_t  *QCLASS;
} Question;

typedef struct {
  RR        *rr;      // The size of this array is contained in Header.ANCOUNT 
  char      *raw_begin;
  uint16_t  raw_size;
} Answer;

typedef struct {
  Header    header;
  char      raw_body[500]; 	/* data */
  Question  question;
  Answer    answer;
} Message;

typedef struct {
  struct  sockaddr client_addr;
  Message message;
} Packet;

#endif
