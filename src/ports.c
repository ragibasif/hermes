/*
 * File: ports.c
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

// https://en.wikipedia.org/wiki/Port_(computer_networking)

#include "ports.h"

#define WELL_KNOWN_PORT_MIN 0
#define WELL_KNOWN_PORT_MAX 1023
#define REGISTERED_PORT_MIN 1024
#define REGISTERED_PORT_MAX 49151
#define DYNAMIC_PORT_MIN    49152
#define DYNAMIC_PORT_MAX    65535

bool is_well_known_port( const int port ) {
    return port >= WELL_KNOWN_PORT_MIN && port <= WELL_KNOWN_PORT_MAX;
}

bool is_registered_port( const int port ) {
    return port >= REGISTERED_PORT_MIN && port <= REGISTERED_PORT_MAX;
}

bool is_dynamic_port( const int port ) {
    return port >= DYNAMIC_PORT_MIN && port <= DYNAMIC_PORT_MAX;
}
