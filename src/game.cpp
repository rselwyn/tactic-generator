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
        std::cout << "Loaded file: " << TACTIC_FILE_EXT + s << std::endl;
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

    game *g = NULL;
    while (getline(in, line)) {
        if (line == "TACTIC") {
            startNewTactic = true;
            if (g != NULL) {
                ret.push_back(g);
            }
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
    if (g != NULL) {
        ret.push_back(g);
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
void game::evaluateGame() {
    int i = 1;
    for (std::string move : moveOrder) {
        int moveNumber = (++i)/2;
        b->MakeMove(move);
        if (moveNumber < DONT_EVALUATE_FIRST_N) {
            evaluation.push_back(0);
        }
        else {
            evaluation.push_back(engine::bestmove(b).evaluation);
        }
        std::cout << "Evaluated move " << moveNumber  << std::endl;
    }
}

void game::csvEvaluation() {
    for (double d : evaluation) {
        std::cout << d << ", " << std::endl;
    }
}

int game::ScanForTactic() {
    if (evaluation.size() == 0) return -1; // If there is no data, we can't find any tactics.
    return 0;
}
