# Makefile
CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -O3
COMPILE = g++ -g -Wall -std=c++11 -O3 
CXXFILES = projet.cc simulation.cc gisement.cc base.cc robot.cc message.cc geomod.cc 
OFILES = projet.o simulation.o gisement.o base.o robot.o message.o geomod.o 



projet: $(OFILES)
	$(COMPILE) $(OFILES) -o projet
	
projet.o: projet.cc geomod.h simulation.h message.h
	$(COMPILE) -c projet.cc -o projet.o
	
simulation.o: simulation.cc simulation.h gisement.h base.h geomod.h
	$(COMPILE) -c simulation.cc -o simulation.o
	
gisement.o: gisement.cc gisement.h message.h geomod.h
	$(COMPILE) -c gisement.cc -o gisement.o
	
base.o: base.cc base.h robot.h message.h geomod.h gisement.h
	$(COMPILE) -c base.cc -o base.o
	
robot.o: robot.cc robot.h message.h geomod.h gisement.h
	$(COMPILE) -c robot.cc -o robot.o
	
message.o: message.cc message.h
	$(COMPILE) -c message.cc -o message.o
	
geomod.o: geomod.cc geomod.h
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




