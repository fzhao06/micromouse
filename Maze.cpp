#include "Maze.h"

Maze::Maze(suint vv[16][17], suint hh[17][16]) {

	for (int ii = 0 ; ii < 16 ; ii++) {
		vWalls[ii][16] = 1 ;
		vWalls[ii][0] = 1 ;
		hWalls[16][ii] = 1 ;
		hWalls[0][ii] = 1 ;
		for (int jj = 0 ; jj < 16 ; jj++)
			maze[ii][jj] = 1 ;
	}

	for (int ii = 0 ; ii < 2 ; ii++) {
		maze[ii + 7][7] = 0 ;
		maze[ii + 7][8] = 0 ;
	}

    for (int jj =  7 ; jj < 9 ; jj++) {
        floodfill(jj, 6) ;
        floodfill(jj, 9) ;
        floodfill(6, jj) ;
        floodfill(9, jj) ;
    }

	for (int ii = 0 ; ii < 16 ; ii++) {
		for (int jj = 0 ; jj < 16 ; jj++) {
			vWalls[ii][jj] = vv[ii][jj] ;
			hWalls[ii][jj] = hh[ii][jj] ;
		}
	}
}
void Maze::setNorthWall(suint x, suint y) {
    hWalls[x][y] = 1 ;
} 
void Maze::setSouthWall(suint x, suint y) {
    hWalls[x + 1][y] = 1 ;
}
void Maze::setWestWall(suint x, suint y) {
    vWalls[x][y] = 1 ;
}
void Maze::setEastWall(suint x, suint y) {
    vWalls[x][y + 1] = 1 ;
}
suint Maze::getNorthWall(suint x, suint y) {
    return hWalls[x][y] ;
}
suint Maze::getSouthWall(suint x, suint y) {
    return hWalls[x + 1][y] ;
}
suint Maze::getWestWall(suint x, suint y) {
    return vWalls[x][y] ;
}
suint Maze::getEastWall(suint x, suint y) {
    return vWalls[x][y + 1] ;
}
void Maze::setVal(suint x, suint y, uuint v) {
    maze[x][y] = v ;
}
uuint Maze::getVal(suint x, suint y) { 
    return maze[x][y] ;
}
suint Maze::checkCenter(suint x, suint y) {
    if ((x == 7 && (y == 7 || y == 8)) || (x == 8 && (y == 7 || y == 8)))
        return 1 ;
    return 0 ;
}
// update vWalls and hWalls before calling floodfill first time, also push to cstack
void Maze::floodfill(suint x, suint y) {
    suint aa, bb ;
    // may pass min to this function
    uuint min = minNeighbor(x, y, aa, bb) ;

    uuint currVal = getVal(x, y) ;
    if (currVal != UINT16_MAX) {
       if (currVal == min + 1)
           return ; 
       setVal(x, y, min + 1) ;

       if (x < 15 && (getVal(x + 1, y) > 0))
           floodfill(x + 1, y) ;
       if (x > 0 && (getVal(x - 1, y) > 0))
           floodfill(x - 1, y) ;
       if (y < 15 && (getVal(x, y + 1) > 0))
           floodfill(x, y + 1) ;
       if (y > 0 && (getVal(x, y - 1) > 0))
           floodfill(x, y - 1) ;
    }
}
uuint Maze::minNeighbor(suint x, suint y, suint& a, suint& b) {
    uuint min = UINT16_MAX ;
    uuint temp ;
    if (!getNorthWall(x, y)) {
        temp = getVal(x - 1, y) ;
        if (temp < min) {
            min = temp ;
            a = x - 1 ;
            b = y ;
        }
    }
    if (!getSouthWall(x, y)) {
        temp = getVal(x + 1, y) ;
        if (temp < min) {
            min = temp ;
            a = x + 1 ;
            b = y ;
        }
    }
    if (!getWestWall(x, y)) {
        temp = getVal(x, y - 1) ;
        if (temp < min) {
            min = temp ;
            a = x ;
            b = y - 1 ;
        }
    }
    if (!getEastWall(x, y)) {
        temp = getVal(x, y + 1) ;
        if (temp < min) {
            min = temp ;
            a = x ;
            b = y + 1 ;
        }
    }

    return min ;
}
