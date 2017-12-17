/*
 * Copyright (c) 2016, Fuzhou Rockchip Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#ifndef __PLAT_RK_VENDOR_STORAGE_H
#define __PLAT_RK_VENDOR_STORAGE_H

int rk_vendor_read(u32 id, void *pbuf, u32 size);
int rk_vendor_write(u32 id, void *pbuf, u32 size);
int rk_vendor_register(void *read, void *write);

#endif
