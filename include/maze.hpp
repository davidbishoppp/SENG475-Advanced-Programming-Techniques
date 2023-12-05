#ifndef MAZE_H
#define MAZE_H

#include <ostream>
#include <iostream>

namespace maze {
    class Maze {
        public:
        // size type
        using size_type = std::size_t;

        struct cell {
            bool wall;
            bool solution;
        };

        /**
         * @brief Construct a new Maze object.
         * 
         */
        Maze(size_type m, size_type n) : m_(m), n_(n) {
            grid_ = (cell *) ::operator new[](sizeof(cell)*m*n);
            for (size_type i = 0; i < m*n; ++i) {
                grid_[i] = {true, false};
            }
        }

        /**
         * @brief Destroy the Maze object
         * 
         */
        ~Maze() {
            delete[] grid_;
        }

        /**
         * @brief Return the maze's height
         * 
         * @return size_type 
         */
        size_type get_M() const {
            return m_;
        }

        /**
         * @brief Return the maze's width
         * 
         * @return size_type 
         */
        size_type get_N() const {
            return n_;
        }

        /**
         * @brief Returns if the location in the maze is a wall or not. If i or j out of bounds return is false.
         * 
         * @param i 
         * @param j 
         * @return true 
         * @return false 
         */
        bool is_wall(size_type i, size_type j) {
            return grid_[(i*n_)+j].wall;
        }

        void remove_wall(size_type i, size_type j) {
            grid_[(i*n_)+j].wall = false;
        }

        void add_wall(size_type i, size_type j) {
            grid_[(i*n_)+j].wall = true;
        }

        void set_solution(size_type i, size_type j) {
            grid_[(i*n_)+j].solution = true;
        }

        /**
         * @brief Output the maze to output stream is P1 PNM format.
         * 
         * @param os 
         */
        void output(std::ostream& os) const {
            os << "P1\n"; // P1 header
            os << n_ << " " << m_ << "\n"; // width and height
            for (size_type i = 0; i < m_; ++i) {
                for (size_type j = 0; j < n_; ++j) {
                    os << grid_[(i*n_)+j].wall;
                }
                os << "\n";
            }
        }

        /**
         * @brief Output the maze to output stream is P1 PNM format.
         * 
         * @param os 
         */
        void output_solution(std::ostream& os) const {
            os << "P3\n"; // P3 header
            os << n_ << " " << m_ << " 1\n"; // width and height
            for (size_type i = 0; i < m_; ++i) {
                for (size_type j = 0; j < n_; ++j) {
                    if (grid_[(i*n_)+j].solution) {
                        os << "1 0 0 ";
                    } else if (grid_[(i*n_)+j].wall) {
                        os << "0 0 0 ";
                    } else {
                        os << "1 1 1 ";
                    }
                }
                os << "\n";
            }
        }

        private:

        cell* grid_;
        size_type m_;
        size_type n_;
    };
}

#endif