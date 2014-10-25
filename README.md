# AFRL Problem: Spotty Data Connection for Personnel Monitoring

This is a working development repository for LabHack 2014, USAF AFRL's first 
hackathon! Specifically, this github repo will include the server-client C files 
used to address the "Spotty Data Connection for Personnel Monitoring" problem.

## High-level overview

Our solution for addressing spotty data connection is by applying the simplicty of 
the User Datagram Protocol, UDP, and amplifying it with high-frequency, by-the-minute  
check-ins using a customized, standardized binary-formatted datagrams such that 
reliablity is maximized in spite of the risk of data-loss or corruption via 
unreliable connection sources and connection interruptions. Such risk 
is mitigated by the robustness of server-side validation and data analysis reinforced 
by a near-constant datagram firing tempo from devices belonging to personnel used for 
physiology reporting.  

## Physiology Datagram
1. Opcode (1 bit)
2. Timestamp (~4 bytes) 
3. Normalized EEG (~4 bytes)
4. Normaized Respiratory Rate (~4 bytes)

 

