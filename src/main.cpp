#include <iostream>
#include "console.h"
#include "display.h"
#include "board.h"
#include "simpio.h"
#include "engine.h"

using namespace std;

int main() {
    display d;
    board *b = new board;
    d.ShowBoard(b);
//    b->MakeMove({0, 6, 0, 5, {true, board::ptype::PAWN}, false});

//    d.ShowBoard(b);
//    b->MakeMove({0, 1, 0, 6, {true, board::ptype::PAWN}, false});
//    d.ShowBoard(b);
//    b->UndoLast();
//    getLine();
//    d.ShowBoard(b);

    while(true) {
        string s = getLine();
        b->MakeMove(s);
        d.ShowBoard(b);

        engine::kConsidered = 0;
        cout << "Evaluation: " << engine::evalb(b) << endl;
        std::set<board::move> moves = b->PossibleMoves();
        engine::moveoption move = engine::bestmove(b);
        cout << "Nodes: " << engine::kConsidered << endl;

        cout << "Best Move: ";
        cout << move.m.toString() << " with value " << move.evaluation << endl;
        b->MakeMove(move.m);
        d.ShowBoard(b);
        engine::kConsidered = 0;
    }
    return 0;
}
