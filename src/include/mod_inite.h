#ifndef __MOD_INITE_H
#define __MOD_INITE_H

void init_inite();
void exec_inite(Packet *packet);
int  registered(char ip[16]);
void resolve_query(Message *message);

#endif
