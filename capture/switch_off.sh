sudo echo '1-1' | sudo tee /sys/bus/usb/drivers/usb/unbind
sudo ifconfig eth0 down
sudo /opt/vc/bin/tvservice -o
sudo ifconfig wlan0 down
./video 50 200 > /dev/null
sudo ifconfig wlan0 up
