#include "game.h"
#include <string>
#include <iostream>
#include <istream>
#include <fstream>

#include "dirent.h"

game::game()
{
    b = new board;
}

game::game(std::string fen, bool whiteToPlay) {
    b = new board(fen, whiteToPlay);
}

void game::OpenPossibleTactics(std::vector<std::ifstream*> &streams) {
    for (std::string s : TACTIC_FILES) {
        std::ifstream* stream = new std::ifstream;
        std::cout << TACTIC_FILE_EXT + s << std::endl;
        stream->open((TACTIC_FILE_EXT + s));
        if (!stream->good()) std::cout << "Error loading " << s << std::endl;
        streams.push_back(stream);
        std::cout << streams.size() << std::endl;
    }
}

/**
 * File Format:
 * Tactic
 * Game FEN
 * Moves in algebraic-algebraic notation
 */
std::vector<game*> game::LoadFile(std::ifstream &in) {
    std::string line;
    bool startNewTactic = false;
//    std::cout << "line" << std::endl;

    std::vector<game*> ret;

    game *g;
    while (getline(in, line)) {
//        std::cout << line << std::endl;
        if (line == "TACTIC") {
            startNewTactic = true;
            if (g != NULL) ret.push_back(g);
        }
        else if (startNewTactic) {
            startNewTactic = false;
            bool whiteToPlay = (line[0] == 'w');
            line = line.substr(1);
            g = new game(line, whiteToPlay); // Passes in the FEN
        }
        else {
            g->Advance(line);
        }
    }
    return ret;
}

void game::Advance(std::string move) {
    moveOrder.push_back(move);
}

void game::Advance(board::move m) {
    moveOrder.push_back(m.toString());
}

// Can only be called once
void game::evaluateGame(engine e) {
    for (std::string move : moveOrder) {
        b->MakeMove(move);
        evaluation.push_back(engine::bestmove(b).evaluation);
    }
}
