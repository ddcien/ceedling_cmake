#include "config.h"

#include "flash.h"
#include <string.h>
#include <stdbool.h>


void config_load(config_t *cfg) {
    int ret = flash_read((uint8_t*)cfg, CONFIG_FLASH_ADDR, sizeof(config_t));
    if(ret != sizeof(config_t) || cfg->load_count == 0xffffffff) {
        memset(cfg, 0, sizeof(config_t));
        cfg->load_count = 0;
        cfg->valid = false;
        return;
    }
    cfg->valid = true;
    return;
}
