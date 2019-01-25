#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_FLASH_ADDR 0u

typedef struct {
  int load_count;
  bool valid;
} config_t;

void config_load(config_t *cfg);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CONFIG_H */
