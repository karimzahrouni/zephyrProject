#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/dma.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(dma_mem2mem, LOG_LEVEL_INF);

/* Use Devicetree alias "dmatest" for portability */
#define DMA_NODE      DT_ALIAS(dmatest)
#define DMA_CHANNEL   0

static const uint8_t tx_buffer[] = "HELLO_DMA_TEST!!";
#define BUFFER_SIZE   (sizeof(tx_buffer))

static uint8_t rx_buffer[BUFFER_SIZE];

static struct dma_config       dma_cfg       = {0};
static struct dma_block_config dma_block_cfg = {0};

const struct device *dma_dev;

static void dma_callback(const struct device *dev, void *user_data,
                         uint32_t channel, int status)
{
  ARG_UNUSED(dev);
  ARG_UNUSED(user_data);
  ARG_UNUSED(channel);

  if (status == 0)
  {
    LOG_INF("DMA transfer completed!");

    if (memcmp(tx_buffer, rx_buffer, BUFFER_SIZE) == 0)
    {
      LOG_INF("Data compare success: %s", rx_buffer);
    }
    else
    {
      LOG_ERR("Data mismatch!");
    }
  }
  else
  {
    LOG_ERR("DMA transfer error: %d", status);
  }
}

int main(void)
{
  LOG_INF("Starting DMA memory transfer test");

  dma_dev = DEVICE_DT_GET(DMA_NODE);
  if (!device_is_ready(dma_dev))
  {
    LOG_ERR("DMA device not ready");
    return -1;
  }

  /* Clear receive buffer */
  memset(rx_buffer, 0, sizeof(rx_buffer));

  dma_cfg.channel_direction    = MEMORY_TO_MEMORY;
  dma_cfg.source_data_size     = 1;       /* byte */
  dma_cfg.dest_data_size       = 1;
  dma_cfg.source_burst_length  = 1;
  dma_cfg.dest_burst_length    = 1;
  dma_cfg.dma_callback         = dma_callback;
  dma_cfg.block_count          = 1;
  dma_cfg.head_block           = &dma_block_cfg;

  dma_block_cfg.block_size     = BUFFER_SIZE;
  dma_block_cfg.source_address = (uint32_t)tx_buffer;
  dma_block_cfg.dest_address   = (uint32_t)rx_buffer;

  if (dma_config(dma_dev, DMA_CHANNEL, &dma_cfg) != 0)
  {
    LOG_ERR("DMA config failed!");
    return -1;
  }

  LOG_INF("DMA start transfer ...");

  if (dma_start(dma_dev, DMA_CHANNEL) != 0)
  {
    LOG_ERR("DMA start failed!");
    return -1;
  }

	return 0;
}
