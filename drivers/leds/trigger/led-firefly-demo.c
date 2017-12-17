/*
 * Driver for led demo on Firefly board.
 *
 * Copyright (C) 2016, Zhongshan T-chip Intelligent Technology Co.,ltd.
 * Copyright 2006  jp chen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/platform_device.h>

DEFINE_LED_TRIGGER(ledtrig_default_control);

static int firefly_led_control_probe(struct platform_device *pdev)
{
    printk("Firefly LED demo program\n");
    
	led_trigger_register_simple("ir-user-click", &ledtrig_default_control);
	
	led_trigger_event(ledtrig_default_control, LED_FULL);     //yellow led on
	//led_trigger_event(ledtrig_default_control, LED_OFF);    //yellow led off
	
	return 0;
}

static int firefly_led_control_remove(struct platform_device *pdev)
{
	led_trigger_unregister_simple(ledtrig_default_control);
	
	return 0;
}

static struct of_device_id of_firefly_leds_match[] = {
	{ .compatible = "firefly,rk3399-led" },
	{},
};

static struct platform_driver firefly_led_driver = {
	.driver		= {
		.name	= "firefly_led",
		.owner = THIS_MODULE,
		.of_match_table = of_firefly_leds_match,
	},
	.probe		= firefly_led_control_probe,
	.remove		= firefly_led_control_remove,
};


static int __init firefly_led_init(void)
{
    return platform_driver_register(&firefly_led_driver);
}

static void __exit firefly_led_exit(void)
{
    return platform_driver_unregister(&firefly_led_driver);
}

module_init(firefly_led_init);
module_exit(firefly_led_exit);

MODULE_AUTHOR("jpchen <service@t-firefly.com>");
MODULE_DESCRIPTION("Firefly LED demo driver");
MODULE_ALIAS("platform:firefly-led");
MODULE_LICENSE("GPL");
