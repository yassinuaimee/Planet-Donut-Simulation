# Makefile
COMPILE = g++ -g -Wall -std=c++11 -O2

geomod.o : geomod.cc geomod.h
					$(COMPILE) -c geomod.cc -o fruits.o

message_geomod.o : message_geomod.h message_geomod.cc
					$(COMPILE) -c message_geomod.cc -o message_geomod.o

projet.o : message_geomod.h geomod.h projet.cc
					$(COMPILE) -c projet.cc -o projet.o

projet: geomod.o message_geomod.o projet.o
					$(COMPILE)  geomod.o message_geomod.o projet.o -o projet
