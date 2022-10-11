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
Exept building a project it also needs particular Database. In current repo there is market.dump. This file contains schema of database for normal functionality of server.
To start up DB it requres to create role in PostgreSQL, that will be used while this server is running.
Then DB should be created. And finally use market.dampt to create all relations.
It can be done like this (for example):
```
sudo -su postgres psql
create database <database_name>
create user <role_name> WITH PASSWORD '<password>';
grant all privileges on database <database_name> to <role_name>;
```
```
sudo -su postgres psql <database_name> < ./market.dump
```

In base directory of project run:
```
configurary.sh <database_name> <role_name> <password> <postgres host> <postgres port>
```
Use following host and port: "localhost", 5432 or specify it in dependance of PostgreSQL server config.

To build project, it is better to create directory for building (mkdir build), in order to avoid polution of project directory with build files.
After creating a directory, run following commands in build directory (cd build):
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

To start server just run file Server. It will listen port 5556 on particular localhost.
```
./Server
```
To run client application runs client. It is possible to launch client if and only if server was started.
```
./client
````
When client has been started it shows LogIn/Registration menu. You can authorize or create new user.
Then client provides menu, that consists 9 options:
- Hello request
- Make order
- Show balance
- Show active orders
- Show closed orders
- Show compleated deals
- Delete active order
- Show quotations
- Exit 