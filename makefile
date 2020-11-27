all: tradutor

trimfunc.o:
	g++ -g -c -std=c++11 trimfunc.cpp

allCaps.o:
	g++ -g -c -std=c++11 allCaps.cpp

generatePre.o:
	g++ -g -c -std=c++11 generatePre.cpp

translate.o:
	g++ -g -c -std=c++11 translate.cpp

tradutor: trimfunc.o allCaps.o generatePre.o translate.o tradutor.cpp
	g++ -Wall -o tradutor trimfunc.o allCaps.o generatePre.o translate.o tradutor.cpp
	rm trimfunc.o allCaps.o generatePre.o translate.o

clean: 
	rm tradutor