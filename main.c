/*
	*** AU Software Research Application Suite ***
		** Wake On LAN Network Broadcast **

	This software program looks for an interface file
	provided by the first command line argument and 
	performs a Wake-On-Lan broadcast through the entire
	local network with the first 48Bit from the interface
	file used as MAC-Address. The program will always print
	status and progress information on the command line.
	Help, version and manpage should be installed. If not,
	install the .deb package provided with this Software.

	Written By Alexander Ulmer.
	Copyright (C) 2015 AU Software Toolchain

	The WakeOnLan software is part of the AU Software
	Research Application Suite. This readme file has to be
	included in every redistribution of this program.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define BROADCAST_ADDR "192.168.0.255"
#define BROADCAST_PORT 7
#define BROADCAST_SIZE 102

#define HELP_FILE "/usr/share/iwakeonlan/help"
#define VERS_FILE "/usr/share/iwakeonlan/version"

void print_mac (const char *mac)
{
    int i;
    for (i = 0; i < 6; i++)
        printf ("%02x:", (unsigned char)mac[i]);
    printf ("\b ");
}
char *get_mac (char *file)
{
    int fd;
    char *mac = (char*) malloc (6);

    fd = open (file, O_RDONLY);

    if (fd < 0)
    {
        printf ("wakeonlan: %s: could not load interface!\n", file);
        exit(4);
    }

    read (fd, mac, 6);
    return mac;
}
int send_magic (int fd, const char *MAC_ADDR, struct sockaddr_in server)
{
    int i;

    char       *UDP_PACKET      = (char*) malloc (BROADCAST_SIZE);
    const char  MAGIC_NUMBER[]  = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    memcpy (UDP_PACKET, MAGIC_NUMBER, 6);
    for (i = 1; i <= 16; i++)
        memcpy (UDP_PACKET + (6 * i), MAC_ADDR, 6);

    if (sendto (fd, UDP_PACKET, BROADCAST_SIZE, 0, (struct sockaddr*)&server, sizeof (server)) < 0)
        return errno;
    return 0;
}
void print_doc (char *file)
{
    FILE *f;
    f = fopen (file, "r");

    if (f == NULL)
        printf ("wakeonlan: help file not installed!\n");

    char c;
    while ((c = getc (f)) != EOF)
        putchar (c);

    fclose (f);
}

int main (int argc, char *argv[])
{
    int                 sock, err, pem;
    struct sockaddr_in  server;

    if (argc < 2)
    {
        print_doc (HELP_FILE);
        return 3;
    }

    if (strcmp (argv[1], "-v") == 0)
    {
        print_doc (VERS_FILE);
        return 0;
    }
    else if (strcmp (argv[1], "-h") == 0)
    {
        print_doc (HELP_FILE);
        return 0;
    }

    printf ("wakeonlan: loading interface ...\n");
    const char *MAC_ADDR = get_mac (argv[1]);

    printf ("Using interface '%s' with MAC ", argv[1]);
    print_mac (MAC_ADDR);
    printf ("...\n");

    printf ("wakeonlan: preparing for broadcasting ...\n");

    if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf ("wakeonlan: could not create UDP socket.\n");
        return 1;
    }

    pem = 1;
    setsockopt (sock, SOL_SOCKET, SO_BROADCAST, &pem, sizeof (pem));

    bzero (&server, sizeof (server));
    server.sin_family       = AF_INET;
    server.sin_addr.s_addr  = inet_addr (BROADCAST_ADDR);
    server.sin_port         = htons     (BROADCAST_PORT);

    printf ("wakeonlan: sending broadcast ...\n");
    if ((err = send_magic (sock, MAC_ADDR, server)))
    {
        printf ("wakeonlan: failed to send broadcast (errno: %d)\n", err);
        return 2;
    }

    close (sock);

    printf ("wakeonlan: successfully broadcasted wake-up request!\n");
    return 0;
}
