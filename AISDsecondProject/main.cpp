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
    int getLeftChildPos(int pos) {
        return 2*pos+1;
    }
    int getRightChildPos(int pos) {
        return 2*pos+2;
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
    void heapifyDown(int parentPos) {
        int leftChildPos = getLeftChildPos(parentPos);
        int rightChildPos = getRightChildPos(parentPos);
        while (heapArray[parentPos].distance>heapArray[leftChildPos].distance || heapArray[parentPos].distance > heapArray[rightChildPos].distance) {
            if (heapArray[leftChildPos].distance<heapArray[rightChildPos].distance) {
                swap(parentPos,leftChildPos);
                parentPos = leftChildPos;
            }else {
                swap(parentPos,rightChildPos);
                parentPos = rightChildPos;
            }
            leftChildPos = getLeftChildPos(parentPos);
            rightChildPos = getRightChildPos(parentPos);
            if (leftChildPos>heap_size-1 || rightChildPos>heap_size-1) {
                break;
            }
        }
    }
    Node getRoot() {
        Node root = heapArray[0];
        heapArray[0] = heapArray[--heap_size];
        cout<<"new root :"<<heapArray[0].distance<<endl;
        cout<<"new  :"<<heapArray[heap_size].distance<<endl;
        heapArray[heap_size].distance = UNKNOWN;
        heapifyDown(0);
        return root;
    }
};

int getPos(int row, int col, int COLUMNS) {
    return row*COLUMNS + col;
}
int calculateMinutes(int A, int B) {
    if (A>B) {
        return A-B+1;
    } else {
        return 1;
    }
}
void addNewDefined(int* distance, bool* visited, Node newDefined, int COLUMNS) {
    visited[getPos(newDefined.row,newDefined.col, COLUMNS)] = true;
    distance[getPos(newDefined.row,newDefined.col, COLUMNS)] = newDefined.distance;
}

void addNeighboursToHeap(bool*defined, int* distance,int * minutes,Node current, int COLUMNS, MinHeap* heap) {
    int newMinutes;
    int currentPos = getPos(current.row,current.col, COLUMNS);
    int otherPos;
    //UP
    if (current.row!=0) {
        otherPos = getPos(current.row-1,current.col, COLUMNS);
        if (!defined[otherPos]) {
            newMinutes = calculateMinutes(minutes[otherPos],minutes[currentPos])+distance[currentPos];
            heap->addField(new Node(current.row-1, current.col,newMinutes));
        }
    }
}
int main() {
    int ROWS=5, COLUMNS=5, startingRow=1,startingCol=0, destinationCol=1,destinationRow=1, numberOfLifts;
    // cin>>COLUMNS>>ROWS>>startingCol>>startingRow>>destinationCol>>destinationRow>>numberOfLifts;

    int* minutes = new int[ROWS * COLUMNS];
    int* distance = new int[ROWS * COLUMNS];
    bool* visited = new bool[ROWS * COLUMNS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            // cin>>minutes[i * COLUMNS + j];
            distance[i * COLUMNS + j] = UNKNOWN;
            visited[i * COLUMNS + j] = false;
        }
    }
    minutes[0]=1;
    minutes[5]=0;
    distance[getPos(startingRow, startingCol, COLUMNS)] = 0;
    visited[getPos(startingRow, startingCol, COLUMNS)] = true;
    MinHeap minHeap(ROWS*COLUMNS);
    minHeap.addField(new Node(startingRow,startingCol,0));
    Node shortest = minHeap.getRoot();
    addNewDefined(distance,visited,shortest,COLUMNS);
    addNeighboursToHeap(visited,distance,minutes,shortest,COLUMNS,&minHeap);
    shortest = minHeap.getRoot();
    addNewDefined(distance,visited,shortest,COLUMNS);
    cout<<shortest.distance<<endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cout<<distance[i * COLUMNS + j]<<" ";
        }
        cout<<endl;
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cout<<visited[i * COLUMNS + j]<<" ";
        }
        cout<<endl;
    }
    // while (!visited[getPos(destinationRow, destinationCol, COLUMNS)]) {
    //     Node shortest = minHeap.getRoot();
    //     addNewDefined(distance,visited,shortest,COLUMNS);
    //
    // }
    // minHeap.addField(new Node(1,3,5));
    // // minHeap.printHeap();
    // minHeap.addField(new Node(1,3,1));
    // minHeap.printHeap();
    // cout<<"\nnext"<<endl;
    // minHeap.addField(new Node(1,3,0));
    // minHeap.printHeap();
    // cout<<"root:\n"<<minHeap.getRoot().distance<<"\n";
    // minHeap.printHeap();
}