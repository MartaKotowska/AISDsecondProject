#include <iostream>
using namespace std;
#define MAX_ROWS 10000
#define  MAX_COLUMNS 10000
#define EDGE (-1)
#define UNKNOWN (-1)

class Field {
    int row, col;
public:
    bool defined;
    //defined distance
    int distance;
    //read from input
    int minutes;
    Field() {
        this->row = 0;
        this->col = 0;
        defined = false;
        distance = UNKNOWN;
    }
    Field(int row, int col) {
        this->row = row;
        this->col = col;
        defined = false;
        distance = UNKNOWN;
    }
    int getRow() const {
        return row;
    }
    int getCol() const {
        return col;
    }

};
class PriorityQueue {
    public:
    int distance;
    int row, col;
    int numberOfVertices;
    PriorityQueue() {
        distance = UNKNOWN;
        row = 0;
        col = 0;
    }
    PriorityQueue(int row, int col, int distance) {
        this->row = row;
        this->col = col;
        this->distance = distance;
    }
};

int calculateMinutes(int A, int B) {
    if (A>B) {
        return A-B+1;
    } else {
        return 1;
    }
}
int returnPos(int row, int col, int COLUMNS) {
    return row*COLUMNS+col;
}

void calculatingDist(Field * fields, Field*B, Field*A, PriorityQueue* pQueue, int COLUMNS) {
    bool found = false;
    int posInArray =0;
    int distance = 0;
    if (!A->defined) {
        while (!found) {
            if (pQueue[posInArray].distance == UNKNOWN) {
                pQueue[posInArray].row=A->getRow();
                pQueue[posInArray].col=A->getCol();
                pQueue[posInArray].distance = calculateMinutes(A->minutes, B->minutes)+B->distance;
                found = true;
            }
            posInArray++;

        }

    }
}
void calculateNeighbours(Field * fields, Field* starting, PriorityQueue* pQueue, int ROWS, int COLUMNS) {
    //UP
    if (starting->getRow()!=0) {
        calculatingDist(fields, starting, &fields[returnPos(starting->getRow()-1, starting->getCol(), COLUMNS)],pQueue, COLUMNS);
    }
    //DOWN
    if (starting->getRow()!=ROWS-1) {
        calculatingDist(fields, starting, &fields[returnPos(starting->getRow()+1, starting->getCol(), COLUMNS)],pQueue,COLUMNS);
    }
    //LEFT
    if (starting->getCol()!=0) {
        calculatingDist(fields, starting, &fields[returnPos(starting->getRow(), starting->getCol()-1, COLUMNS)],pQueue, COLUMNS);
    }
    //RIGHT
    if (starting->getCol()!=COLUMNS-1) {
        calculatingDist(fields, starting, &fields[returnPos(starting->getRow(), starting->getCol()+1, COLUMNS)],pQueue, COLUMNS);
    }
}
void deleteShortestFromQueue(PriorityQueue* shortest, PriorityQueue* pQueue, int COLUMNS, int current) {
    int next=current+1;
    int row = pQueue[current].row;
    int col = pQueue[current].col;
    while (pQueue[next-1].distance!=UNKNOWN) {
        if ( pQueue[next].row == row && pQueue[next].col == col) {
            next++;
        }
        pQueue[current].row =  pQueue[next].row;
        pQueue[current].col =  pQueue[next].col;
        pQueue[current].distance = pQueue[next].distance;

        current++;
        next++;
    }


}
Field* findShortest(Field * fields, PriorityQueue* pQueue, int COLUMNS) {
    bool found = false;
    int row=pQueue[0].row, col=pQueue[0].col;
    int posInQueue =0;
    int valueOfShortest=pQueue[0].distance;
    int posInArray =0;

    while (!found) {
        // cout<<"dist: "<<pQueue[posInArray].distance<<"row: "<<pQueue[posInArray].row<<" col: "<<pQueue[posInArray].col<<endl;
        posInArray++;

        if (pQueue[posInArray].distance == UNKNOWN) {

            found = true;
            break;
        }
        if (pQueue[posInArray].distance < valueOfShortest) {
            // cout<<"dist: "<<pQueue[posInArray].distance<<"row: "<<pQueue[posInArray].row<<" col: "<<pQueue[posInArray].col<<endl;
            col = pQueue[posInArray].col;
            row = pQueue[posInArray].row;
            posInQueue = posInArray;
            valueOfShortest = pQueue[posInArray].distance;
        }
    }

        fields[row*COLUMNS+col].distance = valueOfShortest;
        fields[row*COLUMNS+col].defined=true;

        deleteShortestFromQueue(&pQueue[posInQueue],pQueue,COLUMNS, posInQueue);

        return & fields[row*COLUMNS+col];

}
bool checkIfAllDefined(Field* fields, int ROWS, int COLUMNS) {
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLUMNS; j++) {
            if (!fields[i * COLUMNS + j].defined) return false;
        }
    }
    return true;
}
    int main() {
        int ROWS, COLUMNS, startingRow,startingCol, destinationCol,destinationRow, numberOfLifts;
        cin>>COLUMNS>>ROWS>>startingCol>>startingRow>>destinationCol>>destinationRow>>numberOfLifts;

        Field* fields = new Field[ROWS*COLUMNS];
        PriorityQueue* pQueue = new PriorityQueue[ROWS*COLUMNS*4];
        for (int i=0; i<ROWS; i++) {
            for (int j=0; j<COLUMNS; j++) {
                new (&fields[i * COLUMNS + j]) Field(i, j);
            }
        }
        for (int i=0; i<ROWS; i++) {
            for (int j=0; j<COLUMNS; j++) {
               cin>>fields[i * COLUMNS + j].minutes;
            }
        }
        fields[startingRow*COLUMNS +startingCol].defined = true;
        fields[startingRow*COLUMNS +startingCol].distance = 0;
        calculateNeighbours(fields,&fields[startingRow*COLUMNS +startingCol],pQueue,ROWS, COLUMNS);
    while (!checkIfAllDefined(fields, ROWS, COLUMNS)) {
        // for (int i=0; i<ROWS; i++) {
        //     for (int j=0; j<COLUMNS; j++) {
        //         cout<<fields[i * COLUMNS + j].distance<<" ";
        //     }
        //     cout<<endl;
        // }cout<<endl;
        Field* shortest =findShortest(fields,pQueue,COLUMNS);

        calculateNeighbours(fields,shortest,pQueue,ROWS, COLUMNS);

    }
    cout<<fields[destinationRow*COLUMNS + destinationCol].distance<<endl;

       delete[] fields;
        return 0;
    }