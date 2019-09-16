
ST_test.out: test.o STnode.o ST.o 
	g++ -o ST_test.out test.o STnode.o ST.o

test.o: test.cpp ST.h
	g++ -c -O3 test.cpp

ST.o: ST.cpp ST.h STnode.h
	g++ -c -O3 ST.cpp

STnode.o: STnode.cpp STnode.h
	g++ -c -O3 STnode.cpp