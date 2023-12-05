#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <iostream>
#include "../include/generator.hpp"
#include "../include/solver.hpp"

using namespace maze;

TEST_CASE("dfs solver", "[generator]") {
    for (std::size_t m = 3; m < 20; ++m) {
        for (std::size_t n = 3; n < 20; ++n) {
            Maze maze(m, n);
            Generator g;
            CHECK_NOTHROW(g.dfs(maze));
            CHECK(solve(maze));
        }
    }
}

TEST_CASE("prim solver", "[generator]") {
    for (std::size_t m = 3; m < 20; ++m) {
        for (std::size_t n = 3; n < 20; ++n) {
            Maze maze(m, n);
            Generator g;
            CHECK_NOTHROW(g.prim(maze));
            CHECK(solve(maze));
        }
    }
}