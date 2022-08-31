/* Finding Partitions Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <assert.h>
#include "esp_partition.h"
#include "esp_log.h"

static const char *TAG = "example";
static const volatile char test  __attribute__((section(".config\n\t#")));

void TASK1(void *param)
{
  const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "storage0");
  assert(partition != NULL);

  static const char store_data[] = "ESP-IDF Partition Memory Map Example";
  const void *map_ptr;
  spi_flash_mmap_handle_t map_handle;
  char read_data[sizeof(store_data)];

while (1) {
  /* code */
  // Prepare data to be read later using the mapped address
  ESP_ERROR_CHECK(esp_partition_erase_range(partition, 0, partition->size));
  ESP_ERROR_CHECK(esp_partition_write(partition, 0, store_data, sizeof(store_data)));
  ESP_LOGI(TAG, "Written sample data to partition: %s", store_data);

  // Map the partition to data memory
  ESP_ERROR_CHECK(esp_partition_mmap(partition, 0, partition->size, SPI_FLASH_MMAP_DATA, &map_ptr, &map_handle));
  ESP_LOGI(TAG, "Mapped partition to data memory address %p", map_ptr);

  // Read back the written verification data using the mapped memory pointer
  memcpy(read_data, map_ptr, sizeof(read_data));
  ESP_LOGI(TAG, "Read sample data from partition using mapped memory: %s", (char*) read_data);

  assert(strcmp(store_data, read_data) == 0);
  ESP_LOGI(TAG, "Data matches");

  //printf("Pointer %p\n",map_ptr);
  //printf("Text = %s\n",map_ptr);
  spi_flash_munmap(map_handle);


  // Unmap mapped memory
  //spi_flash_munmap(map_handle);
  //ESP_LOGI(TAG, "Unmapped partition from data memory");

  //ESP_LOGI(TAG, "Example end");
  for (size_t i = 0; i < 3; i++) {
    printf("***********************************************\n");
  }
  vTaskDelay(5000 / portTICK_PERIOD_MS);
}
}


void TASK2(void *param)
{
  const void *map_ptr = (void*)0x3f410000;
  const char read_data[36];
  while(1)
  {
    printf("Pointer %p\n",map_ptr);
    memcpy((void*)read_data, (void*)map_ptr, 36);
    printf("Text = %s\n",read_data);
    vTaskDelay(5001 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
    static const void *map_ptr2;
    spi_flash_mmap_handle_t map_handle2;
    ESP_ERROR_CHECK(spi_flash_mmap(0x110000,0x40000,SPI_FLASH_MMAP_DATA,&map_ptr2, &map_handle2));
    printf("Text = %p\n",map_ptr2);
    xTaskCreatePinnedToCore(&TASK1, "TASK1", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(&TASK2, "TASK2", 4096, NULL, 1, NULL, 1);
      return;
}
