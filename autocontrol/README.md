# Autocontrol

With this modification you can shutdown your Mi Robot over Wifi. We add an additional ESP Wifi module to simulate a power button press.

The robot can only be shut down when it is not charging, so either turn off the charging station or place the robot outside the station.



## Prerequisites:

- You need Valetudo installed on the robot auto shutdown.
- Install Arduino IDE
- Download and install "esp8266" board module in Arduino IDE (Tools/Board/Boards Manager)
- Download ArduinoJSON module (Tools/Manage libraries)
- Select your board or "Generic ESP8266 Board"
- For first flashing connect ESP to your PC with a serial port dongle (see german description for ESP-07 here: http://stefanfrings.de/esp8266/ or use google...)



## Hardware

### ESP8266

I used an ESP-07 module, but you could use any of the ESP-xx modules (but you may have to edit the firmware then) with at least 1MB of flash memory. Connect with serial connector for first firmware flashing.

After the first flashing you can update the firmware over the webserver (OTA).





To shutdown the robot, we have to pull down the power button Pin to GND for about 5 seconds. You can use a mosfet for this task. Choose a (cheap) n-channel mosfet with at least 15V Drain-Source maximum Voltage and threshold voltage of maximum 3V.

Connect like this:

![schematic](img/schematic.png)



I had some problems with the ESP boot and sometimes the ESP got stuck/not working after powering it up, so I used a RC lowpass on RESET pin to delay the startup a bit after powering the board.

R3 and R4 seem to be important too for reliable startup.



![esp_board](img/esp_board.png)

![esp_board_wrapped](img/esp_board_wrapped.png)



You can get 3,3V power supply for your ESP at the TPP7 point on the robot board (see picture below). This is the output of the built-in 3,3V voltage regulator. Keep the wires away from the white marked area around the buttons because the top cover is directly pressed on the board in this area.

![Verbau](img/Verbau.png)



## Installation:

1. Open the config_template.h file and enter your Wifi SSID and password and maybe change the used pin for the mosfet. You can also edit the auto start and auto shutdown delays here. **Rename file to config.h**

2. Build and flash ESP module with serial connection

3. ESP should connect to your wifi now (check your wifi router for IP) and you can open its IP in your browser:

   ![website_main](img/website_main.png)

4. You can switch the auto start and auto shutdown feature on and off by pressing the toggle button. The settings are saved on reboot.

5. You can also press "Shutdown now" button (or open the url x.x.x.x/shutdown) to shutdown directly.

6. For updating firmware after changing the config, compile in Arduino IDE (Strg + Alt + S) and upload generated .bin file over "Upload firmware" Button.

