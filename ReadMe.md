# Read me
A short description of each file in this folder

## Development
### Teensy_Blink
This makes the integrated LED on the teensy blink. Good for establishing USB connection between PC and Teensy.

### Teensy_Blink
Establishes connection between teensy and PC 


### odrive_example
Taken from the odrive github. This is the example code which is provided to control the odrive through serial coms. Note that the current limit needs commenting out for closed loop control - original is too low.
Note that connections to be made:

Teensy | odrive 
------------ | -------------
GND | GND
0 | 1
1 | 2

## odrive_example_mod
Same as odrive_example, but with some modifications.

- Comment out current setting
- Add gain to sinwave example

### Write_to_Serial
How to write data from teensy to .dat. Also includes programme to convert .dat to easy to use vars in matlab





