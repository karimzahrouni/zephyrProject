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

#include <zephyr/logging/log.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/dhcpv4.h>

#include "network.h"

LOG_MODULE_REGISTER(Network, LOG_LEVEL_INF);

/**
 * @brief Network context structure for internal state management.
 */
struct network_ctx
{
  struct net_if *p_iface;                    /**< Pointer to the active network interface */
  struct net_mgmt_event_callback mgmt_cb;    /**< Network management event callback handle */
  network_ip_callback_t p_cb;                /**< User callback when an IP is assigned */
  bool is_initialized;                       /**< Flag to check if initialized */
};

static struct network_ctx g_net = {0};

/**
 * @brief Network management event handler.
 * This handler reacts to IPv4 address assignment events and invokes
 * the registered user callback with the assigned IP address.
 * @param p_cb     Network management event callback (unused).
 * @param event    Network management event code.
 * @param p_iface  Network interface associated with the event.
 */
static void mgmt_handler(struct net_mgmt_event_callback *p_cb,
                         uint32_t event, struct net_if *p_iface)
{
  ARG_UNUSED(p_cb);

  if (event != NET_EVENT_IPV4_ADDR_ADD)
  {
    return;
  }

  /* Ensure IPv4 configuration exists */
  if (!p_iface || !p_iface->config.ip.ipv4)
  {
    return;
  }

  const struct net_if_addr_ipv4 *p_ua = &p_iface->config.ip.ipv4->unicast[0];

  /* Only process DHCP-assigned addresses */
  if (p_ua->ipv4.addr_type != NET_ADDR_DHCP)
	{
    return;
  }

  char ip_buf[NET_IPV4_ADDR_LEN] = {0};

  if (!net_addr_ntop(AF_INET, &p_ua->ipv4.address.in_addr, ip_buf, sizeof(ip_buf)))
  {
    LOG_ERR("Failed to convert IPv4 to string");
    return;
  }

  LOG_INF("Got IP: %s", ip_buf);

  if (g_net.p_cb)
  {
    g_net.p_cb(ip_buf);
  }
}

/**
 * @brief Initialize the network module.
 * This function sets up the default network interface and registers
 * the management event callback for IPv4 address assignment.
 * @return 0 on success, negative error code on failure.
 */
int network_init(void)
{
  if (g_net.is_initialized)
  {
    return 0;
  }

  g_net.p_iface = net_if_get_default();
  if (!g_net.p_iface)
  {
    LOG_ERR("No default network interface");
    return -ENODEV;
  }

  net_mgmt_init_event_callback(&g_net.mgmt_cb,
                              mgmt_handler,
	                            NET_EVENT_IPV4_ADDR_ADD);

  net_mgmt_add_event_callback(&g_net.mgmt_cb);

  g_net.is_initialized = true;

  return 0;
}

/**
 * @brief Start the network connection using DHCP.
 * This function starts the DHCPv4 client on the default interface.
 * @return 0 on success, negative error code on failure.
 */
int network_start(void)
{
  if (!g_net.is_initialized)
  {
    int rc = network_init();
    if (rc)
    {
      return rc;
    }
  }

  net_dhcpv4_start(g_net.p_iface);

  return 0;
}

/**
 * @brief Register a callback to be called when an IP address is assigned.
 * @param p_cb  Callback function to invoke with the assigned IP string.
 * @return 0 on success, negative error code if the callback is NULL.
 */
int network_register_ip_acquired_callback(network_ip_callback_t p_cb)
{
  if (p_cb == NULL)
  {
    LOG_ERR("NULL callback");
    return -EINVAL;
  }

  g_net.p_cb = p_cb;
  return 0;
}
