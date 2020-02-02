all: shell379

clean:
	@rm -rf shell379

timer: thing.cpp
	g++ thing.cpp -o timer

luke: luke.cpp
	g++ luke.cpp -o luke
shell379: shell379.cpp
	g++ shell379.cpp -o shell379