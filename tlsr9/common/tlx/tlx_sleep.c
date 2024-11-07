/******************************************************************************
 * Copyright (c) 2024 Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *****************************************************************************/
#include "tlx_sleep.h"
#include <ext_driver/ext_pm.h>

#if CONFIG_BT_TLX
#if CONFIG_SOC_RISCV_TELINK_TL721X
#include <stack/ble/B95/controller/os_sup.h>
#elif CONFIG_SOC_RISCV_TELINK_TL321X
#include <stack/ble/TL321X/controller/os_sup.h>
#endif
#include <tlx_bt.h>
#include <tlx_rf_power.h>
#endif /*  CONFIG_BT_TLX */

/**
 * @brief     This function sets TLX MCU to suspend mode
 * @param[in] wake_stimer_tick - wake-up stimer tick
 * @return    true if suspend mode entered otherwise false
 */
bool tlx_suspend(uint32_t wake_stimer_tick)
{
	bool result = false;

#if CONFIG_BT_TLX
	enum tlx_bt_controller_state state = tlx_bt_controller_state();

	if (state == TLX_BT_CONTROLLER_STATE_ACTIVE ||
		state == TLX_BT_CONTROLLER_STATE_STOPPING) {
		blc_pm_setAppWakeupLowPower(wake_stimer_tick, 1);
		if (!blc_pm_handler()) {
			rf_set_power_level(tlx_tx_pwr_lt[CONFIG_TLX_BLE_CTRL_RF_POWER
			- TLX_TX_POWER_MIN]);
			result = true;
		}
		blc_pm_setAppWakeupLowPower(0, 0);
	} else {
		if (cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
			wake_stimer_tick) != STATUS_GPIO_ERR_NO_ENTER_PM) {
			result = true;
		}
	}
#else
	if (cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
		wake_stimer_tick) != STATUS_GPIO_ERR_NO_ENTER_PM) {
		result = true;
	}
#endif /* CONFIG_BT_TLX */

	return result;
}

#if CONFIG_SOC_SERIES_RISCV_TELINK_TLX_RETENTION

#if CONFIG_SOC_RISCV_TELINK_TL721X
#define DEEPSLEEP_MODE_RET_SRAM DEEPSLEEP_MODE_RET_SRAM_LOW96K
#elif CONFIG_SOC_RISCV_TELINK_TL321X
#define DEEPSLEEP_MODE_RET_SRAM DEEPSLEEP_MODE_RET_SRAM_LOW96K
#endif

bool tlx_deep_sleep(uint32_t wake_stimer_tick)
{
	bool result = false;
	static volatile bool tl_sleep_retention
	__attribute__ ((section (".retention_data"))) = false;

	extern void tl_context_save(void);
	extern void soc_tlx_restore(void);

#if CONFIG_BT_TLX
	enum tlx_bt_controller_state state = tlx_bt_controller_state();

	if (state == TLX_BT_CONTROLLER_STATE_ACTIVE ||
		state == TLX_BT_CONTROLLER_STATE_STOPPING) {
		blc_pm_setAppWakeupLowPower(wake_stimer_tick, 1);
		if (!blc_pm_handler()) {
			rf_set_power_level(tlx_tx_pwr_lt[CONFIG_TLX_BLE_CTRL_RF_POWER
			- TLX_TX_POWER_MIN]);
			result = true;
		}
		blc_pm_setAppWakeupLowPower(0, 0);
	} else {
		tl_context_save();
		if (!tl_sleep_retention) {
			tl_sleep_retention = true;
			(void)cpu_sleep_wakeup_32k_rc(DEEPSLEEP_MODE_RET_SRAM,
				PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
				wake_stimer_tick);
			tl_sleep_retention = false;
		} else {
			soc_tlx_restore();
			tl_sleep_retention = false;
			result = true;
		}
	}
#else
	tl_context_save();
	if (!tl_sleep_retention) {
		tl_sleep_retention = true;
		(void)cpu_sleep_wakeup_32k_rc(DEEPSLEEP_MODE_RET_SRAM,
			PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
			wake_stimer_tick);
		tl_sleep_retention = false;
	} else {
		soc_tlx_restore();
		tl_sleep_retention = false;
		result = true;
	}
#endif /* || CONFIG_BT_TLX */

	return result;
}

#endif /* CONFIG_SOC_SERIES_RISCV_TELINK_TLX_RETENTION */
