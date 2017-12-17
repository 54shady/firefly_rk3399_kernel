# global variable
# run this script on the kernel source code top dir

set -x

# marco
KERNEL_TOP_DIR=$PWD
KERNEL_OUT_DIR="$PWD/out"
LOCAL_CROSS_COMPILE="/home/zeroway/rk3399/tool/gcc-linaro-4.9.4-2017.01-i686_aarch64-linux-gnu/bin/aarch64-linux-gnu-"
BUILD_JOBS="-j8"

# do compile for rk3399
genk_rk3399()
{
	# local variable
	KERNEL_CONFIG_FILE="firefly_linux_defconfig"
	KERNEL_TARGET_IMAGE="rk3399-firefly-mini-linux.img"

	# some suck thing
	mkdir -p $KERNEL_OUT_DIR/drivers/char
	cp $KERNEL_TOP_DIR/drivers/char/virtd $KERNEL_OUT_DIR/drivers/char/

	# okay, it's time to do the real thing
	# generate dot config file if not exist
	if [ ! -f $KERNEL_OUT_DIR/.config ]
	then
		make -C $KERNEL_TOP_DIR ARCH=arm64 CROSS_COMPILE=$LOCAL_CROSS_COMPILE O=$KERNEL_OUT_DIR $KERNEL_CONFIG_FILE
	fi

	# make kernel
	make -C $KERNEL_TOP_DIR ARCH=arm64 CROSS_COMPILE=$LOCAL_CROSS_COMPILE O=$KERNEL_OUT_DIR $KERNEL_TARGET_IMAGE $BUILD_JOBS

	# make modules and install
	make -C $KERNEL_TOP_DIR ARCH=arm64 CROSS_COMPILE=$LOCAL_CROSS_COMPILE O=$KERNEL_OUT_DIR modules $BUILD_JOBS
	make -C $KERNEL_TOP_DIR ARCH=arm64 CROSS_COMPILE=$LOCAL_CROSS_COMPILE O=$KERNEL_OUT_DIR $BUILD_JOBS INSTALL_MOD_PATH=$KERNEL_OUT_DIR/modules_out modules_install
}

generate_boot_image() {
	BOOT=${KERNEL_OUT_DIR}/boot.img
	KERNEL_IMAGE="$KERNEL_OUT_DIR/arch/arm64/boot/Image"
	DTB_FILE="$KERNEL_OUT_DIR/arch/arm64/boot/dts/rockchip/rk3399-firefly-mini-linux.dtb"

	rm -rf ${BOOT}

	echo -e "\e[36m Generate Boot image start\e[0m"

	# 100 Mb
	mkfs.vfat -n "boot" -S 512 -C ${BOOT} $((100 * 1024))

	mmd -i ${BOOT} ::/extlinux

	# rk3399.conf ==> /extlinux/extlinux.conf
	mcopy -i ${BOOT} -s extlinux/rk3399.conf ::/extlinux/extlinux.conf

	# copy Image ==> /Image
	mcopy -i ${BOOT} -s $KERNEL_IMAGE ::

	# copy rk3399-firefly-mini-linux.dtb ==> /rk3399-firefly-linux.dtb
	mcopy -i ${BOOT} -s $DTB_FILE ::/rk3399-firefly-linux.dtb

	echo -e "\e[36m Generate Boot image : ${BOOT} success! \e[0m"
}

# let's get things done
genk_rk3399

# pack boot.img
generate_boot_image
