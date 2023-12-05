#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <iostream>
#include "../include/generator.hpp"

using namespace maze;

TEST_CASE("dfs", "[generator]") {
    for (std::size_t m = 1; m <= 5; ++m) {
        for (std::size_t n = 1; n <= 5; ++n) {
            Maze maze(m, n);
            Generator g;
            CHECK_NOTHROW(g.dfs(maze));
        }
    }
}

TEST_CASE("kruskal", "[generator]") {
    for (std::size_t m = 1; m <= 10; ++m) {
        for (std::size_t n = 1; n <= 10; ++n) {
            Maze maze(m, n);
            Generator g;
            CHECK_NOTHROW(g.kruskal(maze));
        }
    }
}

TEST_CASE("prim", "[generator]") {
    for (std::size_t m = 1; m <= 10; ++m) {
        for (std::size_t n = 1; n <= 10; ++n) {
            Maze maze(m, n);
            Generator g;
            CHECK_NOTHROW(g.prim(maze));
        }
    }
}

TEST_CASE("wilson", "[generator]") {
    for (std::size_t m = 1; m <= 10; ++m) {
        for (std::size_t n = 1; n <= 10; ++n) {
            Maze maze(m, n);
            Generator g;
            CHECK_NOTHROW(g.wilson(maze));
        }
    }
}

TEST_CASE("aldous", "[generator]") {
    for (std::size_t m = 1; m <= 10; ++m) {
        for (std::size_t n = 1; n <= 10; ++n) {
            Maze maze(m, n);
            Generator g;
            CHECK_NOTHROW(g.aldous(maze));
        }
    }
}