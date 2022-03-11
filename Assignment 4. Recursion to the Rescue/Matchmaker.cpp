#include "Matchmaker.h"
using namespace std;

/**
 * @brief To determine if any one of the given pair exits in the given matching.
 * @param pair
 * @param matching
 * @return true/false
 */
bool isPairedOff(const Pair &pair, const Set<Pair> &matching) {
    bool paired = false;
    for (const Pair &link : matching) {
        if (pair.first() == link.first() || pair.first() == link.second() ||
            pair.second() == link.first() || pair.second() == link.second()) {
            paired = true;
        }
    }
    return paired;
}

bool hasPerfectMatchingRec(const Map<string, Set<string>> &possibleLinks, Set<Pair> &matching) {
    // Base case: no more links
    if (possibleLinks.size() == 0) {
        return true;
    }

    // Recursive Case: iteration for each option
    string person1 = possibleLinks.firstKey();
    auto group = possibleLinks[person1];

    for (const string &person2 : group) {
        Pair pair = {person1, person2};
        if (!isPairedOff(pair, matching)) {
            matching.add(pair);
            auto remaining = possibleLinks;
            remaining.remove(person1);
            remaining.remove(person2);
            if (hasPerfectMatchingRec(remaining, matching)) {
                return true; // If this option meets the perfect matching, return true.
            } else {
                matching.remove(pair); // If not, restore matching and continue to the next loop.
            }
        }
    }

    return false; // Reaching here means there is no perfect matching.
}

bool hasPerfectMatching(const Map<string, Set<string>> &possibleLinks, Set<Pair> &matching) {
    // edge case: the number of people is odd.
    if (possibleLinks.size() % 2) {
        return false;
    }
    // edge case: someone who isn't linked to anyone.
    for (const string &item : possibleLinks) {
        if (possibleLinks[item].size() == 0) {
            return false;
        }
    }

    return hasPerfectMatchingRec(possibleLinks, matching);
}

#define VERSION_6

#ifdef VERSION_6 // Best - focus on edges
void maximumWeightMatchingRec(const Map<Pair, int> &possiblePairs, const Set<Pair> &matching,
                              int weight, Set<Pair> &maxWeightMatching, int &maxWeight) {
    if (possiblePairs.size() == 0) {
        if (weight > maxWeight) {
            maxWeight = weight;
            maxWeightMatching = matching;
        }
        return;
    }

    // Pick one pair from lists
    Pair pair1 = possiblePairs.firstKey();
    // Option 1: include pair1
    Map<Pair, int> remainingPairs = possiblePairs;
    for (const Pair &item : possiblePairs) {
        if (pair1.first() == item.first() || pair1.first() == item.second() ||
            pair1.second() == item.first() || pair1.second() == item.second()) {
            remainingPairs.remove(item);
        }
    }
    maximumWeightMatchingRec(remainingPairs, matching + pair1, weight + possiblePairs[pair1],
                             maxWeightMatching, maxWeight);

    // Option 2: exclude pair1
    Map<Pair, int> remainingPairs2 = possiblePairs;
    remainingPairs2.remove(pair1);
    maximumWeightMatchingRec(remainingPairs2, matching, weight, maxWeightMatching, maxWeight);

    return;
}

Map<Pair, int> allPossiblePairs(const Map<string, Map<string, int>> &possibleLinks) {
    Map<Pair, int> possiblePairs;
    for (const string &person : possibleLinks) {
        for (const string &pairedPerson : possibleLinks.get(person)) {
            possiblePairs[{person, pairedPerson}] = possibleLinks[person][pairedPerson];
        }
    }
    return possiblePairs;
}

Set<Pair> maximumWeightMatching(const Map<string, Map<string, int>> &possibleLinks) {
    Map<Pair, int> possiblePairs = allPossiblePairs(possibleLinks);
    Set<Pair> maxWeightMatching = {};
    int maxWeight = 0;

    maximumWeightMatchingRec(possiblePairs, {}, 0, maxWeightMatching, maxWeight);
    return maxWeightMatching;
}
#endif

#ifdef VERSION_5 // Better - about 8s (using reference to avoid copy)
void maximumWeightMatchingRec(const Map<string, Map<string, int>> &possibleLinks,
                              const Set<Pair> &matching, int weight, Set<Pair> &maxWeightMatching,
                              int &maxWeight,
                              const Set<string> &allPeople) {
    if (allPeople.size() < 2) {
        if (weight > maxWeight) {
            maxWeight = weight;
            maxWeightMatching = matching;
        }
        return ;
    }

    // Pick one person from lists
    string person1 = allPeople.first();
    // Option 1: include person1
    for (const string &person2 : possibleLinks[person1]) {
        Pair pair(person1, person2);
        if (!isPairedOff(pair, matching)) {
            maximumWeightMatchingRec(possibleLinks, matching + pair,
                                     weight + possibleLinks.get(pair.first()).get(pair.second()),
                                     maxWeightMatching, maxWeight, allPeople - person1 - person2);
        }
    }
    // Option 2: exclude person1
    maximumWeightMatchingRec(possibleLinks, matching, weight, maxWeightMatching, maxWeight, allPeople - person1);

    return ;
}

Set<Pair> maximumWeightMatching(const Map<string, Map<string, int>> &possibleLinks) {
    Set<string> allPeople;
    for (const string &person : possibleLinks) {
        allPeople += person;
    }

    Set<Pair> maxWeightMatching = {};
    int maxWeight = 0;

    maximumWeightMatchingRec(possibleLinks, {}, 0, maxWeightMatching, maxWeight, allPeople);
    return maxWeightMatching;
}
#endif

#ifdef VERSION_4 // better - about 22s (calculating weight wastes much more time)
/**
 * @brief To get the weight of the given matching from the given links.
 * @param possibleLinks
 * @param matching
 * @return int
 */
int weightOf(const Map<string, Map<string, int>> &possibleLinks, const Set<Pair> &matching) {
    int weight = 0;
    for (const Pair &pair : matching) {
        weight += possibleLinks.get(pair.first()).get(pair.second());
    }
    return weight;
}

Set<Pair> maximumWeightMatchingRec(const Map<string, Map<string, int>> &possibleLinks,
                                   const Set<Pair> &matching, const Set<string> &persons) {
    if (persons.size() < 2) {
        return matching;
    }

    string person1 = persons.first();

    // Option 1: include person1
    Set<Pair> with;
    int weightOfWith = 0;
    for (const string &person2 : possibleLinks[person1]) {
        Pair pair(person1, person2);
        if (!isPairedOff(pair, matching)) {
            Set<Pair> tempMatch =
                maximumWeightMatchingRec(possibleLinks, matching + pair, persons - person1 - person2);
            int tempWeight = weightOf(possibleLinks, tempMatch);
            if (tempWeight > weightOfWith) {
                weightOfWith = tempWeight;
                with = tempMatch;
            }
        }
    }
    // Option 2: exclude person1
    Set<Pair> without = maximumWeightMatchingRec(possibleLinks, matching, persons - person1);
    int weightOfWithout = weightOf(possibleLinks, without);

    return weightOfWith > weightOfWithout ? with : without;
}

Set<Pair> maximumWeightMatching(const Map<string, Map<string, int>> &possibleLinks) {
    Set<string> allPeople;
    for (const string &person : possibleLinks) {
        allPeople += person;
    }
    return maximumWeightMatchingRec(possibleLinks, {}, allPeople);
}
#endif

#ifdef VERSION_3 // Better - about 10s
void maximumWeightMatchingRec(const Map<string, Map<string, int>> &possibleLinks,
                              const Set<Pair> &matching, int weight, Set<Pair> &maxWeightMatching,
                              int &maxWeight) {
    if (possibleLinks.size() < 2) {
        if (weight > maxWeight) {
            maxWeight = weight;
            maxWeightMatching = matching;
        }
        return ;
    }

    string person1 = possibleLinks.firstKey();
    auto matchingGroup = possibleLinks[person1];
    auto remaining = possibleLinks;
    remaining.remove(person1);

    // Option 1: include person1
    for (const string &person2 : matchingGroup) {
        Pair pair(person1, person2);
        if (!isPairedOff(pair, matching)) {
            auto remaining1 = remaining;
            remaining1.remove(person2);
            maximumWeightMatchingRec(remaining1, matching + pair,
                                     weight + possibleLinks.get(pair.first()).get(pair.second()),
                                     maxWeightMatching, maxWeight);
        }
    }
    // Option 2: exclude person1
    maximumWeightMatchingRec(remaining, matching, weight, maxWeightMatching, maxWeight);

    return ;
}

Set<Pair> maximumWeightMatching(const Map<string, Map<string, int>> &possibleLinks) {
    Set<Pair> maxWeightMatching = {};
    int maxWeight = 0;

    maximumWeightMatchingRec(possibleLinks, {}, 0, maxWeightMatching, maxWeight);
    return maxWeightMatching;
}
#endif

#ifdef VERSION_2 // good - about 15s
/**
 * @brief To get the weight of the given matching from the given links.
 * @param possibleLinks
 * @param matching
 * @return int
 */
int weightOf(const Map<string, Map<string, int>> &possibleLinks, const Set<Pair> &matching) {
    int weight = 0;
    for (const Pair &pair : matching) {
        weight += possibleLinks.get(pair.first()).get(pair.second());
    }
    return weight;
}

Set<Pair> maximumWeightMatchingRec(const Map<string, Map<string, int>> &possibleLinks,
                                   const Set<Pair> &matching) {
    if (possibleLinks.size() < 2) {
        return matching;
    }

    string person1 = possibleLinks.firstKey();
    auto matchingGroup = possibleLinks[person1];
    auto remaining = possibleLinks;
    remaining.remove(person1);

    // Option 1: include person1
    Set<Pair> with;
    int weightOfWith = 0;
    for (const string &person2 : matchingGroup) {
        Pair pair(person1, person2);
        if (!isPairedOff(pair, matching)) {
            auto remaining1 = remaining;
            remaining1.remove(person2);
            Set<Pair> tempMatch = maximumWeightMatchingRec(remaining1, matching + pair);
            int tempWeight = weightOf(possibleLinks, tempMatch);
            if(tempWeight > weightOfWith) {
                weightOfWith = tempWeight;
                with = tempMatch;
            }
        }
    }
    // Option 2: exclude person1
    Set<Pair> without = maximumWeightMatchingRec(remaining, matching);
    int weightOfWithout = weightOf(possibleLinks, without);

    return weightOfWith > weightOfWithout ? with : without;
}

Set<Pair> maximumWeightMatching(const Map<string, Map<string, int>> &possibleLinks) {
    return maximumWeightMatchingRec(possibleLinks, {});
}
#endif

#ifdef VERSION_1 // slower - about 2min40s

/**
 * @brief To get the weight of the given matching from the given links.
 * @param possibleLinks
 * @param matching
 * @return int
 */
int weightOf(const Map<string, Map<string, int>> &possibleLinks, const Set<Pair> &matching) {
    int weight = 0;
    for (const Pair &pair : matching) {
        weight += possibleLinks[pair.first()][pair.second()];
    }
    return weight;
}

Set<Set<Pair>> allPossibleMatchingsRec(const Map<string, Map<string, int>> &possibleLinks,
                                        const Set<Pair> &matching) {
    Set<Set<Pair>> result;

    if (possibleLinks.size() < 2) {
        return {matching};
    }

    string person1 = possibleLinks.firstKey();
    auto group = possibleLinks[person1];
    auto remaining = possibleLinks;
    remaining.remove(person1);

    // Option 1: include person1
    for (const string &person2 : group) {
        Pair pair(person1, person2);
        if (!isPairedOff(pair, matching)) {
            // matching.add(pair);
            auto remaining1 = remaining;
            remaining1.remove(person2);
            result += allPossibleMatchingsRec(remaining1, matching + pair);
            // matching.remove(pair);
        }
    }
    // Option 2: exclude person1
    result += allPossibleMatchingsRec(remaining, matching);

    return result;
}

Set<Pair> maximumWeightMatching(const Map<string, Map<string, int>> &possibleLinks) {
    // Set<Pair> matching = {};
    Set<Set<Pair>> allPossibleMatchings = allPossibleMatchingsRec(possibleLinks, {});

    int maxWeight = 0;
    Set<Pair> maxWeightMatching = {};
    for (const auto &possibleMatching : allPossibleMatchings) {
        int currentWeight = 0;
        for (const Pair &pair : possibleMatching) {
            currentWeight += possibleLinks[pair.first()][pair.second()];
        }
        if (currentWeight > maxWeight) {
            maxWeight = currentWeight;
            maxWeightMatching = possibleMatching;
        }
    }

    return maxWeightMatching;
}
#endif

/* * * * * Test Cases Below This Point * * * * */

namespace {
    /* Utility to go from a list of triples to a world. */
    struct WeightedLink {
        string from;
        string to;
        int cost;
    };
    Map<string, Map<string, int>> fromWeightedLinks(const Vector<WeightedLink>& links) {
        Map<string, Map<string, int>> result;
        for (const auto& link: links) {
            result[link.from][link.to] = link.cost;
            result[link.to][link.from] = link.cost;
        }
        return result;
    }

    /* Pairs to world. */
    Map<string, Set<string>> fromLinks(const Vector<Pair>& pairs) {
        Map<string, Set<string>> result;
        for (const auto& link: pairs) {
            result[link.first()].add(link.second());
            result[link.second()].add(link.first());
        }
        return result;
    }

    /* Checks if a set of pairs forms a perfect matching. */
    bool isPerfectMatching(const Map<string, Set<string>>& possibleLinks,
                           const Set<Pair>& matching) {
        /* Need to check that
         *
         * 1. each pair is indeed a possible link,
         * 2. each person appears in exactly one pair.
         */
        Set<string> used;
        for (const Pair &p: matching) {
            /* Are these folks even in the group of people? */
            if (!possibleLinks.containsKey(p.first())) return false;
            if (!possibleLinks.containsKey(p.second())) return false;

            /* If these people are in the group, are they linked? */
            if (!possibleLinks[p.first()].contains(p.second()) ||
                !possibleLinks[p.second()].contains(p.first())) {
                return false;
            }

            /* Have we seen them before? */
            if (used.contains(p.first()) || used.contains(p.second())) {
                return false;
            }

            /* Add them both. */
            used += p.first();
            used += p.second();
        }

        /* Confirm that's everyone. */
        return used.size() == possibleLinks.size();
    }
}

#include "GUI/SimpleTest.h"

STUDENT_TEST("maximumWeightMatching: Works on a person don't link to anyone.") {
    /*
     *      A     B
     *            |
     *            | 3
     *            |
     *            C
     *
     */
    Map<string, Map<string, int>> links = {};
    links["A"] = {};
    links["B"]["C"] = 3;
    links["C"]["B"] = 3;

    EXPECT_EQUAL(maximumWeightMatching(links), {{"B", "C"}});
}

STUDENT_TEST("maximumWeightMatching works on a small test.") {
    /*
     *
     *               D
     *               | 1
     *               A
     *            1 / \ 5
     *             C---B
     *          1 /  1  \ 1
     *           F       E
     *
     */
    Vector<string> people = {"A", "B", "C", "D", "E", "F"};
    auto links = fromWeightedLinks({
        {"A", "B", 5},
        {"B", "C", 1},
        {"C", "A", 1},
        {"D", "A", 1},
        {"E", "B", 1},
        {"F", "C", 1},
    });

    Set<Pair> expected = {{people[0], people[1]}, {people[2], people[5]}};

    EXPECT_EQUAL(maximumWeightMatching(links), expected);
}

STUDENT_TEST("hasPerfectMatching works on a bad sample.") {
    /*
     * "A": {"C","D"},
     * "B": {"C"},
     * "C": {"A", "B"},
     * "D": {"C"}
     */
    Map<string, Set<string>> links = fromLinks({{"B", "C"}, {"C", "A"}});
    links["D"].add("C");

    Set<Pair> matching;
    EXPECT(!hasPerfectMatching(links, matching));
    EXPECT(!isPerfectMatching(links, matching));
}

STUDENT_TEST("hasPerfectMatching works on a good sample.") {
    /*
     * "A": {"C","D"},
     * "B": {"C"},
     * "C": {"A", "B"},
     * "D": {"A"}
     */
    Vector<string> people = {"B", "C", "A", "D"};
    Map<string, Set<string>> links =
        fromLinks({{people[0], people[1]}, {people[1], people[2]}, {people[2], people[3]}});

    Set<Pair> matching;
    EXPECT(hasPerfectMatching(links, matching));
    EXPECT(isPerfectMatching(links, matching));
}

PROVIDED_TEST("hasPerfectMatching works on a world with just one person.") {
    /* The world is just a single person A, with no others. How sad. :-(
     *
     *                 A
     *
     * There is no perfect matching.
     */

    Set<Pair> unused;
    EXPECT(!hasPerfectMatching({ { "A", {} } }, unused));
}

PROVIDED_TEST("hasPerfectMatching works on an empty set of people.") {
    /* There actually is a perfect matching - the set of no links meets the
     * requirements.
     */
    Set<Pair> unused;
    EXPECT(hasPerfectMatching({}, unused));
}

PROVIDED_TEST("hasPerfectMatching works on a world with two linked people.") {
    /* This world is a pair of people A and B. There should be a perfect matching.
     *
     *               A -- B
     *
     * The matching is {A, B}
     */
    auto links = fromLinks({
        { "A", "B" }
    });

    Set<Pair> unused;
    EXPECT(hasPerfectMatching(links, unused));
}

PROVIDED_TEST("hasPerfectMatching works on a world with two linked people, and produces output.") {
    /* This world is a pair of people A and B. There should be a perfect matching.
     *
     *               A -- B
     *
     * The matching is {A, B}
     */
    auto links = fromLinks({
        { "A", "B" }
    });

    Set<Pair> expected = {
        { "A", "B" }
    };

    Set<Pair> matching;
    EXPECT(hasPerfectMatching(links, matching));
    EXPECT_EQUAL(matching, expected);
}

PROVIDED_TEST("hasPerfectMatching works on a triangle of people.") {
    /* Here's the world:
     *
     *               A --- B
     *                \   /
     *                 \ /
     *                  C
     *
     * There is no perfect matching here, unfortunately.
     */
    auto links = fromLinks({
        { "A", "B" },
        { "B", "C" },
        { "C", "A" }
    });

    Set<Pair> unused;
    EXPECT(!hasPerfectMatching(links, unused));
}

PROVIDED_TEST("hasPerfectMatching works on a square of people.") {
    /* Here's the world:
     *
     *               A --- B
     *               |     |
     *               |     |
     *               D --- C
     *
     * There are two different perfect matching here: AB / CD, and AD/BD.
     * Either will work.
     */
    auto links = fromLinks({
        { "A", "B" },
        { "B", "C" },
        { "C", "D" },
        { "D", "A" }
    });

    Set<Pair> unused;
    EXPECT(hasPerfectMatching(links, unused));
}

PROVIDED_TEST("hasPerfectMatching works on a square of people, and produces output.") {
    /* Here's the world:
     *
     *               A --- B
     *               |     |
     *               |     |
     *               C --- D
     *
     * There are two different perfect matching here: AB / CD, and AC/BC.
     * Either will work.
     */
    auto links = fromLinks({
        { "A", "B" },
        { "B", "C" },
        { "C", "D" },
        { "D", "A" }
    });

    Set<Pair> matching;
    EXPECT(hasPerfectMatching(links, matching));
    EXPECT(isPerfectMatching(links, matching));
}

PROVIDED_TEST("hasPerfectMatching works on a pentagon of people.") {
    /* Here's the world:
     *
     *               A --- B
     *             /       |
     *            E        |
     *             \       |
     *               D --- C
     *
     * There is no perfect matching here, since the cycle has odd
     * length.
     */
    auto links = fromLinks({
        { "A", "B" },
        { "B", "C" },
        { "C", "D" },
        { "D", "E" },
        { "E", "A" }
    });

    Set<Pair> unused;
    EXPECT(!hasPerfectMatching(links, unused));
}

PROVIDED_TEST("hasPerfectMatching works on a line of six people.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over people when making decisions is sensitive
     * to the order of those peoples' names. This test looks at a group like
     * this one, trying out all possible orderings of peoples' names:
     *
     *
     *
     *        * -- * -- * -- * -- * -- *
     *
     *
     *
     * There are 6! = 720 possible permutations of the ordering of these six
     * people. If your code is able to solve the problem correctly for all of
     * those orderings, there's a good chance that you're correctly tracking
     * who is matched at each step. On the other hand, if your code runs into
     * issues here, it may indicate that there's a bug in how you mark who's
     * paired and who isn't.
     */
    Vector<string> people = { "A", "B", "C", "D", "E", "F" };
    do {
        Map<string, Set<string>> links = fromLinks({
            { people[0], people[1] },
            { people[1], people[2] },
            { people[2], people[3] },
            { people[3], people[4] },
            { people[4], people[5] }
        });

        Set<Pair> matching;
        EXPECT(hasPerfectMatching(links, matching));
        EXPECT(isPerfectMatching(links, matching));
    } while (next_permutation(people.begin(), people.end()));
}

PROVIDED_TEST("hasPerfectMatching works on a more complex negative example.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over people when making decisions is sensitive
     * to the order of those peoples' names. This test looks at a group like
     * this one, trying out all possible orderings of peoples' names:
     *
     *
     *         *        *
     *          \      /
     *           * -- *
     *          /      \
     *         *        *
     *
     * There are 6! = 720 possible permutations of the ordering of these six
     * people. If your code is able to solve the problem correctly for all of
     * those orderings, there's a good chance that you're correctly tracking
     * who is matched at each step. On the other hand, if your code runs into
     * issues here, it may indicate that there's a bug in how you mark who's
     * paired and who isn't.
     */
    Vector<string> people = { "A", "B", "C", "D", "E", "F" };
    do {
        Map<string, Set<string>> links = fromLinks({
            { people[0], people[2] },
            { people[1], people[2] },
            { people[2], people[3] },
            { people[3], people[4] },
            { people[3], people[5] },
        });

        Set<Pair> matching;
        EXPECT(!hasPerfectMatching(links, matching));
    } while (next_permutation(people.begin(), people.end()));
}

PROVIDED_TEST("hasPerfectMatching works on a more complex positive example.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over people when making decisions is sensitive
     * to the order of those peoples' names. This test looks at a group like
     * this one, trying out all possible orderings of peoples' names:
     *
     *               *
     *               |
     *               *
     *              / \
     *             *---*
     *            /     \
     *           *       *
     *
     * There are 6! = 720 possible permutations of the ordering of these
     * people. If your code is able to solve the problem correctly for all of
     * those orderings, there's a good chance that you're correctly tracking
     * who is matched at each step. On the other hand, if your code runs into
     * issues here, it may indicate that there's a bug in how you mark who's
     * paired and who isn't.
     */
    Vector<string> people = { "A", "B", "C", "D", "E", "F" };
    do {
        Map<string, Set<string>> links = fromLinks({
            { people[0], people[1] },
            { people[1], people[2] },
            { people[2], people[3] },
            { people[3], people[1] },
            { people[2], people[4] },
            { people[3], people[5] },
        });

        Set<Pair> matching;
        EXPECT(hasPerfectMatching(links, matching));
        EXPECT(isPerfectMatching(links, matching));
    } while (next_permutation(people.begin(), people.end()));
}

PROVIDED_TEST("hasPerfectMatching works on a caterpillar.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over people when making decisions is sensitive
     * to the order of those peoples' names. This test looks at a group like
     * this one, trying out all possible orderings of peoples' names:
     *
     *         *---*---*
     *         |   |   |
     *         *   *   *
     *
     * There are 6! = 720 possible permutations of the ordering of these six
     * people. If your code is able to solve the problem correctly for all of
     * those orderings, there's a good chance that you're correctly tracking
     * who is matched at each step. On the other hand, if your code runs into
     * issues here, it may indicate that there's a bug in how you mark who's
     * paired and who isn't.
     */
    Vector<string> people = { "A", "B", "C", "D", "E", "F" };
    do {
        Map<string, Set<string>> links = fromLinks({
            { people[0], people[1] },
            { people[1], people[2] },
            { people[0], people[3] },
            { people[1], people[4] },
            { people[2], people[5] },
        });

        Set<Pair> matching;
        EXPECT(hasPerfectMatching(links, matching));
        EXPECT(isPerfectMatching(links, matching));
    } while (next_permutation(people.begin(), people.end()));
}

PROVIDED_TEST("hasPerfectMatching stress test: negative example (should take under a second).") {
    /* Here, we're giving a "caterpillar" of people, like this:
     *
     *    *   *   *   *     *   *
     *    |   |   |   |     |   |
     *    *---*---*---* ... *---*
     *    |   |   |   |     |   |
     *    *   *   *   *     *   *
     *
     * This doesn't have a perfect matching, However, it may take some searching
     * to confirm this is the case. At this size, however, it should be
     * almost instanteous to find the solution, since the search space is fairly
     * small and most "wrong" decisions can be detected quickly.
     *
     * On the other hand, if your implementation repeatedly constructs the same
     * matchings over and over again, or keeps exploring even when a person who
     * couldn't be paired with the current setup is found, then the number of
     * options you need to consider will be too large for your computer to handle
     * in any reasonable time.
     *
     * If you're passing the other tests and this test hangs, double-check your
     * code to make sure you aren't repeatedly constructing the same matchings
     * multiple times.
     */

    /* Number of "body segments". */
    const int kRowSize = 10;

    Vector<Pair> links;
    for (int i = 0; i < kRowSize - 1; i++) {
        links.add({ to_string(i), to_string(i + 1) });
    }
    for (int i = 0; i < kRowSize; i++) {
        links.add({ to_string(i), to_string(i + kRowSize) });
    }
    for (int i = 0; i < kRowSize; i++) {
        links.add({ to_string(i), to_string(i + 2 * kRowSize) });
    }

    Set<Pair> matching;
    EXPECT(!hasPerfectMatching(fromLinks(links), matching));
}

PROVIDED_TEST("hasPerfectMatching stress test: positive example (should take under a second).") {
    /* Here, we're giving a "millipede" of people, like this:
     *
     *    *---*---*---* ... *---*
     *    |   |   |   |     |   |
     *    *   *   *   *     *   *
     *
     * This always has a perfect matching, which is found by pairing each person
     * with the person directly below them. However, it may take some searching
     * to find this particular configuration. At this size, however, it should be
     * almost instanteous to find the solution, since the search space is fairly
     * small and most "wrong" decisions can be detected quickly.
     *
     * On the other hand, if your implementation repeatedly constructs the same
     * matchings over and over again, or keeps exploring even when a person who
     * couldn't be paired with the current setup is found, then the number of
     * options you need to consider will be too large for your computer to handle
     * in any reasonable time.
     *
     * If you're passing the other tests and this test hangs, double-check your
     * code to make sure you aren't repeatedly constructing the same matchings
     * multiple times.
     */

    /* Number of "body segments". */
    const int kRowSize = 10;

    Vector<Pair> links;
    for (int i = 0; i < kRowSize - 1; i++) {
        links.add({ to_string(i), to_string(i + 1) });
    }
    for (int i = 0; i < kRowSize; i++) {
        links.add({ to_string(i), to_string(i + kRowSize) });
    }

    Set<Pair> matching;
    EXPECT(hasPerfectMatching(fromLinks(links), matching));
    EXPECT(isPerfectMatching(fromLinks(links), matching));
}

PROVIDED_TEST("maximumWeightMatching: Works for empty group.") {
    EXPECT_EQUAL(maximumWeightMatching({}), {});
}

PROVIDED_TEST("maximumWeightMatching: Works for group of one person.") {
    Map<string, Map<string, int>> links = {
        { "A", {} }
    };

    EXPECT_EQUAL(maximumWeightMatching(links), {});
}

PROVIDED_TEST("maximumWeightMatching: Works for a single pair of people.") {
    /* This world:
     *
     *  A --- B
     *     1
     *
     * Best option is to pick A -- B.
     */
    auto links = fromWeightedLinks({
        { "A", "B", 1 }
    });

    /* Should pick A--B. */
    EXPECT_EQUAL(maximumWeightMatching(links), {{"A", "B"}});
}

PROVIDED_TEST("maximumWeightMatching: Won't pick a negative edge.") {
    /* This world:
     *
     *  A --- B
     *     -1
     *
     * Best option is to not match anyone!
     */
    auto links = fromWeightedLinks({
        { "A", "B", -1 }
    });

    /* Should pick A--B. */
    EXPECT_EQUAL(maximumWeightMatching(links), {});
}

PROVIDED_TEST("maximumWeightMatching: Works on a line of three people.") {
    /* This world:
     *
     *  A --- B --- C
     *     1     2
     *
     * Best option is to pick B -- C.
     */
    auto links = fromWeightedLinks({
        { "A", "B", 1 },
        { "B", "C", 2 },
    });

    /* Should pick B--C. */
    EXPECT_EQUAL(maximumWeightMatching(links), { {"B", "C"} });
}

PROVIDED_TEST("maximumWeightMatching: Works on a triangle.") {
    /* This world:
     *
     *         A
     *      1 / \ 2
     *       B---C
     *         3
     *
     * Best option is to pick B -- C.
     */
    auto links = fromWeightedLinks({
        { "A", "B", 1 },
        { "B", "C", 3 },
        { "A", "C", 2 }
    });

    /* Should pick B--C. */
    EXPECT_EQUAL(maximumWeightMatching(links), { {"B", "C"} });
}

PROVIDED_TEST("maximumWeightMatching: Works on a square.") {
    /* This world:
     *
     *         1
     *      A --- B
     *      |     |
     *    8 |     | 2
     *      |     |
     *      D --- C
     *         4
     *
     * Best option is to pick BC/AD.
     */
    auto links = fromWeightedLinks({
        { "A", "B", 1 },
        { "B", "C", 2 },
        { "C", "D", 4 },
        { "D", "A", 8 },
    });

    EXPECT_EQUAL(maximumWeightMatching(links), { {"A", "D"}, {"B", "C"} });
}

PROVIDED_TEST("maximumWeightMatching: Works on a line of four people.") {
    /* This world:
     *
     *  A --- B --- C --- D
     *     1     3     1
     *
     * Best option is to pick B -- C, even though this is not a perfect
     * matching.
     */
    auto links = fromWeightedLinks({
        { "A", "B", 1 },
        { "B", "C", 3 },
        { "C", "D", 1 },
    });

    /* Should pick B--C. */
    EXPECT_EQUAL(maximumWeightMatching(links), { {"B", "C"} });
}

PROVIDED_TEST("maximumWeightMatching: Small stress test (should take at most a second or two).") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over people when making decisions is sensitive
     * to the order of those peoples' names. This test looks at a group like
     * this one, trying out all possible orderings of peoples' names:
     *
     *               *
     *               | 1
     *               *
     *            1 / \ 5
     *             *---*
     *          1 /  1  \ 1
     *           *       *
     *
     * (Best option is to pick the 5-cost edge and the opposite-side 1-cost
     * edge.)
     *
     * There are 6! = 720 possible permutations of the ordering of these
     * people. If your code is able to solve the problem correctly for all of
     * those orderings, there's a good chance that you're correctly tracking
     * who is matched at each step. On the other hand, if your code runs into
     * issues here, it may indicate that there's a bug in how you mark who's
     * paired and who isn't.
     */
    Vector<string> people = { "A", "B", "C", "D", "E", "F" };
    do {
        auto links = fromWeightedLinks({
            { people[0], people[1], 5 },
            { people[1], people[2], 1 },
            { people[2], people[0], 1 },
            { people[3], people[0], 1 },
            { people[4], people[1], 1 },
            { people[5], people[2], 1 },
        });

        Set<Pair> expected = {
            { people[0], people[1] },
            { people[2], people[5] }
        };

        EXPECT_EQUAL(maximumWeightMatching(links), expected);
    } while (next_permutation(people.begin(), people.end()));
}

PROVIDED_TEST("maximumWeightMatching: Large stress test (should take at most a second or two).") {
    /* Here, we're giving a chain of people, like this:
     *
     *    *---*---*---*---*---*---*---* ... *---*
     *      1   1   1   1   1   1   1         1
     *
     * The number of different matchings in a chain of n people is given by the
     * nth Fibonacci number. (Great exercise - can you explain why?) This means
     * that if we have a chain of 21 people, there are F(21) = 10,946 possible
     * matchings to check. If your program tests every single one of them exactly
     * once, then it should be pretty quick to determine what the best matching
     * here is. (It's any matching that uses exactly floor(21 / 2) = 10 edges.
     *
     * On the other hand, if your implementation repeatedly constructs the same
     * matchings over and over again, then the number of options you need to consider
     * will be too large for your computer to handle in any reasonable time.
     *
     * If you're passing the other tests and this test hangs, double-check your
     * code to make sure you aren't repeatedly constructing the same matchings
     * multiple times.
     */
    const int kNumPeople = 21;
    Vector<WeightedLink> links;
    for (int i = 0; i < kNumPeople - 1; i++) {
        links.add({ to_string(i), to_string(i + 1), 1 });
    }

    auto matching = maximumWeightMatching(fromWeightedLinks(links));
    EXPECT_EQUAL(matching.size(), kNumPeople / 2);

    /* Confirm it's a matching. */
    Set<string> used;
    for (Pair p: matching) {
        /* No people paired more than once. */
        EXPECT(!used.contains(p.first()));
        EXPECT(!used.contains(p.second()));
        used += p.first();
        used += p.second();

        /* Must be a possible links. */
        EXPECT_EQUAL(abs(stringToInteger(p.first()) - stringToInteger(p.second())), 1);
    }
}
