CPP = g++ 
CFLAGS = -Wall -g 
CFILES = main.cpp matrix.cpp shell.cpp
HFILES = matrix.h shell.h
OFILES = main.o matrix.o shell.o
OBJNAME = main

%.o: %.cpp $(HFILES)
	$(CPP) $(CFLAGS) -c $< -o $@

main: $(OFILES)
	@ $(CPP) $(CFLAGS) $(OFILES) -o $(OBJNAME)
clean:
	rm -f *.o
	rm -f main 