# IDD-Fa19-Final - A self-balancing robot
Final project for Cornell's Interactive Device Design.

I wanted to find out how to build a self-balancing motorbike (and thus an autonomous motorbike eventually!), but that would take too long for 1 semester. Instead, I decided to build something simpler: a self-balancing robot, just to learn how PID controller works, and just because I like robots in general :)

## 1. Paper prototype

![Paper prototype](/paper_prototype.JPG)

## 2. 3d-printed prototype & electronics

![3d-printed prototype & electronics](/3d_printed_bot.png)

## 3. Final product video (coming soon)
[Click here to watch Self-balancing Robot video](https://youtu.be/pT9M0uZvIQ8)

## 4. How it's done
The robot is auto-balanced using an algorithm called PID controller. This basically sets motors' speed based on the deviation of the robot from the balanced position. The higher the deviation, the faster the motors run to counter the fall.

![PID controller for self-balancing](/PID_self_balance.png)

I refered to several open sources and put the pieces together as below. The main work was to assemble these pieces together and lots of tuning for PID parameters to make the robot balanced & smooth. 

- Arduino & breadboard: included in class's kit
- Motors & driver ($10): https://www.amazon.com/gp/product/B07T5SVCSX/
- Gyro + accelerometer sensor ($4): https://www.amazon.com/Ximimark-MPU-6050-Accelerometer-Gyroscope-Converter/dp/B07MMZ37PT/
- Robot body: simplified from [this thingiverse design](https://www.thingiverse.com/thing:3369262)
- PID algorithm for self-balancing: followed this [Instructables tutorial](https://www.instructables.com/id/Arduino-Self-Balancing-Robot-1/)

