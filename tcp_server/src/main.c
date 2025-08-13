#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "network.h"

LOG_MODULE_REGISTER(LOG_LEVEL_INF);

/* Callback when IP is obtained */
static void on_ip(const char *ip)
{
	/* Use %s directly if log_strdup not available */
	LOG_INF("MY Device IP: %s", ip);
}

void main(void)
{
	/* Initialize network module */
	network_init();

	/* Register IP callback */
	network_register_ip_acquired_callback(on_ip);

	/* Start DHCP */
	network_start();
}
