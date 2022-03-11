#include "Labyrinth.h"
#include "set.h"
using namespace std;

bool isPathToFreedomRec(MazeCell *start, const string &moves, Set<Item> magicTools) {
    // Collect items from current cell if possible
    if (start->whatsHere != Item::NOTHING) {
        magicTools.add(start->whatsHere);
    }

    // Base case: no more paths
    if (moves.size() == 0) {
        return magicTools.size() == 3;
    }

    // Recursive case: try next cell if possible
    if (start->east != nullptr) {
        if (isPathToFreedomRec(start->east, moves.substr(1), magicTools))
            return true;
    }
    if (start->north != nullptr) {
        if (isPathToFreedomRec(start->north, moves.substr(1), magicTools))
            return true;
    }
    if (start->west != nullptr) {
        if (isPathToFreedomRec(start->west, moves.substr(1), magicTools))
            return true;
    }
    if (start->south != nullptr) {
        if (isPathToFreedomRec(start->south, moves.substr(1), magicTools))
            return true;
    }

    return false;
}

bool isPathToFreedom(MazeCell* start, const string& moves) {
    return isPathToFreedomRec(start, moves, {});
}

/* * * * * * Test Cases Below This Point * * * * * */
#include "GUI/SimpleTest.h"
#include "Demos/MazeGenerator.h"

/* Utility function to free all memory allocated for a maze. */
void deleteMaze(const Grid<MazeCell*>& maze) {
    for (auto* elem: maze) {
        delete elem;
    }
    /* Good question to ponder: why don't we write 'delete maze;'
     * rather than what's shown above?
     */
}

PROVIDED_TEST("Checks paths in the sample maze.") {
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN"));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES"));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES"));

    /* These paths don't work, since they don't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW"));
    EXPECT(!isPathToFreedom(maze[2][2], "SWWN"));
    EXPECT(!isPathToFreedom(maze[2][2], "WNNE"));

    /* These paths don't work, since they aren't legal paths. */
    EXPECT(!isPathToFreedom(maze[2][2], "WW"));
    EXPECT(!isPathToFreedom(maze[2][2], "NN"));
    EXPECT(!isPathToFreedom(maze[2][2], "EE"));
    EXPECT(!isPathToFreedom(maze[2][2], "SS"));

    deleteMaze(maze);
}

PROVIDED_TEST("Can't walk through walls.") {
    auto maze = toMaze({"* S *",
                        "     ",
                        "W * P",
                        "     ",
                        "* * *"});

    EXPECT(!isPathToFreedom(maze[1][1], "WNEES"));
    EXPECT(!isPathToFreedom(maze[1][1], "NWSEE"));
    EXPECT(!isPathToFreedom(maze[1][1], "ENWWS"));
    EXPECT(!isPathToFreedom(maze[1][1], "SWNNEES"));

    deleteMaze(maze);
}

PROVIDED_TEST("Works when starting on an item.") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "EE"));
    EXPECT(isPathToFreedom(maze[0][1], "WEE"));
    EXPECT(isPathToFreedom(maze[0][2], "WW"));

    deleteMaze(maze);
}

