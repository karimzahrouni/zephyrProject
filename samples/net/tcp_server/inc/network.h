/***
   * @file network.h
   * @brief Network initialization and DHCP event handling for Zephyr.
   *
   * This module provides functionality to initialize the network interface,
   * start DHCP, and notify the application when an IPv4 address is assigned.
   *
   * @author Zahrouni Karim
   * @date 2025
   */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*network_ip_callback_t)(const char *ip);

/* Initialize module: registers net_mgmt callback and grabs default iface */
int network_init(void);

/* Start DHCPv4 on the default interface */
int network_start(void);

/* Register a callback invoked when a DHCP IPv4 address is obtained */
int network_register_ip_acquired_callback(network_ip_callback_t cb);

#ifdef __cplusplus
}
#endif

#endif /* NETWORK_H_ */
