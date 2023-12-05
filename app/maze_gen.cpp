#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <algorithm>
#include <time.h>

#include "../include/generator.hpp"
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
    int width = 25;
    int height = 25;
    std::string algorithm("dfs");
    unsigned int seed = 0;
    std::string output = "./out.pnm";

    // get the width argument if it exists
    std::string width_str = get_option(argv, argv+argc, "--width");
    if (width_str.size()) {
        try {
            width = std::stoi(width_str);
        } catch(...) {
            std::cout << "ERROR: Width argument '" << width_str << "' is invalid. Argument must be an integer >= 0.\n";
            return 1;
        }
        if (width < 3) {
            std::cout << "ERROR: Width argument '" << width << "' is to small. Width must be >= 3.\n";
            return 1;
        }
    }

    // get the height argument if it exists
    std::string height_str = get_option(argv, argv+argc, "--height");
    if (height_str.size()) {
        try {
            height = std::stoi(height_str);
        } catch(...) {
            std::cout << "ERROR: Height argument '" << height_str << "' is invalid. Argument must be an integer >= 0.\n";
            return 1;
        }
        if (height < 3) {
            std::cout << "ERROR: Height argument '" << height << "' is to small. Height must be >= 3.\n";
            return 1;
        }
    }

    // get the algorithm argument if it exists
    std::string algorithm_str = get_option(argv, argv+argc, "--algorithm");
    if (algorithm_str.size()) {
        if (algorithm_str.compare("dfs") && 
            algorithm_str.compare("kruskal") && 
            algorithm_str.compare("prim") &&
            algorithm_str.compare("wilson") &&
            algorithm_str.compare("aldous")) {
            std::cout << "ERROR: Algorithm agument '" << algorithm_str << "' is invalid. Select from dfs, kruskal, prim, wilson, or aldous.\n";
            return 1;
        }
        algorithm = std::string(algorithm_str);
    }

    // get the output argument if it exists
    std::string output_str = get_option(argv, argv+argc, "--output");
    if (output_str.size()) {
        output = output_str;
    }

    // get the seed argument if it exists
    std::string seed_str = get_option(argv, argv+argc, "--seed");
    if (seed_str.size()) {
        try {
            seed = std::stoul(seed_str);
        } catch (...) {
            std::cout << "ERROR: Seed argument '" << seed_str << "' is invalid. Argument must be an integer.\n";
            return 1;
        }
    } else {
        seed = (unsigned int) time( NULL );
    }

    Maze maze(height, width);
    Generator g(seed);
    if (algorithm == "dfs") {
        g.dfs(maze);
    } else if (algorithm == "kruskal") {
        g.kruskal(maze);
    } else if (algorithm == "prim") {
        g.prim(maze);
    } else if (algorithm == "wilson") {
        g.wilson(maze);
    } else if (algorithm == "aldous") {
        g.aldous(maze);
    } else {
        g.dfs(maze);
    }

    std::ofstream file;
    try {
        file.open(output, std::ios::out | std::ios::trunc);
    } catch (...) {
        std::cout << "ERROR: Unable to open file at '" << output << "'.\n";
        return 1;
    }
    if (!file.is_open()) {
        std::cout << "ERROR: Unable to open file at '" << output << "'. Directory for file must exits before hand.\n";
        return 1;
    }
    maze.output(file);
    file.close();
    return 0;
}