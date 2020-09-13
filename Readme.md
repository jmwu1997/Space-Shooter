## Star Shooter Arcade

This project use [hzeller](https://github.com/hzeller)‘s **[rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix)** quite extensively. Since this led matrix library is developed with Raspberry Pi in mind, running it on Beaglebone might no work at all. Provided you have a Pi and Arduino, here are the steps to get it up and running. Detailed reports in PDF file.

### Wiring

Wiring guide can be found on the github page https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/wiring.md

Wiring guide for LCD can be found here  https://www.arduino.cc/en/Tutorial/HelloWorld 

### Arduino setup

Using Arduino IDE, upload **arduino_lcd/serial_lcd.ino** to your Arduino board

After finish, connect your Arduino to Raspberry Pi

### Xbox driver install

1. `sudo apt-get update`

2. `sudo apt install xboxdrv`

### Make

1. `cd matrix/example-api-use`

2. `make`
3. `nano rungame.sh`
4. change the rotation of the screen or brightness as you like
5. `./rungame.sh`

Now you should see the image comes up on the led matrix

### Short gameplay demo
Link: https://drive.google.com/open?id=12EcEZppbxfB3WhS9oTiQqW1d3CG7j7AC
