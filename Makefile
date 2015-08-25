# Makefile
# For use with [Arduino Makefile](https://github.com/sudar/Arduino-Makefile)
# [Variables](https://github.com/sudar/Arduino-Makefile/blob/master/arduino-mk-vars.md)
# Depends on env variable: 
#   - export ARDUINO_MK=/path/to/Arduino.mk
# On OSX will also need:
#   - export ARDUINO_DIR=/path/to/Arduino.app/Contents/Java
# 
# Standard Arduino Uno does not require any other changes.
#
# May need to uncomment `AVRDUDE_OPTS = -B4` or connect the USBasp Jumper 3 in
# order to slow things down enough to `make set_fuses`

ARDUINO_LIBS = SoftwareSerial espduino

# Replace this with your value
MONITOR_PORT = /dev/tty.usbmodem1234

include $(ARDUINO_MK)
