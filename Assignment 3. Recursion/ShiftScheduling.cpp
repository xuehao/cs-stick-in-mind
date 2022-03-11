#include "ShiftScheduling.h"
using namespace std;

#define VERSION_2 // Modify MACRO to select different version

#ifdef VERSION_3 // better
/**
 * @brief Calculate the value of the given schedule
 * @param possibleSchedule
 * @return int
 */
int valueOfSchedule(Set<Shift> schedule) {
    int value = 0;
    for (Shift shift : schedule) {
        value += valueOf(shift);
    }
    return value;
}

/**
 * @brief Check if the given shift overlap with the given schedule
 * @param shift
 * @param schedule
 * @return true/false
 */
bool overlapWithSchedule(Shift shift, Set<Shift> schedule) {
    bool overlap = false;
    for (Shift item : schedule) {
        if (overlapsWith(shift, item)) {
            overlap = true;
        }
    }
    return overlap;
}

Set<Shift> highestValueScheduleForRec(const Set<Shift> &shifts, int maxHours,
                                      const Set<Shift> &sofar) {
    if (maxHours == 0 || shifts.size() == 0) {
        return sofar;
    }

    Shift shift = shifts.first();
    Set<Shift> remainingShifts = shifts - shift;

    auto without = highestValueScheduleForRec(remainingShifts, maxHours, sofar);
    int value1 = valueOfSchedule(without);

    int remainingHours = maxHours - lengthOf(shift);
    if (!overlapWithSchedule(shift, sofar)) { // is overlap?
        if (remainingHours < 0) {             // is over limit hour?
            int value2 = valueOfSchedule(sofar);
            return value1 > value2 ? without : sofar;
        } else {
            auto with = highestValueScheduleForRec(remainingShifts, remainingHours, sofar + shift);
            int value2 = valueOfSchedule(with);
            return value1 > value2 ? without : with;
        }
    }

    return without;
}

Set<Shift> highestValueScheduleFor(const Set<Shift> &shifts, int maxHours) {
    if (maxHours < 0) {
        error("You can't provide non-positive hours.");
    }

    return highestValueScheduleForRec(shifts, maxHours, {});
}
#endif

#ifdef VERSION_2 // better but still not good idea
bool overlapWithSchedule(Shift shift, Set<Shift> schedule) {
    bool overlap = false;
    for (Shift item : schedule) {
        if (overlapsWith(shift, item)) {
            overlap = true;
        }
    }
    return overlap;
}

Set<Set<Shift>> allPossibleSchedulesRec(const Set<Shift> &shifts, int maxHours,
                                        const Set<Shift> &sofar) {
    if (maxHours == 0 || shifts.size() == 0) {
        return {sofar};
    }

    Shift shift = shifts.first();
    int remainingHour = maxHours - lengthOf(shift);
    Set<Shift> remainingShifts = shifts - shift;

    auto without = allPossibleSchedulesRec(remainingShifts, maxHours, sofar);

    if (!overlapWithSchedule(shift, sofar)) { // is overlap?
        if (remainingHour < 0) {              // is over limit hour?
            return without + sofar;
        } else {
            auto with = allPossibleSchedulesRec(remainingShifts, remainingHour, sofar + shift);
            return with + without;
        }
    }

    return without;
}

Set<Shift> pickHighestValueSchedule(Set<Set<Shift>> allSchedules) {
    int highValue = 0;
    Set<Shift> highestValueSchedule = allSchedules.first();
    for (const Set<Shift> &schedule : allSchedules) {
        int tempValue = 0;
        for (Shift shift : schedule) {
            tempValue += valueOf(shift);
        }
        if (tempValue > highValue) {
            highValue = tempValue;
            highestValueSchedule = schedule;
        }
    }
    return highestValueSchedule;
}

Set<Shift> highestValueScheduleFor(const Set<Shift> &shifts, int maxHours) {
    if (maxHours < 0) {
        error("You can't provide non-positive hours.");
    }

    Set<Set<Shift>> allPossibleSchedules = allPossibleSchedulesRec(shifts, maxHours, {});

    return pickHighestValueSchedule(allPossibleSchedules);
}
#endif

#ifdef VERSION_1 // rough thought - not good idea
Set<Set<Shift>> allPossibleSchedulesRec(const Set<Shift> &shifts, int maxHours, const Set<Shift> &sofar) {
    if (maxHours == 0 || shifts.size() == 0) {
        return {sofar};
    }

    Shift shift = shifts.first();
    int remainingHour = maxHours - lengthOf(shift);
    Set<Shift> remainingShifts = shifts - shift;

    bool isOverlap = false;
    for (Shift item : sofar) { // is overlap?
        if (overlapsWith(shift, item)) {
            isOverlap = true;
        }
    }

    if (isOverlap) {
        auto without = allPossibleSchedulesRec(remainingShifts, maxHours, sofar);
        return without;
    } else {
        auto without = allPossibleSchedulesRec(remainingShifts, maxHours, sofar);
        if (remainingHour < 0) { // is over limit hour?
            auto with = allPossibleSchedulesRec(remainingShifts, 0, sofar);
            return with + without;
        } else {
            auto with = allPossibleSchedulesRec(remainingShifts, remainingHour, sofar + shift);
            return with + without;
        }
    }
}

Set<Shift> highestValueScheduleFor(const Set<Shift> &shifts, int maxHours) {
    if (maxHours < 0) {
        error("You can't provide non-positive hours.");
    }

    Set<Set<Shift>> allPossibleSchedules = allPossibleSchedulesRec(shifts, maxHours, {});

    int highValue = 0;
    Set<Shift> highestValueSchedule = allPossibleSchedules.first();
    for (const Set<Shift> &schedule : allPossibleSchedules) {
        int tempValue = 0;
        for (Shift shift : schedule) {
            tempValue += valueOf(shift);
        }
        if (tempValue > highValue) {
            highValue = tempValue;
            highestValueSchedule = schedule;
        }
    }

    return highestValueSchedule;
}
#endif

/* * * * * * Test Cases * * * * * */
#include "GUI/SimpleTest.h"
#include "vector.h"
#include "error.h"

/* This nice utility function lets you call highestValueScheduleFor, passing in
 * a Vector of shifts rather than a Set. This makes it a bit easier to test things.
 * You shouldn't need this function outside of these test cases.
 */
Set<Shift> asSet(const Vector<Shift>& shifts) {
    Set<Shift> result;
    for (Shift s: shifts) {
        result += s;
    }
    return result;
}

/* TODO: Add your own tests here. You know the drill - look for edge cases, think about
 * very small and very large cases, etc.
 */
STUDENT_TEST("Don't use all maxHours.") {
    Vector<Shift> shifts = {
        { Day::MONDAY,    10, 17, 21 }, //  7-hour shift, value is 21 ($3 / hour)
        { Day::TUESDAY,   10, 16, 12 }, //  6-hour shift, value is 12 ($2 / hour)
        { Day::WEDNESDAY, 10, 16, 12 }, //  6-hour shift, value is 12 ($2 / hour)
    };

    auto schedule = highestValueScheduleFor(asSet(shifts), 11);

    EXPECT_EQUAL(schedule, { shifts[0] });
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Pick only shift if you have time for it.") {
    Set<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday, 9AM - 5PM, value is 1000
    };

    EXPECT_EQUAL(highestValueScheduleFor(shifts, 24).size(), 1);
    EXPECT_EQUAL(highestValueScheduleFor(shifts, 24), shifts);
}

PROVIDED_TEST("Don't pick only shift if you don't have time for it.") {
    Set<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday, 9AM - 5PM, value is 1000
    };

    EXPECT_EQUAL(highestValueScheduleFor(shifts, 3).size(), 0);
}

PROVIDED_TEST("Don't pick overlapping shifts.") {
    Vector<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday,  9AM - 5PM, value is 1000
        { Day::MONDAY, 8, 18, 2000 },  // Monday, 10AM - 6PM, value is 2000
    };

    EXPECT_EQUAL(highestValueScheduleFor(asSet(shifts), 100), { shifts[1] });
}

PROVIDED_TEST("Doesn't always use highest-value shift.") {
    Vector<Shift> shifts = {
        { Day::MONDAY,    10, 20, 1000 }, // 10-hour shift, value is 1000
        { Day::TUESDAY,   10, 15,  500 }, //  5-hour shift, value is 500
        { Day::WEDNESDAY, 10, 16,  501 }, //  6-hour shift, value is 501
    };

    /* The correct strategy is to forgo the highest-value shift in favor of the two
     * shorter shifts.
     */
    auto schedule = highestValueScheduleFor(asSet(shifts), 11);

    EXPECT_EQUAL(schedule, { shifts[1], shifts[2] });
}

PROVIDED_TEST("Doesn't always use the shift with the highest value per unit time.") {
    Vector<Shift> shifts = {
        { Day::MONDAY,    10, 17, 21 }, //  7-hour shift, value is 21 ($3 / hour)
        { Day::TUESDAY,   10, 16, 12 }, //  6-hour shift, value is 12 ($2 / hour)
        { Day::WEDNESDAY, 10, 16, 12 }, //  6-hour shift, value is 12 ($2 / hour)
    };

    /* If you have 12 hours, the correct strategy is to pick the two six-hour shifts
     * for a total of $24 value. Picking the shift with the highest value per unit
     * time (the seven-hour shift) produces only $21 value.
     */
    auto schedule = highestValueScheduleFor(asSet(shifts), 12);

    EXPECT_EQUAL(schedule, { shifts[1], shifts[2] });
}

PROVIDED_TEST("Passes the example from the assignment description.") {
    Vector<Shift> shifts = {
        { Day::MONDAY,     8, 12, 27 },  // Mon  8AM - 12PM, value 27 *
        { Day::MONDAY,    12, 16, 28 },  // Mon 12PM -  4PM, value 28 *
        { Day::MONDAY,    16, 20, 25 },  // Mon  4PM -  8PM, value 25 *
        { Day::MONDAY,     8, 14, 39 },  // Mon  8AM -  2PM, value 39
        { Day::MONDAY,    14, 20, 31 },  // Mon  2PM -  8PM, value 31
        { Day::TUESDAY,    8, 12,  7 },  // Tue  8AM - 12PM, value  7
        { Day::TUESDAY,   12, 16,  7 },  // Tue 12PM -  4PM, value  7
        { Day::TUESDAY,   16, 20, 11 },  // Tue  4PM -  8PM, value 11
        { Day::TUESDAY,    8, 14, 10 },  // Tue  8AM -  2PM, value 10
        { Day::TUESDAY,   14, 20,  8 },  // Tue  2PM -  8PM, value  8
        { Day::WEDNESDAY,  8, 12, 10 },  // Wed  8AM - 12PM, value 10
        { Day::WEDNESDAY, 12, 16, 11 },  // Wed 12PM -  4PM, value 11
        { Day::WEDNESDAY, 16, 20, 13 },  // Wed  4PM -  8PM, value 13
        { Day::WEDNESDAY,  8, 14, 19 },  // Wed  8AM -  2PM, value 19
        { Day::WEDNESDAY, 14, 20, 25 },  // Wed  2PM -  8PM, value 25 *
    };

    /* Get back the solution. */
    Set<Shift> computedSolution = highestValueScheduleFor(asSet(shifts), 20);

    /* Form the correct answer. It's the starred entries. */
    Set<Shift> actualSolution = {
        shifts[0], shifts[1], shifts[2], shifts[14]
    };

    EXPECT_EQUAL(computedSolution, actualSolution);
}

PROVIDED_TEST("Handles no shifts.") {
    EXPECT_EQUAL(highestValueScheduleFor({}, 137).size(), 0);
}

PROVIDED_TEST("Reports an error with negative hours.") {
    /* From the assignment description. */
    Vector<Shift> shifts = {
        { Day::MONDAY,     8, 12, 27 },  // Mon  8AM - 12PM, value 27
        { Day::MONDAY,    12, 16, 28 },  // Mon 12PM -  4PM, value 28
        { Day::MONDAY,    16, 20, 25 },  // Mon  4PM -  8PM, value 25
        { Day::MONDAY,     8, 14, 39 },  // Mon  8AM -  2PM, value 39
        { Day::MONDAY,    14, 20, 31 },  // Mon  2PM -  8PM, value 31
        { Day::TUESDAY,    8, 12,  7 },  // Tue  8AM - 12PM, value  7
        { Day::TUESDAY,   12, 16,  7 },  // Tue 12PM -  4PM, value  7
        { Day::TUESDAY,   16, 20, 11 },  // Tue  4PM -  8PM, value 11
        { Day::TUESDAY,    8, 14, 10 },  // Tue  8AM -  2PM, value 10
        { Day::TUESDAY,   14, 20,  8 },  // Tue  2PM -  8PM, value  8
        { Day::WEDNESDAY,  8, 12, 10 },  // Wed  8AM - 12PM, value 10
        { Day::WEDNESDAY, 12, 16, 11 },  // Wed 12PM -  4PM, value 11
        { Day::WEDNESDAY, 16, 20, 13 },  // Wed  4PM -  8PM, value 13
        { Day::WEDNESDAY,  8, 14, 19 },  // Wed  8AM -  2PM, value 19
        { Day::WEDNESDAY, 14, 20, 25 },  // Wed  2PM -  8PM, value 25
    };

    /* Should be an error. */
    EXPECT_ERROR(highestValueScheduleFor(asSet(shifts), -1));

    /* Still an error even if there are no shifts. */
    EXPECT_ERROR(highestValueScheduleFor({}, -1));
}

PROVIDED_TEST("Handles zero free hours.") {
    /* From the assignment description. */
    Vector<Shift> shifts = {
        { Day::MONDAY,     8, 12, 27 },  // Mon  8AM - 12PM, value 27
        { Day::MONDAY,    12, 16, 28 },  // Mon 12PM -  4PM, value 28
        { Day::MONDAY,    16, 20, 25 },  // Mon  4PM -  8PM, value 25
        { Day::MONDAY,     8, 14, 39 },  // Mon  8AM -  2PM, value 39
        { Day::MONDAY,    14, 20, 31 },  // Mon  2PM -  8PM, value 31
        { Day::TUESDAY,    8, 12,  7 },  // Tue  8AM - 12PM, value  7
        { Day::TUESDAY,   12, 16,  7 },  // Tue 12PM -  4PM, value  7
        { Day::TUESDAY,   16, 20, 11 },  // Tue  4PM -  8PM, value 11
        { Day::TUESDAY,    8, 14, 10 },  // Tue  8AM -  2PM, value 10
        { Day::TUESDAY,   14, 20,  8 },  // Tue  2PM -  8PM, value  8
        { Day::WEDNESDAY,  8, 12, 10 },  // Wed  8AM - 12PM, value 10
        { Day::WEDNESDAY, 12, 16, 11 },  // Wed 12PM -  4PM, value 11
        { Day::WEDNESDAY, 16, 20, 13 },  // Wed  4PM -  8PM, value 13
        { Day::WEDNESDAY,  8, 14, 19 },  // Wed  8AM -  2PM, value 19
        { Day::WEDNESDAY, 14, 20, 25 },  // Wed  2PM -  8PM, value 25
    };

    /* Shouldn't be an error if time is zero - that means we just don't pick anything. */
    EXPECT_EQUAL(highestValueScheduleFor(asSet(shifts), 0).size(), 0);
}

PROVIDED_TEST("Stress test: Don't generate shift combinations with overlapping shifts.") {
    /* All of these shifts overlap one another. If you try producing all combinations
     * of these shifts and only check at the end whether they're valid, you'll be
     * checking 2^100 ~= 10^30 combinations of shifts, which will take so long the
     * sun will have burnt out before you're finished.
     *
     * Instead, as you're going through your decision tree and building up your shifts,
     * make sure not to include any shifts that clearly conflict with something you
     * picked earlier.
     */
    Set<Shift> trickySet;
    for (int i = 0; i < 100; i++) {
        trickySet += Shift{ Day::MONDAY, 1, 2, i };
    }
    EXPECT_EQUAL(trickySet.size(), 100);

    auto result = highestValueScheduleFor(trickySet, 1);
    EXPECT_EQUAL(result.size(), 1);
}

PROVIDED_TEST("Stress test: Don't generate shift combinations that exceed time limits.") {
    /* Here's a collection of one shift per hour of the week. Your worker has exactly
     * one hour free. If you try all possible combinations of these shifts, ignoring time
     * constraints, you will have to check over 2^100 = 10^30 combinations, which will
     * take longer than the length of the known universe to process.
     *
     * Instead, as you're exploring the decision tree to generate shift combinations,
     * make sure not to add shifts that would exceed the time limit.
     */
    Set<Shift> trickySet;
    for (Day day: { Day::SUNDAY,
                    Day::MONDAY,
                    Day::TUESDAY,
                    Day::WEDNESDAY,
                    Day::THURSDAY,
                    Day::FRIDAY,
                    Day::SATURDAY}) {
        for (int start = 0; start < 24; start++) {
            trickySet += Shift{ day, start, start + 1, 10 };
        }
    }
    EXPECT_EQUAL(trickySet.size(), 7 * 24);

    auto result = highestValueScheduleFor(trickySet, 1);
    EXPECT_EQUAL(result.size(), 1);
}

PROVIDED_TEST("Stress test: Can handle a decent number of shifts (should take at most 10-15 seconds)") {
    /* Available shifts. */
    Vector<Shift> shifts = {
        { Day::SUNDAY,  8, 14, 12 },
        { Day::SUNDAY, 12, 18, 36 },

        { Day::MONDAY,  8, 12, 44 },
        { Day::MONDAY, 12, 16, 32 },
        { Day::MONDAY, 16, 20,  0 },
        { Day::MONDAY,  8, 16, 16 },
        { Day::MONDAY, 12, 20, 22 },

        { Day::TUESDAY,  8, 12, 48 },
        { Day::TUESDAY, 12, 16, 20 },
        { Day::TUESDAY, 16, 20, 24 },
        { Day::TUESDAY,  8, 16, 24 },
        { Day::TUESDAY, 12, 20, 80 },

        { Day::WEDNESDAY,  8, 12, 20 },
        { Day::WEDNESDAY, 12, 16,  8 },
        { Day::WEDNESDAY, 16, 20,  8 },
        { Day::WEDNESDAY,  8, 16, 40 },
        { Day::WEDNESDAY, 12, 20, 16 },

        { Day::THURSDAY,  8, 12, 40 },
        { Day::THURSDAY, 12, 16,  0 },
        { Day::THURSDAY, 16, 20, 24 },
        { Day::THURSDAY,  8, 16, 56 },
        { Day::THURSDAY, 12, 20, 32 },

        { Day::FRIDAY,  8, 12,  4 },
        { Day::FRIDAY, 12, 16,  8 },
        { Day::FRIDAY, 16, 20, 40 },
        { Day::FRIDAY,  8, 16, 72 },
        { Day::FRIDAY, 12, 20, 40 },

        { Day::SATURDAY,  8, 14, 18 },
        { Day::SATURDAY, 12, 18, 66 },
    };

    auto answer = highestValueScheduleFor(asSet(shifts), 30);
    EXPECT_EQUAL(answer, { shifts[2], shifts[7], shifts[11], shifts[17], shifts[24], shifts[28] });
}
