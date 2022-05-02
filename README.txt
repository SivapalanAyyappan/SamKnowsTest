SamKnowsTest:
This is a program to send http request to "http://www.google.com/" and find:
IP address of HTTP server
HTTP response code
median of LookupTime
median of Connect time
median of Start transfer time
median of Total time
Program contains a shared library "libHttpStats.so" and "StatsMedianApp" application.

Arguments: -H is optional, -n is mandatory
-H \"Header-name: Header-value\" : Can be used multiple times, each time specifying an extra HTTP header to add to your request.
-n <integer> : Number of HTTP requests to make (i.e. the number of samples you will have to take the median of)

PreRequisits:
-n option is mandatory
Minimum 3 arguments required.

Usages:
./StatsMedianApp -n 5
./StatsMedianApp -n 5 -H "TEST:SAMKNOWS"
./StatsMedianApp -n 5 -H "TEST:SAMKNOWS" -H "Domain:Networking"

Outputs:
SKTEST;<IP address of HTTP server>;<HTTP response code>;<median of LookUpTime>;<median of ConnectTime>;<median of StartTransferTime>;<median of Total time>

Sample Outputs:
siva# ./StatsMedianApp -n 5
SKTEST;142.250.179.228;200;0.000054;0.000054;0.133940;0.133977
siva# ./StatsMedianApp -n 5 -H "TEST:SAMKNOWS"
SKTEST;142.250.179.228;200;0.000080;0.000081;0.133681;0.133916
siva# ./StatsMedianApp -n 5 -H "TEST:SAMKNOWS" -H "Domain:Networking"
SKTEST;142.250.179.228;200;0.000030;0.000030;0.135651;0.135697


Build Process:
To Make:
cd to SamKnowsTest folder
make

clean:
make clean

