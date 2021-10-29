sudo echo '1-1' | sudo tee /sys/bus/usb/drivers/usb/unbind
sudo ifconfig eth0 down
sudo /opt/vc/bin/tvservice -o
cam -c 1 --capture=15
echo "setting exposure to $1"
v4l2-ctl --set-ctrl vertical_blanking=$2
v4l2-ctl --set-ctrl analogue_gain=232
v4l2-ctl --set-ctrl exposure=$1
v4l2-ctl --set-ctrl analogue_gain=232
v4l2-ctl --set-ctrl green_red_pixel_value=0
v4l2-ctl --set-ctrl red_pixel_value=1023
v4l2-ctl --set-ctrl vertical_blanking=$2
v4l2-ctl --set-ctrl exposure=$1
echo "setting vertical blanking to $2"
v4l2-ctl --set-ctrl red_pixel_value=1023
