Program files:
1) CSP.c
2) SP.c

Simulation input files for Station Process:
1) StationProcess1.txt
2) StationProcess2.txt
3) StationProcess3.txt
4) StationProcess4.txt
5) StationProcess5.txt
6) StationProcess6.txt
7) StationProcess7.txt
8) StationProcess8.txt
9) StationProcess9.txt
10) StationProcess10.txt

Output log file of CommunicationBusProcess:
CommunicationSwitchProcess_OUTPUT.txt

Output log file of StationProcess:
StationProcess_OUTPUT.txt


Steps to follow

STEP1:
Compile the CSP.c program:
"$gcc CSP.c -o csp"

STEP2:
Complie the SP.c program:
"$gcc SP.c -o sp"

STEP3:
Execute the CommunicationBusProcess.c program:

./csp 2655

STEP4:
Execute the StationProcess program for different stations simultaneously:

./sp 2655 1 &
./sp 2655 2 &
./sp 2655 3 &
./sp 2655 4 &
./sp 2655 5 &
./sp 2655 6 &
./sp 2655 7 &
./sp 2655 8 &
./sp 2655 9 &
./sp 2655 10
