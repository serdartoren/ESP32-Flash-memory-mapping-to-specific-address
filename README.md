| Supported Targets | ESP32 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- |

# Location of linker script files
/build/esp-idf/esp_system/ld/memory.ld
/build/esp-idf/esp_system/ld/sections.ld

# Explanation
My goal is to map the flash memory address to a specific address that the CPU can access.
![image](https://user-images.githubusercontent.com/42543700/187783304-5fc37ccf-6987-43e5-9005-8f8b282541ad.png)
If I define my variable to 0x3f410000 with linker script commands, I get the above error.

"spi_flash_mmap()" or "esp_partition_mmap()" gives me the starting address of the mapped area with dynamic memory management.
I couldn't find how to match the flash memory with the addresses I want.
I need help on this.
Below are two important links. I suggest you review:

https://esp32.com/viewtopic.php?t=4821

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spi_flash.html#_CPPv214spi_flash_mmap6size_t6size_t23spi_flash_mmap_memory_tPPKvP23spi_flash_mmap_handle_t:~:text=maps%20a%20region%20of%20physical%20flash%20addresses%20into%20instruction%20space%20or%20data%20space%20of%20the%20CPU.

# Partition Memory Map Example

This example demonstrates how to use `esp_partition_mmap` to configure MMU and map a partition into memory address space for read operations.

# Example Flow

The example uses a [custom partition table](./partitions_example.csv), with a data partition `storage` used for demonstration. Before mapping this partition to memory,
data is written to the partition used for verification.

The partition API function `esp_partition_mmap` is used to get a pointer to the mapped memory region and a handle to the mapping. The pointer is used to transparently read back the
verification data written previously. Once the data written and read are verified to be the same, the function `spi_flash_munmap` is used to release the mapping.

### Output
```
I (309) example: Written sample data to partition: ESP-IDF Partition Memory Map Example
I (309) example: Mapped partition to data memory address 0x3f410000
I (319) example: Read sample data from partition using mapped memory: ESP-IDF Partition Memory Map Example
I (329) example: Data matches
I (329) example: Unmapped partition from data memory
I (339) example: Example end
```

# Others

Detailed functional description of partition API is provided in [documentation](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/storage/spi_flash.html).

See the README.md file in the upper level 'examples' directory for more information about examples.
