/* SPDX-License-Identifier: Apache-2.0 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <types.h>

#include <log.h>
#include <mmu.h>

#include <common.h>

#include <pmu/axp.h>
#include <sys-dram.h>
#include <sys-i2c.h>

extern sunxi_serial_t uart_dbg;

extern sunxi_i2c_t i2c_pmu;

extern void set_rpio_power_mode(void);
extern void rtc_set_vccio_det_spare(void);

static void set_axp323_pmu_fin_voltage(char *power_name, uint32_t voltage) {
    int set_vol = voltage;
    int temp_vol, src_vol = pmu_axp1530_get_vol(&i2c_pmu, power_name);
    if (src_vol > voltage) {
        for (temp_vol = src_vol; temp_vol >= voltage; temp_vol -= 50) {
            pmu_axp1530_set_vol(&i2c_pmu, power_name, temp_vol, 1);
        }
    } else if (src_vol < voltage) {
        for (temp_vol = src_vol; temp_vol <= voltage; temp_vol += 50) {
            pmu_axp1530_set_vol(&i2c_pmu, power_name, temp_vol, 1);
        }
    }
    mdelay(30); /* Delay 300ms for pmu bootup */
}

static void set_axp717_pmu_fin_voltage(char *power_name, uint32_t voltage) {
    int set_vol = voltage;
    int temp_vol, src_vol = pmu_axp2202_get_vol(&i2c_pmu, power_name);
    if (src_vol > voltage) {
        for (temp_vol = src_vol; temp_vol >= voltage; temp_vol -= 50) {
            pmu_axp2202_set_vol(&i2c_pmu, power_name, temp_vol, 1);
        }
    } else if (src_vol < voltage) {
        for (temp_vol = src_vol; temp_vol <= voltage; temp_vol += 50) {
            pmu_axp2202_set_vol(&i2c_pmu, power_name, temp_vol, 1);
        }
    }
    mdelay(30); /* Delay 300ms for pmu bootup */
}

void arm32_do_prefetch_abort(struct arm_regs_t *regs) {
    main();
}

static int boot_dram_init_status = 0;

int main(void) {
    if (boot_dram_init_status == 0) {
        sunxi_serial_init(&uart_dbg);

        show_banner();

        sunxi_clk_init();

        sunxi_clk_dump();

        rtc_set_vccio_det_spare();

        set_rpio_power_mode();

        sunxi_i2c_init(&i2c_pmu);

        pmu_axp1530_init(&i2c_pmu);

        pmu_axp2202_init(&i2c_pmu);

        mdelay(30); /* Delay 300ms for pmu bootup */

        pmu_axp1530_dump(&i2c_pmu);

        pmu_axp2202_dump(&i2c_pmu);

        enable_sram_a3();

        boot_dram_init_status = 1;

        printk_info("DRAM: DRAM Size = %dMB\n", sunxi_dram_init(NULL));
    }

    sunxi_clk_dump();

    int i = 0;

    while (1) {
        i++;
        printk_info("Count: %d\n", i);
        mdelay(1000);
    }

    abort();

    return 0;
}