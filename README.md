# VGAball
Implemented on the fpga a memory-mapped peripheral that can receive communication from the arm processors on the Cyclone V. Communicated with my peripheral through a Linux userspace program that accesses a device driver I have written.

My peripheral displays a ball on the vga screen at coordinates given to it through software. My device driver implements an ioctl that takes coordinates from the user and sends it to my peripheral. 

Implements a video bouncing ball in this setting. My peripheral will generate an vga raster consisting of a ball at a particular location, my userspace C program (software) will make this ball bounce around the screen, and my device driver will mediate between my program and my peripheral.
