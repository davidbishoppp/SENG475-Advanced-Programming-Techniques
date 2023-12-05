#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>

#include "../include/solver.hpp"
#include "../include/maze.hpp"

using namespace maze;

std::string get_option(char** begin, char** end, const std::string& option) {
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return std::string(*itr);
    }
    return std::string();
}

int main(int argc, char** argv) {
    std::string input("./out.pnm");
    std::string output("./solved.pnm");

    // get the input argument if it exists
    std::string input_str = get_option(argv, argv+argc, "--input");
    if (input_str.size()) {
        input = input_str;
    }

    // get the output argument if it exists
    std::string output_str = get_option(argv, argv+argc, "--output");
    if (output_str.size()) {
        output = output_str;
    }

    std::ifstream in_file;
    try {
        in_file.open(input, std::ios::in);
    } catch (...) {
        std::cout << "ERROR: Unable to open file at '" << input << "'.\n";
		return 1;
    }

    if (!in_file.is_open()) {
        std::cout << "ERROR: Unable to open input file '" << input << "'.\n";
        return 1; 
    }

    std::string line;

    // get the first line of the pnm
    getline(in_file, line);

    // pnm file format should be P1
    if (line != "P1") {
        std::cout << "ERROR: Input pnm file format must be P1. Got '" << line << "'.\n";
		return 1;
    }

    // get width and height from next line
    getline(in_file, line);
    
    std::size_t split = line.find(' ');

    if (split == std::string::npos) {
        std::cout << "ERROR: Unable to determine height and width of input file. Delimiter returned npos.\n";
        return 1;
    }

    std::string width_str = line.substr(0, split);
    std::string height_str = line.substr(split);

    std::size_t width;
    std::size_t height;

    try {
        width = std::stoul(width_str);
    } catch (...) {
        std::cout << "ERROR: Unable to convert width string '" << width_str << "' to unsigned long.\n";
        return 1;
    }

    try {
        height = std::stoul(height_str);
    } catch (...) {
        std::cout << "ERROR: Unable to convert height string '" << height_str << "' to unsigned long.\n";
        return 1;
    }

    Maze maze(height, width);

    // read the remaining lines and re-create the maze
    for (std::size_t i = 0; i < height; ++i) {
        getline(in_file, line);
        if (line.size() != width) {
            std::cout << "ERROR: Line " << i+3 << " of input file is expected to have width '" << width << "' but actually has width '" << line.size() << "'.\n";
            return 1;
        }
        for (std::size_t j = 0; j < line.size(); ++j) {
            if (line[j] == '0') {
                maze.remove_wall(i, j);
            }
        }
    }

    in_file.close();

    bool res = solve(maze);

    if (!res) {
        std::cout << "ERROR: Unable to solve maze at '" << input << "'.\n";
        return 1;
    }

    std::ofstream out_file;
    try {
        out_file.open(output, std::ios::out | std::ios::trunc);
    } catch (...) {
        std::cout << "ERROR: Unable to open file at '" << output << "'.\n";
		return 1;
    }
    if (!out_file.is_open()) {
        std::cout << "ERROR: Unable to open file at '" << output << "'. Directory for file must exits before hand.\n";
        return 1;
    }
    maze.output_solution(out_file);
    out_file.close();

    return 0;
}