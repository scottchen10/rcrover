# The rcrover Project

# What is this project?
I was looking to learn more about CAD, 3D printing and embedded systems programming so I decided to build a model car. 

Beginning with the chassis, I designed a basic chassis using OnShape and printed the parts using my Creality Ender V3 3D printer. I spent a few days deciding how to and creating test prints to figure out how to connect different parts together.

See the OnShape Document here: [link](https://cad.onshape.com/documents/12d0c673e413079d624da790/w/821ae4069d81f2e3a4ff1f6f/e/b785c43bee72ff9bdb8584b0?renderMode=0&uiState=6798614a1209b4586b586871)

After the chassis was created I soldered and wired together all the hardware which included 

 - 2x Micro DC Motors
 - 1x L293D Motor Driver
 - 1x Arduino Nano
 - 2x NRF2l01 RF Wireless Transmission Modules
 - 1x 2-Axis Joystick
 - 1x Battery Pack/Adapter
 
Using the Platformio IDE, the code was created.

 # Future Projects
One thing I originally wanted was for the car to be able to travel from one location to another autonomously.
However after some research I realized my MPU6050 and 4x Ultrasonic sensors was not going to be able to localize my rover.
For my next project I'm probably going to learn to use a lidar, raspberry pi and wheel encoders to try to achieve that goal this so stay tuned for more in the coming months.