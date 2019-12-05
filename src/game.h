#ifndef GAME_H
#define GAME_H

#include "stdlib.h"
#include "board.h"
#include "stdio.h"

#include <string>
#include <iostream>
#include <vector>

class game
{
    public:
        game();
        game(std::string fromFileExt);
    private:
        std::vector<std::string> moveOrder;
        std::vector<double> evaluation;
        board *b;
        void LoadFile(std::ifstream &in);

};

#endif // GAME_H
