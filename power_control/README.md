# Shutdown control



I use this docker container and python script for shutting down the robot when battery is charged above 80%. You can of course run the python script without docker directly on your machine too.

You need valetudo running on your robot.

Charging station is switched off with a 433Mhz wireless socket. You have to edit "steuerung.cpp" and enter the codes for your wireless socket.

Edit get_batt.py with the IPs from your robot (XXX...) and your autocontrol ESP module (YYY...).



### Docker:

docker run with:
command: '/start.sh'
device: /dev/mem
capability: SYS_RAWIO 