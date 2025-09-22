/*
 * File: ports.h
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

#ifndef PORTS_H
#define PORTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define NO_PORT  0
#define MIN_PORT 1
#define MAX_PORT 65535

extern bool is_well_known_port( const int port ); // 0 - 1024
extern bool is_registered_port( const int port ); // 1024 - 49151
extern bool is_dynamic_port( const int port );    // 49152 - 65535

#ifdef __cplusplus
}
#endif

#endif // PORTS_H
