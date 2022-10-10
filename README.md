# Small server-client currency market

## Requirenments
- boost
- libpq
- postgresql server
- psql

## Downloading and build
To clone this repository use following command:
```
git clone https://github.com/david-dems/tcp_server_market.git --recursive
```
To build project, it is better to create directory for building, in order to avoid polution of project directory with buikd files.
After creating a directory, run following commands:
```
cmake ..
make
```
Binary files will appear in bin directory. 
There are 3 targets of building:
- Server;
- client;
- run_tests.

## Running programs

To perform tests run file "run_tests". It contains 16 unit-tests of matching engine. This tests are required to make shure, that matching logic is correct and engine is working well.

To start server just run file Server. It will listen particular port on particular host.
```
./Server
```
To run client application runs client. It is possible to launch client if and only if server was started.
```
./client
````