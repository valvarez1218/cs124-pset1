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
    if (argc != 5) {
        printf("Usage: ./randmst 0 numpoints numtrials dimension\n");
        return -1;
    }

    long numpoints = std::strtol(argv[2], nullptr, 0);
    long numtrials = std::strtol(argv[3], nullptr, 0);
    long dimension = std::strtol(argv[4], nullptr, 0);

    printf("Number of points: %li \n", numpoints);
    printf("Number of trials: %li \n", numtrials);
    printf("Dimension: %li \n", dimension);

    return 0;
}

float EuclidDist(std::vector<float>, std::vector<float>) {
    return 0;
};