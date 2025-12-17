'''
  Poketoy Python Code

  Pulls data from Arduino serial monitor. 
  The data is then saved to the computer as a CSV file.

  Written 17 December 2025
  by Claire Schwartz

  Final project code for CS 584 Physical Computing
  with Dr. Kristin Williams
'''

import serial
import csv

PORT = 'COM5'      
BAUD = 9600

ser = serial.Serial(PORT, BAUD)

with open('data.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['Time', 'Choice', 'Temperature', 'Stress'])

    while True:
        try:
            line = ser.readline().decode().strip()
            values = line.split(',')

            if len(values) == 4:
                writer.writerow(values)
                print(values)

        except KeyboardInterrupt:
            print("User Stopped")
            break
