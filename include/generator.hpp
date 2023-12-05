#include "./maze.hpp"
#include <utility>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <iostream>
#include <time.h>

namespace maze {
	class Generator {
		public:

		// size type for generator
		using size_type = std::size_t;

		// rename long types
		using vector_pair_type = std::vector<std::pair<size_type, size_type>>;
		using pair_type = std::pair<size_type, size_type>;

		Generator(unsigned int seed = time( NULL )) {
			srand(seed);
		}

		/**
		 * @brief Construct a maze using the Depth First Search algorithm.
		 * 
		 * @param maze 
		 */
		void dfs(maze::Maze& maze) {
			size_type m = maze.get_M();
			size_type n = maze.get_N();

			if (m < 3 || n < 3) {
				return;
			}

			// visited cells
			bool visited[m*n];
			std::fill_n(visited, m*n, false);

			// DFS basics
			vector_pair_type stack;

			// start at [1,1]
			visited[n+1] = true;
			stack.push_back(pair_type(1, 1));
			size_type r;
			while (!stack.empty()) {
				pair_type cell = std::move(stack.back());
				stack.pop_back();

				visited[(cell.first*n)+cell.second] = true;
				maze.remove_wall(cell.first,cell.second);
				r = rand()%4;
				
				for (int k = 0; k < 4; ++k) {
					int i = cell.first+dir_[(k+r)%4];
					int j = cell.second+dir_[(k+r+2)%4];
					int i2 = i+dir_[(k+r)%4];
					int j2 = j+dir_[(k+r+2)%4];
					// indexing out of bounds
					if (i2 < 0 || i2 >= m-1 || j2 < 0 || j2 >= n-1) { 
						continue;
					}
					if (!visited[(i)*n + j]) {
						visited[(i)*n + j] = true;
						// make sure the unvisited neighbor is not next to a path 
						// so we dont create a cycle and connect two sections.
						if (maze.is_wall(i2, j2)) {
							maze.remove_wall(i, j);
							stack.push_back(cell);
							stack.push_back(pair_type(i2, j2));
							break;
						}
					}
				}
			}

			add_entrance_exit(maze);
		}

		/**
		 * @brief Construct a maze using Kruskal's algorithm.
		 * 
		 * @param maze 
		 */
		void kruskal(maze::Maze& maze) {
			size_type m = maze.get_M();
			size_type n = maze.get_N();

			if (m < 3 || n < 3) {
				return;
			}

			// the sets each cell belongs to
			size_type sets[m*n];
			for (size_type i = 0; i < m*n; ++i) {
				sets[i] = i;
			}

			// list of walls to visit
			vector_pair_type to_visit;

			// add horizontal walls
			for (size_type i = 1; i < m-1; i += 2) {
				for (size_type j = 2; j < n-1; j += 2) {
					to_visit.push_back(pair_type(i, j));
				}
			}
			// add vertical walls
			for (size_type i = 2; i < m-1; i += 2) {
				for (size_type j = 1; j < n-1; j += 2) {
					to_visit.push_back(pair_type(i, j));
				}
			}

			size_type r;
			while (!to_visit.empty()) {
				// randomly choose a wall
				r = rand()%to_visit.size();
				pair_type cur = std::move(to_visit[r]);
				to_visit[r] = std::move(to_visit.back());
				to_visit.pop_back();

				int i1, j1, i2, j2;

				// if i value is odd and j value is even then its a horizontal wall
				if (cur.first%2 && !(cur.second%2)) {
					// look left
					i1 = cur.first;
					j1 = cur.second-1;
					// look right
					i2 = cur.first;
					j2 = cur.second+1;
				} else { // vertical wall
					// look up
					i1 = cur.first-1;
					j1 = cur.second;
					// look down
					i2 = cur.first+1;
					j2 = cur.second;
				}

				// indexing out of bounds
				if (i1 <= 0 || i1 >= m-1 || j1 <= 0 || j1 >= n-1 || 
					i2 <= 0 || i2 >= m-1 || j2 <= 0 || j2 >= n-1) { 
					continue;
				}

				size_type x = find(sets, (i1*n)+j1);
				size_type y = find(sets, (i2*n)+j2);
				// if not in the same set remove the wall between them
				if (x != y) {
					sets[x] = y;
					maze.remove_wall(cur.first, cur.second);
					maze.remove_wall(i1, j1);
					maze.remove_wall(i2, j2);
				}
			}

			add_entrance_exit(maze);
		}

		/**
		 * @brief Construct a maze using Prim's algorithm.
		 * 
		 * @param maze 
		 */
		void prim(maze::Maze& maze) {
			size_type m = maze.get_M();
			size_type n = maze.get_N();

			if (m < 3 || n < 3) {
				return;
			}

			// walls to visit
			vector_pair_type walls;
			maze.remove_wall(1, 1);
			walls.push_back(pair_type(1, 2));
			walls.push_back(pair_type(2, 1));

			size_type r;
			while (!walls.empty()) {
				// randomly choose a wall
				r = rand()%walls.size();
				pair_type cell = std::move(walls[r]);
				walls[r] = std::move(walls.back());
				walls.pop_back();
				size_type cur_i = cell.first;
				size_type cur_j = cell.second;

				// for each cell in N, S, E and W directions
				bool one_visited = false;
				pair_type visited;
				for (size_type k = 0; k < 4; ++k) {
					size_type i = cur_i+dir_[k];
					size_type j = cur_j+dir_[(k+2)%4];
					// indexing out of bounds
					if (i <= 0 || i >= m-1 || j <= 0 || j >= n-1) { 
						continue;
					}
					if (!maze.is_wall(i, j)) {
						if (one_visited) {
							one_visited = false;
							break;
						}
						one_visited = true;
						visited = pair_type(i, j);
					}
				}
				if (!one_visited) {
					continue;
				}
				// continue the direction we are going
				size_type next_i = cur_i + (cur_i-visited.first);
				size_type next_j = cur_j + (cur_j-visited.second);
				// indexing out of bounds
				if (next_i <= 0 || next_i >= m-1 || next_j <= 0 || next_j >= n-1) { 
					continue;
				}
				maze.remove_wall(cur_i, cur_j);
				maze.remove_wall(next_i, next_j);

				// add the walls to visit
				for (size_type k = 0; k < 4; ++k) {
					size_type i = next_i+dir_[k];
					size_type j = next_j+dir_[(k+2)%4];
					// indexing out of bounds
					if (i <= 0 || i >= m-1 || j <= 0 || j >= n-1) { 
						continue;
					}
					if (maze.is_wall(i, j)) {
						walls.push_back(pair_type(i, j));
					}
				}
			}

			add_entrance_exit(maze);
		}

		/**
		 * @brief Construct a maze using Wilson's algorithm.
		 * 
		 * @param maze 
		 */
		void wilson(maze::Maze& maze) {
			size_type m = maze.get_M();
			size_type n = maze.get_N();

			if (m < 3 || n < 3) {
				return;
			}

			// visited cells
			bool visited[m*n];
			std::fill_n(visited, m*n, false);

			// the path taken from the cell
			int path[m*n];
			std::fill_n(path, m*n, -1);

			// cells to visit
			vector_pair_type to_visit; 
			for (size_type i = 1; i < m-1; i+=2) {
				for (size_type j = 1; j < n-1; j+=2) {
					to_visit.push_back(pair_type(i, j));
				}
			}

			// randomly select the first cell
			int r = rand()%to_visit.size();
			pair_type cell = std::move(to_visit[r]);
			to_visit[r] = to_visit.back();
			to_visit.pop_back();

			// remove wall at that spot
			visited[(cell.first*n)+cell.second] = true;
			maze.remove_wall(cell.first, cell.second);

			while (!to_visit.empty()) {
				// select a random cell
				r = rand()%to_visit.size();
				// head of walk
				pair_type walk = std::move(to_visit[r]);
				// current cell
				pair_type cur(walk);
				to_visit[r] = to_visit.back();
				to_visit.pop_back();

				// skip the cell if we have already visited it
				if (visited[(walk.first*n)+walk.second]) {
					continue;
				}

				// random walk
				while (true) {
					bool reached = false;
					// randomly go through each direction for valid place to go
					r = rand()%4;
					for (int k = 0; k < 4; ++k) {
						int i = cur.first+dir_[(k+r)%4];
						int j = cur.second+dir_[(k+r+2)%4];
						int i2 = i+dir_[(k+r)%4];
						int j2 = j+dir_[(k+r+2)%4];
						// indexing out of bounds
						if (i2 < 0 || i2 >= m-1 || j2 < 0 || j2 >= n-1) {
							continue;
						}
						// save the direction
						path[(cur.first*n)+cur.second] = (k+r)%4; 
						// reached a visited cell, so we move on to drawing the path
						if (visited[(i2)*n + j2]) {
							reached = true;
							break;
						}
						cur = pair_type(i2, j2);
						break;
					}
					
					if (reached) {
						break;
					}
				}

				// reset to the front of the walk
				cur = walk;

				// draw the path from the walk
				while (true) {
					visited[(cur.first*n)+cur.second] = true;

					int dir_took = path[(cur.first*n)+cur.second];
					// get the direction we took before
					int i = cur.first+dir_[dir_took];
					int j = cur.second+dir_[(dir_took+2)%4];
					int i2 = i+dir_[dir_took];
					int j2 = j+dir_[(dir_took+2)%4];
					
					maze.remove_wall(cur.first, cur.second);
					maze.remove_wall(i, j);

					if (visited[(i2*n)+j2]) {
						break;
					}
					cur = pair_type(i2, j2);
				}
			}

			add_entrance_exit(maze);
		}

		/**
		 * @brief Construct a maze using Aldous' algorithm.
		 * 
		 * @param maze 
		 */
		void aldous(maze::Maze& maze) {
			size_type m = maze.get_M();
			size_type n = maze.get_N();

			if (m < 3 || n < 3) {
				return;
			}

			// number of cells to visit
			size_type to_visit = 0;
			for (size_type i = 1; i < m-1; i+=2) {
				for (size_type j = 1; j < n-1; j+=2) {
					++to_visit;
				}
			}

			// visited cells
			bool visited[m*n];
			std::fill_n(visited, m*n, false); 

			pair_type cur(1, 1);
			visited[n+1] = true;
			--to_visit;

			// remove wall at that spot
			maze.remove_wall(1, 1);

			// bounce around until all cells are visited
			while (to_visit) {
				// chose a random direction to go
				int r = rand()%4;
				int i = cur.first+dir_[r];
				int j = cur.second+dir_[(r+2)%4];
				int i2 = i+dir_[r];
				int j2 = j+dir_[(r+2)%4];
				// indexing out of bounds
				if (i2 < 0 || i2 >= m-1 || j2 < 0 || j2 >= n-1) {
					continue;
				}
				// reached an unvisited cell so we remove the walls
				if (!visited[(i2)*n + j2]) {
					maze.remove_wall(i, j);
					maze.remove_wall(i2, j2);
					visited[(i2*n)+j2] = true;
					--to_visit;
				}
				cur = pair_type(i2, j2);
			}
			add_entrance_exit(maze);
		}

		private:
		// condenses modification of i and j into one array for indexing a cell in a direction.
		// dir_[k]: i modification for direction k
		// dir_[(k+2)%4]: j modification for direction k 
        // k == 0: N, k == 1: S, k == 2: E, k == 3: W
		int dir_[4] = {1, -1, 0, 0};

		/**
		 * @brief Find function for kruskal's algorithm. Finds the parent of the specified cell, also compresses the finding path each time.
		 * 
		 * @param set set of cells to search
		 * @param x cells to find parent of
		 * @return size_type parent of x
		 */
		size_type find(size_type* sets, size_type x) {
			size_type root = x;
			// while the parent of the cell is not itself
			while (sets[root] != root) {
				root = sets[root];
			}

			// compress the find path for all other cells in the set
			while (sets[x] != root) {
				size_type parent = sets[x];
				sets[x] = root;
				x = parent;
			}

			return root;
		}

		/**
		 * @brief Add an entrance and exit to the maze
		 * 
		 * @param maze 
		 */
		void add_entrance_exit(Maze& maze) {
			size_type m = maze.get_M();
			size_type n = maze.get_N();

			// remove walls until we reach the path.
			for (int i = 0; i < n-1; ++i) {
				maze.remove_wall(1, i);
				// check E and S for the path
				if (!maze.is_wall(1, i+1) || !maze.is_wall(2, i)) {
					break;
				}
			}

			for (int i = n-1; i > 0; --i) {
				maze.remove_wall(m-2, i);
				// check N and W for the path
				if (!maze.is_wall(m-2, i-1) || !maze.is_wall(m-3, i)) {
					break;
				}
			}
		}
	};
};