#ifndef __UTILS_H
#define __UTILS_H

int registered(char ip[16]);
void exec_inite(Packet *packet);
void resolve_query(Message *message);

#endif
