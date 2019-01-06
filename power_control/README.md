# Shutdown control



I use this docker container and python script on a Raspberry Pi for switching a wireless socket on and off. Together with autocontrol the robot is fully controlled by this container.

You can of course run the python script without docker directly on your machine too.

You need valetudo running on your robot.

Charging station is switched off with a 433Mhz wireless socket. You have to edit "steuerung.cpp" and enter the codes for your wireless socket.

Edit get_batt.py with the IPs from your robot (XXX...) and your autocontrol ESP module (YYY...).



### Docker:

1. Copy files in same folder
2. Modify `steuerung.cpp` for your wireless socket codes and `get_batt.py` for robot IP and cleaning schedule.
3. Go to folder and run: `docker build -t robot_control .`
4. `docker run -itd -e TZ=Europe/Berlin --device=/dev/mem:/dev/mem --cap-add="SYS_RAWIO" --name=robot_control robot_control`

