#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_SIZE 4096u
#define FLASH_PAGE_SIZE 256u

int flash_read(uint8_t* data, uint32_t address, size_t count);
int flash_write(uint8_t const* data, uint32_t address, size_t count);
int flash_erase(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FLASH_H */
