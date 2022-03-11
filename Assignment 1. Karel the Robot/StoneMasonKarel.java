/*
 * File: StoneMasonKarel.java
 * --------------------------
 * The StoneMasonKarel subclass as it appears here does nothing.
 * When you finish writing it, it should solve the "repair the quad"
 * problem from Assignment 1.  In addition to editing the program,
 * you should be sure to edit this comment so that it no longer
 * indicates that the program does nothing.
 */

import stanford.karel.*;

public class StoneMasonKarel extends SuperKarel {

	@Override
	public void run() {
		// 问题求解：大规模问题（以柱为单位）
		while (frontIsClear()) { // 1. 判断是否还有下一个柱子需要维修
			repairColumn();
			moveToNextColumn();
		}
		repairColumn(); // 2. 直接维修当前柱子(防止只有一根柱子的情况: fencepost error)
	}

	/*
	 * pre-condition: facing east at the bottom post-condition: same as
	 * pre-condition
	 */
	private void repairColumn() {
		turnLeft();

		// 问题求解：小规模问题（以格为单位）
		while (frontIsClear()) {
			repairRow();
			moveToNextRow();
		}
		repairRow();

		turnAround();
		moveToBottom();
		turnLeft();
	}

	// pre-condition: front is clear and facing east at the bottom
	private void moveToNextColumn() {
		move();
		move();
		move();
		move();
	}

	private void repairRow() {
		if (noBeepersPresent()) {
			putBeeper();
		}
	}

	// pre-condition: front is clear and facing north
	private void moveToNextRow() {
		move();
	}

	private void moveToBottom() {
		while (frontIsClear()) {
			move();
		}
	}
}
