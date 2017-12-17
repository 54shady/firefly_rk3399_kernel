/**
 * dwc3-rockchip.c - Rockchip Specific Glue layer
 *
 * Copyright (C) Fuzhou Rockchip Electronics Co.Ltd
 *
 * Authors: William Wu <william.wu@rock-chips.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <linux/extcon.h>
#include <linux/reset.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include "core.h"
#include "io.h"

#define DWC3_ROCKCHIP_AUTOSUSPEND_DELAY  500 /* ms */

struct dwc3_rockchip {
	int			num_clocks;
	bool			connected;
	struct device		*dev;
	struct clk		**clks;
	struct dwc3		*dwc;
	struct reset_control	*otg_rst;
	struct extcon_dev	*edev;
	struct notifier_block	device_nb;
	struct notifier_block	host_nb;
	struct work_struct	otg_work;
};

static int dwc3_rockchip_device_notifier(struct notifier_block *nb,
					 unsigned long event, void *ptr)
{
	struct dwc3_rockchip *rockchip =
		container_of(nb, struct dwc3_rockchip, device_nb);

	schedule_work(&rockchip->otg_work);

	return NOTIFY_DONE;
}

static int dwc3_rockchip_host_notifier(struct notifier_block *nb,
				       unsigned long event, void *ptr)
{
	struct dwc3_rockchip *rockchip =
		container_of(nb, struct dwc3_rockchip, host_nb);

	schedule_work(&rockchip->otg_work);

	return NOTIFY_DONE;
}

static void dwc3_rockchip_otg_extcon_evt_work(struct work_struct *work)
{
	struct dwc3_rockchip	*rockchip =
		container_of(work, struct dwc3_rockchip, otg_work);
	struct dwc3		*dwc = rockchip->dwc;
	struct extcon_dev	*edev = rockchip->edev;
	struct usb_hcd		*hcd;
	unsigned long		flags;
	int			ret;
	u32			reg;

	if (!dwc)
		return;

	if (extcon_get_cable_state_(edev, EXTCON_USB) > 0) {
		if (rockchip->connected)
			return;

		/*
		 * If dr_mode is host only, never to set
		 * the mode to the peripheral mode.
		 */
		if (WARN_ON(dwc->dr_mode == USB_DR_MODE_HOST))
			return;

		pm_runtime_get_sync(dwc->dev);

		spin_lock_irqsave(&dwc->lock, flags);
		dwc3_set_mode(dwc, DWC3_GCTL_PRTCAP_DEVICE);
		spin_unlock_irqrestore(&dwc->lock, flags);

		rockchip->connected = true;
		dev_info(rockchip->dev, "USB peripheral connected\n");
	} else if (extcon_get_cable_state_(edev, EXTCON_USB_HOST) > 0) {
		if (rockchip->connected)
			return;

		/*
		 * If dr_mode is device only, never to
		 * set the mode to the host mode.
		 */
		if (WARN_ON(dwc->dr_mode == USB_DR_MODE_PERIPHERAL))
			return;

		reset_control_assert(rockchip->otg_rst);

		ret = phy_power_on(dwc->usb2_generic_phy);
		if (ret < 0) {
			reset_control_deassert(rockchip->otg_rst);
			return;
		}

		ret = phy_power_on(dwc->usb3_generic_phy);
		if (ret < 0) {
			phy_power_off(dwc->usb2_generic_phy);
			reset_control_deassert(rockchip->otg_rst);
			return;
		}

		reset_control_deassert(rockchip->otg_rst);

		pm_runtime_get_sync(dwc->dev);

		hcd = dev_get_drvdata(&dwc->xhci->dev);

		spin_lock_irqsave(&dwc->lock, flags);
		dwc3_set_mode(dwc, DWC3_GCTL_PRTCAP_HOST);
		spin_unlock_irqrestore(&dwc->lock, flags);

		if (hcd->state == HC_STATE_HALT) {
			usb_add_hcd(hcd, hcd->irq, IRQF_SHARED);
			usb_add_hcd(hcd->shared_hcd, hcd->irq, IRQF_SHARED);
		}

		rockchip->connected = true;
		dev_info(rockchip->dev, "USB HOST connected\n");
	} else {
		if (!rockchip->connected)
			return;

		reg = dwc3_readl(dwc->regs, DWC3_GCTL);

		if (DWC3_GCTL_PRTCAP(reg) == DWC3_GCTL_PRTCAP_HOST ||
		    DWC3_GCTL_PRTCAP(reg) == DWC3_GCTL_PRTCAP_OTG) {
			hcd = dev_get_drvdata(&dwc->xhci->dev);

			if (hcd->state != HC_STATE_HALT) {
				usb_remove_hcd(hcd->shared_hcd);
				usb_remove_hcd(hcd);
			}

			phy_power_off(dwc->usb2_generic_phy);
			phy_power_off(dwc->usb3_generic_phy);

		}

		pm_runtime_put_sync(dwc->dev);

		rockchip->connected = false;
		dev_info(rockchip->dev, "USB unconnected\n");
	}
}

static int dwc3_rockchip_extcon_register(struct dwc3_rockchip *rockchip)
{
	int			ret;
	struct device		*dev = rockchip->dev;
	struct extcon_dev	*edev;

	if (device_property_read_bool(dev, "extcon")) {
		edev = extcon_get_edev_by_phandle(dev, 0);
		if (IS_ERR(edev)) {
			if (PTR_ERR(edev) != -EPROBE_DEFER)
				dev_err(dev, "couldn't get extcon device\n");
			return PTR_ERR(edev);
		}

		INIT_WORK(&rockchip->otg_work,
			  dwc3_rockchip_otg_extcon_evt_work);

		rockchip->device_nb.notifier_call =
				dwc3_rockchip_device_notifier;
		ret = extcon_register_notifier(edev, EXTCON_USB,
					       &rockchip->device_nb);
		if (ret < 0) {
			dev_err(dev, "failed to register notifier for USB\n");
			return ret;
		}

		rockchip->host_nb.notifier_call =
				dwc3_rockchip_host_notifier;
		ret = extcon_register_notifier(edev, EXTCON_USB_HOST,
					       &rockchip->host_nb);
		if (ret < 0) {
			dev_err(dev, "failed to register notifier for USB HOST\n");
			extcon_unregister_notifier(edev, EXTCON_USB,
						   &rockchip->device_nb);
			return ret;
		}

		rockchip->edev = edev;
	}

	return 0;
}

static void dwc3_rockchip_extcon_unregister(struct dwc3_rockchip *rockchip)
{
	if (!rockchip->edev)
		return;

	extcon_unregister_notifier(rockchip->edev, EXTCON_USB,
				   &rockchip->device_nb);
	extcon_unregister_notifier(rockchip->edev, EXTCON_USB_HOST,
				   &rockchip->host_nb);
}

static int dwc3_rockchip_probe(struct platform_device *pdev)
{
	struct dwc3_rockchip	*rockchip;
	struct device		*dev = &pdev->dev;
	struct device_node	*np = dev->of_node, *child;
	struct platform_device	*child_pdev;

	unsigned int		count;
	int			ret;
	int			i;

	rockchip = devm_kzalloc(dev, sizeof(*rockchip), GFP_KERNEL);

	if (!rockchip)
		return -ENOMEM;

	count = of_clk_get_parent_count(np);
	if (!count)
		return -ENOENT;

	rockchip->num_clocks = count;

	rockchip->clks = devm_kcalloc(dev, rockchip->num_clocks,
				      sizeof(struct clk *), GFP_KERNEL);
	if (!rockchip->clks)
		return -ENOMEM;

	platform_set_drvdata(pdev, rockchip);

	rockchip->dev = dev;

	for (i = 0; i < rockchip->num_clocks; i++) {
		struct clk	*clk;

		clk = of_clk_get(np, i);
		if (IS_ERR(clk)) {
			ret = PTR_ERR(clk);
			goto err0;
		}

		ret = clk_prepare_enable(clk);
		if (ret < 0) {
			clk_put(clk);
			goto err0;
		}

		rockchip->clks[i] = clk;
	}

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	ret = pm_runtime_get_sync(dev);
	if (ret < 0) {
		dev_err(dev, "get_sync failed with err %d\n", ret);
		goto err1;
	}

	rockchip->otg_rst = devm_reset_control_get(dev, "usb3-otg");
	if (IS_ERR(rockchip->otg_rst)) {
		dev_err(dev, "could not get reset controller\n");
		ret = PTR_ERR(rockchip->otg_rst);
		goto err1;
	}

	child = of_get_child_by_name(np, "dwc3");
	if (!child) {
		dev_err(dev, "failed to find dwc3 core node\n");
		ret = -ENODEV;
		goto err1;
	}

	/* Allocate and initialize the core */
	ret = of_platform_populate(np, NULL, NULL, dev);
	if (ret) {
		dev_err(dev, "failed to create dwc3 core\n");
		goto err1;
	}

	child_pdev = of_find_device_by_node(child);
	if (!child_pdev) {
		dev_err(dev, "failed to find dwc3 core device\n");
		ret = -ENODEV;
		goto err2;
	}

	rockchip->dwc = platform_get_drvdata(child_pdev);
	if (!rockchip->dwc) {
		dev_err(dev, "failed to get drvdata dwc3\n");
		ret = -EPROBE_DEFER;
		goto err2;
	}

	ret = dwc3_rockchip_extcon_register(rockchip);
	if (ret < 0)
		goto err2;

	if (rockchip->edev) {
		pm_runtime_set_autosuspend_delay(&child_pdev->dev,
						 DWC3_ROCKCHIP_AUTOSUSPEND_DELAY);
		pm_runtime_allow(&child_pdev->dev);

		if (rockchip->dwc->dr_mode == USB_DR_MODE_HOST ||
		    rockchip->dwc->dr_mode == USB_DR_MODE_OTG) {
			struct usb_hcd *hcd =
				dev_get_drvdata(&rockchip->dwc->xhci->dev);
			if (!hcd) {
				dev_err(dev, "fail to get drvdata hcd\n");
				ret = -EPROBE_DEFER;
				goto err3;
			}
			if (hcd->state != HC_STATE_HALT) {
				usb_remove_hcd(hcd->shared_hcd);
				usb_remove_hcd(hcd);
			}
		}

		pm_runtime_put_sync(dev);

		if ((extcon_get_cable_state_(rockchip->edev,
					     EXTCON_USB) > 0) ||
		    (extcon_get_cable_state_(rockchip->edev,
					     EXTCON_USB_HOST) > 0))
			schedule_work(&rockchip->otg_work);
	}

	return ret;

err3:
	dwc3_rockchip_extcon_unregister(rockchip);

err2:
	of_platform_depopulate(dev);

err1:
	pm_runtime_put_sync(dev);
	pm_runtime_disable(dev);

err0:
	for (i = 0; i < rockchip->num_clocks && rockchip->clks[i]; i++) {
		if (!pm_runtime_status_suspended(dev))
			clk_disable(rockchip->clks[i]);
		clk_unprepare(rockchip->clks[i]);
		clk_put(rockchip->clks[i]);
	}

	return ret;
}

static int dwc3_rockchip_remove(struct platform_device *pdev)
{
	struct dwc3_rockchip	*rockchip = platform_get_drvdata(pdev);
	struct device		*dev = &pdev->dev;
	int			i;

	dwc3_rockchip_extcon_unregister(rockchip);

	of_platform_depopulate(dev);

	if (!rockchip->edev)
		pm_runtime_put_sync(dev);

	pm_runtime_disable(dev);
	pm_runtime_set_suspended(dev);

	for (i = 0; i < rockchip->num_clocks; i++) {
		if (!pm_runtime_status_suspended(dev))
			clk_disable(rockchip->clks[i]);
		clk_unprepare(rockchip->clks[i]);
		clk_put(rockchip->clks[i]);
	}

	return 0;
}

#ifdef CONFIG_PM
static int dwc3_rockchip_runtime_suspend(struct device *dev)
{
	struct dwc3_rockchip	*rockchip = dev_get_drvdata(dev);
	int			i;

	for (i = 0; i < rockchip->num_clocks; i++)
		clk_disable(rockchip->clks[i]);

	return 0;
}

static int dwc3_rockchip_runtime_resume(struct device *dev)
{
	struct dwc3_rockchip	*rockchip = dev_get_drvdata(dev);
	int			i;

	for (i = 0; i < rockchip->num_clocks; i++)
		clk_enable(rockchip->clks[i]);

	return 0;
}

static const struct dev_pm_ops dwc3_rockchip_dev_pm_ops = {
	SET_RUNTIME_PM_OPS(dwc3_rockchip_runtime_suspend,
			   dwc3_rockchip_runtime_resume, NULL)
};

#define DEV_PM_OPS      (&dwc3_rockchip_dev_pm_ops)
#else
#define DEV_PM_OPS	NULL
#endif /* CONFIG_PM */

static const struct of_device_id rockchip_dwc3_match[] = {
	{ .compatible = "rockchip,rk3399-dwc3" },
	{ /* Sentinel */ }
};

MODULE_DEVICE_TABLE(of, rockchip_dwc3_match);

static struct platform_driver dwc3_rockchip_driver = {
	.probe		= dwc3_rockchip_probe,
	.remove		= dwc3_rockchip_remove,
	.driver		= {
		.name	= "rockchip-dwc3",
		.of_match_table = rockchip_dwc3_match,
		.pm	= DEV_PM_OPS,
	},
};

module_platform_driver(dwc3_rockchip_driver);

MODULE_ALIAS("platform:rockchip-dwc3");
MODULE_AUTHOR("William Wu <william.wu@rock-chips.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("DesignWare USB3 ROCKCHIP Glue Layer");
