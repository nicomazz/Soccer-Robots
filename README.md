# Soccer-robots

Code used by 2 robots that can play soccer autonomously. 

![](https://github.com/nicomazz/Soccer-Robots/blob/master/20160219_172459.jpg)

The two robots uses different microcontrollers (Arduino/Funduino) and a lot of different sensors.

# Components

Gina (striker, sx in the top photo):
- 1x Arduino Mega (Atmel ATmega2560)
- 1x Arduino Nano (Atmel ATmega328)
- 2x Half H-bridge L298 con relativa elettronica
- 14x IR sensors (TSOP1736)
- 1x Lux sensor I2C (TSL2561)
- 2x RGB Color sensor (TCS3200)
- 3x Motors (GHM-03)
- 3x Omnidirectional wheels (Kornylak)
- 1x Digital compass I2C (HMC5883)
- 4x US sensor (HC-SR04) 
![](https://github.com/nicomazz/Soccer-Robots/blob/master/logicaGINA.png)

Gino (defender, dx in the top photo):
- 1x Arduino Nano (Atmel ATmega328)
- 2x L298 Breakout
- 7x IR sensors (TSOP1736)
- 1x Lux sensor I2C (TSL2561)
- 2x RGB Color sensor (TCS3200)
- 1x I2C I/O Expander (PCF8574)
- 3x Motors (GHM-03)
- 3x Omnidirectional wheels (Kornylak)
- 1x Digital compass I2C (HMC5883)
- 4x US sensor (HC-SR04) 
- 1x Multiplexer (74HC4067)
![](https://github.com/nicomazz/Soccer-Robots/blob/master/logicaGINO.png)



# Competitions

* 2014 - Italian Robocup jr - Pisa (Second place)
* 2015 - Italian Robocup jr - Milano (Second place)
* 2016 - Romecup

>It's been several years since I started writing this code. Now I would have done many things in a totally different way. Many parts are not an example of "good programming".
