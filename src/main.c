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

void h_sigchld_handler( int s ) {
    (void)s; // quiet unused variable warning
    while ( waitpid( -1, NULL, WNOHANG ) > 0 );
}

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

// NOTE: get sockaddr, IPv4 or IPv6
void *h_get_in_addr( struct sockaddr *sa ) {
    if ( sa->sa_family == AF_INET ) {
        return &( ( (struct sockaddr_in *)sa )->sin_addr );
    }
    return &( ( (struct sockaddr_in6 *)sa )->sin6_addr );
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

    int socket_fd;
    // NOTE: iterate through the res linked list and bind to the first valid
    struct addrinfo *curr;
    // NOTE: to stop bind() from failing with a "Address already in use." error,
    // set an option to reuse addresses
    int reuse = 1;
    for ( curr = res; curr; curr = curr->ai_next ) {
        // NOTE: get a socket file descriptor
        if ( ( socket_fd = socket( res->ai_family, res->ai_socktype,
                                   res->ai_protocol ) ) == -1 ) {
            perror( "server: socket" );
            continue;
        }

        if ( setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse,
                         sizeof reuse ) == -1 ) {
            perror( "setsockopt" );
            exit( EXIT_FAILURE );
        }

        // NOTE: bind the socket to a port
        if ( bind( socket_fd, res->ai_addr, res->ai_addrlen ) == -1 ) {
            close( socket_fd );
            perror( "server: bind" );
            continue;
        }
        break;
    }
    // NOTE: must remember to free allocated memory
    freeaddrinfo( res ); // cleanup res
    res = NULL;

    // NOTE: if curr reaches past the last node of the list, then server failed
    // to bind
    if ( !curr ) {
        fprintf( stderr, "server: failed to bind\n" );
        exit( EXIT_FAILURE );
    }

    // NOTE: listen on the socket
    if ( listen( socket_fd, BACKLOG ) == -1 ) {
        perror( "listen" );
        exit( EXIT_FAILURE );
    }

    struct sigaction sa;
    sa.sa_handler = h_sigchld_handler; // reap all dead processes
    sigemptyset( &sa.sa_mask );
    sa.sa_flags = SA_RESTART;
    if ( sigaction( SIGCHLD, &sa, NULL ) == -1 ) {
        perror( "sigaction" );
        exit( EXIT_FAILURE );
    }

    printf( "server: waiting for connection...\n" );

    char                    s[INET6_ADDRSTRLEN];
    struct sockaddr_storage client_addr;
    socklen_t               client_addr_size;
    int                     client_fd;
    for ( ;; ) {
        // NOTE: accept incoming connections
        client_addr_size = sizeof client_addr;
        client_fd        = accept( socket_fd, (struct sockaddr *)&client_addr,
                                   &client_addr_size );

        if ( client_fd == -1 ) {
            perror( "accept" );
            continue;
        }

        inet_ntop( client_addr.ss_family,
                   h_get_in_addr( (struct sockaddr *)&client_addr ), s,
                   sizeof s );
        printf( "server: got connection from %s\n", s );

        // NOTE: send() and recv() are blocking calls
        // NOTE: recv() will block until data arrives
        // NOTE: send() will block but its very rare
        // NOTE: blocking == sleep
        if ( !fork() ) {        // child process
            close( socket_fd ); // child doesn't need the listener
            if ( send( client_fd, "Hello, World!", 13, 0 ) == -1 ) {
                perror( "send" );
            }
            close( client_fd );
            exit( EXIT_SUCCESS );
        }
        close( client_fd ); // parent doesn't need this
    }

    // NOTE: must remember to free allocated memory
    free( port );
    port = NULL;

    return EXIT_SUCCESS;
}
