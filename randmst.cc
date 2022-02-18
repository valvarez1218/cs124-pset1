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


// generates random float in [0, 1]
float GenerateRandFloat();

// calculate euclidDist
float EuclidDist(vector<float>, vector<float>);

// Vertex class (should we consider a simpler node class for when dimension is 0?)
struct Vertex {
    // We should label them somehow to make it easier to generate
    //      all edges
    string id;
    vector<float> coords;

    // constructor takes as input the node's coordinates
    Vertex(string i, int dim) {
        if (dim == 0) {
            coords = {};
        } else {
            for (int i = 0; i < dim; i++) {
                coords.push_back(GenerateRandFloat());
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
    vector<pair<Vertex*, float>> heap;

    // swap location of two elements
    void swap(int, int);

    // sift element up
    void siftUp(int);

    // when we call getMin() we add last element to top and sift down
    void siftDown(int);

    // iterate through heap and update weights, indices, etc.
    void update(Vertex, int);
    
    // add element to end of heap and sift up
    void insert(Vertex*, float);

    // remove smallest element from heap
    pair<Vertex*, float> getMin();

    // return whether or not the heap is empty
    bool empty();
};

// Constructor function
MinHeap::MinHeap() {
    heapSize = 0;
}

float runTrial(int numpoints, int dimension) {
    float mstWeight = 0;
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

        pair<Vertex*, float> vToMST = NotS.getMin();

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


    double cpu_time_used;
    clock_t start, end;
    start = clock();

    float TrialsSum = 0;

    for (int i = 0; i < numtrials; i++) {
        TrialsSum += runTrial(numpoints, dimension);
    }

    // printf("MST weight: %f\n", mstWeight);
    printf("%f %li %li %li\n", TrialsSum/numtrials, numpoints, numtrials, dimension);


    end = clock();
    cpu_time_used = ((double)(end - start)) / (CLOCKS_PER_SEC);

    // printf("Finished in %fs\n", cpu_time_used);

    return 0;
}


void MinHeap::swap(int idx1, int idx2) {
    pair<Vertex*, float> elmnt1 = heap[idx1];
    pair<Vertex*, float> elmnt2 = heap[idx2];

    // swap their index values in dictionary
    VertexIndices[elmnt1.first] = idx2;
    VertexIndices[elmnt2.first] = idx1;

    // swap locations in heap vector
    pair<Vertex*, float> buffer = elmnt1;
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
    if (lChild < heapSize && heap[lChild] < heap[idx]) {
        smallest = lChild;
    }
    if (rChild < heapSize && heap[rChild] < heap[smallest]) {
        smallest = rChild;
    }
    // if current idx is not smallest index, bubble down
    if (smallest != idx)
    {
        swap(idx, smallest);
        siftDown(smallest);
    }

}

void MinHeap::update (Vertex SVertex, int dim) {
    for (auto elmnt : VertexIndices) {
        Vertex v = *(elmnt.first);
        int idx = elmnt.second;
        float currWeight = heap[idx].second;
        // cout << v.id << " | " << idx << " | " << currWeight << endl;
        // if dimension zero generate random weight in [0, 1]
        if (dim == 0) {
            float new_weight = GenerateRandFloat();
            if (new_weight < currWeight) {
                heap[idx].second = new_weight;
                siftUp(idx);
            }
        } 
        // if dimension is 2, 3 or 4 use Euclidea distances
        else {
            float new_weight = EuclidDist(v.coords, SVertex.coords);
            if (new_weight < currWeight) {
                heap[idx].second = new_weight;
                siftUp(idx);
            }
        }
    }
}

void MinHeap::insert(Vertex* v, float weight) {
    pair<Vertex*, float> entry = make_pair(v, weight);
    heap.push_back(entry);
    int idx = heapSize;
    VertexIndices[entry.first] = idx;
    siftUp(idx);
    heapSize++;
}

pair<Vertex*, float> MinHeap::getMin () {
    pair<Vertex*, float> toReturn = heap[0];
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
float EuclidDist(vector<float> coords1, vector<float> coords2) {
    if (coords1.size() != coords2.size()) {
        exit;
    }
    float rad = 0;
    for (int i = 0; i < coords1.size(); i++) {
        rad += pow((coords1[i] - coords2[i]), 2);
    }
    return sqrt(rad);
}

float GenerateRandFloat() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    return dist(mt);
}