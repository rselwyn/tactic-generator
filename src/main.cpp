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
    TACTIC,
    DIFFICULTY_EVAL
};

Mode mode = UNSET;

std::vector<game*> loadSavedTactics() {
    std::vector<ifstream*> files;
    game::OpenPossibleTactics(files);

    std::vector<game*> games;

    for (ifstream* f : files) {
        std::vector<game*> temp = game::LoadFile(*f);
        games.insert(games.end(), temp.begin(), temp.end());
    }
    return games;
}

void humanVsEngine(display &d) {
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
}

void playAgainstTactic(display &d) {

}




int main() {
    display d;
    d.ChangeModeText("No Mode Selected");
    d.ChangePromptText("Engine Waiting...");
    d.ChangeResponseMove("Placeholder");
    d.DisplaySidebar();
    std::vector<game*> tact = loadSavedTactics();

    tact[0]->b->ToString();
    getLine();

    return 0;
}
