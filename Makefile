all: shell379

clean:
	@rm -rf shell379 luke timer

timer: thing.cpp
	g++ -std=c++11 thing.cpp -o timer

luke: luke.cpp
	g++ -std=c++11 luke.cpp -o luke
shell379: shell379.cpp operations.cpp operations.h exit.cpp exit.h
	g++ -std=c++11 shell379.cpp operations.cpp operations.h exit.cpp exit.h -o shell379
