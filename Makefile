all: shell379

clean:
	@rm -rf shell379 luke timer

timer: thing.cpp
	g++ -std=c++11 thing.cpp -o timer

luke: luke.cpp
	g++ -std=c++11 luke.cpp -o luke
shell379: shell379.cpp
	g++ -std=c++11 shell379.cpp -o shell379