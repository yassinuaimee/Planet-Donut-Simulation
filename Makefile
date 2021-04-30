OUT = projet
CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -O3
COMPILE = g++ -g -Wall -std=c++11 -O3
CXXFILES = projet.cc simulation.cc gui.cc gisement.cc base.cc robot.cc message.cc geomod.cc graphic.cc
OFILES =   projet.o  simulation.o  gui.o  gisement.o  base.o  robot.o  message.o  geomod.o  graphic.o
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
LGTKMM = `pkg-config gtkmm-3.0 --cflags --libs`


all: $(OUT)
	
geomod.o: geomod.cc geomod.h graphic.h
	$(COMPILE) $(LINKING) -c geomod.cc -o geomod.o $(LINKING)
graphic.o: graphic.cc graphic.h
	$(COMPILE) $(LINKING) -c graphic.cc -o graphic.o $(LINKING)
	
robot.o: robot.cc robot.h message.h geomod.h gisement.h
	$(COMPILE) -c robot.cc -o robot.o
	
message.o: message.cc message.h
	$(COMPILE) -c message.cc -o message.o

gisement.o: gisement.cc gisement.h message.h geomod.h
	$(COMPILE) -c gisement.cc -o gisement.o
	
base.o: base.cc base.h robot.h message.h geomod.h gisement.h
	$(COMPILE) -c base.cc -o base.o

simulation.o: simulation.cc simulation.h gisement.h base.h geomod.h
	$(COMPILE) -c simulation.cc -o simulation.o

gui.o: gui.cc gui.h simulation.h geomod.h graphic.h
	$(COMPILE) $(LINKING) -c gui.cc -o gui.o $(LINKING)

projet.o: projet.cc geomod.h simulation.h message.h
	$(COMPILE) $(LINKING) -c projet.cc -o projet.o $(LINKING)

projet: $(OFILES)
	$(CXX) $(LINKING) $(OFILES) -o projet $(LDLIBS)




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




simulation.o: simulation.cc gisement.h geomod.h base.h robot.h \
  simulation.h message.h constantes.h
gisement.o: gisement.cc gisement.h geomod.h message.h constantes.h
base.o: base.cc base.h robot.h gisement.h geomod.h message.h constantes.h
robot.o: robot.cc robot.h gisement.h geomod.h message.h
message.o: message.cc message.h
