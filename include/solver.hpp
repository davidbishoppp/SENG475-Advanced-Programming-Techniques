#include <list>

#include "./maze.hpp"

using pair_type = std::pair<std::size_t, std::size_t>;

namespace maze {
    /**
     * @brief Find the solution to the maze using BFS algorithm.
     * 
     * @param maze 
     * @return true 
     * @return false 
     */
    bool solve(Maze& maze) {
        std::size_t m = maze.get_M();
        std::size_t n = maze.get_N();

        // start index
        std::size_t src_i = 1;
        std::size_t src_j = 0;

        // end index
        std::size_t dst_i = m-2;
        std::size_t dst_j = n-1;

        // need start and end to at least not be walls
        if (maze.is_wall(src_i, src_j) || maze.is_wall(dst_i, dst_j)) {
            return false;
        }

        // standard BFS items
        std::list<pair_type> queue;

        // initialize visited to false
        bool visited[m*n];
        std::fill_n(visited, m*n, false); 

        // initialize visited to -1, -1
        pair_type prev[m*n];
        std::fill_n(prev, m*n, pair_type(-1, -1));

        visited[(src_i*n)+src_j] = true; // starting cell
        queue.push_back(pair_type(src_i, src_j));

        // condensing direction modification into one array.
        // dir[k]: i modification for direction k
        // dir[(k+2)%4]: j modification for direction k
        // k == 0: N, k == 1: S, k == 2: E, k == 3: W
        int dir[4] = {1, -1, 0, 0};

        // BFS
        bool found = false;
        while(!queue.empty()) {
            pair_type cur = std::move(queue.front());
            queue.pop_front();
            // search all 4 directions for the path
            for (int k = 0; k < 4; ++k) {
                int i = cur.first+dir[k];
                int j = cur.second+dir[(k+2)%4];
                // indexing out of bounds
                if (i < 0 || i > m-1 || j < 0 || j > n-1) { 
                    continue;
                }
                if (!maze.is_wall(i, j) && !visited[(i*n)+j]) {
                    visited[(i*n)+j] = true;
                    prev[(i*n)+j] = pair_type(cur.first, cur.second);
                    queue.push_back(pair_type(i, j));

                    if (i == dst_i && j == dst_j) {
                        found = true;
                        break;
                    }
                }
            }
            // break early if we found the exit.
            if (found) { 
                break;
            }
        }

        // not able to solve maze
        if (!found) {
            return false;
        }

        std::size_t path_i = dst_i;
        std::size_t path_j = dst_j;
        pair_type cur;
        // backtrack from the exit and highlight the solution
        while (cur != pair_type(-1, -1)) {
            maze.set_solution(path_i, path_j);
            cur = prev[(path_i*n)+path_j];
            path_i = cur.first;
            path_j = cur.second;
        }

        return true;
    }
};