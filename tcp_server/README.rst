=========================
TCP LED Control Server
=========================

Overview
========
This application implements a **TCP server** that allows a remote TCP client
to send text commands to **control an LED** — turning it on, off, or making it blink.

It demonstrates how to integrate **Ethernet networking** with **GPIO control** in Zephyr,
using simple socket communication.

**Tested hardware:**
NUCLEO-F429ZI board with LAN865x Ethernet module.
Thanks to Zephyr's hardware abstraction, it can be adapted to other
Ethernet-capable boards and modules by updating the configuration
(`.conf`) and devicetree overlay (`.overlay`) files.

Supported commands:

- ``LED_ON`` → turns the LED on
- ``LED_OFF`` → turns the LED off

For each command, the server sends a confirmation message.
All events are logged using Zephyr’s logging API.

This example demonstrates:

- Initializing Ethernet networking using Zephyr’s networking API.
- Using DHCPv4 to automatically obtain an IP address.
- Setting up a socket-based TCP server.
- Controlling GPIO outputs from network events.
- Logging system events with Zephyr’s logging subsystem.

Requirements
============

Hardware
--------
You will need:

- Ethernet-capable development board (tested with NUCLEO-F429ZI + LAN865x)
- Ethernet cable and access to a DHCP-enabled network
- A client device with TCP client software (e.g., ``netcat`` or ``telnet``)

Software Configuration
-----------------------
Enable the Ethernet driver and required networking features in your board-specific
configuration file, for example:

``boards/arm/nucleo_f429zi/nucleo_f429zi.conf``::

    # Enable Ethernet module
    CONFIG_ETH_LAN865X=y

    # Enable networking features
    CONFIG_NET_IPV4=y
    CONFIG_NET_DHCPV4=y
    CONFIG_NET_TCP=y

    # Enable logging
    CONFIG_LOG=y
    CONFIG_NET_LOG=y

Pin mappings and hardware settings must be defined in the devicetree overlay file,
for example:

``boards/arm/nucleo_f429zi/nucleo_f429zi.overlay``

Building and Running
====================
To build with board-specific configuration and overlay files:

.. code-block:: bash

   west build -b nucleo_f429zi . -p \
     -- -DEXTRA_CONF_FILE='boards/arm/nucleo_f429zi.conf' \
        -DEXTRA_DTC_OVERLAY_FILE='boards/arm/nucleo_f429zi.overlay'

Flash the firmware:

.. code-block:: bash

   west flash

After flashing, the application will:

1. Initialize the Ethernet interface.
2. Start DHCP to obtain an IP address.
3. Log the assigned IP address via `LOG_INF()`.
4. Start listening for TCP connections on the configured port.

Checking the Assigned IP
========================
You can view the assigned IP address in the board's serial console:

.. code-block:: none

   [00:00:03.456,000] <inf> Network: Got IP: 192.168.1.13

Connecting to the TCP Server
============================
From another machine on the same network:

.. code-block:: bash

   telnet 192.168.1.13 5000

Send commands:

.. code-block:: none

   LED_ON
   LED_OFF

The board will toggle or blink the LED and respond:

.. code-block:: none

   LED TURNED ON
   LED TURNED OFF

Troubleshooting
===============
- **No IP address?**
  Ensure your Ethernet hardware is wired correctly and your network has DHCP enabled.
- **Cannot connect to the server?**
  Check firewall rules and confirm the TCP port is allowed.
- **LED not toggling?**
  Verify the LED’s GPIO pin is correctly defined in the devicetree.

Author
======
Karim Zahrouni
