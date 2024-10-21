Fast Ethernet Simulation
This project simulates the Fast Ethernet using multiple processes on UNIX systems. Each station in the Ethernet is simulated by a process, and the communication switch is also represented by a process. The simulation allows communication between stations via the switch, using socket programming with either TCP or UDP.

Project Details
Features
Classical vs. Fast Ethernet: This project simulates Fast Ethernet (IEEE 802.3u), a collision-free protocol that operates at 100 Mbps.

Processes:
Station Process (SP): Each station is simulated by a process. The number of stations is configurable, with a maximum of 10.
Communication Switch Process (CSP): This process simulates the Ethernet switch, managing communication between stations.

Frame Types:
Request Frame: Sent by SP to CSP for permission to send a data frame.
Data Frame: Contains the message from one station to another.
Positive/Negative Reply: Sent by CSP to SP in response to request frames.

Requirements
Operating System: Linux (CS Department Linux Lab)
Programming Language: C/C++
Socket Programming: Inter-process communication using UNIX sockets with TCP/UDP.
