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
    b->MakeMove({3, 1, 3, 3, {true, board::ptype::PAWN}, false});
    b->MakeMove({4, 1, 4, 3, {true, board::ptype::PAWN}, false});
    b->MakeMove({2, 0, 5, 3, {true, board::ptype::BISHOP}, false});

    d.ShowBoard(b);
    cout << "Evaluation " << e.evalb(b) << endl;
    std::set<board::move> moves = b->PossibleMoves();

    for (board::move s : moves) {
        cout << "(" << ((char) ('a' + s.s1)) << (s.s2+1) << " " << ((char) ('a' + (s.d1))) << (s.d2+1) << ")"  << endl;
    }

    while(true) {
        string s = getLine();
        cout << s << endl;
        d.ShowBoard(b);
        cout << "Evaluation " << e.evalb(b) << endl;
    }
    return 0;
}
