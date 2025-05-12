#include <iostream>
using namespace std;
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
class Lifts {

public:
    int start_row, start_col, end_row, end_col;
    int travel_time;
    int departure_time;
    Lifts(int start_row, int start_col, int end_row, int end_col, int travel_time, int departure_time) {
        this->start_row = start_row;
        this->start_col = start_col;
        this->end_row = end_row;
        this->end_col = end_col;
        this->travel_time = travel_time;
        this->departure_time = departure_time;
    }
    Lifts() {
        this->start_row = UNKNOWN;
        this->start_col = UNKNOWN;
        this->end_row = UNKNOWN;
        this->end_col = UNKNOWN;
        this->travel_time = UNKNOWN;
        this->departure_time = UNKNOWN;
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
        return (pos - 1) / 2;
    }
    Node* getParent(int pos) {
        if (pos == 0) return nullptr;
        return &heapArray[(pos - 1) / 2];
    }
    int getLeftChildPos(int pos) {
        return 2 * pos + 1;
    }
    int getRightChildPos(int pos) {
        return 2 * pos + 2;
    }
    void swap(int parent, int child) {
        Node temp = heapArray[parent];
        heapArray[parent] = heapArray[child];
        heapArray[child] = temp;
    }

    void heapifyUp(Node* current, int pos) {
        if (pos != 0) {
            while (pos != 0 && getParent(pos)->distance > current->distance) {
                // cout<<getParent(pos)->distance<<endl;
                swap(getParentPos(pos), pos);
                current = getParent(pos);
                pos = getParentPos(pos);
            }
        }
    }
    void addField(const Node& node) {
        if (heap_size < max_size) {
            heapArray[heap_size++] = node;
            if (heap_size != 1) {
                heapifyUp(&heapArray[heap_size - 1], heap_size - 1);
            }
        }
    }
    void printHeap() {
        for (int i = 0; i < heap_size; i++) {
            cout << heapArray[i].distance << " ";
        }
    }
    void heapifyDown(int parentPos) {
        while (true) {
            int shorter = parentPos;
            int leftChildPos = getLeftChildPos(shorter);
            int rightChildPos = getRightChildPos(shorter);
            if (leftChildPos<heap_size && heapArray[shorter].distance > heapArray[leftChildPos].distance) {
                    shorter = leftChildPos;
            }
            if (rightChildPos<heap_size && heapArray[shorter].distance > heapArray[rightChildPos].distance) {
                    shorter = rightChildPos;
            }
            if (shorter == parentPos) {
                break;
            }
            swap(parentPos, shorter);
            parentPos = shorter;
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
    return row * COLUMNS + col;
}
int calculateMinutes(int A, int B) {
    if (A > B) {
        return A - B + 1;
    }
    else {
        return 1;
    }
}
int calculateMinutesLifts(int currentMinutes, int travel_time, int departure_time) {
    int buffer =  (departure_time - (currentMinutes % departure_time)) % departure_time;
    return(buffer+travel_time+currentMinutes);
}

void addLiftToHeap(MinHeap* heap, int numberOfLifts, int currentRow,  int currentCol, int currentMinutes,Lifts * lifts, bool *hasLift, int COLUMNS) {

    if (hasLift[getPos(currentRow, currentCol, COLUMNS)]) {
        int i=0;
        while (i<numberOfLifts) {
            if (lifts[i].start_col==currentCol && lifts[i].start_row==currentRow) {
                heap->addField(Node(lifts[i].end_row,lifts[i].end_col,calculateMinutesLifts(currentMinutes, lifts[i].travel_time, lifts[i].departure_time)));
            }
            i++;
        }
    }
}
void addNewDefined(int* distance, bool* visited, Node newDefined, int COLUMNS) {
    visited[getPos(newDefined.row, newDefined.col, COLUMNS)] = true;
    distance[getPos(newDefined.row, newDefined.col, COLUMNS)] = newDefined.distance;
}
void addToHeap(int otherPos, int currentPos, int* minutes, bool* defined, int* distance, int otherRow, int otherCol, MinHeap* heap,Lifts * lifts, bool *hasLift, int COLUMNS, int numberOfLifts) {
    if (!defined[otherPos]) {
        // addLiftToHeap(heap, numberOfLifts, otherRow,minutes[otherPos],othe )
        int newMinutes = calculateMinutes(minutes[otherPos], minutes[currentPos]) + distance[currentPos];
        heap->addField(Node(otherRow, otherCol, newMinutes));
        // addLiftToHeap(heap,numberOfLifts, otherRow, otherCol,distance[newMinutes], lifts, hasLift, COLUMNS);
    }
}
void addNeighboursToHeap(bool* defined, int* distance, int* minutes, Node current, int COLUMNS, int ROWS, MinHeap* heap,Lifts * lifts, bool *hasLift,int numberOfLifts) {
    int currentPos = getPos(current.row, current.col, COLUMNS);
    int otherPos;
    //UP
    if (current.row != 0) {
        otherPos = getPos(current.row - 1, current.col, COLUMNS);
        addToHeap(otherPos, currentPos, minutes, defined, distance, current.row - 1, current.col, heap,lifts, hasLift, COLUMNS, numberOfLifts);

    }
    //DOWN
    if (current.row != ROWS - 1) {
        otherPos = getPos(current.row + 1, current.col, COLUMNS);
        addToHeap(otherPos, currentPos, minutes, defined, distance, current.row + 1, current.col, heap,lifts, hasLift, COLUMNS, numberOfLifts);

    }
    //LEFT
    if (current.col != 0) {
        otherPos = getPos(current.row, current.col - 1, COLUMNS);
        addToHeap(otherPos, currentPos, minutes, defined, distance, current.row, current.col - 1,heap,lifts, hasLift, COLUMNS, numberOfLifts);

    }
    //RIGHT
    if (current.col != COLUMNS - 1) {
        otherPos = getPos(current.row, current.col + 1, COLUMNS);
        addToHeap(otherPos, currentPos, minutes, defined, distance, current.row, current.col + 1, heap,lifts, hasLift, COLUMNS, numberOfLifts);

    }
}


int main() {
    int ROWS, COLUMNS, startingRow, startingCol, destinationCol, destinationRow, numberOfLifts;

    cin >> COLUMNS >> ROWS >> startingCol >> startingRow >> destinationCol >> destinationRow >> numberOfLifts;

    MinHeap minHeap(ROWS * COLUMNS);
    Lifts* lifts = new Lifts[numberOfLifts];
    int* minutes = new int[ROWS * COLUMNS];
    int* distance = new int[ROWS * COLUMNS];
    bool* visited = new bool[ROWS * COLUMNS];
    bool* hasLift = new bool[ROWS * COLUMNS];


    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            hasLift[i*COLUMNS + j] = false;
            distance[i * COLUMNS + j] = UNKNOWN;
            visited[i * COLUMNS + j] = false;
        }
    }

    if (numberOfLifts != 0) {
        int start_row, start_col, end_row, end_col, travel_time, departure_time;
        for (int i = 0; i < numberOfLifts; i++) {
            cin >> start_col >> start_row >> end_col >> end_row >> travel_time >> departure_time;
            lifts[i] = Lifts(start_row, start_col, end_row, end_col, travel_time, departure_time);
            hasLift[getPos(start_row, start_col, COLUMNS)] = true;
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cin >> minutes[i * COLUMNS + j];
        }
    }
    distance[getPos(startingRow, startingCol, COLUMNS)] = 0;
    minHeap.addField(Node(startingRow, startingCol, distance[getPos(startingRow, startingCol, COLUMNS)]));
    while (!visited[getPos(destinationRow, destinationCol, COLUMNS)]) {
        Node shortest = minHeap.getRoot();
        if (!visited[getPos(shortest.row, shortest.col, COLUMNS)]) {
            addNewDefined(distance, visited, shortest, COLUMNS);
            addLiftToHeap(&minHeap,numberOfLifts,shortest.row,shortest.col,shortest.distance,lifts,hasLift,COLUMNS);
            addNeighboursToHeap(visited, distance, minutes, shortest, COLUMNS, ROWS, &minHeap,lifts, hasLift, numberOfLifts);

        }
    }
    cout << distance[getPos(destinationRow, destinationCol, COLUMNS)] << endl;

    delete[] minutes;
    delete[] distance;
    delete[] visited;
    delete[] lifts;
    delete[] hasLift;
    return 0;
}