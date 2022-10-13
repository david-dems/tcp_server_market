# Small server-client currency market

## Requirenments
- boost
- libpq
- postgresql server
- psql
- qt5

## Downloading and build
To clone this repository use following command:
```
git clone https://github.com/david-dems/tcp_server_market.git --recursive
```
Except building a project it also needs particular Database. In current repo there is market.dump. This file contains schema of database for normal functionality of server.
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
Without ./configurary.sh it will be impossible to build a project.

To build project, it is better to create directory for building (mkdir build), in order to avoid polution of project directory with build files.
After creating a directory, run following commands in build directory (cd build):
```
cmake ..
make
```
Binary files will appear in bin directory. 
There are 3 targets of building:
- Server;
- CLI_client;
- GUI_client
- run_tests.

## Running programs

To perform tests run file "run_tests". It contains 16 unit-tests of matching engine. This tests are required to make shure, that matching logic is correct and engine is working well.

To start server just run file Server. It will listen port 5556 on particular localhost.
```
./Server
```
To run client application in terminal runs CLI_client. It is possible to launch client if and only if server was started. It provides CLI interface.
```
./CLI_client
````
To start GUI client run:
```
./GUI_client
```
All binary files placed in ./bin.
When CLI_client has been started it shows LogIn/Registration menu. You can authorize or create new user.
Then CLI_client provides menu, that consists 9 options:
- Hello request
- Make order
- Show balance
- Show active orders
- Show closed orders
- Show compleated deals
- Delete active order
- Show quotations
- Exit 
You can type nuber of option in terminal and recive results.

When GUI_client is running, at first login/registration dialog appears. After registration main window of GUI starts. It provides following operations:
- show active orders (active orders tab);
- show commited deals (Completed deals tab);
- publish new order (fields USD and RUB and button "Publish");
- delete active order with id (delete button);
- show balance;
- show market quotations.

All user info (balance, deals, orders) updates automatically.

## DB schema:
```xml
   <mxGraphModel><root><mxCell id="1"/><mxCell id="1" parent="0"/><mxCell id="10" style="collection" parent="1" vertex="1" connectable="0"><Object schemaType="Collection" name="Tag" defaultValue="" description="" as="value"/><mxGeometry x="600" y="760" width="250" height="106" as="geometry"><mxRectangle width="250" height="28" as="alternateBounds"/></mxGeometry></mxCell><mxCell id="11" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" parent="10" vertex="1"><Field as="value">{"schemaType":"Field","name":"_id","type":"ObjectId","required":true,"unique":true,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="28" width="250" height="26" as="geometry"/></mxCell><mxCell id="12" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" parent="10" vertex="1"><Field as="value">{"schemaType":"Field","name":"name","type":"String","required":true,"unique":true,"defaultValue":"","description":"","index":2,"customProps":[]}</Field><mxGeometry y="54" width="250" height="26" as="geometry"/></mxCell><mxCell id="13" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" parent="10" vertex="1"><Field as="value">{"schemaType":"Field","name":"slug","type":"String","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="80" width="250" height="26" as="geometry"/></mxCell><mxCell id="42" style="collection" vertex="1" connectable="0" parent="1"><Field as="value">{"schemaType":"Collection","name":"Users","defaultValue":"","description":"Contain users all users info for authorization"}</Field><mxGeometry x="30" y="60" width="250" height="158" as="geometry"><mxRectangle x="30" y="60" width="250" height="26" as="alternateBounds"/></mxGeometry></mxCell><mxCell id="43" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="42"><Object schemaType="Field" name="userid" type="Integer" required="1" unique="1" defaultValue="" description="" index="0" as="value"><Array as="customProps"/></Object><mxGeometry y="28" width="250" height="26" as="geometry"/></mxCell><mxCell id="44" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="42"><Field as="value">{"schemaType":"Field","name":"firstname","type":"String","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="54" width="250" height="26" as="geometry"/></mxCell><mxCell id="45" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="42"><Field as="value">{"schemaType":"Field","name":"lastname","type":"String","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="80" width="250" height="26" as="geometry"/></mxCell><mxCell id="46" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="42"><Field as="value">{"schemaType":"Field","name":"login","type":"String","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="106" width="250" height="26" as="geometry"/></mxCell><mxCell id="47" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="42"><Field as="value">{"schemaType":"Field","name":"password","type":"String","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="132" width="250" height="26" as="geometry"/></mxCell><mxCell id="48" style="collection" vertex="1" connectable="0" parent="1"><Field as="value">{"schemaType":"Collection","name":"Balance","defaultValue":"","description":""}</Field><mxGeometry x="30" y="300" width="250" height="106" as="geometry"><mxRectangle width="250" height="28" as="alternateBounds"/></mxGeometry></mxCell><mxCell id="49" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="48"><Field as="value">{"schemaType":"Field","name":"userid","type":"Integer","required":true,"unique":true,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="28" width="250" height="26" as="geometry"/></mxCell><mxCell id="50" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="48"><Field as="value">{"schemaType":"Field","name":"USD","type":"Double","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="54" width="250" height="26" as="geometry"/></mxCell><mxCell id="51" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="48"><Field as="value">{"schemaType":"Field","name":"RUB","type":"Double","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="80" width="250" height="26" as="geometry"/></mxCell><mxCell id="52" edge="1" parent="1" source="43" target="49"><mxGeometry relative="1" as="geometry"/></mxCell><mxCell id="53" style="collection" vertex="1" connectable="0" parent="1"><Field as="value">{"schemaType":"Collection","name":"New Collection","defaultValue":"","description":""}</Field><mxGeometry x="410" y="280" width="250" height="210" as="geometry"><mxRectangle width="250" height="28" as="alternateBounds"/></mxGeometry></mxCell><mxCell id="54" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="53"><Object schemaType="Field" name="appid" type="Integer" required="1" unique="1" defaultValue="" description="" index="0" as="value"><Array as="customProps"/></Object><mxGeometry y="28" width="250" height="26" as="geometry"/></mxCell><mxCell id="55" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="53"><Field as="value">{"schemaType":"Field","name":"userid","type":"Integer","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="54" width="250" height="26" as="geometry"/></mxCell><mxCell id="56" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="53"><Field as="value">{"schemaType":"Field","name":"vol","type":"Boolean","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="80" width="250" height="26" as="geometry"/></mxCell><mxCell id="57" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="53"><Field as="value">{"schemaType":"Field","name":"price","type":"Boolean","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="106" width="250" height="26" as="geometry"/></mxCell><mxCell id="60" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="53"><Object schemaType="Field" name="direction" type="String" required="0" unique="0" defaultValue="" description="" index="0" as="value"><Array as="customProps"/></Object><mxGeometry y="132" width="250" height="26" as="geometry"/></mxCell><mxCell id="59" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="53"><Field as="value">{"schemaType":"Field","name":"status","type":"String","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="158" width="250" height="26" as="geometry"/></mxCell><mxCell id="58" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="53"><Object schemaType="Field" name="date" type="Date" required="0" unique="0" defaultValue="" description="" index="0" as="value"><Array as="customProps"/></Object><mxGeometry y="184" width="250" height="26" as="geometry"/></mxCell><mxCell id="62" edge="1" parent="1" source="43" target="55"><mxGeometry relative="1" as="geometry"/></mxCell><mxCell id="63" style="collection" vertex="1" connectable="0" parent="1"><Field as="value">{"schemaType":"Collection","name":"Deals","defaultValue":"","description":""}</Field><mxGeometry x="410" y="60" width="250" height="184" as="geometry"><mxRectangle width="250" height="28" as="alternateBounds"/></mxGeometry></mxCell><mxCell id="64" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="63"><Field as="value">{"schemaType":"Field","name":"dealid","type":"Integer","required":true,"unique":true,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="28" width="250" height="26" as="geometry"/></mxCell><mxCell id="65" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="63"><Field as="value">{"schemaType":"Field","name":"sellerid","type":"Integer","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="54" width="250" height="26" as="geometry"/></mxCell><mxCell id="66" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="63"><Field as="value">{"schemaType":"Field","name":"buyerid","type":"Integer","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="80" width="250" height="26" as="geometry"/></mxCell><mxCell id="67" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="63"><Field as="value">{"schemaType":"Field","name":"vol","type":"Double","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="106" width="250" height="26" as="geometry"/></mxCell><mxCell id="68" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="63"><Field as="value">{"schemaType":"Field","name":"price","type":"Double","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="132" width="250" height="26" as="geometry"/></mxCell><mxCell id="69" style="whiteSpace=nowrap;strokeColor=#a4c0cd;html=1;autosize=1;resizable=0;" vertex="1" parent="63"><Field as="value">{"schemaType":"Field","name":"date","type":"Date","required":false,"unique":false,"defaultValue":"","description":"","index":0,"customProps":[]}</Field><mxGeometry y="158" width="250" height="26" as="geometry"/></mxCell><mxCell id="71" edge="1" parent="1" source="43" target="65"><mxGeometry relative="1" as="geometry"/></mxCell><mxCell id="72" edge="1" parent="1" source="43" target="66"><mxGeometry relative="1" as="geometry"/></mxCell></root></mxGraphModel>
   ```