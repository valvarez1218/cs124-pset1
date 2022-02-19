#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <math.h>
#include <time.h>
#include <unordered_map>
#include <algorithm>
#include <random>

using namespace std;
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

// RAND_MAX 2147483647

int TEST_RUN = 0;
double SOLUTION = 0.0;

// generates random double in [0, 1]
double GenerateRandDouble(double lower=0.0, double upper=1.0);

// calculate euclidDist
double EuclidDist(vector<double>, vector<double>);

// Vertex class (should we consider a simpler node class for when dimension is 0?)
struct Vertex {
    // We should label them somehow to make it easier to generate
    //      all edges
    string id;
    vector<double> coords;

    // constructor takes as input the node's coordinates
    Vertex(string i, int dim) {
        if (dim == 0) {
            coords = {};
        } else {
            for (int i = 0; i < dim; i++) {
                coords.push_back(GenerateRandDouble());
            }
        }
        id = i;
    }
};

// structure for min heap
struct MinHeap {

private:
    // dictionary of vertex : heapIndex pairs
    unordered_map<Vertex*, int> VertexIndices;
    int heapSize;

public:
    MinHeap();
    // vector representing heap
    vector<pair<Vertex*, double>> heap;

    // swap location of two elements
    void swap(int, int);

    // sift element up
    void siftUp(int);

    // when we call getMin() we add last element to top and sift down
    void siftDown(int);

    // iterate through heap and update weights, indices, etc.
    void update(Vertex, int);
    
    // add element to end of heap and sift up
    void insert(Vertex*, double);

    // remove smallest element from heap
    pair<Vertex*, double> getMin();

    // return whether or not the heap is empty
    bool empty();
};

// Constructor function
MinHeap::MinHeap() {
    heapSize = 0;
}

double runTrial(int numpoints, int dimension) {
    double mstWeight = 0;
    // vector S initialized with 1 vertex
    // vector<Vertex*> S;
    Vertex* currSVertex = new Vertex("v0", dimension);

    // MinHeap for vertices not in S
    MinHeap NotS;

    // initialize vertices in S and Not S
    for (int i=1; i < numpoints; i++) {
        string id = "v" + to_string(i);
        Vertex* v = new Vertex(id, dimension);
        NotS.insert(v, 2.0);
    }

    // while NotS is not empty update FibonacciHeap and get minimum edge
    while (NotS.empty() == false) {
        // update heap
        NotS.update(*currSVertex, dimension);

        pair<Vertex*, double> vToMST = NotS.getMin();

        // Add min weight to MST
        // cout << "Weight to be added: " << vToMST.second << endl;
        mstWeight += vToMST.second;
        currSVertex = vToMST.first;
    }

    return mstWeight;
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::printf("Usage: ./randmst 0 numpoints numtrials dimension\n");
        return -1;
    }

    long numpoints = strtol(argv[2], nullptr, 0);
    long numtrials = strtol(argv[3], nullptr, 0);
    long dimension = strtol(argv[4], nullptr, 0);

    if (numpoints <= 0) {
        printf("Number of points must be a positive integer\n");
        return -1;
    }
    if (numtrials <= 0) {
        printf("Number of trials must be a positive integer\n");
        return -1;
    }
    if (dimension != 0 && dimension != 2 && dimension != 3 && dimension != 4) {
        printf("Dimension must be 0, 2, 3 or 4 \n");
        return -1;
    }
    if (strtol(argv[1], nullptr, 0) == 1) {
        TEST_RUN = 1;
    }

    double cpu_time_used;
    clock_t start, end;
    start = clock();

    double TrialsSum = 0.0;

    for (int i = 0; i < numtrials; i++) {
        srand(time(nullptr));
        TrialsSum += runTrial(numpoints, dimension);
    }

    if (TEST_RUN == 1) {
        printf("Running test...\n");
        printf("Solution: %f\n", SOLUTION);
    }

    printf("%f %li %li %li\n", TrialsSum/numtrials, numpoints, numtrials, dimension);

    end = clock();
    cpu_time_used = ((double)(end - start)) / (CLOCKS_PER_SEC);

    int h = (cpu_time_used/3600); 
	int m = (cpu_time_used -(3600*h))/60;
	float s = (cpu_time_used -(3600.*h)-(m*60.));

    printf("Finished in %d hours, %d minutes, and %f seconds\n",h,m,s);

    return 0;
}


void MinHeap::swap(int idx1, int idx2) {
    pair<Vertex*, double> elmnt1 = heap[idx1];
    pair<Vertex*, double> elmnt2 = heap[idx2];

    // swap their index values in dictionary
    VertexIndices[elmnt1.first] = idx2;
    VertexIndices[elmnt2.first] = idx1;

    // swap locations in heap vector
    pair<Vertex*, double> buffer = elmnt1;
    heap[idx1] = elmnt2;
    heap[idx2] = buffer;
}

void MinHeap::siftUp (int idx) {
    // if we are at root we're done
    if (idx == 0) {
        return;
    }
    int parIdx = (idx-1)/2;
    // if weight is less than parent's weight bubble up
    if (heap[idx].second < heap[parIdx].second) {
        swap(idx, parIdx);
        siftUp(parIdx);
    }
}

void MinHeap::siftDown (int idx) {
    int lChild = (2*idx) + 1;
    int rChild = (2*idx) + 2;

    int smallest = idx;
    // find smallest value index
    if (lChild < heapSize && heap[lChild].second < heap[idx].second) {
        smallest = lChild;
    }
    if (rChild < heapSize && heap[rChild].second < heap[smallest].second) {
        smallest = rChild;
    }
    // if current idx is not smallest index, bubble down
    if (smallest != idx)
    {
        swap(idx, smallest);
        siftDown(smallest);
    }

}

// This is where Primms algorithm is implemented
void MinHeap::update (Vertex SVertex, int dim) {
    // iterate through all vertices in heap
    for (auto elmnt : VertexIndices) {
        Vertex v = *(elmnt.first);
        int idx = elmnt.second;
        double currWeight = heap[idx].second;
        // cout << "Vertex: " << v.id << " | Index: " << idx << " | Current Weight: " << currWeight << endl;
        // if dimension zero generate random weight in [0, 1]
        if (dim == 0) {
            double new_weight = 0;
            // If we are running a test generate small and large weights deterministically
            if (TEST_RUN == 1) {
                if (SVertex.id == "v0") {
                    new_weight = GenerateRandDouble(0.0, 0.2);
                    SOLUTION+=new_weight;
                } else {
                    new_weight = GenerateRandDouble(0.3, 1.0);
                }
            }
            else {
                new_weight = GenerateRandDouble();
                // cout << "Generated weight: " << new_weight << endl;
            }
            if (new_weight < currWeight) {
                heap[idx].second = new_weight;
                siftUp(idx);
            }
            // cout << "New Weight of " << v.id << ": " << heap[VertexIndices[elmnt.first]].second << endl;
        }
        // if dimension is 2, 3 or 4 use Euclidea distances
        else {
            double new_weight = EuclidDist(v.coords, SVertex.coords);
            if (new_weight < currWeight) {
                heap[idx].second = new_weight;
                siftUp(idx);
            }
        }
    }
}

void MinHeap::insert(Vertex* v, double weight) {
    pair<Vertex*, double> entry = make_pair(v, weight);
    heap.push_back(entry);
    int idx = heapSize;
    VertexIndices[entry.first] = idx;
    siftUp(idx);
    heapSize++;
}

pair<Vertex*, double> MinHeap::getMin () {
    // for (auto elmnt : heap) {
    //     cout << elmnt.first->id << " | " << elmnt.second << endl;;
    // }
    // cout << "MINIMUM: " << heap[0].first->id << ": " << heap[0].second << endl;
    pair<Vertex*, double> toReturn = heap[0];
    heap[0] = heap[heapSize-1];
    siftDown(0);
    VertexIndices.erase(toReturn.first);
    heapSize--;
    return toReturn;
}

bool MinHeap::empty() {
    return heapSize == 0;
}

// EudlidDist takes as input the locations/coordinates of two points and returns
// their Euclidean distance
double EuclidDist(vector<double> coords1, vector<double> coords2) {
    if (coords1.size() != coords2.size()) {
        exit(0);
    }
    double rad = 0;
    for (int i = 0; i < coords1.size(); i++) {
        rad += pow((coords1[i] - coords2[i]), 2);
    }
    return sqrt(rad);
}

double GenerateRandDouble(double lower, double upper) {
    // double random = double(rand())/RAND_MAX;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double> dist(0.000, 1.000);
    double random = dist(generator);
    // cout << random << endl;

    if (TEST_RUN == 1) {
        if (random <= lower) {
            random += lower;
        } else if (random >= upper) {
            random = random * upper;
        }
    }
    
    // return random;
    return random;
}