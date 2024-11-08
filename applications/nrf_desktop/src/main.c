/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <app_event_manager.h>

#define MODULE main
#include <caf/events/module_state_event.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(MODULE);

#include <zephyr/storage/flash_map.h>

int main(void)
{
	int err;
	const struct flash_area *fa = NULL;

	LOG_ERR("");
	err = flash_area_open(PM_MCUBOOT_ID, &fa);
	if (!err) {
		err = flash_area_erase(fa, 0x1000, 0x1000);
	}

	if (fa) {
		flash_area_close(fa);
		return 0;
	}

	if (err) {
		LOG_ERR("Erase operation failed for the MCUboot partition");
		return 0;
	}

	if (app_event_manager_init()) {
		LOG_ERR("Application Event Manager not initialized");
	} else {
		module_set_state(MODULE_STATE_READY);
	}
	return 0;
}
