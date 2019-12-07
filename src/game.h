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
const std::vector<std::string> TACTIC_FILES = {"group.epgn", "lichess.epgn"};

class game
{
    public:
        game();
        game(std::string fen, bool whiteToPlay);
        void evaluateGame(engine e);

        static std::vector<game*> LoadFile(std::ifstream &in);
        static void OpenPossibleTactics(std::vector<std::ifstream*> &streams);


    private:
        std::vector<std::string> moveOrder;
        std::vector<double> evaluation;
        board *b;
        void Advance(std::string move);
        void Advance(board::move m);

};

#endif // GAME_H
