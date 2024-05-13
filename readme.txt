Source Codes:
==============
email-service1.c
email-service2.c
email-client1.c
email-client2.c
email-client3.c
email-client4.c

Stimulation Input Files for Email Client Processes:
===================================================
email-client1_input.txt
email-client2_input.txt
email-client3_input.txt
email-client4_input.txt

Output Log Files:
=================
email-server1_log.txt
email-server2_log.txt
email-client1_log.txt
email-client2_log.txt
email-client3_log.txt
email-client4_log.txt


Steps involved in compiling and Running the Project:
===================================================

STEP1:
=====
Compile the email-service.c program:
"gcc -o email-service email-service.c"

STEP2:
=====
Compile the email-client1.c, email-client2.c, email-client3.c, and email-client4.c programs:
"gcc -o email-client1 email-client1.c"
"gcc -o email-client2 email-client2.c"
"gcc -o email-client3 email-client3.c"
"gcc -o email-client4 email-client4.c"

STEP3:
=====
Execute the email-service program:
"./email-service <PORT NUMBER>"

STEP4:
=====
Execute the css program:
"./csp <ANY PORT NUMBER> &"

Example
"./csp 22222 &" 

STEP5:
=====
Execute the StationProcess program for different stations simultaneously:
"./sp 127.0.0.1 <PORT NO> <STATION NUMBER> & ./sp 127.0.0.1 <PORT NO> <STATION NUMBER> & ./sp 127.0.0.1 <PORT NO> <STATION NUMBER>"

Example 
"./sp 127.0.0.1 22222 1 & ./sp 127.0.0.1 22222 2 & ./sp 127.0.0.1 22222 3"