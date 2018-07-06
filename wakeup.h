#ifndef WAKEUP_H
#define WAKEUP_H

#define BROADCAST_ADDR "192.168.0.255"
#define BROADCAST_PORT 7
#define BROADCAST_SIZE 102
#define MAC_ADDRESS_SIZE 6

#define E_SUCCESS 0

typedef const char * mac_address_t;

int wakeup(mac_address_t);

#endif
