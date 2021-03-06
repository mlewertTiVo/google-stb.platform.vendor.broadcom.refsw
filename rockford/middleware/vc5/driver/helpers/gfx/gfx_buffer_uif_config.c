/*=============================================================================
Copyright (c) 2014 Broadcom Europe Limited.
All rights reserved.

Project  :  helpers
Module   :

FILE DESCRIPTION
=============================================================================*/

#include "gfx_buffer_uif_config.h"

#ifdef GFX_UIF_SYSTEM_CONFIG_AT_RUNTIME

/* I'm not keen on adding static data to gfx_buffer, but I'm also not keen on
 * making functions like desc_gen() more cumbersome for a corner-case
 * feature */

static struct {
   uint32_t page_size;
   uint32_t num_banks;
   uint32_t xor_addr;

   uint32_t dram_map_mode;
} uif_system_config = {
   GFX_DEFAULT_UIF_PAGE_SIZE,
   GFX_DEFAULT_UIF_NUM_BANKS,
   GFX_DEFAULT_UIF_XOR_ADDR,

   GFX_DEFAULT_DRAM_MAP_MODE};

uint32_t gfx_buffer_get_uif_page_size(void) { return uif_system_config.page_size; }
void gfx_buffer_set_uif_page_size(uint32_t page_size) { uif_system_config.page_size = page_size; }
uint32_t gfx_buffer_get_uif_num_banks(void) { return uif_system_config.num_banks; }
void gfx_buffer_set_uif_num_banks(uint32_t num_banks) { uif_system_config.num_banks = num_banks; }
uint32_t gfx_buffer_get_uif_xor_addr(void) { return uif_system_config.xor_addr; }
void gfx_buffer_set_uif_xor_addr(uint32_t xor_addr) { uif_system_config.xor_addr = xor_addr; }

uint32_t gfx_buffer_get_dram_map_mode(void) { return uif_system_config.dram_map_mode; }
void gfx_buffer_set_dram_map_mode(uint32_t map_mode) { uif_system_config.dram_map_mode = map_mode; }

#endif
