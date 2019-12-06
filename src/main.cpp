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

//    b->MakeMove({3, 0, 3, 2, {true, board::ptype::QUEEN}, false});
//    b->MakeMove({0, 6, 0, 5, {true, board::ptype::PAWN}, false});

    d.ShowBoard(b);
    cout << "Evaluation: " << engine::evalb(b) << endl;
    std::set<board::move> moves = b->PossibleMoves();
    for (board::move s : moves) {
        cout << s.toString() << endl;
    }

    engine::moveoption move = engine::bestmove(b);
    cout << "Best Move: ";
    cout << move.m.toString() << " with value " << move.evaluation << endl;

    while(true) {
        std::set<board::move> mo = b->PossibleMoves();
        for (board::move s : mo) {
            cout << "Possibility " <<  s.toString() << endl;
        }
        string s = getLine();
        b->MakeMove(s);
        d.ShowBoard(b);

        cout << "Evaluation: " << engine::evalb(b) << endl;
        std::set<board::move> moves = b->PossibleMoves();
        engine::moveoption move = engine::bestmove(b);
        cout << "Best Move: ";
        cout << move.m.toString() << " with value " << move.evaluation << endl;
        b->MakeMove(move.m);
        d.ShowBoard(b);
    }
    return 0;
}
