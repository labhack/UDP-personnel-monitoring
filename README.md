# AFRL Problem: Personnel Monitoring with Intermittent Data Connection 

#### By: Joe Baylor, David McAffee , Benjamin Natarian Val Red, Devin Spatz 

This is the development repository for LabHack 2014, USAF AFRL's first 
hackathon! Specifically, this github repo will include the server-client C files 
used to address the AFRL "Spotty Data Connection for Personnel Monitoring" problem.

## High-level overview

Our solution for addressing intermittent data connection is by applying the simplicty and robustness of 
the User Datagram Protocol, UDP, and amplifying it with high-frequency, by-the-minute  
check-ins using customized, standardized datagrams such that 
reliability is maximized in spite of the risk of data-loss or corruption via 
weak connection sources and connection interruptions. Such risk 
is mitigated by aspects of server-side validation and data analysis reinforced 
by the near-constant device datagram firing tempo from devices belonging to personnel over large distances in cases such as disaster relief operations.  

## Chronological File Function Order
1. ebola.str -- A sample string of datasets from a theoretical disaster relief worker. 
2. phys\_client.c -- A C application that takes the data set (can be modularly 
substituted with raw sensor data) and interprets the data into the less-than-80 
byte UDP datagram before sending the datagram over Internet to a C2 server. 
3. phys\_server.c -- This C daemon receives datagrams, interprets them, and 
then generates individual files of space-delimited files to be further 
processed by Python and PHP web applications.
4. server\_script.py - This Python scripts reads the space-delimited data and 
then formats it into an XML-like form that can be read and presented by the 
PHP web application for visualization. 
5. PHP web application - These are the web files that present all our data 
virtually in real-time!  

## Physiology Datagram RFC (feel free to comment here!) 
1. Opcode - single integer determining status (check-in, medical emergency)
2. Environment - whether data is human or environment data.
3. Timestamp - Uses the UNIX Epoch for timing (~4 bytes) 
4. Latitude - GPS-based coordinate (Double)
5. Longitude - GPS-based coordinate (Double)
6. Altitude (Integer)
7. Electrocardiogram (ECG) (Integer)
8. Oxygen percentage (Integer)
9. Respiratory rate (Integer)
10. Body temperature (Float)

In total, the above standard can communicate all vital personnel data within 
under 80-byte UDP payloads! This is especially reliable since this does not 
require datagram dis/reassembly and limits the risk of data loss or corruption.

For even further reliability, engineers with a mastery of network protocols and 
low-level C could condense this implementation even further to a binary 32-byte payload! Thus, the above format is highly flexible, scalable, and reliable. 

### Licensing 


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

