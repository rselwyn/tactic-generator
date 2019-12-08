#ifndef GAME_H
#define GAME_H

#include "stdlib.h"
#include "board.h"
#include "stdio.h"
#include "engine.h"
#include "simpio.h"

#include <string>
#include <iostream>
#include <vector>

const std::string TACTIC_FILE_EXT = "processed/";
const std::vector<std::string> TACTIC_FILES = {"lichess.epgn"};
const int DONT_EVALUATE_FIRST_N = 10; // Don't evaluate the first 10 moves for tactics because
                                      // they aren't that interesting.

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

    private:
        std::vector<std::string> moveOrder;
        std::vector<double> evaluation;
        void Advance(std::string move);
        void Advance(board::move m);
        int ScanForTactic();

};

#endif // GAME_H
