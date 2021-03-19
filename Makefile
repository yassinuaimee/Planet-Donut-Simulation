# Makefile
COMPILE = g++ -g -Wall -std=c++11 -O3
CXXFILES = projet.cc geomod.cc message_geomod.cc
OFILES = projet.o geomod.o message_geomod.o


projet: geomod.o  message_geomod.o  projet.o
					$(COMPILE)  geomod.o message_geomod.o projet.o -o projet
					
projet.o : message_geomod.h  geomod.h  projet.cc
					$(COMPILE) -c projet.cc -o projet.o

message_geomod.o : message_geomod.h  message_geomod.cc
					$(COMPILE) -c message_geomod.cc -o message_geomod.o


geomod.o : geomod.cc  geomod.h
					$(COMPILE) -c geomod.cc -o geomod.o
					
					
depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile


clean:
					@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***" 
					@/bin/rm -f *.o *.x *.cc~ *.h~ projet



projet.o: projet.cc geomod.h message_geomod.h
geomod.o: geomod.cc geomod.h
message_geomod.o: message_geomod.cc message_geomod.h
