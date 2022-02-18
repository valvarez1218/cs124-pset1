all: randmst.cc
	g++ --std=c++11 randmst.cc -o randmst

clean:
	rm randmst