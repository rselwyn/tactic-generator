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

class game
{
    static std::vector<game*> LoadFile(std::istream &in);

    public:
        game();
        game(std::string fen);
        void evaluateGame(engine e);


    private:
        std::vector<std::string> moveOrder;
        std::vector<double> evaluation;
        board *b;
        void Advance(std::string move);
        void Advance(board::move m);

};

#endif // GAME_H
