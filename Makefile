test: MazeTester.cpp Maze.cpp Maze.h
	g++ -o test MazeTester.cpp Maze.cpp
reset:
	rm -f output.txt
clean:
	rm -f *.o test
