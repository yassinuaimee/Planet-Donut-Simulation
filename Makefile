# Makefile
COMPILE = g++ -g -Wall -std=c++11 -O3
CXXFILES = projet.cc simulation.cc gisement.cc base.cc robot.cc message.cc geomod.cc 
OFILES = projet.o simulation.o gisement.o base.o robot.o message.o geomod.o 


projet: projet.o simulation.o geomod.o  message_geomod.o  
					$(COMPILE)  $(OFILES) -o projet
					
simulation.o : simulation.cc simulation.h gisement.h base.h geomod.h
			$(COMPILE) simulation.cc gisement.cc base.cc geomod.cc -o simulation.o
			
gisement.o : gisement.cc gisement.h message.h geomod.h
			$(COMPILE) gisement.cc message.cc geomod.cc -o gisement.o
			
base.o : base.cc base.h gisement.h message.h geomod.h robot.h
			$(COMPILE) base.cc gisement.cc message.cc geomod.cc robot.cc
					
robot.o : robot.cc robot.h gisement.h message.h geomod.h
			$(COMPILE) robot.cc gisement.cc message.cc geomod.cc -o robot.o
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



