#!/bin/bash

clear
clear
avrdude -p m2560 -c usbasp -U flash:w:emulator.ino.mega.hex:i
