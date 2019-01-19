#ifndef MAZE_H
#define MAZE_H

#include <stdint.h>
/* #include <StackList.h> */

typedef uint16_t uuint ;
typedef uint8_t suint ;
/* typedef struct { */
/*     int xx ; */
/*     int yy ; */
/* } tuple ; */

class Maze {
    uuint maze[16][16] ; 
    suint vWalls[16][17] {{0}}; 
    suint hWalls[17][16] {{0}}; 
    /* StackList<tuple> cstack ; */
    public:
        Maze(suint vv[16][17], suint hh[17][16]) ;
        void setNorthWall(suint x, suint y) ;
        void setSouthWall(suint x, suint y) ;
        void setWestWall(suint x, suint y) ;
        void setEastWall(suint x, suint y) ;
        suint getNorthWall(suint x, suint y) ;
        suint getSouthWall(suint x, suint y) ;
        suint getWestWall(suint x, suint y) ;
        suint getEastWall(suint x, suint y) ;
        void setVal(suint x, suint y, uuint v) ;
        uuint getVal(suint x, suint y) ;
        suint checkCenter(suint x, suint y) ;
        void floodfill(suint x, suint y) ;
        uuint minNeighbor(suint x, suint y, suint& a, suint& b) ;
} ;

#endif
