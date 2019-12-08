#ifndef GAME_H
#define GAME_H

#include "stdlib.h"
#include "board.h"
#include "stdio.h"
#include "simpio.h"
#include "display.h"

#include <string>
#include <iostream>
#include <vector>

const std::string TACTIC_FILE_EXT = "processed/";
const std::vector<std::string> TACTIC_FILES = {"lichess.epgn"};
const int DONT_EVALUATE_FIRST_N = 11; // Don't evaluate the first 10 moves for tactics because
                                      // they aren't that interesting.

const double RAPID_TRANSITION_EVAL = 2.5; // in Pawn Values
                                       // essentially, if a move is bad enough that your position is 2 pawns worse as a result of something

class game
{
    public:
        game();
        game(std::string fen, bool whiteToPlay);
        void evaluateGame();
        void csvEvaluation();

        static std::vector<game*> LoadFile(std::ifstream &in);
        static void OpenPossibleTactics(std::vector<std::ifstream*> &streams);
        board *b;
        int ScanForTactic();
        void gotoMove(int move, display &d);

    private:
        std::vector<std::string> moveOrder;
        std::vector<double> evaluation;
        void Advance(std::string move);
        void Advance(board::move m);
};

#endif // GAME_H
