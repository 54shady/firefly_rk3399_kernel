/*
 * Header providing constants for Rockchip pinctrl bindings.
 *
 * Copyright (c) 2013 MundoReader S.L.
 * Author: Heiko Stuebner <heiko@sntech.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __DT_BINDINGS_ROCKCHIP_PINCTRL_H__
#define __DT_BINDINGS_ROCKCHIP_PINCTRL_H__

#define RK_GPIO0	0
#define RK_GPIO1	1
#define RK_GPIO2	2
#define RK_GPIO3	3
#define RK_GPIO4	4
#define RK_GPIO5	5
#define RK_GPIO6	6
#define RK_GPIO7	7
#define RK_GPIO8	8


#define RK_FUNC_GPIO	0
#define RK_FUNC_1	1
#define RK_FUNC_2	2
#define RK_FUNC_3	3
#define RK_FUNC_4	4
#define RK_FUNC_5	5
#define RK_FUNC_6	6
#define RK_FUNC_7	7



/*special virtual pin for vcc domain setting*/
#define VIRTUAL_PIN_FOR_AP0_VCC		0xfA00
#define VIRTUAL_PIN_FOR_AP1_VCC		0xfA10
#define VIRTUAL_PIN_FOR_CIF_VCC		0xfA20
#define VIRTUAL_PIN_FOR_FLASH_VCC	0xfA30
#define VIRTUAL_PIN_FOR_VCCIO0_VCC	0xfA40
#define VIRTUAL_PIN_FOR_VCCIO1_VCC	0xfA50
#define VIRTUAL_PIN_FOR_LCDC0_VCC	0xfA60
#define VIRTUAL_PIN_FOR_LCDC1_VCC	0xfA70


#define RK32_VIRTUAL_PIN_FOR_LCDC_VCC		0xfA00
#define RK32_VIRTUAL_PIN_FOR_DVP_VCC		0xfA10
#define RK32_VIRTUAL_PIN_FOR_FLASH0_VCC		0xfA20
#define RK32_VIRTUAL_PIN_FOR_FLASH1_VCC		0xfA30
#define RK32_VIRTUAL_PIN_FOR_WIFI_VCC		0xfA40
#define RK32_VIRTUAL_PIN_FOR_BB_VCC		0xfA50
#define RK32_VIRTUAL_PIN_FOR_AUDIO_VCC		0xfA60
#define RK32_VIRTUAL_PIN_FOR_SDCARD_VCC		0xfA70

#define RK32_VIRTUAL_PIN_FOR_GPIO30_VCC		0xfB00
#define RK32_VIRTUAL_PIN_FOR_GPIO1830_VCC	0xfB10


#define TYPE_PULL_REG		0x01
#define TYPE_VOL_REG		0x02
#define TYPE_DRV_REG		0x03
#define TYPE_TRI_REG		0x04

#define RK2928_PULL_OFFSET		0x118
#define RK2928_PULL_PINS_PER_REG	16
#define RK2928_PULL_BANK_STRIDE		8

#define RK3188_PULL_BITS_PER_PIN	2
#define RK3188_PULL_PINS_PER_REG	8
#define RK3188_PULL_BANK_STRIDE		16

#define RK3036_PULL_BITS_PER_PIN	1
#define RK3036_PULL_PINS_PER_REG	16
#define RK3036_PULL_BANK_STRIDE		8

#define RK312X_PULL_BITS_PER_PIN	1
#define RK312X_PULL_PINS_PER_REG	16
#define RK312X_PULL_BANK_STRIDE		8


/*warning:don not chang the following value*/
#define VALUE_PULL_NORMAL	0
#define VALUE_PULL_UP		1
#define VALUE_PULL_DOWN		2
#define VALUE_PULL_KEEP		3
#define VALUE_PULL_DISABLE	4 //don't set and keep pull default
#define VALUE_PULL_DEFAULT	4 //don't set and keep pull default


//for rk2928,rk3036
#define VALUE_PULL_UPDOWN_DISABLE		0
#define VALUE_PULL_UPDOWN_ENABLE		1

#define VALUE_VOL_DEFAULT	0
#define VALUE_VOL_3V3		0
#define VALUE_VOL_1V8		1

#define VALUE_DRV_DEFAULT	0
#define VALUE_DRV_2MA		0
#define VALUE_DRV_4MA		1
#define VALUE_DRV_8MA		2
#define VALUE_DRV_12MA		3

#define VALUE_TRI_DEFAULT	0
#define VALUE_TRI_FALSE		0
#define VALUE_TRI_TRUE		1


/*
 * pin config bit field definitions
 *
 * pull-up:	1..0	(2)
 * voltage:	3..2	(2)
 * drive:		5..4	(2)
 * trisiate:	7..6	(2)
 *
 * MSB of each field is presence bit for the config.
 */
#define PULL_SHIFT		0
#define PULL_PRESENT		(1 << 2)
#define VOL_SHIFT		3
#define VOL_PRESENT		(1 << 5)
#define DRV_SHIFT		6
#define DRV_PRESENT		(1 << 8)
#define TRI_SHIFT		9
#define TRI_PRESENT		(1 << 11)

#define CONFIG_TO_PULL(c)	((c) >> PULL_SHIFT & 0x3)
#define CONFIG_TO_VOL(c)	((c) >> VOL_SHIFT & 0x3)
#define CONFIG_TO_DRV(c)	((c) >> DRV_SHIFT & 0x3)
#define CONFIG_TO_TRI(c)	((c) >> TRI_SHIFT & 0x3)


#define MAX_NUM_CONFIGS 	4
#define POS_PULL		0
#define POS_VOL			1
#define POS_DRV			2
#define POS_TRI			3


#define	GPIO_A0			0
#define	GPIO_A1			1
#define	GPIO_A2			2
#define	GPIO_A3			3
#define	GPIO_A4			4
#define	GPIO_A5			5
#define	GPIO_A6			6
#define	GPIO_A7			7
#define	GPIO_B0			8
#define	GPIO_B1			9
#define	GPIO_B2			10
#define	GPIO_B3			11
#define	GPIO_B4			12
#define	GPIO_B5			13
#define	GPIO_B6			14
#define	GPIO_B7			15
#define	GPIO_C0			16
#define	GPIO_C1			17
#define	GPIO_C2			18
#define	GPIO_C3			19
#define	GPIO_C4			20
#define	GPIO_C5			21
#define	GPIO_C6			22
#define	GPIO_C7			23
#define	GPIO_D0			24
#define	GPIO_D1			25
#define	GPIO_D2			26
#define	GPIO_D3			27
#define	GPIO_D4			28
#define	GPIO_D5			29
#define	GPIO_D6			30
#define	GPIO_D7			31

#define	A0			0
#define	A1			1
#define	A2			2
#define	A3			3
#define	A4			4
#define	A5			5
#define	A6			6
#define	A7			7
#define	B0			8
#define	B1			9
#define	B2			10
#define	B3			11
#define	B4			12
#define	B5			13
#define	B6			14
#define	B7			15
#define	C0			16
#define	C1			17
#define	C2			18
#define	C3			19
#define	C4			20
#define	C5			21
#define	C6			22
#define	C7			23
#define	D0			24
#define	D1			25
#define	D2			26
#define	D3			27
#define	D4			28
#define	D5			29
#define	D6			30
#define	D7			31

/* GPIO BANK 0 */
#define	GPIO0_A0	0	0
#define	GPIO0_A1	0	1
#define	GPIO0_A2	0	2
#define	GPIO0_A3	0	3
#define	GPIO0_A4	0	4
#define	GPIO0_A5	0	5
#define	GPIO0_A6	0	6
#define	GPIO0_A7	0	7
#define	GPIO0_B0	0	8
#define	GPIO0_B1	0	9
#define	GPIO0_B2	0	10
#define	GPIO0_B3	0	11
#define	GPIO0_B4	0	12
#define	GPIO0_B5	0	13
#define	GPIO0_B6	0	14
#define	GPIO0_B7	0	15
#define	GPIO0_C0	0	16
#define	GPIO0_C1	0	17
#define	GPIO0_C2	0	18
#define	GPIO0_C3	0	19
#define	GPIO0_C4	0	20
#define	GPIO0_C5	0	21
#define	GPIO0_C6	0	22
#define	GPIO0_C7	0	23
#define	GPIO0_D0	0	24
#define	GPIO0_D1	0	25
#define	GPIO0_D2	0	26
#define	GPIO0_D3	0	27
#define	GPIO0_D4	0	28
#define	GPIO0_D5	0	29
#define	GPIO0_D6	0	30
#define	GPIO0_D7	0	31

/* GPIO BANK 1 */
#define	GPIO1_A0	1	0
#define	GPIO1_A1	1	1
#define	GPIO1_A2	1	2
#define	GPIO1_A3	1	3
#define	GPIO1_A4	1	4
#define	GPIO1_A5	1	5
#define	GPIO1_A6	1	6
#define	GPIO1_A7	1	7
#define	GPIO1_B0	1	8
#define	GPIO1_B1	1	9
#define	GPIO1_B2	1	10
#define	GPIO1_B3	1	11
#define	GPIO1_B4	1	12
#define	GPIO1_B5	1	13
#define	GPIO1_B6	1	14
#define	GPIO1_B7	1	15
#define	GPIO1_C0	1	16
#define	GPIO1_C1	1	17
#define	GPIO1_C2	1	18
#define	GPIO1_C3	1	19
#define	GPIO1_C4	1	20
#define	GPIO1_C5	1	21
#define	GPIO1_C6	1	22
#define	GPIO1_C7	1	23
#define	GPIO1_D0	1	24
#define	GPIO1_D1	1	25
#define	GPIO1_D2	1	26
#define	GPIO1_D3	1	27
#define	GPIO1_D4	1	28
#define	GPIO1_D5	1	29
#define	GPIO1_D6	1	30
#define	GPIO1_D7	1	31

/* GPIO BANK 2 */
#define	GPIO2_A0	2	0
#define	GPIO2_A1	2	1
#define	GPIO2_A2	2	2
#define	GPIO2_A3	2	3
#define	GPIO2_A4	2	4
#define	GPIO2_A5	2	5
#define	GPIO2_A6	2	6
#define	GPIO2_A7	2	7
#define	GPIO2_B0	2	8
#define	GPIO2_B1	2	9
#define	GPIO2_B2	2	10
#define	GPIO2_B3	2	11
#define	GPIO2_B4	2	12
#define	GPIO2_B5	2	13
#define	GPIO2_B6	2	14
#define	GPIO2_B7	2	15
#define	GPIO2_C0	2	16
#define	GPIO2_C1	2	17
#define	GPIO2_C2	2	18
#define	GPIO2_C3	2	19
#define	GPIO2_C4	2	20
#define	GPIO2_C5	2	21
#define	GPIO2_C6	2	22
#define	GPIO2_C7	2	23
#define	GPIO2_D0	2	24
#define	GPIO2_D1	2	25
#define	GPIO2_D2	2	26
#define	GPIO2_D3	2	27
#define	GPIO2_D4	2	28
#define	GPIO2_D5	2	29
#define	GPIO2_D6	2	30
#define	GPIO2_D7	2	31

/* GPIO BANK 3 */
#define	GPIO3_A0	3	0
#define	GPIO3_A1	3	1
#define	GPIO3_A2	3	2
#define	GPIO3_A3	3	3
#define	GPIO3_A4	3	4
#define	GPIO3_A5	3	5
#define	GPIO3_A6	3	6
#define	GPIO3_A7	3	7
#define	GPIO3_B0	3	8
#define	GPIO3_B1	3	9
#define	GPIO3_B2	3	10
#define	GPIO3_B3	3	11
#define	GPIO3_B4	3	12
#define	GPIO3_B5	3	13
#define	GPIO3_B6	3	14
#define	GPIO3_B7	3	15
#define	GPIO3_C0	3	16
#define	GPIO3_C1	3	17
#define	GPIO3_C2	3	18
#define	GPIO3_C3	3	19
#define	GPIO3_C4	3	20
#define	GPIO3_C5	3	21
#define	GPIO3_C6	3	22
#define	GPIO3_C7	3	23
#define	GPIO3_D0	3	24
#define	GPIO3_D1	3	25
#define	GPIO3_D2	3	26
#define	GPIO3_D3	3	27
#define	GPIO3_D4	3	28
#define	GPIO3_D5	3	29
#define	GPIO3_D6	3	30
#define	GPIO3_D7	3	31

/* GPIO BANK 4 */
#define	GPIO4_A0	4	0
#define	GPIO4_A1	4	1
#define	GPIO4_A2	4	2
#define	GPIO4_A3	4	3
#define	GPIO4_A4	4	4
#define	GPIO4_A5	4	5
#define	GPIO4_A6	4	6
#define	GPIO4_A7	4	7
#define	GPIO4_B0	4	8
#define	GPIO4_B1	4	9
#define	GPIO4_B2	4	10
#define	GPIO4_B3	4	11
#define	GPIO4_B4	4	12
#define	GPIO4_B5	4	13
#define	GPIO4_B6	4	14
#define	GPIO4_B7	4	15
#define	GPIO4_C0	4	16
#define	GPIO4_C1	4	17
#define	GPIO4_C2	4	18
#define	GPIO4_C3	4	19
#define	GPIO4_C4	4	20
#define	GPIO4_C5	4	21
#define	GPIO4_C6	4	22
#define	GPIO4_C7	4	23
#define	GPIO4_D0	4	24
#define	GPIO4_D1	4	25
#define	GPIO4_D2	4	26
#define	GPIO4_D3	4	27
#define	GPIO4_D4	4	28
#define	GPIO4_D5	4	29
#define	GPIO4_D6	4	30
#define	GPIO4_D7	4	31

#define FUNC_TO_GPIO(m)		((m) & 0xfff0)


#endif
