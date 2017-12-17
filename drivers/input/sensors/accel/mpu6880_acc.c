/* drivers/input/sensors/access/mpu6880_acc.c
 *
 * Copyright (C) 2012-2015 ROCKCHIP.
 * Author: oeh<oeh@rock-chips.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/freezer.h>
#include <linux/of_gpio.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <linux/sensor-dev.h>
#include <linux/mpu6880.h>



static int sensor_active(struct i2c_client *client, int enable, int rate)
{
	struct sensor_private_data *sensor =
	    (struct sensor_private_data *) i2c_get_clientdata(client);	
	int result = 0;
	int status = 0;
	u8 pwrm1 = 0;	
	
	sensor->ops->ctrl_data = sensor_read_reg(client, sensor->ops->ctrl_reg);
	pwrm1 = sensor_read_reg(client, MPU6880_PWR_MGMT_1);
	//�ر�
	if(!enable)
	{	
		status = BIT_ACCEL_STBY;	
		sensor->ops->ctrl_data |= status;	
		//gyro��acc��������ʱ��ģ���������
		if(sensor->ops->ctrl_data && (BIT_ACCEL_STBY | BIT_GYRO_STBY) != 0)
		{
			pwrm1 |= MPU6880_PWRM1_SLEEP;
		}
	}
	else//��
	{
		status = ~BIT_ACCEL_STBY;	
		sensor->ops->ctrl_data &= status;
		pwrm1 &=~MPU6880_PWRM1_SLEEP;
	}
	result = sensor_write_reg(client, sensor->ops->ctrl_reg, sensor->ops->ctrl_data);
	if(result)
	{
		printk("%s:fail to set pwrm2\n",__func__);
		return -1;
	}
	msleep(20);

	result = sensor_write_reg(client, MPU6880_PWR_MGMT_1,pwrm1);
	if(result)
	{
		printk("%s:fail to set pwrm1\n",__func__);
		return -1;
	}
	msleep(20);
	
	return result;

}

static int sensor_init(struct i2c_client *client)
{
	int res=0; 
	u8 read_data = 0;
	struct sensor_private_data *sensor =
	    (struct sensor_private_data *) i2c_get_clientdata(client);	

	//���IC�Ƿ�ΪMPU6880
	read_data = sensor_read_reg(client,sensor->ops->id_reg);
	if(read_data != sensor->ops->id_data)
	{
		printk("%s:check id err,read_data:%d,ops->id_data:%d\n",__func__,read_data,sensor->ops->id_data);
		return -1;
	}
	
	//�Ĵ�����ʼ��
	res = sensor_write_reg(client, MPU6880_PWR_MGMT_1,0x80);
	if (res) 
	{
		printk("set MPU6880_PWR_MGMT_1 error,res: %d!\n", res);
		return res;
	}
	msleep(40);

	res = sensor_write_reg(client, MPU6880_GYRO_CONFIG,0x18);  //config gyro for 2000dps
	if (res) 
	{
		printk("set MPU6880_GYRO_CONFIG error,res: %d!\n", res);
		return res;
	}
	msleep(10);

	res = sensor_write_reg(client, MPU6880_ACCEL_CONFIG,0x00);  //config Accel for +_2G
	if (res) 
	{
		printk("set MPU6880_ACCEL_CONFIG error,res: %d!\n", res);
		return res;
	}
	msleep(10);

	res = sensor_write_reg(client, MPU6880_ACCEL_CONFIG2,0x00);
	if (res) 
	{
		printk("set MPU6880_ACCEL_CONFIG2 error,res: %d!\n", res);
		return res;
	}
	res = sensor_write_reg(client, MPU6880_PWR_MGMT_2,0x3F); //set accl and gyro all axis into standby mode
	if (res) 
	{
		printk("set MPU6880_PWR_MGMT_2 error,res: %d!\n", res);
		return res;
	}	
	msleep(10);	
	res = sensor_write_reg(client, MPU6880_PWR_MGMT_1,0x41);
	if (res) 
	{
		printk("set MPU6880_PWR_MGMT_1 error,res: %d!\n", res);
		return res;
	}	
	msleep(10); 

	//Ĭ�Ϲر�
	res = sensor->ops->active(client,0,0);
	if(res)
	{
		printk("%s:line=%d,error\n",__func__,__LINE__);
		return res;
	}
	return res;
}

static int gsensor_report_value(struct i2c_client *client, struct sensor_axis *axis)
{
	struct sensor_private_data *sensor =
	    (struct sensor_private_data *) i2c_get_clientdata(client);	

	/* Report acceleration sensor information */
	input_report_abs(sensor->input_dev, ABS_X, axis->x);
	input_report_abs(sensor->input_dev, ABS_Y, axis->y);
	input_report_abs(sensor->input_dev, ABS_Z, axis->z);
	input_sync(sensor->input_dev);
	DBG("Gsensor x==%d  y==%d z==%d\n",axis->x,axis->y,axis->z);

	return 0;
}

#define GSENSOR_MIN  		10
static int sensor_report_value(struct i2c_client *client)
{
	struct sensor_private_data *sensor =
		(struct sensor_private_data *) i2c_get_clientdata(client);	
	struct sensor_platform_data *pdata = sensor->pdata;
	int ret = 0;
	short x,y,z;
	struct sensor_axis axis;
	u8 buffer[6] = {0};	
	char value = 0;
	
	if(sensor->ops->read_len < 6)	//sensor->ops->read_len = 6
	{
		printk("%s:lenth is error,len=%d\n",__func__,sensor->ops->read_len);
		return -1;
	}
	
	memset(buffer, 0, 6);
	
	/* Data bytes from hardware xL, xH, yL, yH, zL, zH */	
	do {
		*buffer = sensor->ops->read_reg;
		ret = sensor_rx_data(client, buffer, sensor->ops->read_len);
		if (ret < 0)
		return ret;
	} while (0);

	x = ((buffer[0] << 8) & 0xff00) + (buffer[1] & 0xFF);
	y = ((buffer[2] << 8) & 0xff00) + (buffer[3] & 0xFF);
	z = ((buffer[4] << 8) & 0xff00) + (buffer[5] & 0xFF);
	
	printk("mpu6880_acc: x:%d,y:%d,z:%d,-4:%d\n",x,y,z,-4);
	printk("mpu6880_acc:orientation:\n%d %d %d\n%d %d %d\n%d %d %d\n",pdata->orientation[0],
		pdata->orientation[1],pdata->orientation[2],pdata->orientation[3],pdata->orientation[4],
		pdata->orientation[5],pdata->orientation[6],pdata->orientation[7],pdata->orientation[8]);
	axis.x = (pdata->orientation[0])*x + (pdata->orientation[1])*y + (pdata->orientation[2])*z;
	axis.y = (pdata->orientation[3])*x + (pdata->orientation[4])*y + (pdata->orientation[5])*z; 
	axis.z = (pdata->orientation[6])*x + (pdata->orientation[7])*y + (pdata->orientation[8])*z;

	//Ϊ�˲��޸�hal����룬����ת�����ϱ�
	axis.x = 61*axis.x;
	axis.y = 61*axis.y;
	axis.z = 61*axis.z;
	
	//if((abs(sensor->axis.x - axis.x) > GSENSOR_MIN) || (abs(sensor->axis.y - axis.y) > GSENSOR_MIN) || (abs(sensor->axis.z - axis.z) > GSENSOR_MIN))
	{
		gsensor_report_value(client, &axis);

		/* ����ػ�������. */
		mutex_lock(&(sensor->data_mutex) );
		sensor->axis = axis;
		mutex_unlock(&(sensor->data_mutex) );
	}

	if((sensor->pdata->irq_enable)&& (sensor->ops->int_status_reg >= 0))	//read sensor intterupt status register
	{
		
		value = sensor_read_reg(client, sensor->ops->int_status_reg);
		DBG("%s:sensor int status :0x%x\n",__func__,value);
	}
	
	return ret;
}


struct sensor_operate gsensor_mpu6880_ops = {
	.name				= "mpu6880_acc",
	.type				= SENSOR_TYPE_ACCEL,			//sensor type and it should be correct
	.id_i2c				= ACCEL_ID_MPU6880,				//i2c id number
	.read_reg			= MPU6880_ACCEL_XOUT_H,		//read data
	.read_len			= 6,							//data length
	.id_reg				= MPU6880_WHOAMI,			//read device id from this register
	.id_data 			= MPU6880_DEVICE_ID,			//device id
	.precision			= MPU6880_PRECISION,			//16 bit
	.ctrl_reg 			= MPU6880_PWR_MGMT_2,		//enable or disable 	
	.int_status_reg 	= MPU6880_INT_STATUS,			//intterupt status register
	.range				= {-32768*61,32768*61},	//range
	.trig				= IRQF_TRIGGER_HIGH |IRQF_ONESHOT,		
	.active				= sensor_active,	
	.init				= sensor_init,
	.report 			= sensor_report_value,
};

/****************operate according to sensor chip:end************/

//function name should not be changed
static struct sensor_operate *gsensor_get_ops(void)
{
	return &gsensor_mpu6880_ops;
}


static int __init gsensor_mpu6880_init(void)
{
	struct sensor_operate *ops = gsensor_get_ops();
	int result = 0;
	int type = ops->type;
	result = sensor_register_slave(type, NULL, NULL, gsensor_get_ops);	
	return result;
}

static void __exit gsensor_mpu6880_exit(void)
{
	struct sensor_operate *ops = gsensor_get_ops();
	int type = ops->type;
	sensor_unregister_slave(type, NULL, NULL, gsensor_get_ops);
}


module_init(gsensor_mpu6880_init);
module_exit(gsensor_mpu6880_exit);



