CXX = g++ 
CXXFLAGS = -Wall -g 
CXXFILES = main_gui.cpp matrix.cpp shell.cpp 
HFILES = matrix.h shell.h
OFILES = main_gui.o matrix.o shell.o 
OBJNAME = main
LIBS = 

CXXFLAGS += `wx-config --cxxflags`
LIBS += `wx-config --libs`

%.o: %.cpp $(HFILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

main: $(OFILES)
	@ $(CXX) $(OFILES) -o $(OBJNAME) $(LIBS)
clean:
	rm -f *.o
	rm -f main 