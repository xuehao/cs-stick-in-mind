/*
 * File: CheckerboardKarel.java
 * ----------------------------
 * When you finish writing it, the CheckerboardKarel class should draw
 * a checkerboard using beepers, as described in Assignment 1.  You
 * should make sure that your program works for all of the sample
 * worlds supplied in the starter folder.
 */

import stanford.karel.*;

public class CheckerboardKarel extends SuperKarel {

	@Override
	public void run() {
		putBeeper();
		if (frontIsBlocked()) {
			fixOneColumnCase(); // Special case
		} else {
			fixGeneralCase(); // General case
		}
	}

	/**
	 * Make a checker board in a one-column world.
	 * 
	 * Precondition: Karel is facing east and blocked at the start point within
	 * a one-column world.
	 * 
	 * Postcondition: Karel is blocked at the end of that column and make a
	 * checker board in that world.
	 */
	private void fixOneColumnCase() {
		turnLeft();
		while (frontIsClear()) {
			move();
			if (frontIsClear()) {
				move();
				putBeeper();
			}
		}
	}

	/**
	 * Make a checker board in any other kind of world(except for a one-column
	 * world).
	 * 
	 * Precondition: Karel is facing east at the start point(lower left corner).
	 * 
	 * Postcondition: Karel is blocked at the end of that column(upper corner)
	 * and make a checker board in that world.
	 */
	private void fixGeneralCase() {
		while (frontIsClear()) { // General case
			// pre-condition0: front is clear
			move();
			// post-condition0: front is clear/blocked
			// So, you need to deal with two conditions.
			if (frontIsClear()) {
				// pre-condition1: front is clear
				move();
				putBeeper();
				// post-condition1: front is clear/blocked
				// Again, you need to double check the blocked condition.
				// pre-condition3: front is blocked
				if (frontIsBlocked()) {
					moveUp();
					if (frontIsClear()) { // just in case moveUp() is failure
						move();
						putBeeper();
					}
				}
				/* post-condition3: front is clear */
			} else {
				// pre-condition 2: front is blocked
				moveUp();
				if (frontIsClear()) // just in case moveUp() is failure
					putBeeper();
				/* post-condition2: front is clear */
			}
		}
	}

	/**
	 * Make Karel to move up to the next row and change direction.
	 * 
	 * Precondition: Karel is blocked and facing ease/west in the current row.
	 * 
	 * Postcondition: Karel is free and facing the opposite direction in the
	 * upper row. If there's no more row, Karel will not move up, but change to
	 * face north and stop instead.
	 */
	private void moveUp() {
		if (facingEast()) {
			turnLeft();
			if (frontIsClear()) {
				move();
				turnLeft();
			}
		} else {
			turnRight();
			if (frontIsClear()) {
				move();
				turnRight();
			}
		}
	}
}
