#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "wakeup.h"

static int send_magic_packet(int socket_descriptor,
							struct sockaddr_in socket_address,
							mac_address_t mac_address);

int wakeup(mac_address_t mac_address)
{
	int	socket_descriptor,
		permission,
		error_number;
	struct sockaddr_in socket_address;

	if ((socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return errno;

    permission = 1;
    setsockopt(socket_descriptor, SOL_SOCKET, SO_BROADCAST, &permission, sizeof (permission));

    socket_address.sin_family       = AF_INET;
    socket_address.sin_addr.s_addr  = inet_addr(BROADCAST_ADDR);
    socket_address.sin_port         = htons(BROADCAST_PORT);

    if ((error_number = send_magic_packet(socket_descriptor, socket_address, mac_address)) != E_SUCCESS)
        return error_number;

    close(socket_descriptor);
    return E_SUCCESS;
}

static int send_magic_packet(int socket_descriptor, struct sockaddr_in socket_address, mac_address_t mac_address)
{
	int error;
    const char magic_number[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    char *udp_packet = malloc(BROADCAST_SIZE);

    memcpy(udp_packet, magic_number, MAC_ADDRESS_SIZE);
    for (int i = 1; i <= 16; i++)
        memcpy(udp_packet + (MAC_ADDRESS_SIZE * i), mac_address, MAC_ADDRESS_SIZE);

    error = sendto(socket_descriptor,
    			udp_packet,
    			BROADCAST_SIZE,
    			0,
    			(struct sockaddr*)&socket_address,
    			sizeof(socket_address));
    			
   	if (error < 0)
        return errno;        
    return E_SUCCESS;
}
