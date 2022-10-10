#! /bin/bash

echo "#define DBNAME \"${1}\""  >  ./include/ServerConfig.h
echo "#define ROLE   \"${2}\""  >> ./include/ServerConfig.h 
echo "#define PASS   \"${3}\""  >> ./include/ServerConfig.h
echo "#define DBHOST \"${4}\""  >> ./include/ServerConfig.h
echo "#define DBPORT   ${5}"    >> ./include/ServerConfig.h