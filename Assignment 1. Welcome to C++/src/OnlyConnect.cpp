/* File: OnlyConnect.cpp
 * Author: xuehao
 *
 * TODO: Edit these comments to describe anything interesting or noteworthy in
 * your implementation.
 *
 * - extract a general function to test vowels.
 * - boundary detection for the empty string
 *
 * TODO: Edit these comments to leave a puzzle for your section leader to solve!
 */

#include "OnlyConnect.h"
#include "GUI/SimpleTest.h"
#include "strlib.h"
#include <cctype>
using namespace std;

/**
 * Returns true if the character ch is vowel.
 *
 * @param ch
 * @return true/false
 */
bool isVowel(char ch) {
    switch (ch) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return true;
        break;
    default:
        return false;
    }
}

string onlyConnectize(string phrase) {
    int len = phrase.size();
    if (len == 0) { // special case
        return "";
    }

    if (len == 1) { // simple case
        if (!isalpha(phrase[0]) || isVowel(tolower(phrase[0]))) {
            return "";
        } else {
            return toUpperCase(phrase);
        }
    } else { // general case
        if (!isalpha(phrase[0]) || isVowel(tolower(phrase[0]))) {
            return onlyConnectize(phrase.substr(1));
        } else {
            return toUpperCase(charToString(phrase[0])) + onlyConnectize(phrase.substr(1));
        }
    }
}

/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

PROVIDED_TEST("Handles non-letter characters properly.") {
    EXPECT_EQUAL(onlyConnectize("2.718281828459045"), "");
    EXPECT_EQUAL(onlyConnectize("'Hi, Mom!'"), "HMM");
}

PROVIDED_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("+"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}

/* TODO: You will need to add your own tests into this suite of test cases. Think about the sorts
 * of inputs we tested here, and, importantly, what sorts of inputs we *didn't* test here. Some
 * general rules of testing:
 *
 *    1. Try extreme cases. What are some very large cases to check? What are some very small cases?
 *
 *    2. Be diverse. There are a lot of possible inputs out there. Make sure you have tests that account
 *       for cases that aren't just variations of one another.
 *
 *    3. Be sneaky. Don't just try standard inputs. Try weird ones that you wouldn't expect anyone to
 *       actually enter, but which are still perfectly legal.
 *
 * Happy testing!
 */

STUDENT_TEST("My test case.")
{
    EXPECT_EQUAL(onlyConnectize(""), ""); // very small case
    EXPECT_EQUAL(onlyConnectize("taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                                "aaaaaaaaaaaaaaaaaaaaaaaaaa"
                                "geeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
                                "eeeeeeeeeeeeeeeeeeeeeeeeee"
                                "biiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"
                                "iiiiiiiiiiiiiiiiiiiiiiiiii"
                                "Yuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu"
                                "uuuuuuuuuuuuuuuuuuuuuuuuuu"
                                "Hooooooooooooooooooooooooooooooooooooooooooooo"
                                "oooooooooooooooooooooooooo"
                                "Naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                                "aaaaaaaaaaaaaaaaaaaaaaaaaa"),
                 "TGBYHN"); // very large case
    EXPECT_EQUAL(onlyConnectize(" a     \n k    \t \0"), "K");
}
