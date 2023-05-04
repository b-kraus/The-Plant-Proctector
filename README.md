# The-Plant-Proctector
Group 40: David Myers, Beth Kraus final project.

This project will help a plant owner take care of their plant by being notified if the humidity or soil moisture of said plant is not optimal. A buzzer, and a notification via wifi will trigger so that the user can tend to their plant.

Schematic Information:
See attached docs for schematic information.

C-Code Information: 
This is the combined code for the Plant Proctector Project.  When I was designing the code, I first designed each part seperately and tested them accordingly. Both sensors requried calibration in order to get the right result.  After I finished testing each part, I put the code together and tested it as a whole. 

The code can be broken down into four parts:
- Buzzer
- DHT11
- Soil Moisture Sensor
- UART

Before I explain how the code funtions, I must explain how the data is stored for each sensor.  The soil moisture sensor is stores a number value and you are suppose to calibrate it to produce a percent. The DHT11 stores data in 40 bit, temperature high, temperature low, humidity high, humidity low, and a checksum. High is the whole number part of the number and the low is the decimal part.

The buzzer code is very simple, just a simple if statement that will cause the buzzer to sound if the hh, th, moisturePercent go below the set range. 

The DHT11 does not use the ADC but uses a if statement. First it reads the hh, then hl, and so on.  It then adds the data and compares it to the check sum.

The Gitkin Soil Moisture works by creating a value.  Using the ADC, we are able to collect this value. In order to get the percentage we need to calibrate this value. First you take a reading of the sensor dry and record this data.  Then we take a reading of the sensor in water.  Final we put these values in the following formula:  moisturePercent = ((DRY_VALUE - adcResult ) * 100.0) / (DRY_VALUE - WET_VALUE).

UART sends a string stating what the problem is if the sensor does not fall within the range.

ONE IMPORTANT NOTE: Both sensors use TB2 that are configured different, but to get around this, I made an if statement that will bounce between the sensors when collecting data and each time it switches it reconfigures it. 

All Wifi code was provided.

I had several issues with the wifi and plan to investigate further on how we can debug these issues. 


PCB Information:
The PCB was specifically designed for this project using a provided template from the class. It uses an MSP430 microcontroller soldered onto the board and is powered by a micro USB header at 5 volts. It also utilizies a copper pour on its bottom layer as this reduced complexity and gives us an entire ground layer to work with. The PCB underwent various revisions, however we ultimately decided on our final design as it made soldering easier going forward.

The PCB encountered some problems in our final design. We could not correctly flash the MSP430 MC on our board. We think we may have needed more pins to work with as we only had 4 set up. This error would require us to reprint the board which we did not have enough time for. Despite this short coming, we were able to confirm everything connected properly and the board was able to recieve and show that is was in fact being powered. However not being able to flash it was a major set back.

Another issue regarding the PCB was our buzzer. In our design, a buzzer was supposed to go off when pin P1.5 sent it a signal. However we could not replicate this effect. This is likely due to a circuit we removed from the final design. Originally, we were advised to build a BJT circuit on our PCB. However we scrapped it, trying to keep our board more clean and simple. This may have been what resulted in our buzzer not working correctly. We could only get it to make a faint noise.

Going forward, the PCB design needed more time and understanding to be developed fully. Further research, investigation, and experience would have yielded a better result. 
