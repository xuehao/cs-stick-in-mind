#include "Plotter.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

/* 抽象一个 Pen 位置结构体 */
struct PenPosition {
    double x;
    double y;
    bool up; // Z 方向只有两个状态
};

// 仅练习引用传参，可直接修改
static void updatePosition(PenPosition &pos, double x, double y) {
    pos.x = x;
    pos.y = y;
}

void runPlotterScript(istream &input) {
    // 初始化画笔
    PenStyle style;
    style.width = 1;
    style.color = "black";
    PenPosition position;
    position.x = 0;
    position.y = 0;
    position.up = true;

    // 处理指令
    for (string line; getline(input, line);) {
        Vector<string> scripts = stringSplit(line, " ");
        string command = toLowerCase(scripts[0]);

        /**
         * PenDown
         *    position.up = false
         */
        if (startsWith(command, "pendown")) {
            position.up = false;
        }

        /**
         * PenUp
         *    position.up = true
         */
        if (startsWith(command, "penup")) {
            position.up = true;
        }

        /**
         * MoveAbs x y
         *    x = stringToReal(scripts[1]);
         *    y = stringToReal(scripts[2]);
         *
         *    penDown - drawLine(curX, curY, x, y)
         *    penUp - none
         *
         *    update position
         */
        if (startsWith(command, "moveabs")) {
            auto curX{stringToReal(scripts[1])};
            auto curY{stringToReal(scripts[2])};
            if (!position.up)
                drawLine(position.x, position.y, curX, curY, style);
            updatePosition(position, curX, curY);
        }

        /**
         * MoveRel x y
         *    x = stringToReal(scripts[1]) + dx;
         *    y = stringToReal(scripts[2]) + dy;
         *
         *    penDown - drawLine(curX, curY, x, y)
         *    penUp - none
         *
         *    update position
         */
        if (startsWith(command, "moverel")) {
            auto curX{stringToReal(scripts[1]) + position.x};
            auto curY{stringToReal(scripts[2]) + position.y};
            if (!position.up)
                drawLine(position.x, position.y, curX, curY, style);
            updatePosition(position, curX, curY);
        }

        /**
         * PenColor color
         *    style.color = color
         */
        if (startsWith(command, "pencolor")) {
            style.color = scripts[1];
        }

        /**
         * PenWidth width
         *    style.width = width
         */
        if (startsWith(command, "penwidth")) {
            style.width = stringToReal(scripts[1]);
        }
    }
}
