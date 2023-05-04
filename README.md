# The-Plant-Proctector
Group 40: David Myers, Beth Kraus final project.

This project will help a plant owner take care of their plant by being notified if the humidity or soil moisture of said plant is not optimal. A buzzer, and a notification via wifi will trigger so that the user can tend to their plant.

Schematic Information:
See attached docs for schematic information.

C-Code Information: 
This is the combined code for the Plant Proctector Project.  When I was designing the code, I first designed each part seperately and tested them accordingly. Both sensors requried calibration in order to get the right result.  After I finished testing each part, I put the code together and tested it as a whole. 

The code can be broken down into four parts:
- Buzzer
- DH11
- Soil Moisture Sensor
- UART

Before I explain how the code funtions, I must explain how the data is stored for each sensor.  The soil moisture sensor is stores a number value and you are suppose to calibrate it to produce a percent. The DHT11 stores data in 40 bit, temperature high, temperature low, humidity high, humidity low, and a checksum. High is the whole number part of the number and the low is the decimal part.

The buzzer code is very simple, just a simple if statement that will cause the buzzer to sound if the hh, th, moisturePercent go below the set range. 

The DHTT does not use the ADC but uses a if statement. First it reads the hh, then hl, and so on.  It then adds the data and compares it to the check sum.

The Gitkin Soil Moisture works by creating a value.  Using the ADC, we are able to collect this value. In order to get the percentage we need to calibrate this value. First you take a reading of the sensor dry and record this data.  Then we take a reading of the sensor in water.  Final we put these values in the following formula:  moisturePercent = ((DRY_VALUE - adcResult ) * 100.0) / (DRY_VALUE - WET_VALUE).

UART sends a string stating what the problem is if the sensor does not fall within the range.

ONE IMPORTANT NOTE: Both sensors use TB2 that are configured different, but to get around this, I made an if statement that will bounce between the sensors when collecting data and each time it switches it reconfigures it. 

All Wifi code was provided.

I had several issues with the wifi and plan to investigate further on how we can debug these issues. 
