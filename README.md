
# Smart Overcrowding Monitor

This project is a simple Arduino-based system designed to monitor how crowded an enclosed space is while also keeping track of air quality and basic environmental conditions. It combines multiple sensors to give a more reliable understanding of the situation instead of depending on just one input.

The system uses two IR sensors to count the number of people entering and leaving a space. An ultrasonic sensor is used to estimate crowd density by measuring how close objects are in front of it. An MQ gas sensor helps detect poor air quality, and a DHT11 sensor provides temperature readings. All of this information is displayed on an I2C LCD screen in real time.

Whenever unsafe conditions are detected, such as overcrowding, high density, or bad air quality, the system triggers a buzzer to alert nearby users. This makes it useful for small indoor spaces where basic monitoring and alerts are needed.

The project is built using an Arduino Uno along with IR sensors, an ultrasonic sensor (HC-SR04), an MQ gas sensor, a DHT11 temperature sensor, an I2C LCD display, and a buzzer. To use the system, upload the code to the Arduino and connect the components according to the pin configuration defined in the code.

For best performance, the IR sensors should be aligned properly and placed at an appropriate distance to ensure accurate counting.
