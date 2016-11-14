# Thermostat

As winter comes, I bought a thermostat connect to my new boiler. It is look like this:

![](https://raw.githubusercontent.com/redforks/thermostat/gh-pages/1039273313.jpg)

Soon I found YiFAER not good as I expected. The temperature value isn't stable,
such as 20.0°C, next second it will be 20.4°C, next second maybe 19.7°C. I
think the precision is about 1°C. 

One degree in temperature is huge, people may feel code or warn for one degree drop or
rising. Obviously the temperature sensor not good enough to me.

The thermostat use High/Low control logic, if the set point is 21°C, it will
shutdown the boiler at 21°C, turn on again at 19.5°C°, 1.5°C gap. To maintain a
desired temperature, such as 21°C, need set to 22.5°C.

Try more expansive thermostats, why not built my self?

## Hardware

 1. Arduino uno
 1. DHT22 temperature/humidity sensor
 1. 1602 LCD screen
 1. Relay shield
 1. Resistances, breadboard etc.

![](https://raw.githubusercontent.com/redforks/thermostat/gh-pages/board.png)

## Software

Compare the commercial one, DIY version better at:

 1. Narrow the HIGH/LOW gap to 0.5°C.
 1. More smooth temperature reading.
  1. Since DHT22 has 0.1~0.2°C precision, if a reading only changed 0.1°C, drop
     it. It won't get higher precision, except more dampling, jump between
     21.1°C and 21.2°C back and forth.
  1. If temperature changes more than 2°C , drop it because it is possible a
     wrong value.
  1. If continuous got the same reading, then it should be the real value,
     override rule 1 & 2.
 1. Add boiler throttle control. Keep boiler on or off at least 15 minutes. 

During build the software, I found most arduino sample programs have strong
coupling, sensors, display, keyboard, relays all coupled together. Maybe
arduino program just too simple.

I create a framework to loose the coupling: <https://github.com/redforks/arduinoCore>.
