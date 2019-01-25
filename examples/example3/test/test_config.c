#include "unity.h"

#include "mock_flash.h" // Mock the flash interface.

#include "config.h"

#include <stdint.h>
#include <string.h>

uint8_t simulated_flash[FLASH_SIZE];

static int mock_erase(int num_calls) {
  memset(simulated_flash, 0xff, FLASH_SIZE);
  return 0;
}
static int mock_read(uint8_t *data, uint32_t address, size_t count, int num_calls) {
  memcpy(data, &simulated_flash[address], count);
  return count;
}

static int mock_write(uint8_t const *data, uint32_t address, size_t count, int num_calls) {
  memcpy(&simulated_flash[address], data, count);
  return count;
}

void setUp(void) {
  flash_erase_StubWithCallback(mock_erase);
  flash_read_StubWithCallback(mock_read);
  flash_write_StubWithCallback(mock_write);
}

void tearDown(void) {}

// Add tests here.
void test_write_read(void) {
  uint8_t write_data = 0x88;
  uint8_t read_data = 0;
  int ret;
  uint32_t flash_address = 0;

  flash_write_ExpectAndReturn(&write_data, flash_address, 1, 1);
  flash_read_ExpectAndReturn(&read_data, flash_address, 1, 1);

  ret = flash_write(&write_data, flash_address, 1);
  TEST_ASSERT_EQUAL(1, ret);

  ret = flash_read(&read_data, flash_address, 1);
  TEST_ASSERT_EQUAL(1, ret);

  TEST_ASSERT_EQUAL(write_data, read_data);
}

void test_when_flash_is_erased_then_the_config_load_count_is_reset(void) {
  config_t config = {0};
  int ret;

  flash_erase_ExpectAndReturn(0);
  flash_read_ExpectAndReturn((uint8_t *)(&config), CONFIG_FLASH_ADDR, sizeof(config_t), sizeof(config_t));

  ret = flash_erase();
  TEST_ASSERT_EQUAL(0, ret);

  config_load(&config);

  TEST_ASSERT_EQUAL(0, config.load_count);
  TEST_ASSERT_EQUAL(false, config.valid);
}
