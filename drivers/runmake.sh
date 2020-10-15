make -C /lib/modules/$(uname -r)/build M=$(pwd) modules
sudo cp imx219.ko /lib/modules/$(uname -r)/kernel/drivers/media/i2c/imx219.ko
sudo cp bcm2835-unicam.ko /lib/modules/$(uname -r)/kernel/drivers/media/platform/bcm2835/
sudo modprobe -r bcm2835_unicam
sudo modprobe -r imx219
sudo modprobe bcm2835_unicam
sudo modprobe imx219
