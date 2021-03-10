# Testing Serial over USB with JUCE apps

Test JUCE application that reads sensor data coming from Arduinos, into
the JUCE app over USB. There are companion Arduino sketches that 
wait for the JUCE app to call the sensor values and then send them. 

Sensors are the IMU Accelerometer/Gyroscope from an Arduino Nano 33 IoT,
Time of Flight distance sensor, and Resistive toucscreen X, Y, & Pressure.
