#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <math.h>
#include <unordered_map>
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
            coords = {1.0, 1.0};
        }
        id = i;
    }
};

// Fibonacci Heap structs
struct node {
    node* parent; // Parent pointer
    node* child; // Child pointer
    node* left; // Pointer to the node on the left
    node* right; // Pointer to the node on the right
    float weight; // Value of the node
    string id; // ID of vertex this represents
    int degree; // Degree of the node
    char mark; // Black or white mark of the node
    char c; // Flag for assisting in the Find node function
};

// Creating min pointer as "mini"
node* mini = NULL;

// TODO:
// 1. generate all vertices
// 2. generate edges from one vertex in s to all vertices not in s
    // these should result in adding/updating nodes in the fib heap
// 3. add/update nodes in the heap based on generated edges
// 4. extract min value
    // this value needs to have a pointer to the original edge
// 5. add edge to min spanning tree

// Given a spanning tree as input gets the weight of the tree
float GetTreeWeight();

// generates random float in [0, 1]
float GenerateRandFloat();

// calculate euclidDist
float EuclidDist(vector<float>, vector<float>);

// generates a single vertex, giving it an ID and (eventually) coordinates
// TODO: generate coordinates
// Vertex GenerateVertex(string, int);

// takes NotS, current vertex in S (call it v), dimension
//      iterate through NotS
//          for each node in NotS get edge weight to v
//          update FibHeap depending on edge weight
void OptimizeHeap(unordered_map<string, Vertex*>, Vertex, int);

// Fibonacci Heap implemenation

// Declare an integer for number of nodes in the heap
int no_of_nodes = 0;

// Function to insert a node in heap
void insertion(float weight, string id);

// Linking the heap nodes in parent child relationship
void Fibonnaci_link(node* ptr2, node* ptr1);

// Consolidating the heap
// consolidate root list by continually linking trees until we have at most one binomial tree for a degree k
// called after extract min
void Consolidate();
  
// Function to extract minimum node in the heap
// sort the root list with bucket sort according to increasing degree (number of k children directly below node)
node* Extract_min();
  
// Cutting a node in the heap to be placed in the root list
void Cut(node* found, node* temp);
  
// Recursive cascade cutting function
void Cascase_cut(node* temp);
  
// Function to decrease the value of a node in the heap
void Decrease_weight(node* found, float val);
  
// Function to find the given node
void Find(node* mini, float old_val, float val);
  
  
// Function to display the heap
void display();

int main(int argc, char** argv) {
    if (argc != 5) {
        std::printf("Usage: ./randmst 0 numpoints numtrials dimension\n");
        return -1;
    }

    long numpoints = strtol(argv[2], nullptr, 0);
    long numtrials = strtol(argv[3], nullptr, 0);
    long dimension = strtol(argv[4], nullptr, 0);

    float mstWeight = 0;

    // vector S initialized with 1 vertex
    // vector<Vertex*> S;
    Vertex* currSVertex = new Vertex("v0", dimension);

    // hashmap NotS initialized with all other vertices (n-1)
    unordered_map<string, Vertex*> NotS;

    // initialize vertices in S and Not S
    for (int i=1; i < numpoints; i++) {
        string id = "v" + to_string(i);
        Vertex* v = new Vertex(id, dimension);
        NotS[id] = v;
    }

    // initialize fibonacci heap
    for (auto entry : NotS) {
        Vertex notSV = *(entry.second);
        // cout << notSV.id << endl;
        insertion(GenerateRandFloat(), notSV.id);
    }

    // while NotS is not empty update FibonacciHeap and get minimum edge
    while (NotS.empty() == false) {
        // pass in current version of NotS, last element in S (most recently added vertex) and dimension
        OptimizeHeap(NotS, *currSVertex, dimension);
        // get minimum FibNode
        node* minimum = Extract_min();
        // If the FibHeap is empty break out
        if (minimum == NULL) {
            break;
        }
        // Add min weight to MST
        mstWeight += minimum->weight;
        // Vertex that gets connected to MST from Not S
        Vertex* vToMST = NotS[minimum->id];
        currSVertex = vToMST;
        NotS.erase(vToMST->id);
    }

    printf("MST weight: %f\n", mstWeight);

    return 0;
}


// Vertex GenerateVertex(string id, int dim) {
//     if (dim == 0) {
//         Vertex v(id);
//         return v;
//     }
//     else {
//         vector<float> coord(0, 0);
//         Vertex v(id, coord);
//         return v;
//     }
// }

// EudlidDist takes as input the locations/coordinates of two points and returns
// their Euclidean distance
float EuclidDist(vector<float>, vector<float>) {
    return 0;
}

float GenerateRandFloat() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    double doesntmatter = dist(mt);
    return (float)rand()/(float)(RAND_MAX/1.0); //
}

// NotS: vertices currently not in tree
// SVertex: vertex in S we are currently creating edge weights to
// dim: dimension is necessary to calculate Euclidean distance between vertices
void OptimizeHeap(unordered_map<string, Vertex*> NotS, Vertex SVertex, int dim) {
    for (auto entry : NotS) {
        Vertex v = *(entry.second);
        // if dimension is zero generate random weight in [0, 1]
        if (dim == 0) {
            float weight = GenerateRandFloat();
            printf("%f\n", weight);
        }
    }
}

// --- FIBONACCI HEAP IMPLEMENTATION ---- //

void insertion(float weight, string id) {
    node* new_node = new node();
    new_node->weight = weight;
    new_node->id = id;
    new_node->degree = 0;
    new_node->mark = 'W';
    new_node->c = 'N';
    new_node->parent = NULL;
    new_node->child = NULL;
    new_node->left = new_node;
    new_node->right = new_node;
    // if this is the first node in our FibHeap
    if (mini == NULL) {
        mini = new_node;
    }
    // If we already have nodes in our FibHeap
    else {
        (mini->left)->right = new_node;
        new_node->right = mini;
        new_node->left = mini->left;
        mini->left = new_node;
        if (new_node->weight < mini->weight)
            mini = new_node;

    }
    no_of_nodes++;
    printf("%i\n", no_of_nodes);
}

void Fibonnaci_link(node* ptr2, node* ptr1) {
    (ptr2->left)->right = ptr2->right;
    (ptr2->right)->left = ptr2->left;
    if (ptr1->right == ptr1)
        mini = ptr1;
    ptr2->left = ptr2;
    ptr2->right = ptr2;
    ptr2->parent = ptr1;
    if (ptr1->child == NULL)
        ptr1->child = ptr2;
    ptr2->right = ptr1->child;
    ptr2->left = (ptr1->child)->left;
    ((ptr1->child)->left)->right = ptr2;
    (ptr1->child)->left = ptr2;
    if (ptr2->weight < (ptr1->child)->weight)
        ptr1->child = ptr2;
    ptr1->degree++;
}
// Consolidating the heap
// consolidate root list by continually linking trees until we have at most one binomial tree for a degree k
// called after extract min
void Consolidate() {
    int temp1;
    float temp2 = (log(no_of_nodes)) / (log(2));
    int temp3 = temp2;
    node* arr[temp3+1];
    for (int i = 0; i <= temp3; i++)
        arr[i] = NULL;
    node* ptr1 = mini;
    node* ptr2;
    node* ptr3;
    node* ptr4 = ptr1;
    do {
        ptr4 = ptr4->right;
        temp1 = ptr1->degree;
        while (arr[temp1] != NULL) {
            ptr2 = arr[temp1];
            if (ptr1->weight > ptr2->weight) {
                ptr3 = ptr1;
                ptr1 = ptr2;
                ptr2 = ptr3;
            }
            if (ptr2 == mini)
                mini = ptr1;
            Fibonnaci_link(ptr2, ptr1);
            if (ptr1->right == ptr1)
                mini = ptr1;
            arr[temp1] = NULL;
            temp1++;
        }
        arr[temp1] = ptr1;
        ptr1 = ptr1->right;
    } while (ptr1 != mini);
    mini = NULL;
    for (int j = 0; j <= temp3; j++) {
        if (arr[j] != NULL) {
            arr[j]->left = arr[j];
            arr[j]->right = arr[j];
            if (mini != NULL) {
                (mini->left)->right = arr[j];
                arr[j]->right = mini;
                arr[j]->left = mini->left;
                mini->left = arr[j];
                if (arr[j]->weight < mini->weight)
                    mini = arr[j];
            }
            else {
                mini = arr[j];
            }
            if (mini == NULL)
                mini = arr[j];
            else if (arr[j]->weight < mini->weight)
                mini = arr[j];
        }
    }
}
  
// Function to extract minimum node in the heap
// sort the root list with bucket sort according to increasing degree (number of k children directly below node)
node* Extract_min() {
    if (mini == NULL) {
        std::cout << "The heap is empty" << endl;
        return NULL;
    }
    else {
        node* temp = mini;
        node* pntr;
        pntr = temp;
        node* x = NULL;
        if (temp->child != NULL) {
            x = temp->child;
            do {
                pntr = x->right;
                (mini->left)->right = x;
                x->right = mini;
                x->left = mini->left;
                mini->left = x;
                if (x->weight < mini->weight)
                    mini = x;
                x->parent = NULL;
                x = pntr;
            } while (pntr != temp->child);
        }
        (temp->left)->right = temp->right;
        (temp->right)->left = temp->left;
        mini = temp->right;
        if (temp == temp->right && temp->child == NULL)
            mini = NULL;
        else {
            mini = temp->right;
            Consolidate();
        }
        no_of_nodes--;
        printf("Nodes left: %i\n", no_of_nodes);
        return temp;
    }
}
  
// Cutting a node in the heap to be placed in the root list
void Cut(node* found, node* temp)
{
    if (found == found->right)
        temp->child = NULL;
  
    (found->left)->right = found->right;
    (found->right)->left = found->left;
    if (found == temp->child)
        temp->child = found->right;
  
    temp->degree = temp->degree - 1;
    found->right = found;
    found->left = found;
    (mini->left)->right = found;
    found->right = mini;
    found->left = mini->left;
    mini->left = found;
    found->parent = NULL;
    found->mark = 'B';
}
  
// Recursive cascade cutting function
void Cascase_cut(node* temp)
{
    node* ptr5 = temp->parent;
    if (ptr5 != NULL) {
        if (temp->mark == 'W') {
            temp->mark = 'B';
        }
        else {
            Cut(temp, ptr5);
            Cascase_cut(ptr5);
        }
    }
}
  
// Function to decrease the value of a node in the heap
void Decrease_weight(node* found, float new_weight)
{
    if (mini == NULL)
        std::cout << "The Heap is Empty" << endl;
  
    if (found == NULL)
        std::cout << "Node not found in the Heap" << endl;
  
    found->weight = new_weight;
  
    node* temp = found->parent;
    if (temp != NULL && found->weight < temp->weight) {
        Cut(found, temp);
        Cascase_cut(temp);
    }
    if (found->weight < mini->weight)
        mini = found;
}

// Function to find the given node
void Find(node* mini, string id, float new_weight)
{
    node* found = NULL;
    node* temp5 = mini;
    temp5->c = 'Y';
    node* found_ptr = NULL;
    if (temp5->id == id) {
        found_ptr = temp5;
        temp5->c = 'N';
        found = found_ptr;
        float old_weight = temp5->weight;
        if (new_weight < old_weight) {
            Decrease_weight(found, new_weight);
        }
    }
    if (found_ptr == NULL) {
        if (temp5->child != NULL)
            Find(temp5->child, id, new_weight);
        if ((temp5->right)->c != 'Y')
            Find(temp5->right, id, new_weight);
    }
    temp5->c = 'N';
    found = found_ptr;
}
  
// Function to display the heap
void display()
{
    node* ptr = mini;
    if (ptr == NULL)
        std::cout << "The Heap is Empty" << endl;
  
    else {
        std::cout << "The root nodes of Heap are: " << endl;
        do {
            std::cout << ptr->weight;
            ptr = ptr->right;
            if (ptr != mini) {
                std::cout << "-->";
            }
        } while (ptr != mini && ptr->right != NULL);
        std::cout << endl
             << "The heap has " << no_of_nodes << " nodes" << endl
             << endl;
    }
}