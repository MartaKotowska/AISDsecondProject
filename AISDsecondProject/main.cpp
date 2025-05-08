#include <iostream>
using namespace std;
#define MAX_ROWS 10000
#define  MAX_COLUMNS 10000
#define EDGE (-1)
#define UNKNOWN (-1)

class Node {
    public:
    int distance, row, col;
    Node(int row, int col, int distance) {
        this->row = row;
        this->col = col;
        this->distance = distance;
    }
    Node() {
        this->row = UNKNOWN;
        this->col = UNKNOWN;
        this->distance = UNKNOWN;
    }
};

class MinHeap {
    int max_size;

    Node* heapArray;
    public:
    int heap_size;
    MinHeap(int max_size) {
        this->max_size = max_size;
        heap_size = 0;
        heapArray = new Node[max_size];
    };
    int getParentPos(int pos) {
        return (pos-1)/2;
    }
    Node* getParent(int pos) {
        if (pos == 0) return nullptr;
        return &heapArray[(pos-1)/2];
    }
    Node* getLeftChild(int pos) {
        if (heapArray[2*pos +1].distance == UNKNOWN) {
            return nullptr;
        }
        return &heapArray[2*pos +1];
    }
    Node* getRightChild(int pos) {
        if (heapArray[2*pos+2].distance == UNKNOWN) {
            return nullptr;
        }
        return &heapArray[2*pos+2];
    }
    void swap(int parent, int child) {
        Node temp = heapArray[parent];
        heapArray[parent] = heapArray[child];
        heapArray[child] = temp;
    }
    // Node* getRoot() {
    //
    // }
    void heapifyUp(Node* current, int pos) {
        if (pos!=0) {
            while (pos!=0 && getParent(pos)->distance > current->distance) {
                cout<<getParent(pos)->distance<<endl;
                    swap(getParentPos(pos),pos);
                    current = getParent(pos);
                    pos = getParentPos(pos);
            }
        }
    }
    void addField(Node *node) {
        if (!(heap_size == max_size)) {
            heapArray[heap_size++] = *node;
            if (heap_size!=1) {
                heapifyUp(&heapArray[heap_size-1],heap_size-1);
            }
        }
    }
    void printHeap(){
        for (int i = 0; i < heap_size; i++) {
            cout << heapArray[i].distance << " ";
        }
    }
};

int getPos(int row, int col, int COLUMNS) {
    return row*COLUMNS + col;
}

int main() {
    int ROWS=5, COLUMNS=5, startingRow,startingCol, destinationCol,destinationRow, numberOfLifts;
    // cin>>COLUMNS>>ROWS>>startingCol>>startingRow>>destinationCol>>destinationRow>>numberOfLifts;

    int* minutes = new int[ROWS * COLUMNS];
    int* dist = new int[ROWS * COLUMNS];
    bool* visited = new bool[ROWS * COLUMNS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            // cin>>minutes[i * COLUMNS + j];
            dist[i * COLUMNS + j] = UNKNOWN;
            visited[i * COLUMNS + j] = false;
        }
    }
    MinHeap minHeap(ROWS*COLUMNS);
    Node* n1 = new Node(1,2,3);
    minHeap.addField(n1);

    minHeap.addField(new Node(1,3,5));
    // minHeap.printHeap();
    minHeap.addField(new Node(1,3,1));
    minHeap.printHeap();
    cout<<"\nnext"<<endl;
    minHeap.addField(new Node(1,3,0));
    minHeap.printHeap();
}