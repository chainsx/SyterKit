/* SPDX-License-Identifier: Apache-2.0 */

#include <io.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <types.h>

#include <log.h>

uint64_t __attribute__((weak)) sunxi_get_dram_size() {
    printk_warning("DRAM: dram driver not impl\n");
    return 0;
}

uint64_t __attribute__((weak)) sunxi_dram_init(void *para) {
    printk_warning("DRAM: dram driver not impl\n");
    return 0;
}