#include <iostream>
#include "console.h"
#include "display.h"
#include "board.h"
#include "simpio.h"
#include "engine.h"
#include "game.h"

using namespace std;

enum Mode {
    UNSET,
    FREE_PLAY,
    TACTIC
};

Mode mode = UNSET;

std::vector<game*> loadSavedTactics() {
    std::vector<ifstream*> files;
    game::OpenPossibleTactics(files);

    std::vector<game*> games;
    cout << "m iterate" << endl;

    for (ifstream* f : files) {
        cout << "iterate" << endl;
        std::vector<game*> temp = game::LoadFile(*f);
        cout << "Past " << endl;
        games.insert(games.end(), temp.begin(), temp.end());
        cout << "Past " << endl;
    }
    return games;
//    return {};
}

int main() {
    display d;
//    board *b = new board("2r2rk1/pp1bqpp1/2np1n1p/2pp2N1/1bP5/1P4P1/PBQPPPBP/3R1RK1");
    std::vector<game*> tact = loadSavedTactics();
    cout << tact.size() << endl;
    getLine();
    board *b = new board;
    b->ToString();
    getLine();
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
        std::vector<board::move> moves = b->PossibleMoves();
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
