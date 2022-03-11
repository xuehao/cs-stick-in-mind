#include "HumanPyramids.h"
#include "error.h"
#include "grid.h"
using namespace std;

double weightOnBackOfRec(int row, int col, int pyramidHeight, Grid<double> &table) {
    if (row == 0 && col == 0) {
        return 0;
    }

    if (table.get(row, col)) { // memorization
        return table.get(row, col);
    } else {
        if (col == 0) {
            table.set(row, col, 80 + weightOnBackOfRec(row - 1, col, pyramidHeight - 1, table) / 2);
            return 80 + weightOnBackOfRec(row - 1, col, pyramidHeight - 1, table) / 2;
        } else if (col == row) {
            table.set(row, col,
                      80 + weightOnBackOfRec(row - 1, col - 1, pyramidHeight - 1, table) / 2);
            return 80 + weightOnBackOfRec(row - 1, col - 1, pyramidHeight - 1, table) / 2;
        } else {
            table.set(row, col,
                      160 + weightOnBackOfRec(row - 1, col - 1, pyramidHeight - 1, table) / 2 +
                          weightOnBackOfRec(row - 1, col, pyramidHeight - 1, table) / 2);
            return 160 + weightOnBackOfRec(row - 1, col - 1, pyramidHeight - 1, table) / 2 +
                   weightOnBackOfRec(row - 1, col, pyramidHeight - 1, table) / 2;
        }
    }
}

double weightOnBackOf(int row, int col, int pyramidHeight) {
    // deal with egde case
    if ((row < 0) || (col < 0) || (col > row) || (pyramidHeight < row)) {
        error("You provide the wrong value.");
    }

    // Version 1.0 - Basic implementation
    //    if (row == 0 && col == 0) {
    //        return 0;
    //    } else {
    //        if (col == 0 || col == row) {
    //            return 80 + weightOnBackOf(row - 1, 0, pyramidHeight - 1) / 2;
    //        } else {
    //            return 160 +
    //                   weightOnBackOf(row - 1, col - 1, pyramidHeight - 1) / 2
    //                   + weightOnBackOf(row - 1, col, pyramidHeight - 1) / 2;
    //        }
    //    }

    // Version 2.0 - Speed up
    Grid<double> table = {row + 1, col + 1, 0};
    return weightOnBackOfRec(row, col, pyramidHeight, table);
}

/* * * * * * Test Cases * * * * * */
#include "GUI/SimpleTest.h"

/* * * * * * Student Test Cases * * * * * */
STUDENT_TEST("Small and large cases.") {
    EXPECT_EQUAL(weightOnBackOf(0, 0, 5), 0);
    EXPECT(weightOnBackOf(40, 10, 50) > 2000);
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Check Person E from the handout.") {
    /* Person E is located at row 2, column 1. */
    EXPECT_EQUAL(weightOnBackOf(2, 1, 5), 240);
}

PROVIDED_TEST("Function reports errors in invalid cases.") {
    EXPECT_ERROR(weightOnBackOf(-1, 0, 10));
    EXPECT_ERROR(weightOnBackOf(10, 10, 5));
    EXPECT_ERROR(weightOnBackOf(-1, 10, 20));
}

PROVIDED_TEST("Stress test: Memoization is implemented (should take under a second)") {
    /* Do not delete anything below this point. :-) */

    /* This will take a LONG time to complete if memoization isn't implemented.
     * We're talking "heat death of the universe" amounts of time. :-)
     *
     * If you did implement memoization but this test case is still hanging, make
     * sure that in your recursive function (not the wrapper) that your recursive
     * calls are to your new recursive function and not back to the wrapper. If you
     * call the wrapper again, you'll get a fresh new memoization table rather than
     * preserving the one you're building up in your recursive exploration, and the
     * effect will be as if you hadn't implemented memoization at all.
     */
    EXPECT(weightOnBackOf(100, 50, 200) >= 10000);
}

/* TODO: Add your own tests here. You know the drill - look for edge cases, think about
 * very small and very large cases, etc.
 */
STUDENT_TEST("Small and large cases.") {
    EXPECT_EQUAL(weightOnBackOf(0, 0, 5), 0);
    EXPECT(weightOnBackOf(400, 200, 500) > 20000);
}
