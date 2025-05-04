#include <iostream>
using namespace std;
#define ROWS 2
#define  COLUMNS 2
#define EDGE -1
class Field {
        int row, col;
    public:
    bool defined;
    int minutes;
    int UP, DOWN,LEFT, RIGHT;
        int getRow() {
            return row;
        }
        int getCol() {
            return col;
        }
        Field(int row, int col) {
            this->row = row;
            this->col = col;
            defined = false;
            minutes = -1;
        }
};
int calculateMinutes(int A, int B) {
    if (A>B) {
        return A-B+1;
    } else {
        return 1;
    }
}
void assignNewDistances(int (*map)[COLUMNS], Field* (*fields)[COLUMNS], Field* startingPoint) {
    //DOWN
    int currentMinutes, newMinutes;
    if (startingPoint->getRow()!=ROWS-1) {
        currentMinutes = fields[startingPoint->getRow()+1][startingPoint->getCol()]->minutes;
        newMinutes = calculateMinutes(map[startingPoint->getRow()+1][startingPoint->getCol()],startingPoint->minutes);
        if (currentMinutes==-1||(currentMinutes > newMinutes)) {
            fields[startingPoint->getRow()+1][startingPoint->getCol()]->minutes = newMinutes;
        }
    }
    //UP
    if (startingPoint->getRow()!=0) {
        currentMinutes = fields[startingPoint->getRow()-1][startingPoint->getCol()]->minutes;
        newMinutes = calculateMinutes(map[startingPoint->getRow()-1][startingPoint->getCol()],startingPoint->minutes);
        if (currentMinutes==-1||(currentMinutes > newMinutes)) {
            fields[startingPoint->getRow()-1][startingPoint->getCol()]->minutes = newMinutes;
        }
    }
    //LEFT
    if (startingPoint->getCol()!=0) {
        currentMinutes = fields[startingPoint->getRow()][startingPoint->getCol()-1]->minutes;
        newMinutes = calculateMinutes(map[startingPoint->getRow()][startingPoint->getCol()-1],startingPoint->minutes);
        if (currentMinutes==-1||(currentMinutes > newMinutes)) {
            fields[startingPoint->getRow()][startingPoint->getCol()-1]->minutes = newMinutes;
        }
    }
    //RIGHT
    if (startingPoint->getCol()!=COLUMNS-1) {
        currentMinutes = fields[startingPoint->getRow()][startingPoint->getCol()+1]->minutes;
        newMinutes = calculateMinutes(map[startingPoint->getRow()][startingPoint->getCol()+1],startingPoint->minutes);
        if (currentMinutes==-1||(currentMinutes > newMinutes)) {
            fields[startingPoint->getRow()][startingPoint->getCol()+1]->minutes = newMinutes;
        }
    }
}
//
Field* returnShortestWayFromStarting(int (*map)[COLUMNS], Field* (*fields)[COLUMNS],Field* shortest, Field* current, int length) {
    int minutes1 = -1;
    if (current->defined) {
        //DOWN
        if (current->DOWN != EDGE) {
            if (!fields[current->getRow()+1][current->getCol()]->defined) {
                // minutes1 = calculateMinutes(fields[current->getRow()+1][current->getCol()]->minutes,current->minutes);
                if (current->DOWN < length) {
                    length = current->DOWN;
                    shortest =fields[current->getRow()+1][current->getCol()];
                }
            }
        }
        if (current->UP != EDGE) {
            if (!fields[current->getRow()-1][current->getCol()]->defined) {
                minutes1 = calculateMinutes(fields[current->getRow()-1][current->getCol()]->minutes,current->minutes);
                if (current->UP < length) {
                    length = current->UP;
                    shortest =fields[current->getRow()-1][current->getCol()];
                }
            }
        }
        if (current->LEFT != EDGE) {
            if (!fields[current->getRow()][current->getCol()-1]->defined) {
                minutes1 = calculateMinutes(fields[current->getRow()][current->getCol()-1]->minutes,current->minutes);
                if (current->LEFT < length) {
                    length = current->LEFT;
                    shortest =fields[current->getRow()][current->getCol()-1];
                }
            }
        }
        if (current->RIGHT != EDGE) {
            if (!fields[current->getRow()][current->getCol()+1]->defined) {
                minutes1 = calculateMinutes(fields[current->getRow()][current->getCol()+1]->minutes,current->minutes);
                if (current->RIGHT < length) {
                    length = current->RIGHT;
                    shortest =fields[current->getRow()][current->getCol()+1];
                }
            }
        }
        if (!(current->getRow()==ROWS-1 && current->getCol()==COLUMNS-1)) {
            if (current->getCol()!=COLUMNS-1) {
               shortest =  returnShortestWayFromStarting(map, fields, shortest,fields[current->getRow()][current->getCol()+1], length);
            } else {
                if (current->getRow()!=ROWS-1) {
                    shortest = returnShortestWayFromStarting(map, fields, shortest,fields[current->getRow()+1][0], length);
                }
            }
        }
    }
    return shortest;
}
void calculateLeft( int (*map)[COLUMNS], Field* (*fields)[COLUMNS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (i!=0) {
                fields[i][j]->UP = calculateMinutes(map[i-1][j],map[i][j]);
            }else {
                fields[i][j]->UP = EDGE;
            }

            if (i!=ROWS-1) {
                fields[i][j]->DOWN = calculateMinutes(map[i+1][j],map[i][j]);
            }else {
                fields[i][j]->DOWN = EDGE;
            }

            if (j!=0) {
                fields[i][j]->LEFT = calculateMinutes(map[i][j-1],map[i][j]);
            }else {
                fields[i][j]->LEFT = EDGE;
            }

            if (j!=COLUMNS-1) {
                fields[i][j]->RIGHT = calculateMinutes(map[i][j+1],map[i][j]);
            }else {
                fields[i][j]->RIGHT = EDGE;
            }

        }
    }
}
int main() {
    int map[ROWS][COLUMNS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cin >> map[i][j];
        }
    }
    Field *fields[ROWS][COLUMNS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            fields[i][j] = new Field(i, j);
            cout<<fields[i][j]->minutes;
        }
        cout<<endl;
    }
    fields[0][0]->defined = true;
    fields[0][0]->minutes = 0;
    calculateLeft(map, fields);
    // assignNewDistances(map, fields, fields[0][0]);
   Field* f = returnShortestWayFromStarting(map,fields,fields[0][0],fields[0][0],10000);
    f->defined = true;
    cout<<"row :"<<f->getRow()<<endl<<f->getCol()<<endl;
    f = returnShortestWayFromStarting(map,fields,fields[0][0],fields[0][0],10000);
    f->defined = true;
    cout<<"row :"<<f->getRow()<<endl<<f->getCol()<<endl;
    f = returnShortestWayFromStarting(map,fields,fields[0][0],fields[0][0],10000);
    f->defined = true;
    cout<<"row :"<<f->getRow()<<endl<<f->getCol()<<endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cout<<fields[i][j]->defined<<" ";
        }
        cout<<endl;
    }
    return 0;
}