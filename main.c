/*
 * wake on LAN tool
 *
 * Written by Alexander Ulmer
 * Copyright (C) 2015-2018
 * All Rights reserved.
 *
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

#include "wakeup.h"

// begin: to get rid of

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

// end: to get rid of


int main (int argc, char *argv[])
{
	int error;

    if (argc < 2)
    {
        // TODO: print usage
        exit(EXIT_SUCCESS);
    }

    // TODO: obtain mac address to wake up
    mac_address_t mac_address = NULL;

    error = wakeup(mac_address);
    if (error != E_SUCCESS)
    {
    	fprintf(stderr,
    			"wakeonlan: error: %s\n",
    			strerror(error));
    }
    
    return EXIT_SUCCESS;
}
