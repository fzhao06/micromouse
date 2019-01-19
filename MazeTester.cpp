#include "Maze.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std ;

int main(int argc, char *argv[]) {

    suint vWalls[16][17] ;
    suint hWalls[17][16] ;

    ifstream file (argv[1]) ;

    string line ;
    if (file.is_open()) {
        int vcount = 0, hcount = 0 ;
        while (getline(file, line)) {
            if (line[0] == '|') {
                for (int ii = 0 ; ii < 33 ; ii += 2) {
                    if (line[ii] == '|')
                        vWalls[vcount][ii/2] = 1 ; 
                    else
                        vWalls[vcount][ii/2] = 0 ;
                    
                }
                vcount++ ;
            }
            else if (line[0] == '+') {
                for (int ii = 1 ; ii < 32 ; ii += 2) {
                    if (line[ii] == '-')
                        hWalls[hcount][ii/2] = 1 ; 
                    else
                        hWalls[hcount][ii/2] = 0 ;
                    
                }
                hcount++ ;
            }
        }
        file.close() ;
		cout << "Input is done" << endl;
    }
	/* for (int jj = 0 ; jj < 16 ; jj++) { */
	/* 		string line1, line2 ; */
	/* 		for (int ii = 0 ; ii < 16 ; ii++) { */
	/* 			line1 += "+" ; */
	/* 			if (hWalls[jj][ii]) */
	/* 				line1 += "-" ; */
	/* 			else */
	/* 				line1 += " " ; */
	/* 			if (vWalls[jj][ii]) */
	/* 				line2 += "|" ; */
	/* 			else */
	/* 				line2 += " " ; */
	/* 			line2 += " " ; */
	/* 		} */
	/* 		line1 += "+\n" ; */
	/* 		line2 += "|\n" ; */
	/* 		cout << line1 << line2 ; */
	/* 	} */
	/* cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" ; */
	/* cout << endl ; */
	/* return 0 ; */

    Maze mm = Maze(vWalls, hWalls) ;
    suint x = 15, y = 0 ;
    while (!mm.checkCenter(x, y)) {
		/* cout << "New Graph" << endl ; */
		/* for (int jj = 0 ; jj < 16 ; jj++) { */
		/* 	string line1, line2 ; */
		/* 	for (int ii = 0 ; ii < 16 ; ii++) { */
		/* 		line1 += "+" ; */
		/* 		if (mm.getNorthWall(jj, ii)) */
		/* 			line1 += "--" ; */
		/* 		else */
		/* 			line1 += "  " ; */
		/* 		if (mm.getWestWall(jj, ii)) */
		/* 			line2 += "|" ; */
		/* 		else */
		/* 			line2 += " " ; */
		/* 		string temp = to_string(mm.getVal(jj, ii)) ; */
		/* 		if (temp.length() < 2) */
		/* 			line2 += " " ; */
		/* 		line2 += temp ; */
		/* 	} */
		/* 	line1 += "+\n" ; */
		/* 	line2 += "|\n" ; */
		/* 	cout << line1 << line2 ; */
		/* } */
		/* cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" ; */
		/* cout << endl ; */
        mm.floodfill(x, y) ;
        suint xx(x), yy(y) ;
		mm.minNeighbor(x, y, xx, yy) ;
        x = xx ;
        y = yy ;

    }

	cout << "Calculations are done" << endl ;

    ofstream output("output.txt") ;
    if (output.is_open()) {
		for (int jj = 0 ; jj < 16 ; jj++) {
			string line1, line2 ;
			for (int ii = 0 ; ii < 16 ; ii++) {
				line1 += "+" ;
				if (mm.getNorthWall(jj, ii))
					line1 += "--" ;
				else
					line1 += "  " ;
				if (mm.getWestWall(jj, ii))
					line2 += "|" ;
				else
					line2 += " " ;
				string temp = to_string(mm.getVal(jj, ii)) ;
				if (temp.length() < 2)
					line2 += " " ;
				line2 += temp ;
			}
			line1 += "+\n" ;
			line2 += "|\n" ;
			output << line1 << line2 ;
		}
		output << "+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+" ;
    }
	cout << "Output is complete" << endl ;
	output.close() ;
    return 0 ;
}
