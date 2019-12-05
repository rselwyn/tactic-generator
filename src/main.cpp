#include <iostream>
#include "console.h"
#include "display.h"
#include "board.h"
#include "simpio.h"
#include "engine.h"

using namespace std;

int main() {
    display d;
    engine e;
    board *b = new board;
    d.ShowBoard(b);
    cout << "Evaluation " << e.evalb(b) << endl;
    while(true) {
        string s = getLine();
        cout << s << endl;
        b->MakeMove({1, 0, 2, 2, {true, board::ptype::KNIGHT}, false});
        d.ShowBoard(b);
        cout << "Evaluation " << e.evalb(b) << endl;
    }
    return 0;
}
