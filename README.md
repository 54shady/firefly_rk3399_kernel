# Usage

Compile boot.img

	./mk-kernel.sh

Flash boot.img

	rkdeveloptool wl 0x8000 out/boot.img
	rkdeveloptool rd
