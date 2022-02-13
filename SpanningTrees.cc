#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>

// =================================================
// USAGE AND ARGUMENT EXPLANATIONS
// =================================================
// Run at command line as follows
//      ./randmst 0 numpoints numtrials dimension

// Each argument corresponds to the following:
// - 0         : default should be 0, we are free to use other values here for our own
//                  testing
// - numpoints : number of points/vertices we will include in our graph
// - numtrials : number of runs to be done
// - dimension : the dimension in which we will select our points/nodes; also determines
//                  how we will assign weights to the edges accordingly
//                  0 - weights assigned at random
//                  2 - weights assigned by 2D Euclidean distance
//                  3 - weights assigned by 3D Euclidean distance
//                  4 - weights assigned by 4D Euclidean distance


// Node/Point class (should we consider a simpler node class for when dimension is 0?)
struct Node {
    // We should label them somehow to make it easier to generate
    //      all edges
    std::string label;
    std::vector<float> coords;
    std::vector<Node*> children;

    // constructor takes as input the node's coordinates
    Node(std::vector<float> c) {
        coords = c;
    }
};

// EudlidDist takes as input the locations/coordinates of two points and returns
//      their Euclidean distance
float EuclidDist(std::vector<float>, std::vector<float>);

// Given a spanning tree as input gets the weight of the tree
float GetTreeWeight();

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("You used %i arguments\n", argc);
        return -1;
    }

    printf("%s\n", argv[0]);
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);
    printf("%s\n", argv[3]);

    return 0;
}

float EuclidDist(std::vector<float>, std::vector<float>) {
    return 0;
};