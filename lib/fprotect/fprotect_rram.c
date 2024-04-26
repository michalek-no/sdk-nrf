/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <soc.h>
#include <nrf_erratas.h>
#include <hal/nrf_rramc.h>

static void protect_boot_code(uint32_t size){

	nrf_rramc_region_config_t config={
		.address = 0,
		.permissions = NRF_RRAMC_REGION_PERM_READ_MASK | NRF_RRAMC_REGION_PERM_EXECUTE_MASK,
		.writeonce = true,
		.lock = true,
		.size_kb = size>>10,
	};

	nrf_rramc_region_config_set(NRF_RRAMC, 3, &config);
}
static void protect_boot_code2(uint32_t start, uint32_t size){
	nrf_rramc_region_config_t config={
		.address = start,
		.permissions = NRF_RRAMC_REGION_PERM_READ_MASK | NRF_RRAMC_REGION_PERM_EXECUTE_MASK,
		.writeonce = true,
		.lock = true,
		.size_kb = size>>10, 
	};

	nrf_rramc_region_config_set(NRF_RRAMC, 4, &config);
}

int fprotect_area(uint32_t start, size_t length)
{
	if (length <= 0x7c00) {
		protect_boot_code2(start, length);
		
	}else{
		if (length > 0xf800) {
			return -EINVAL;
		}
		if(start != 0){
			return -EINVAL;
		}
		protect_boot_code(0x7c00);
		protect_boot_code2(0x7c00, length - 0x7c00);
	}
	return 0;
}
