/*
 *
 * Copyright (c) Fuzhou Rockchip Electronics Co.Ltd
 * Authors:
 *       Mark Yao <yzq@rock-chips.com>
 *
 * base on exynos_drm.h
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifndef _UAPI_ROCKCHIP_DRM_H
#define _UAPI_ROCKCHIP_DRM_H

#include <drm/drm.h>

/**
 * User-desired buffer creation information structure.
 *
 * @size: user-desired memory allocation size.
 * @flags: user request for setting memory type or cache attributes.
 * @handle: returned a handle to created gem object.
 *     - this handle will be set by gem module of kernel side.
 */
struct drm_rockchip_gem_create {
	uint64_t size;
	uint32_t flags;
	uint32_t handle;
};

/**
 * A structure for getting buffer offset.
 *
 * @handle: a pointer to gem object created.
 * @pad: just padding to be 64-bit aligned.
 * @offset: relatived offset value of the memory region allocated.
 *     - this value should be set by user.
 */
struct drm_rockchip_gem_map_off {
	uint32_t handle;
	uint32_t pad;
	uint64_t offset;
};

/* acquire type definitions. */
enum drm_rockchip_gem_cpu_acquire_type {
	DRM_ROCKCHIP_GEM_CPU_ACQUIRE_SHARED = 0x0,
	DRM_ROCKCHIP_GEM_CPU_ACQUIRE_EXCLUSIVE = 0x1,
};

/**
 * A structure for acquiring buffer for CPU access.
 *
 * @handle: a handle to gem object created.
 * @flags: acquire flag
 */
struct drm_rockchip_gem_cpu_acquire {
	uint32_t handle;
	uint32_t flags;
};

/*
 * A structure for releasing buffer for GPU access.
 *
 * @handle: a handle to gem object created.
 */
struct drm_rockchip_gem_cpu_release {
	uint32_t handle;
};

struct drm_rockchip_rga_get_ver {
	__u32   major;
	__u32   minor;
};

struct drm_rockchip_rga_cmd {
	__u32   offset;
	__u32   data;
};

enum drm_rockchip_rga_buf_type {
	RGA_BUF_TYPE_USERPTR = 1 << 31,
	RGA_BUF_TYPE_GEMFD   = 1 << 30,
};

struct drm_rockchip_rga_set_cmdlist {
	__u64		cmd;
	__u64		cmd_buf;
	__u32		cmd_nr;
	__u32		cmd_buf_nr;
	__u64		user_data;
};

struct drm_rockchip_rga_exec {
	__u64		async;
};

enum rockchip_plane_feture {
	ROCKCHIP_DRM_PLANE_FEATURE_SCALE,
	ROCKCHIP_DRM_PLANE_FEATURE_ALPHA,
	ROCKCHIP_DRM_PLANE_FEATURE_MAX,
};

#define DRM_ROCKCHIP_GEM_CREATE		0x00
#define DRM_ROCKCHIP_GEM_MAP_OFFSET	0x01
#define DRM_ROCKCHIP_GEM_CPU_ACQUIRE	0x02
#define DRM_ROCKCHIP_GEM_CPU_RELEASE	0x03

#define DRM_ROCKCHIP_RGA_GET_VER	0x20
#define DRM_ROCKCHIP_RGA_SET_CMDLIST	0x21
#define DRM_ROCKCHIP_RGA_EXEC		0x22

#define DRM_IOCTL_ROCKCHIP_GEM_CREATE	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_GEM_CREATE, struct drm_rockchip_gem_create)

#define DRM_IOCTL_ROCKCHIP_GEM_MAP_OFFSET	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_GEM_MAP_OFFSET, struct drm_rockchip_gem_map_off)

#define DRM_IOCTL_ROCKCHIP_GEM_CPU_ACQUIRE	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_GEM_CPU_ACQUIRE, struct drm_rockchip_gem_cpu_acquire)

#define DRM_IOCTL_ROCKCHIP_GEM_CPU_RELEASE	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_GEM_CPU_RELEASE, struct drm_rockchip_gem_cpu_release)

#define DRM_IOCTL_ROCKCHIP_RGA_GET_VER		DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_RGA_GET_VER, struct drm_rockchip_rga_get_ver)

#define DRM_IOCTL_ROCKCHIP_RGA_SET_CMDLIST	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_RGA_SET_CMDLIST, struct drm_rockchip_rga_set_cmdlist)

#define DRM_IOCTL_ROCKCHIP_RGA_EXEC		DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_RGA_EXEC, struct drm_rockchip_rga_exec)

#endif /* _UAPI_ROCKCHIP_DRM_H */
