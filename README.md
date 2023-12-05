# SENG475 Summer 2022 Project
Random Maze Generator and Solver
By: David Bishop
Project Presentation: https://youtu.be/e3OVXGiJRIY

## Building and installing:

let $INSTALL_DIR denote the directory to install the executables.

cmake -H. -Btmp_cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
cmake --build tmp_cmake --clean-first --target install

## Building test scripts:

cmake -H. -Btmp_cmake -DENABLE_TEST=true
cmake --build tmp_cmake

## Maze Generator Options:

--seed s: Where s is the seed for the pseudo random number generator. This value must be >= 0. Default is seed with the current time.

--width w: Where w is the width in pixels of the maze. This value must be >= 3. Default is 25.

--height h: Where h is the height in pixels of the maze. This value must be >= 3. Default is 25.

--algorithm a: Where a is the algorithm for making the maze. a can be 1 of dfs, kruskal, prim, wilson, or aldous. Default is dfs.

--output o: Where o is the path to the pnm file for the maze. Default is './out.pnm'.

## Maze Generator Example:

$INSTALL_DIR/bin/maze_gen --width 100 --height 93 --algorithm wilson --output ./wilson.pnm --seed 1234

## Maze Solver Options:

--input i: Where i is the path to the pnm file for the input maze. Default is './out.pnm'.

--output o: Where o is the path to the pnm file for the solved maze. Default is './solved.pnm'.

## Maze Solver Example:

$INSTALL_DIR/bin/maze_solve --input ./wilson.pnm --output ./wilson_solved.pnm

If the user wants to make their own mazes to input to maze_solve, they must follow these rules:

1. .pnm file must be in P1 format.
2. Height and width of maze should be >= 3.
3. Cells that are walls are black.
4. Cells that are the path are white.
5. The border surrounding the maze must be made up of walls (Except for the entrance and exit).
6. The entrance and exit of the maze are white tiles.
7. The entrance of the maze is at [1, 0]
8. The exit of the maze must be at [m-2, n-1] where m is the maze's height and n is the maze's width

Running the demo script will output example mazes for reference.
