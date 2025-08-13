#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>

#include "network.h"

LOG_MODULE_REGISTER(LOG_LEVEL_INF);

/* Server config */
#define SERVER_PORT 			5000

/* Server file descriptor */
static int server_fd = -1;

/* Callback when IP is obtained */
static void on_ip(const char *ip)
{
	/* Use %s directly if log_strdup not available */
	LOG_INF("MY Device IP: %s", ip);
}

void main(void)
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	char recv_buf[128];
	int client_fd;

	/* Initialize network module */
	network_init();

	/* Register IP callback */
	network_register_ip_acquired_callback(on_ip);

	/* Start DHCP */
	network_start();

	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_fd < 0)
	{
		LOG_ERR("Failed to create socket");
		return;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		LOG_INF("Failed to bind socket");
		return;
	}

	if (listen(server_fd, 1) < 0)
	{
		LOG_INF("Failed to listen");
		return;
	}

	LOG_INF("TCP Server listening on port %d", SERVER_PORT);

	while (1)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client_fd < 0)
		{
			LOG_ERR("Failed to accept client");
			continue;
		}

		LOG_INF("Client connected");

		int len;
		while ((len = recv(client_fd, recv_buf, sizeof(recv_buf) - 1, 0)) > 0)
		{
			recv_buf[len] = '\0';
			LOG_INF("Received: %s", recv_buf);

			/* Turn LED on/off commands */
			if (strncmp(recv_buf, "LED_ON", 6) == 0)
			{
				LOG_INF("Turn LED ON");

				// Echo back
				send(client_fd, "LED TURNED ON", len, 0);
			}
			else if (strncmp(recv_buf, "LED_OFF", 7) == 0)
			{
				LOG_INF("Turn LED OFF");

				// Echo back
				send(client_fd, "LED TURNED OFF", len, 0);
			}
		}

		LOG_INF("Client disconnected");
		close(client_fd);
	}
}
