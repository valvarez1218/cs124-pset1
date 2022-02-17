#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <math.h>

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
    string label;
    vector<float> coords;

    // constructor takes as input the node's coordinates
    Vertex(vector<float> c) {
        coords = c;
    }
};

struct Edge {
    float weight;
    vector<Vertex*> vertices;

    // constructor takes as input the edge's connecting nodes and weight
    Edge(vector<Vertex*> v, float w) {
        vertices = v;
        weight = w;
    }
};

struct MinSpanningTree {
    vector<Vertex*> vertices;
    vector<Edge*> edges;
};

// Fibonacci Heap structs
struct node {
    node* parent; // Parent pointer
    node* child; // Child pointer
    node* left; // Pointer to the node on the left
    node* right; // Pointer to the node on the right
    float key; // Value of the node
    int degree; // Degree of the node
    char mark; // Black or white mark of the node
    char c; // Flag for assisting in the Find node function
};

// Creating min pointer as "mini"
struct node* mini = NULL;

// TODO:
// 1. generate all vertices
// 2. generate edges from one vertex in s to all vertices not in s
    // these should result in adding/updating nodes in the fib heap
// 3. add/update nodes in the heap based on generated edges
// 4. extract min value
    // this value needs to have a pointer to the original edge
// 5. add edge to min spanning tree

// EudlidDist takes as input the locations/coordinates of two points and returns
// their Euclidean distance
float EuclidDist(vector<float>, vector<float>);

// Given a spanning tree as input gets the weight of the tree
float GetTreeWeight(MinSpanningTree);

void GenerateVertice();

// Fibonacci Heap implemenation

// Declare an integer for number of nodes in the heap
int no_of_nodes = 0;

// Function to insert a node in heap
void insertion(float val);

// Linking the heap nodes in parent child relationship
void Fibonnaci_link(struct node* ptr2, struct node* ptr1);

// Consolidating the heap
// consolidate root list by continually linking trees until we have at most one binomial tree for a degree k
// called after extract min
void Consolidate();
  
// Function to extract minimum node in the heap
// sort the root list with bucket sort according to increasing degree (number of k children directly below node)
void Extract_min();
  
// Cutting a node in the heap to be placed in the root list
void Cut(struct node* found, struct node* temp);
  
// Recursive cascade cutting function
void Cascase_cut(struct node* temp);
  
// Function to decrease the value of a node in the heap
void Decrease_key(struct node* found, float val);
  
// Function to find the given node
void Find(struct node* mini, float old_val, float val);
  
// Deleting a node from the heap
// we can not cut off a child directly under root without changing its order of k
void Deletion(float val);
  
// Function to display the heap
void display();

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Usage: ./randmst 0 numpoints numtrials dimension\n");
        return -1;
    }

    // hashmap S initialized with 1 vertex
    // hashmap NotS initialized with all other vertices (n-1)
    
    // For each vertex not in S we generate "weights"

    // 

    // Build Fibheap

    // extract the minimum

    long numpoints = strtol(argv[2], nullptr, 0);
    long numtrials = strtol(argv[3], nullptr, 0);
    long dimension = strtol(argv[4], nullptr, 0);

    printf("Number of points: %li \n", numpoints);
    printf("Number of trials: %li \n", numtrials);
    printf("Dimension: %li \n", dimension);

    cout << "Creating an initial heap" << endl;
    insertion(5.);
    insertion(4.1);
    insertion(8.);
  
    // Now we will display the root list of the heap
    display();
  
    // Now we will extract the minimum value node from the heap
    cout << "Extracting min" << endl;
    Extract_min();
    display();
  
    // Now we will decrease the value of node '8' to '7'
    cout << "Decrease value of 8 to 7" << endl;
    Find(mini, 1.5, 7.);
    display();
  
    // Now we will delete the node '7'
    cout << "Delete the node 7" << endl;
    Deletion(7.);
    display();

    return 0;
}

float EuclidDist(vector<float>, vector<float>) {
    return 0;
};

// --- FIBONACCI HEAP IMPLEMENTATION ---- //

void insertion(float val)
{
    struct node* new_node = new node();
    new_node->key = val;
    new_node->degree = 0;
    new_node->mark = 'W';
    new_node->c = 'N';
    new_node->parent = NULL;
    new_node->child = NULL;
    new_node->left = new_node;
    new_node->right = new_node;
    if (mini != NULL) {
        (mini->left)->right = new_node;
        new_node->right = mini;
        new_node->left = mini->left;
        mini->left = new_node;
        if (new_node->key < mini->key)
            mini = new_node;
    }
    else {
        mini = new_node;
    }
    no_of_nodes++;
}

void Fibonnaci_link(struct node* ptr2, struct node* ptr1)
{
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
    if (ptr2->key < (ptr1->child)->key)
        ptr1->child = ptr2;
    ptr1->degree++;
}
// Consolidating the heap
// consolidate root list by continually linking trees until we have at most one binomial tree for a degree k
// called after extract min
void Consolidate()
{
    int temp1;
    float temp2 = (log(no_of_nodes)) / (log(2));
    int temp3 = temp2;
    struct node* arr[temp3+1];
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
            if (ptr1->key > ptr2->key) {
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
                if (arr[j]->key < mini->key)
                    mini = arr[j];
            }
            else {
                mini = arr[j];
            }
            if (mini == NULL)
                mini = arr[j];
            else if (arr[j]->key < mini->key)
                mini = arr[j];
        }
    }
}
  
// Function to extract minimum node in the heap
// sort the root list with bucket sort according to increasing degree (number of k children directly below node)
void Extract_min()
{
    if (mini == NULL)
        cout << "The heap is empty" << endl;
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
                if (x->key < mini->key)
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
    }
}
  
// Cutting a node in the heap to be placed in the root list
void Cut(struct node* found, struct node* temp)
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
void Cascase_cut(struct node* temp)
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
void Decrease_key(struct node* found, float val)
{
    if (mini == NULL)
        cout << "The Heap is Empty" << endl;
  
    if (found == NULL)
        cout << "Node not found in the Heap" << endl;
  
    found->key = val;
  
    struct node* temp = found->parent;
    if (temp != NULL && found->key < temp->key) {
        Cut(found, temp);
        Cascase_cut(temp);
    }
    if (found->key < mini->key)
        mini = found;
}
  
// Function to find the given node
void Find(struct node* mini, float old_val, float val)
{
    struct node* found = NULL;
    node* temp5 = mini;
    temp5->c = 'Y';
    node* found_ptr = NULL;
    if (temp5->key == old_val) {
        found_ptr = temp5;
        temp5->c = 'N';
        found = found_ptr;
        Decrease_key(found, val);
    }
    if (found_ptr == NULL) {
        if (temp5->child != NULL)
            Find(temp5->child, old_val, val);
        if ((temp5->right)->c != 'Y')
            Find(temp5->right, old_val, val);
    }
    temp5->c = 'N';
    found = found_ptr;
}
  
// Deleting a node from the heap
void Deletion(float val)
{
    if (mini == NULL)
        cout << "The heap is empty" << endl;
    else {
  
        // Decreasing the value of the node to 0
        Find(mini, val, 0);
  
        // Calling Extract_min function to
        // delete minimum value node, which is 0
        Extract_min();
        cout << "Key Deleted" << endl;
    }
}
  
// Function to display the heap
void display()
{
    node* ptr = mini;
    if (ptr == NULL)
        cout << "The Heap is Empty" << endl;
  
    else {
        cout << "The root nodes of Heap are: " << endl;
        do {
            cout << ptr->key;
            ptr = ptr->right;
            if (ptr != mini) {
                cout << "-->";
            }
        } while (ptr != mini && ptr->right != NULL);
        cout << endl
             << "The heap has " << no_of_nodes << " nodes" << endl
             << endl;
    }
}