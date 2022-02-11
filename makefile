all: SpanningTrees.cc
	g++ SpanningTrees.cc -o randmst

clean:
	rm randmst