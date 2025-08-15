DMA Memory-to-Memory Transfer Example
=====================================

This sample demonstrates how to use the STM32 DMA driver in Zephyr to perform
memory-to-memory transfers.

It copies a buffer from a source array to a destination array using DMA and
verifies the data after transfer. Transfer status and verification results
are printed using the Zephyr logging API.

Overview
********

The sample shows how to:

- Configure a DMA channel in memory-to-memory mode.
- Initiate a DMA transfer from a source buffer to a destination buffer.
- Use a DMA callback to check completion and validate the data.
- Print transfer status and results using Zephyr LOG API.

The DMA controller and channel are configured via Devicetree. The example
uses `dma2` channel 0 by default, but can be adjusted for other STM32 boards
or DMA channels by modifying the overlay file.

Building and Running
********************

The DMA peripheral and channel are configured in the board's ``.dts`` or
overlay file. Make sure that the DMA is enabled (`status = "okay";`) and
the channel is configured.

To build and flash for a specific board, e.g., Nucleo F429ZI:

.. code-block:: console

   west build -b nucleo_f429zi path/to/samples/dma_memcpy -p \
     -- -DEXTRA_CONF_FILE='boards/arm/nucleo_f429zi.conf' \
        -DEXTRA_DTC_OVERLAY_FILE='boards/arm/nucleo_f429zi.overlay'

   west flash

To use a different board or DMA channel, adjust the Devicetree overlay
accordingly and rebuild.

Devicetree Overlay Example
**************************

The following overlay can be used for Nucleo F429ZI:

.. code-block:: dts

   / {
       aliases {
           dmatest = &dma2; /* Use this alias in C code */
       };
   };

Sample Output
*************

Expected console output after running:

.. code-block:: console

   [00:00:00.000,000] <inf> dma_mem2mem: Starting DMA memory transfer test
   [00:00:00.000,000] <inf> dma_mem2mem: DMA start transfer ...
   [00:00:00.010,000] <inf> dma_mem2mem: DMA transfer completed!
   [00:00:00.010,000] <inf> dma_mem2mem: Data compare success: HELLO_DMA_TEST!!

.. note::

   - If the DMA device is not ready, an error will be logged.
   - The transfer buffer size can be adjusted by modifying the source array.
   - This example is portable to any STM32 board by changing the DMA node and
     channel in the overlay.
