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

bool confirmValidMove(std::vector<board::move> moves, string in) {
    for (board::move m : moves) {
        if (m.toString() == in) {
            return true;
        }
    }
    return false;
}

void humanVsEngine(display &d) {
    d.ChangeModeText("Human vs. Computer");
    board *b = new board;
    d.ShowBoard(b);

    cout << "How to Play: " << endl;
    cout << "- This is a standard chess game" << endl <<
            "- The format for entering moves is in algebraic notation" << endl <<
            "- Each piece is given a location with a letter (for column) and a letter for row (a number)." << endl <<
            "- For example, the left most pawn is at position a2, the right white knight is at f1, etc." << endl <<
            "- To make a move, type the start square and the destination square (like e2e4)." << endl <<
            "- To castle, simply type the move for the king and the program will move the rook for you!" << endl;
    cout << "Good luck! (type 'exit' to quit) " << endl;

    while(true) {
        std::vector<board::move> whitemoves = b->PossibleMoves();
        d.ChangePromptText("Your Turn");
        d.DisplaySidebar();
        while(true) {
            string s = getLine("Move: ");
            if (s == "exit") {
                delete b;
                return;
            }
            if (confirmValidMove(whitemoves, s)) {
                b->MakeMove(s);
                break;
            }
            else {
                cout << "Invalid move.  Please try again." << endl;
            }
        }
        d.ShowBoard(b);

        engine::kConsidered = 0;
        std::vector<board::move> moves = b->PossibleMoves();
        d.ChangePromptText("Engine thinking...");
        d.DisplaySidebar();
        engine::moveoption move = engine::bestmove(b);
        cout << "Nodes: " << engine::kConsidered << endl;
        cout << "Best Move: ";
        cout << move.m.toString() << " with value " << move.evaluation << endl;
        d.ChangeResponseMove("Engine played " + move.m.toString());
        b->MakeMove(move.m);
        d.ShowBoard(b);
        engine::kConsidered = 0;
    }
}

void playAgainstTactic(display &d) {
    cout << "How to Play: " << endl;
    cout << "- This is similar to a chess game, but your job is only to play a few moves." << endl <<
            "- *** THE MOVES THAT YOU PLAY SHOULD IMPROVE THE POSITION SUBSTANTIALLY ***" << endl <<
            "- The format for entering moves is in algebraic notation" << endl <<
            "- Each piece is given a location with a letter (for column) and a letter for row (a number)." << endl <<
            "- For example, the left most pawn is at position a2, the right white knight is at f1, etc." << endl <<
            "- To make a move, type the start square and the destination square (like e2e4)." << endl;
    cout << "Good luck! (type 'exit' to quit) " << endl;

    std::vector<game*> tact = loadSavedTactics();
    cout << "Loading random tactic" << endl;
    d.ChangeModeText("Tactic Solver");
    d.ChangePromptText("Analyzing...Check console");
    d.ChangeResponseMove(" for progress.");
    d.DisplaySidebar();

    game* attempt = tact[randomInteger(0,tact.size() - 1)];
    attempt->evaluateGame();
    attempt->csvEvaluation();
    int scan = attempt->ScanForTactic();
    attempt->gotoMove(scan, d);
    d.ShowBoard(attempt->b);

    d.ChangePromptText("Your move.");
    d.ChangeResponseMove("");
    d.DisplaySidebar();

    while (true) {
        std::vector<board::move> moves = attempt->b->PossibleMoves();
        cout << moves[0].toString() << endl;
        cout << "Computing Correct Move.  Please standby." << endl;
        engine::moveoption option = engine::bestmove(attempt->b);
        cout << "Right move eval 1 " << option.evaluation << " 2 " << option.evalSecondBest << endl;
        cout << "The Computer knows the best move.  What do you think it is?" << endl;
        string s;
        while(true) {
            s = getLine("Your move: ");
            if (s == "exit") {
                delete attempt;
                return;
            }
            if (!confirmValidMove(moves, s)) {
                 cout << "Malformed input.  Please try again." << endl;
                 continue;
            }
            if (option.m.toString() == s) {
                cout << "Correct!" << endl;
                attempt->b->MakeMove(s);
                break;
            }
            else {
                cout << "Oops.  That isn't the most precise move.  Try again." << endl;
            }
        }
        d.ShowBoard(attempt->b);
        option = engine::bestmove(attempt->b);
        attempt->b->MakeMove(option.m);
        d.ChangeResponseMove("Engine Played " + option.m.toString());
        d.DisplaySidebar();
        d.ShowBoard(attempt->b);

    }
}

int main() {
    display d;
    d.ChangeModeText("No Mode Selected");
    d.ChangePromptText("Engine Waiting...");
    d.ChangeResponseMove("Placeholder");
    d.DisplaySidebar();
    board::InitializeHashTable();

    while (true) {
        std::string option = getLine("Enter the number for your desired mode.\n  "
                                     "1: Play The Engine. \n2: Play a Tactic. \n3: Numerical Tactical Difficulty Evaluation");
        if (option == "1") {
            humanVsEngine(d);
        }
        else if (option == "2") {
            playAgainstTactic(d);
        }
        else if (option == "3") {

        }
        else {
            cout << "Could not set mode.  Are you sure you entered a correct character?" << endl;
        }
    }


    return 0;
}
