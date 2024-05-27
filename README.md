# Lap_Timer
Arduino-based project designed to log distance intervals measured by an ultrasonic sensor into the EEPROM. This values can are stored and than can be viewed by plugging the Arduino into your computer.
# Schematics

<img width="800" alt="Schematics" src="https://github.com/Glenato/Lap_Timer/assets/124728975/e268e161-8bc2-4910-9156-191676ca5cd4">

# Memory usage
Memory (16 cubes (4096 / 256)): 

🟦 🟦 🟦 🟦 🟦  🟥 🟥 🟥 🟥 🟥 🟥 🟥 🟥 🟥 🟥  🟩

🟦 = 0 - 1280

🟥 = 1281 - 3841

🟩 = 3842 - 4096

10 red cubes 🟥 for values and 5 blue cubes 🟦 for adresses of values because int is 1/2 of long and a green cube 🟩 for other small local values including errors and dynamic variable.

At byte 3842 and 2344 the last_adress_of_addr variable is stored 

At byte 3845 and 2347 the last_adress_of_vals variable is stored 

Errors are stored from byte 4000 to 4092

1 cube = 256 bytes

(A int = 2 bytes)

(A long  = 4 bytes)

(1 cube = 64 longs)

# Errors 

## 🛑 No error can be 255 because than the program will think it is not used memory and will overwrite it.

The 4092 byte is only used to know if the errors memory is full, if there is the value of 1 everything is fine if there is another value, this is the number of other errors the arduino got but couldn’t store into memory due to lack of space (note : if there is the value of 255 it means there where 254 or mores errors it couldn’t store).

## Every error value is stored in the EEPROM at adresse 4000 to the 4092 :

### Errors that do require a human intervention are values from 1 to  199:

1 : The ultrasonic sensor is not plugged correctly or distance is < 1000  

2 : Adresses (last_adress_of_addr) overflow 

3 : Values (last_adress_of_vals) overflow 

### Errors that do not require a human intervention are values from 200 to 254 :
(The are none yet)
