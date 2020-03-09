CXX=g++
CXXFLAGS+=-Wall -O3 -std=c++14 -DNDEBUG

all: run ;
	cp run workflows/ranking/ ;
	cp run workflows/mining/ 

debug: CXXFLAGS += -g3 -DDEBUG_MSG
debug: run

run: main.o \
	Utils.o \
	Base/Config.o \
	HRank/HRankSY.o \
	TransitionMatrix/ConstraintMatrix.o \
	TransitionMatrix/TransitionMatrix.o \
	MatrixMultiplier/DynamicOptimizer.o \
	Base/Executor.o \
	MatrixMultiplier/MatrixMultiplier.o \
	Base/FileParser.o
	
	$(CXX) -o $@ $^ -L BaseAlgorithm/ -L TransitionMatrix/ $(CXXFLAGS)

clean_temp:
	rm data/small_relations_2/tmp/*

clean:
	rm *.o
	rm Base/*.o
	rm HRank/*.o
	rm TransitionMatrix/*.o
	rm MatrixMultiplier/*.o
	rm run
	rm workflows/ranking/run
	rm workflows/mining/run
