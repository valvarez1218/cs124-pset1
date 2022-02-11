#include <iostream>
#include <vector>
#include <utility>

// =================================================
// USAGE AND ARGUMENTS
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

struct Node {
    std::vector<float> coords;
    std::vector<Node*> children;

    Node(std::vector<float> c) {
        coords = c;
    }
};

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