#include <iostream>
using namespace std;
#define UNKNOWN (-1)

int getPos(int row, int col, int COLUMNS) {
    return row * COLUMNS + col;
}

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
    int start_row, start_col, end_row, end_col, travel_time, departure_time;
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
    int max_size, heap_size, COLUMNS;
    Node* heapArray;
    int* map;
public:
    MinHeap(int max_size, int*map, int COLUMNS) {
        this->map = map;
        this->COLUMNS = COLUMNS;
        this->max_size = max_size;
        heap_size = 0;
        heapArray = new Node[max_size];
    };
    static int getParentPos(int pos) {
        return (pos - 1) / 2;
    }
    Node* getParent(int pos) {
        if (pos == 0) return nullptr;
        return &heapArray[(pos - 1) / 2];
    }
    static int getLeftChildPos(int pos) {
        return 2 * pos + 1;
    }
    static int getRightChildPos(int pos) {
        return 2 * pos + 2;
    }
    void swap(int parent, int child) {
        Node temp = heapArray[parent];
        heapArray[parent] = heapArray[child];
        heapArray[child] = temp;
        map[getPos(heapArray[child].row, heapArray[child].col, COLUMNS)] = child;
        map[getPos(heapArray[parent].row, heapArray[parent].col, COLUMNS)] = parent;
    }
    void heapifyUp(Node* current, int pos) {
        if (pos != 0) {
            while (pos != 0 && getParent(pos)->distance > current->distance) {
                swap(getParentPos(pos), pos);
                current = getParent(pos);
                pos = getParentPos(pos);
            }
        }
    }
    void replaceValue(int pos, int value) {
        heapArray[pos].distance = value;
        heapifyUp(&heapArray[pos],pos);
    }
    void addField(const Node& node) {
        if (heap_size < max_size) {
            heapArray[heap_size++] = node;
            map[getPos(node.row, node.col, COLUMNS)] = heap_size-1;
            if (heap_size > 1) {
                heapifyUp(&heapArray[heap_size - 1], heap_size - 1);
            }
        }
    }
    int getDistance(int pos) const {
        return heapArray[pos].distance;
    }
    void heapifyDown(int parentPos) {
        while (true) {
            int shorter = parentPos;
            int leftChildPos = getLeftChildPos(shorter);
            int rightChildPos = getRightChildPos(shorter);
            if (leftChildPos < heap_size && heapArray[shorter].distance > heapArray[leftChildPos].distance) {
                    shorter = leftChildPos;
            }
            if (rightChildPos < heap_size && heapArray[shorter].distance > heapArray[rightChildPos].distance) {
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
        map[getPos(heapArray[0].row,heapArray[0].col,COLUMNS)] = 0;
        heapArray[heap_size].distance = UNKNOWN;
        heapifyDown(0);
        return root;
    }
};


int calculateMinutes(int A, int B) {
    if (A > B) {
        return A - B + 1;
    }
    return 1;
}
int calculateMinutesLifts(int currentMinutes, int travel_time, int departure_time) {
    int buffer =  (departure_time - (currentMinutes % departure_time)) % departure_time;
    return(buffer+travel_time+currentMinutes);
}

void addLiftToHeap(MinHeap* heap, int *numberOfLifts, int currentRow,  int currentCol, int currentMinutes,Lifts * lifts, bool *hasLift, int COLUMNS) {
    if (hasLift[getPos(currentRow, currentCol, COLUMNS)]) {
        int i=0;
        while (i<*numberOfLifts) {
            if (lifts[i].start_col==currentCol && lifts[i].start_row==currentRow) {
                heap->addField(Node(lifts[i].end_row,lifts[i].end_col,calculateMinutesLifts(currentMinutes, lifts[i].travel_time, lifts[i].departure_time)));
                lifts[i--]=lifts[*numberOfLifts-1];
                *numberOfLifts = *numberOfLifts - 1;
            }
            i++;
        }
    }
}
void addNewDefined(int* distance, bool* visited, Node newDefined, int COLUMNS) {
    visited[getPos(newDefined.row, newDefined.col, COLUMNS)] = true;
    distance[getPos(newDefined.row, newDefined.col, COLUMNS)] = newDefined.distance;
}
void addToHeap(int otherPos, int currentPos, int* minutes, bool* defined, int* distance, int otherRow, int otherCol, MinHeap* heap, int*map, int COLUMNS) {
    if (!defined[otherPos]) {
        int posInHeap = map[getPos(otherRow,otherCol,COLUMNS)];
        int newMinutes = calculateMinutes(minutes[otherPos], minutes[currentPos]) + distance[currentPos];
        if(posInHeap!=UNKNOWN) {
            //if new distance is shorter than the one stored in heap -> replace the value in heap with the shorter one
            if (newMinutes<heap->getDistance(posInHeap)) {
                heap->replaceValue(posInHeap, newMinutes);
            }
        }else{
            heap->addField(Node(otherRow, otherCol, newMinutes));
        }
    }
}
void addNeighboursToHeap(bool* defined, int* distance, int* minutes, Node current, int COLUMNS, int ROWS, MinHeap* heap,int*map) {
    int currentPos = getPos(current.row, current.col, COLUMNS);
    int otherPos;
    //UP
    if (current.row != 0) {
        otherPos = getPos(current.row - 1, current.col, COLUMNS);
        addToHeap(otherPos, currentPos, minutes, defined, distance, current.row - 1, current.col, heap,map, COLUMNS);
    }
    //DOWN
    if (current.row != ROWS - 1) {
        otherPos = getPos(current.row + 1, current.col, COLUMNS);
        addToHeap(otherPos, currentPos, minutes, defined, distance, current.row + 1, current.col, heap,map, COLUMNS);
    }
    //LEFT
    if (current.col != 0) {
        otherPos = getPos(current.row, current.col - 1, COLUMNS);
        addToHeap(otherPos, currentPos, minutes, defined, distance, current.row, current.col - 1,heap,map, COLUMNS);
    }
    //RIGHT
    if (current.col != COLUMNS - 1) {
        otherPos = getPos(current.row, current.col + 1, COLUMNS);
        addToHeap(otherPos, currentPos, minutes, defined, distance, current.row, current.col + 1, heap,map, COLUMNS);
    }
}

void initialize(int ROWS, int COLUMNS, bool* hasLift, bool*visited, int*map, int*distance) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            hasLift[i*COLUMNS + j] = false;
            distance[i * COLUMNS + j] = UNKNOWN;
            visited[i * COLUMNS + j] = false;
            map[i * COLUMNS + j] = UNKNOWN;
        }
    }
}
void getValuesFromInput(int numberOfLifts, int COLUMNS, int ROWS, bool*hasLift, int*minutes, Lifts* lifts) {
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
}
int main() {
    int ROWS, COLUMNS, startingRow, startingCol, destinationCol, destinationRow, numberOfLifts;

    cin >> COLUMNS >> ROWS >> startingCol >> startingRow >> destinationCol >> destinationRow >> numberOfLifts;

    Lifts* lifts = new Lifts[numberOfLifts];  //stores pointers to object of Lifts class
    int* minutes = new int[ROWS * COLUMNS];    //stores minutes/height of each field read from input
    int* distance = new int[ROWS * COLUMNS];    //stores calculated distance from starting field (initialized to -1 at the beginning)
    bool* visited = new bool[ROWS * COLUMNS];    //stores visited fields as true
    bool* hasLift = new bool[ROWS * COLUMNS];    //if field has lift -> marked as true
    int* map = new int[ROWS * COLUMNS];    //stores position in heap of each field

    int *pLiftsAmount = &numberOfLifts;

    initialize(ROWS,COLUMNS,hasLift,visited,map,distance);
    MinHeap minHeap(ROWS * COLUMNS, map, COLUMNS);
    getValuesFromInput(numberOfLifts,COLUMNS,ROWS,hasLift,minutes,lifts);

    distance[getPos(startingRow, startingCol, COLUMNS)] = 0;
    minHeap.addField(Node(startingRow, startingCol, distance[getPos(startingRow, startingCol, COLUMNS)]));

    while (!visited[getPos(destinationRow, destinationCol, COLUMNS)]) {
        Node shortest = minHeap.getRoot();
        if (!visited[getPos(shortest.row, shortest.col, COLUMNS)]) {
            addNewDefined(distance, visited, shortest, COLUMNS);
            addLiftToHeap(&minHeap,pLiftsAmount,shortest.row,shortest.col,shortest.distance,lifts,hasLift,COLUMNS);
            addNeighboursToHeap(visited, distance, minutes, shortest, COLUMNS, ROWS, &minHeap, map);
        }
    }
    cout <<distance[getPos(destinationRow, destinationCol, COLUMNS)] << endl;

    delete[] minutes;
    delete[] distance;
    delete[] visited;
    delete[] lifts;
    delete[] hasLift;
    delete[] map;
    return 0;
}