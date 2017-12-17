/*
 * Copyright (C) Fuzhou Rockchip Electronics Co.Ltd
 * Author:Mark Yao <mark.yao@rock-chips.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _ROCKCHIP_DRM_VOP_H
#define _ROCKCHIP_DRM_VOP_H

/*
 * major: IP major vertion, used for IP structure
 * minor: big feature change under same structure
 */
#define VOP_VERSION(major, minor)	((major) << 8 | (minor))
#define VOP_MAJOR(version) 	((version) >> 8)
#define VOP_MINOR(version) 	((version) & 0xff)

enum vop_csc_format {
	CSC_BT601,
	CSC_BT709,
	CSC_BT2020,
};

enum vop_csc_mode {
	CSC_RGB,
	CSC_YUV,
};

enum vop_data_format {
	VOP_FMT_ARGB8888 = 0,
	VOP_FMT_RGB888,
	VOP_FMT_RGB565,
	VOP_FMT_YUV420SP = 4,
	VOP_FMT_YUV422SP,
	VOP_FMT_YUV444SP,
};

struct vop_reg_data {
	uint32_t offset;
	uint32_t value;
};

struct vop_reg {
	uint32_t mask;
	uint32_t offset:12;
	uint32_t shift:5;
	uint32_t begin_minor:4;
	uint32_t end_minor:4;
	uint32_t major:3;
	uint32_t write_mask:1;
};

struct vop_csc {
	struct vop_reg y2r_en;
	struct vop_reg r2r_en;
	struct vop_reg r2y_en;

	uint32_t y2r_offset;
	uint32_t r2r_offset;
	uint32_t r2y_offset;
};

struct vop_ctrl {
	struct vop_reg standby;
	struct vop_reg htotal_pw;
	struct vop_reg hact_st_end;
	struct vop_reg vtotal_pw;
	struct vop_reg vact_st_end;
	struct vop_reg vact_st_end_f1;
	struct vop_reg vs_st_end_f1;
	struct vop_reg hpost_st_end;
	struct vop_reg vpost_st_end;
	struct vop_reg vpost_st_end_f1;
	struct vop_reg dsp_interlace;
	struct vop_reg global_regdone_en;
	struct vop_reg auto_gate_en;
	struct vop_reg post_lb_mode;
	struct vop_reg dsp_layer_sel;
	struct vop_reg overlay_mode;
	struct vop_reg core_dclk_div;
	struct vop_reg p2i_en;
	struct vop_reg rgb_en;
	struct vop_reg edp_en;
	struct vop_reg hdmi_en;
	struct vop_reg mipi_en;
	struct vop_reg pin_pol;
	struct vop_reg rgb_pin_pol;
	struct vop_reg hdmi_pin_pol;
	struct vop_reg edp_pin_pol;
	struct vop_reg mipi_pin_pol;

	struct vop_reg dither_up;
	struct vop_reg dither_down;

	struct vop_reg dsp_data_swap;
	struct vop_reg dsp_ccir656_avg;
	struct vop_reg dsp_black;
	struct vop_reg dsp_blank;
	struct vop_reg dsp_outzero;
	struct vop_reg dsp_lut_en;

	struct vop_reg out_mode;

	struct vop_reg xmirror;
	struct vop_reg ymirror;
	struct vop_reg dsp_background;

	struct vop_reg cfg_done;
};

struct vop_intr {
	const int *intrs;
	uint32_t nintrs;
	struct vop_reg line_flag_num;
	struct vop_reg enable;
	struct vop_reg clear;
	struct vop_reg status;
};

struct vop_scl_extension {
	struct vop_reg cbcr_vsd_mode;
	struct vop_reg cbcr_vsu_mode;
	struct vop_reg cbcr_hsd_mode;
	struct vop_reg cbcr_ver_scl_mode;
	struct vop_reg cbcr_hor_scl_mode;
	struct vop_reg yrgb_vsd_mode;
	struct vop_reg yrgb_vsu_mode;
	struct vop_reg yrgb_hsd_mode;
	struct vop_reg yrgb_ver_scl_mode;
	struct vop_reg yrgb_hor_scl_mode;
	struct vop_reg line_load_mode;
	struct vop_reg cbcr_axi_gather_num;
	struct vop_reg yrgb_axi_gather_num;
	struct vop_reg vsd_cbcr_gt2;
	struct vop_reg vsd_cbcr_gt4;
	struct vop_reg vsd_yrgb_gt2;
	struct vop_reg vsd_yrgb_gt4;
	struct vop_reg bic_coe_sel;
	struct vop_reg cbcr_axi_gather_en;
	struct vop_reg yrgb_axi_gather_en;
	struct vop_reg lb_mode;
};

struct vop_scl_regs {
	const struct vop_scl_extension *ext;

	struct vop_reg scale_yrgb_x;
	struct vop_reg scale_yrgb_y;
	struct vop_reg scale_cbcr_x;
	struct vop_reg scale_cbcr_y;
};

struct vop_csc_table {
	const uint32_t *y2r_bt601;
	const uint32_t *y2r_bt601_12_235;
	const uint32_t *y2r_bt601_10bit;
	const uint32_t *y2r_bt601_10bit_12_235;
	const uint32_t *r2y_bt601;
	const uint32_t *r2y_bt601_12_235;
	const uint32_t *r2y_bt601_10bit;
	const uint32_t *r2y_bt601_10bit_12_235;

	const uint32_t *y2r_bt709;
	const uint32_t *y2r_bt709_10bit;
	const uint32_t *r2y_bt709;
	const uint32_t *r2y_bt709_10bit;

	const uint32_t *y2r_bt2020;
	const uint32_t *r2y_bt2020;

	const uint32_t *r2r_bt709_to_bt2020;
	const uint32_t *r2r_bt2020_to_bt709;
};

enum {
	VOP_CSC_Y2R_BT601,
	VOP_CSC_Y2R_BT709,
	VOP_CSC_Y2R_BT2020,
	VOP_CSC_R2Y_BT601,
	VOP_CSC_R2Y_BT709,
	VOP_CSC_R2Y_BT2020,
	VOP_CSC_R2R_BT2020_TO_BT709,
	VOP_CSC_R2R_BT709_TO_2020,
};

struct vop_win_phy {
	const struct vop_scl_regs *scl;
	const uint32_t *data_formats;
	uint32_t nformats;

	struct vop_reg gate;
	struct vop_reg enable;
	struct vop_reg format;
	struct vop_reg xmirror;
	struct vop_reg ymirror;
	struct vop_reg rb_swap;
	struct vop_reg act_info;
	struct vop_reg dsp_info;
	struct vop_reg dsp_st;
	struct vop_reg yrgb_mst;
	struct vop_reg uv_mst;
	struct vop_reg yrgb_vir;
	struct vop_reg uv_vir;

	struct vop_reg dst_alpha_ctl;
	struct vop_reg src_alpha_ctl;
	struct vop_reg alpha_mode;
	struct vop_reg alpha_en;
	struct vop_reg key_color;
	struct vop_reg key_en;
};

struct vop_win_data {
	uint32_t base;
	enum drm_plane_type type;
	const struct vop_win_phy *phy;
	const struct vop_win_phy **area;
	const struct vop_csc *csc;
	unsigned int area_size;
};

#define VOP_FEATURE_OUTPUT_10BIT BIT(0)

struct vop_data {
	const struct vop_reg_data *init_table;
	unsigned int table_size;
	const struct vop_ctrl *ctrl;
	const struct vop_intr *intr;
	const struct vop_win_data *win;
	const struct vop_csc_table *csc_table;
	unsigned int win_size;
	uint32_t version;
	u64 feature;
};

/* interrupt define */
#define DSP_HOLD_VALID_INTR		(1 << 0)
#define FS_INTR				(1 << 1)
#define LINE_FLAG_INTR			(1 << 2)
#define BUS_ERROR_INTR			(1 << 3)
#define FS_NEW_INTR			(1 << 4)
#define ADDR_SAME_INTR 			(1 << 5)
#define LINE_FLAG1_INTR			(1 << 6)
#define WIN0_EMPTY_INTR			(1 << 7)
#define WIN1_EMPTY_INTR			(1 << 8)
#define WIN2_EMPTY_INTR			(1 << 9)
#define WIN3_EMPTY_INTR			(1 << 10)
#define HWC_EMPTY_INTR			(1 << 11)
#define POST_BUF_EMPTY_INTR		(1 << 12)
#define PWM_GEN_INTR			(1 << 13)

#define INTR_MASK			(DSP_HOLD_VALID_INTR | FS_INTR | \
					 LINE_FLAG_INTR | BUS_ERROR_INTR | \
					 FS_NEW_INTR | LINE_FLAG1_INTR | \
					 WIN0_EMPTY_INTR | WIN1_EMPTY_INTR | \
					 WIN2_EMPTY_INTR | WIN3_EMPTY_INTR | \
					 HWC_EMPTY_INTR | POST_BUF_EMPTY_INTR)

#define DSP_HOLD_VALID_INTR_EN(x)	((x) << 4)
#define FS_INTR_EN(x)			((x) << 5)
#define LINE_FLAG_INTR_EN(x)		((x) << 6)
#define BUS_ERROR_INTR_EN(x)		((x) << 7)
#define DSP_HOLD_VALID_INTR_MASK	(1 << 4)
#define FS_INTR_MASK			(1 << 5)
#define LINE_FLAG_INTR_MASK		(1 << 6)
#define BUS_ERROR_INTR_MASK		(1 << 7)

#define INTR_CLR_SHIFT			8
#define DSP_HOLD_VALID_INTR_CLR		(1 << (INTR_CLR_SHIFT + 0))
#define FS_INTR_CLR			(1 << (INTR_CLR_SHIFT + 1))
#define LINE_FLAG_INTR_CLR		(1 << (INTR_CLR_SHIFT + 2))
#define BUS_ERROR_INTR_CLR		(1 << (INTR_CLR_SHIFT + 3))

#define DSP_LINE_NUM(x)			(((x) & 0x1fff) << 12)
#define DSP_LINE_NUM_MASK		(0x1fff << 12)

/* src alpha ctrl define */
#define SRC_FADING_VALUE(x)		(((x) & 0xff) << 24)
#define SRC_GLOBAL_ALPHA(x)		(((x) & 0xff) << 16)
#define SRC_FACTOR_M0(x)		(((x) & 0x7) << 6)
#define SRC_ALPHA_CAL_M0(x)		(((x) & 0x1) << 5)
#define SRC_BLEND_M0(x)			(((x) & 0x3) << 3)
#define SRC_ALPHA_M0(x)			(((x) & 0x1) << 2)
#define SRC_COLOR_M0(x)			(((x) & 0x1) << 1)
#define SRC_ALPHA_EN(x)			(((x) & 0x1) << 0)
/* dst alpha ctrl define */
#define DST_FACTOR_M0(x)		(((x) & 0x7) << 6)

/*
 * display output interface supported by rockchip lcdc
 */
#define ROCKCHIP_OUT_MODE_P888	0
#define ROCKCHIP_OUT_MODE_P666	1
#define ROCKCHIP_OUT_MODE_P565	2
/* for use special outface */
#define ROCKCHIP_OUT_MODE_AAAA	15

#define ROCKCHIP_OUT_MODE_TYPE(x)	((x) >> 16)
#define ROCKCHIP_OUT_MODE(x)		((x) & 0xffff)
#define ROCKCHIP_DSP_MODE(type, mode) \
		(DRM_MODE_CONNECTOR_##type << 16) | \
		(ROCKCHIP_OUT_MODE_##mode & 0xffff)

enum alpha_mode {
	ALPHA_STRAIGHT,
	ALPHA_INVERSE,
};

enum global_blend_mode {
	ALPHA_GLOBAL,
	ALPHA_PER_PIX,
	ALPHA_PER_PIX_GLOBAL,
};

enum alpha_cal_mode {
	ALPHA_SATURATION,
	ALPHA_NO_SATURATION,
};

enum color_mode {
	ALPHA_SRC_PRE_MUL,
	ALPHA_SRC_NO_PRE_MUL,
};

enum factor_mode {
	ALPHA_ZERO,
	ALPHA_ONE,
	ALPHA_SRC,
	ALPHA_SRC_INVERSE,
	ALPHA_SRC_GLOBAL,
};

enum scale_mode {
	SCALE_NONE = 0x0,
	SCALE_UP   = 0x1,
	SCALE_DOWN = 0x2
};

enum lb_mode {
	LB_YUV_3840X5 = 0x0,
	LB_YUV_2560X8 = 0x1,
	LB_RGB_3840X2 = 0x2,
	LB_RGB_2560X4 = 0x3,
	LB_RGB_1920X5 = 0x4,
	LB_RGB_1280X8 = 0x5
};

enum sacle_up_mode {
	SCALE_UP_BIL = 0x0,
	SCALE_UP_BIC = 0x1
};

enum scale_down_mode {
	SCALE_DOWN_BIL = 0x0,
	SCALE_DOWN_AVG = 0x1
};

#define FRAC_16_16(mult, div)    (((mult) << 16) / (div))
#define SCL_FT_DEFAULT_FIXPOINT_SHIFT	12
#define SCL_MAX_VSKIPLINES		4
#define MIN_SCL_FT_AFTER_VSKIP		1

static inline uint16_t scl_cal_scale(int src, int dst, int shift)
{
	return ((src * 2 - 3) << (shift - 1)) / (dst - 1);
}

static inline uint16_t scl_cal_scale2(int src, int dst)
{
	return ((src - 1) << 12) / (dst - 1);
}

#define GET_SCL_FT_BILI_DN(src, dst)	scl_cal_scale(src, dst, 12)
#define GET_SCL_FT_BILI_UP(src, dst)	scl_cal_scale(src, dst, 16)
#define GET_SCL_FT_BIC(src, dst)	scl_cal_scale(src, dst, 16)

static inline uint16_t scl_get_bili_dn_vskip(int src_h, int dst_h,
					     int vskiplines)
{
	int act_height;

	act_height = (src_h + vskiplines - 1) / vskiplines;

	return GET_SCL_FT_BILI_DN(act_height, dst_h);
}

static inline enum scale_mode scl_get_scl_mode(int src, int dst)
{
	if (src < dst)
		return SCALE_UP;
	else if (src > dst)
		return SCALE_DOWN;

	return SCALE_NONE;
}

static inline int scl_get_vskiplines(uint32_t srch, uint32_t dsth)
{
	uint32_t vskiplines;

	for (vskiplines = SCL_MAX_VSKIPLINES; vskiplines > 1; vskiplines /= 2)
		if (srch >= vskiplines * dsth * MIN_SCL_FT_AFTER_VSKIP)
			break;

	return vskiplines;
}

static inline int scl_vop_cal_lb_mode(int width, bool is_yuv)
{
	int lb_mode;

	if (width > 2560)
		lb_mode = LB_RGB_3840X2;
	else if (width > 1920)
		lb_mode = LB_RGB_2560X4;
	else if (!is_yuv)
		lb_mode = LB_RGB_1920X5;
	else if (width > 1280)
		lb_mode = LB_YUV_3840X5;
	else
		lb_mode = LB_YUV_2560X8;

	return lb_mode;
}

extern const struct component_ops vop_component_ops;
#endif /* _ROCKCHIP_DRM_VOP_H */
