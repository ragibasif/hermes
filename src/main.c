/*
 * File: main.c
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT            8080
#define BACKLOG         10 // number of pending connections in queue
#define WWW_EXAMPLE_COM "https://www.example.com/"

char *h_int_to_str( int n ) {
    // NOTE: snprintf is safer than sprintf and should always be used over
    // sprintf
    // NOTE: snprintf returns the length if called with NULL, 0 as first
    // parameters, it returns the length without the null terminator
    size_t size = snprintf( NULL, 0, "%d", n );
    // NOTE: one more byte must be allocated with the length for the null
    // terminator
    // NOTE: remember to free the buffer when done with it
    char *buffer = malloc( sizeof *buffer * ( size + 1 ) );
    snprintf( buffer, size + 1, "%d", n );
    return buffer;
}

int main( int argc, char **argv ) {
    puts( "Hll" );

    char *port = h_int_to_str( PORT );

    // TODO: open a socket file descriptor
    // TODO: bind the socket to a port
    // TODO: listen for connections
    // TODO: accept a connection
    // TODO: send a response with 200

    // NOTE: socket descriptors are ints

    // call to listen to hosts IP address
    // NOTE: struct addrinfo: a linked list
    // used to prep socket address structures for use
    // used in host name lookups and service name lookups
    struct addrinfo  hints;
    struct addrinfo *res;              // will point to results
    memset( &hints, 0, sizeof hints ); // clear the struct
    hints.ai_family   = AF_UNSPEC;   // use IPv4 or IPv6, either, doesn't matter
    hints.ai_socktype = SOCK_STREAM; // using TCP stream sockets as opposed to
                                     // UDP datagram sockets
    hints.ai_flags = AI_PASSIVE;     // use the hosts IP

    // NOTE: getaddrinfo sets up res which is a linked list of results
    // - it can also do DNS and service name lookups
    // NOTE: error checking since getaddrinfo returns non-zero if there is an
    // error
    int status;
    if ( ( status = getaddrinfo( NULL, port, &hints, &res ) ) != 0 ) {
        fprintf( stderr, "gai error: %s\n", gai_strerror( status ) );
        exit( EXIT_FAILURE );
    }

    // NOTE: get a socket file descriptor
    int socket_fd =
        socket( res->ai_family, res->ai_socktype, res->ai_protocol );

    // NOTE: to stop bind() from failing with a "Address already in use." error,
    // set an option to reuse addresses
    bool reuse = true;
    setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse );
    // NOTE: bind the socket to a port
    bind( socket_fd, res->ai_addr, res->ai_addrlen );
    // NOTE: listen on the socket
    listen( socket_fd, BACKLOG );

    // NOTE: accept incoming connections
    struct sockaddr_storage client_addr;
    socklen_t               client_addr_size = sizeof client_addr;
    int                     client_fd =
        accept( socket_fd, (struct sockaddr *)&client_addr, &client_addr_size );

    // NOTE: send() and recv() are blocking calls
    // NOTE: recv() will block until data arrives
    // NOTE: send() will block but its very rare
    // NOTE: blocking == sleep

    // NOTE: must remember to free allocated memory
    freeaddrinfo( res );
    free( port );
    port = NULL;

    return EXIT_SUCCESS;
}
