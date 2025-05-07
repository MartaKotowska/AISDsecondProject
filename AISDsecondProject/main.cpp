#include <iostream>
using namespace std;
#define MAX_ROWS 80
#define  MAX_COLUMNS 90
#define EDGE (-1)
#define UNKNOWN (-1)
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
            minutes = UNKNOWN;
        }
};
int calculateMinutes(int A, int B) {
    if (A>B) {
        return A-B+1;
    } else {
        return 1;
    }
}
void assignNewDistances(int **map, Field***fields, Field* startingPoint) {
    //DOWN
    int currentMinutes, newMinutes;
    if (startingPoint->getRow()!=ROWS-1) {
        currentMinutes = fields[startingPoint->getRow()+1][startingPoint->getCol()]->minutes;
        newMinutes = calculateMinutes(map[startingPoint->getRow()+1][startingPoint->getCol()],startingPoint->minutes);
        if (currentMinutes==UNKNOWN||(currentMinutes > newMinutes)) {
            fields[startingPoint->getRow()+1][startingPoint->getCol()]->minutes = newMinutes;
        }
    }
    //UP
    if (startingPoint->getRow()!=0) {
        currentMinutes = fields[startingPoint->getRow()-1][startingPoint->getCol()]->minutes;
        newMinutes = calculateMinutes(map[startingPoint->getRow()-1][startingPoint->getCol()],startingPoint->minutes);
        if (currentMinutes==UNKNOWN||(currentMinutes > newMinutes)) {
            fields[startingPoint->getRow()-1][startingPoint->getCol()]->minutes = newMinutes;
        }
    }
    //LEFT
    if (startingPoint->getCol()!=0) {
        currentMinutes = fields[startingPoint->getRow()][startingPoint->getCol()-1]->minutes;
        newMinutes = calculateMinutes(map[startingPoint->getRow()][startingPoint->getCol()-1],startingPoint->minutes);
        if (currentMinutes==UNKNOWN||(currentMinutes > newMinutes)) {
            fields[startingPoint->getRow()][startingPoint->getCol()-1]->minutes = newMinutes;
        }
    }
    //RIGHT
    if (startingPoint->getCol()!=COLUMNS-1) {
        currentMinutes = fields[startingPoint->getRow()][startingPoint->getCol()+1]->minutes;
        newMinutes = calculateMinutes(map[startingPoint->getRow()][startingPoint->getCol()+1],startingPoint->minutes);
        if (currentMinutes==UNKNOWN||(currentMinutes > newMinutes)) {
            fields[startingPoint->getRow()][startingPoint->getCol()+1]->minutes = newMinutes;
        }
    }
}
//
Field* returnShortestWayFromStarting(int **map, Field***fields,Field* shortest, Field* current, int length) {
    int minutes1 = UNKNOWN;
    if (current->defined) {
        //DOWN
        if (current->DOWN != EDGE) {
            if (!fields[current->getRow()+1][current->getCol()]->defined) {
                if (fields[current->getRow()+1][current->getCol()]->minutes < length) {
                    length = fields[current->getRow()+1][current->getCol()]->minutes;
                    shortest =fields[current->getRow()+1][current->getCol()];
                }
            }
        }// UP
        if (current->UP != EDGE) {
            if (!fields[current->getRow() - 1][current->getCol()]->defined) {
                if (fields[current->getRow() - 1][current->getCol()]->minutes < length) {
                    length = fields[current->getRow() - 1][current->getCol()]->minutes;
                    shortest = fields[current->getRow() - 1][current->getCol()];
                }
            }
        }

        // LEFT
        if (current->LEFT != EDGE) {
            if (!fields[current->getRow()][current->getCol() - 1]->defined) {
                if (fields[current->getRow()][current->getCol() - 1]->minutes < length) {
                    length = fields[current->getRow()][current->getCol() - 1]->minutes;
                    shortest = fields[current->getRow()][current->getCol() - 1];
                }
            }
        }

        // RIGHT
        if (current->RIGHT != EDGE) {
            if (!fields[current->getRow()][current->getCol() + 1]->defined) {
                if (fields[current->getRow()][current->getCol() + 1]->minutes < length) {
                    length = fields[current->getRow()][current->getCol() + 1]->minutes;
                    shortest = fields[current->getRow()][current->getCol() + 1];
                }
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

    return shortest;
}
void calculateLeft( int** map, Field***fields) {
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
void assignNewDefined(int **map, Field***fields, Field* newDefined) {
 int newMinutes=0;
        if (newDefined->DOWN != EDGE) {
            if (fields[newDefined->getRow()+1][newDefined->getCol()]->defined) {
                newMinutes = fields[newDefined->getRow()+1][newDefined->getCol()]->UP + fields[newDefined->getRow()+1][newDefined->getCol()]->minutes;
            }
        }
        if (newDefined->UP != EDGE) {
            if (fields[newDefined->getRow()-1][newDefined->getCol()]->defined) {
                newMinutes = fields[newDefined->getRow()-1][newDefined->getCol()]->DOWN + fields[newDefined->getRow()-1][newDefined->getCol()]->minutes;
            }
        }
        if (newDefined->LEFT != EDGE) {
            if (fields[newDefined->getRow()][newDefined->getCol()-1]->defined) {
                newMinutes = fields[newDefined->getRow()][newDefined->getCol()-1]->RIGHT + fields[newDefined->getRow()][newDefined->getCol()-1]->minutes;
            }
        }
        if (newDefined->RIGHT != EDGE) {
            if (fields[newDefined->getRow()][newDefined->getCol()+1]->defined) {
                newMinutes = fields[newDefined->getRow()][newDefined->getCol()+1]->LEFT + fields[newDefined->getRow()][newDefined->getCol()+1]->minutes;
            }
        }
    if (newDefined->minutes!=UNKNOWN) {
        if (newDefined->minutes > newMinutes) {
            newDefined->minutes = newMinutes;
        }
    }else {
        newDefined->minutes = newMinutes;
    }
    newDefined->defined = true;

}
bool checkIfAllDefined(Field***fields) {
    for (int i=0; i<ROWS;i++) {
        for (int j=0; j<COLUMNS; j++) {
            if (!fields[i][j]->defined) {
                return false;
            }
        }
    }
    return true;
}
void updateDistances(int **map, Field***fields, Field* newDefined) {
    int newMinutes=0;
    if (newDefined->DOWN != EDGE) {
        if (!fields[newDefined->getRow()+1][newDefined->getCol()]->defined) {
            newMinutes = calculateMinutes(map[newDefined->getRow()+1][newDefined->getCol()], map[newDefined->getRow()][newDefined->getCol()])+newDefined->minutes;
            if (fields[newDefined->getRow()+1][newDefined->getCol()]->minutes!=UNKNOWN) {
                if (newMinutes < fields[newDefined->getRow()+1][newDefined->getCol()]->minutes) {
                    fields[newDefined->getRow()+1][newDefined->getCol()]->minutes = newMinutes;
                }
            } else {
                fields[newDefined->getRow()+1][newDefined->getCol()]->minutes = newMinutes;
            }

        }
    }
    // UP
    if (newDefined->UP != EDGE) {
        if (!fields[newDefined->getRow()-1][newDefined->getCol()]->defined) {
            newMinutes = calculateMinutes(map[newDefined->getRow()-1][newDefined->getCol()], map[newDefined->getRow()][newDefined->getCol()]) + newDefined->minutes;
            if (fields[newDefined->getRow()-1][newDefined->getCol()]->minutes != UNKNOWN) {
                if (newMinutes < fields[newDefined->getRow()-1][newDefined->getCol()]->minutes) {
                    fields[newDefined->getRow()-1][newDefined->getCol()]->minutes = newMinutes;
                }
            } else {
                fields[newDefined->getRow()-1][newDefined->getCol()]->minutes = newMinutes;
            }
        }
    }
    // LEFT
    if (newDefined->LEFT != EDGE) {
        if (!fields[newDefined->getRow()][newDefined->getCol()-1]->defined) {
            newMinutes = calculateMinutes(map[newDefined->getRow()][newDefined->getCol()-1], map[newDefined->getRow()][newDefined->getCol()]) + newDefined->minutes;
            if (fields[newDefined->getRow()][newDefined->getCol()-1]->minutes != UNKNOWN) {
                if (newMinutes < fields[newDefined->getRow()][newDefined->getCol()-1]->minutes) {
                    fields[newDefined->getRow()][newDefined->getCol()-1]->minutes = newMinutes;
                }
            } else {
                fields[newDefined->getRow()][newDefined->getCol()-1]->minutes = newMinutes;
            }
        }
    }
    // RIGHT
    if (newDefined->RIGHT != EDGE) {
        if (!fields[newDefined->getRow()][newDefined->getCol()+1]->defined) {
            newMinutes = calculateMinutes(map[newDefined->getRow()][newDefined->getCol()+1], map[newDefined->getRow()][newDefined->getCol()]) + newDefined->minutes;
            if (fields[newDefined->getRow()][newDefined->getCol()+1]->minutes != UNKNOWN) {
                if (newMinutes < fields[newDefined->getRow()][newDefined->getCol()+1]->minutes) {
                    fields[newDefined->getRow()][newDefined->getCol()+1]->minutes = newMinutes;
                }
            } else {
                fields[newDefined->getRow()][newDefined->getCol()+1]->minutes = newMinutes;
            }
        }
    }

}


int main() {
    int ROWS =9, COLUMNS=8, i, j;

    int** map = (int**)malloc(ROWS * sizeof(int*));
    for (int i = 0; i < ROWS; i++) {
        map[i] = (int*)malloc(COLUMNS * sizeof(int));
    }

    int count = 0;





    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cin >> map[i][j];
        }
    }
    Field *** fields = (Field***)malloc(ROWS * sizeof(Field**));
    for (int i = 0; i < ROWS; i++) {
        fields[i] = (Field**)malloc(COLUMNS * sizeof(Field*));
        for (int j = 0; j < COLUMNS; j++) {
            fields[i][j] = (Field*)malloc(sizeof(Field));
            fields[i][j] = new Field(i, j);

        }

    }
    fields[0][0]->defined = true;
    fields[0][0]->minutes = 0;
    calculateLeft(map, fields);
    Field* f = new Field(ROWS, COLUMNS);
    while (!checkIfAllDefined(fields)) {
        // assignNewDistances(map, fields, fields[0][0]);
        f = returnShortestWayFromStarting(map,fields,fields[0][0],fields[0][0],10000);
        assignNewDefined(map, fields, f);
        updateDistances(map, fields, f);

        // for (int i = 0; i < ROWS; i++) {
        //     for (int j = 0; j < COLUMNS; j++) {
        //         cout<<fields[i][j]->minutes<<" ";
        //     }
        //     cout<<endl;
        // }
    } cout<<fields[7][8]->minutes;
    for (int i = 0; i < ROWS; i++)
        free(map[i]);
    return 0;
}
