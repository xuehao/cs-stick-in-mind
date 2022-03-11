/*
 * File: MidpointFindingKarel.java
 * -------------------------------
 * When you finish writing it, the MidpointFindingKarel class should
 * leave a beeper on the corner closest to the center of 1st Street
 * (or either of the two central corners if 1st Street has an even
 * number of corners).  Karel can put down additional beepers as it
 * looks for the midpoint, but must pick them up again before it
 * stops.  The world may be of any size, but you are allowed to
 * assume that it is at least as tall as it is wide.
 */

import stanford.karel.*;

public class MidpointFindingKarel extends SuperKarel {

	@Override
	public void run() {
		if (frontIsBlocked()) {
			putBeeper();
		} else {
			putBeeperStreet();
			pickSideBeeper();
			while (beepersPresent())
				pickTheOtherSideBeeper();
			putMidPointBeeper();
		}
	}

	// pre-condition: no beeper on the street
	// post-condition: put beeper on the street(except starting point and end
	// point)
	private void putBeeperStreet() {
		while (frontIsClear()) {
			move();
			if (frontIsClear())
				putBeeper();
		}
	}

	// pre-condition: a beeper street in front
	// post-condition: go to the other side, clean the side beeper
	// and stand at the new street side
	private void pickTheOtherSideBeeper() {
		while (frontIsClear() && beepersPresent())
			move();
		pickSideBeeper();
	}

	// pre-condition: a beeper street behind
	// post-condition: facing the beeper street, clean the side beeper
	// and stand at the new street side
	private void pickSideBeeper() {
		changeDirect();
		if (beepersPresent()) {
			pickBeeper();
			move();
		}
	}

	// pre-condition: beside the mid point
	// post-condition: go to the mid point and put beeper
	private void putMidPointBeeper() {
		changeDirect();
		putBeeper();
	}

	private void changeDirect() {
		turnAround();
		if (frontIsClear()) {
			move();
		}
	}
}
