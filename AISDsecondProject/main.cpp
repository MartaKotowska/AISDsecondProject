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
                // cout<<getParent(pos)->distance<<endl;
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
            if (leftChildPos>heap_size-1 || rightChildPos>heap_size-1) {
                break;
            }
            if (heapArray[leftChildPos].distance<heapArray[rightChildPos].distance) {
                swap(parentPos,leftChildPos);
                parentPos = leftChildPos;
            }else {
                swap(parentPos,rightChildPos);
                parentPos = rightChildPos;
            }
            leftChildPos = getLeftChildPos(parentPos);
            rightChildPos = getRightChildPos(parentPos);

        }
    }
    Node getRoot() {
        Node root = heapArray[0];
        heapArray[0] = heapArray[--heap_size];
        // cout<<"new root :"<<heapArray[0].distance<<endl;
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
void addToHeap(int otherPos,int currentPos, int* minutes,bool* defined, int*distance,int otherRow, int otherCol, MinHeap* heap) {
    if (!defined[otherPos]) {
        int newMinutes = calculateMinutes(minutes[otherPos],minutes[currentPos])+distance[currentPos];
        heap->addField(new Node(otherRow, otherCol,newMinutes));
    }
}
void addNeighboursToHeap(bool*defined, int* distance, int * minutes,Node current, int COLUMNS, int ROWS, MinHeap* heap) {
    int currentPos = getPos(current.row,current.col, COLUMNS);
    int otherPos;
    //UP
    if (current.row!=0) {
        otherPos = getPos(current.row-1,current.col, COLUMNS);
        addToHeap(otherPos,currentPos,minutes,defined,distance,current.row-1,current.col,heap);

    }
    //DOWN
    if (current.row!=ROWS-1) {
        otherPos = getPos(current.row+1,current.col, COLUMNS);
        addToHeap(otherPos,currentPos,minutes,defined,distance,current.row+1,current.col,heap);

    }
    //LEFT
    if (current.col!=0) {
        otherPos = getPos(current.row,current.col-1, COLUMNS);
        addToHeap(otherPos,currentPos,minutes,defined,distance,current.row,current.col-1,heap);

    }
    //RIGHT
    if (current.col!=COLUMNS-1) {
        otherPos = getPos(current.row,current.col+1, COLUMNS);
        addToHeap(otherPos,currentPos,minutes,defined,distance,current.row,current.col+1,heap);

    }
}
int main() {
    int ROWS, COLUMNS, startingRow,startingCol, destinationCol,destinationRow, numberOfLifts;

    cin>>COLUMNS>>ROWS>>startingCol>>startingRow>>destinationCol>>destinationRow>>numberOfLifts;

    MinHeap minHeap(ROWS*COLUMNS);
    int* minutes = new int[ROWS * COLUMNS];
    int* distance = new int[ROWS * COLUMNS];
    bool* visited = new bool[ROWS * COLUMNS];
    if (numberOfLifts!=0) {

    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cin>>minutes[i * COLUMNS + j];
            distance[i * COLUMNS + j] = UNKNOWN;
            visited[i * COLUMNS + j] = false;
        }
    }
    distance[getPos(startingRow, startingCol, COLUMNS)] = 0;
    // visited[getPos(startingRow, startingCol, COLUMNS)] = true;


    minHeap.addField(new Node(startingRow,startingCol,distance[getPos(startingRow, startingCol, COLUMNS)]));


    while (!visited[getPos(destinationRow, destinationCol, COLUMNS)]) {
        Node shortest = minHeap.getRoot();
        if (!visited[getPos(shortest.row,shortest.col,COLUMNS)]) {
            addNewDefined(distance,visited,shortest,COLUMNS);
            addNeighboursToHeap(visited, distance,minutes,shortest,COLUMNS,ROWS,&minHeap);
            // cout<<"dist\n";
            // for (int i = 0; i < ROWS; i++) {
            //     for (int j = 0; j < COLUMNS; j++) {
            //         cout<<distance[i * COLUMNS + j]<<" ";
            //     }
            //     cout<<endl;
            // }
            // cout<<"heap:\n";
            // minHeap.printHeap();
            // cout<<endl;
        }
    }
    cout<<distance[getPos(destinationRow, destinationCol, COLUMNS)]<<endl;
    // minHeap.addField(new Node(1,3,5));
    // // minHeap.printHeap();
    // minHeap.addField(new Node(1,3,1));
    // minHeap.printHeap();
    // cout<<"\nnext"<<endl;
    // minHeap.addField(new Node(1,3,0));
    // minHeap.printHeap();
    // cout<<"root:\n"<<minHeap.getRoot().distance<<"\n";
    // minHeap.printHeap();
    delete[] minutes;
    delete[] distance;
    delete[] visited;
    return 0;
}