# Clock_and_Game

Based on a [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/) using the [Arduino](https://arduino-pico.readthedocs.io/en/latest/) ecosystem and [Edge Impulse](https://edgeimpulse.com/), this is a tabletop clock that can display ambient humidity and temperature, and function as a 3D compass and an ultrasonic ruler. Moreover, it can play two games (a 2-player Pong or a single-player Chrome-dinosaur-esque jump-and-dash game with inspirations from Hollow Knight.

Moving between menus and gameplay are implemented using an MPU-6050 which can detect human taps on the table, and moving between the 'clock' and 'game' interface relies on a neural-network keyword detector (using an electret microphone) which was created using [Edge Impulse](https://edgeimpulse.com/) based on the `yes` keyword. 

The keyword-detecting model was first extracted as an Arduino library, and was modified for usage with the Pico and electret microphones (the original library uses digital mics with built-in FIFO buffers for sampling). One processor core is dedicated to keyword detection, which loops every second and infers in ~200ms (at 125 MHz core clock), while another core performs every other task.
