# Modifications for Xiaomi Mi Robot Vacuum

- Autocontrol:

  Integration of an ESP8266 module to simulate a power button press. The module can start the cleaning process automatically when the robot is booting and can shutdown the robot when base station is turned off (e.g. by a wireless socket)

- Power-control:

  Used to turn off the base station power supply by a wireless socket. I used a 433 MHz wireless socket, controlled by a raspberry pi. You can set a state of charge threshold, above this threshold the wireless socket is switched off (spare battery by not charging to 100%). With the autocontrol feature above you can control your robot just by switching the base station on and off.


