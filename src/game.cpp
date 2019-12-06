#include "game.h"
#include <string>
#include <iostream>

game::game()
{
    b = new board;
}

game::game(std::string fen) {
    b = new board(fen);
}

/**
 * File Format:
 * Tactic
 * Game FEN
 * Moves in algebraic-algebraic notation
 */
std::vector<game*> game::LoadFile(std::istream &in) {
    std::string line;
    bool startNewTactic = false;

    std::vector<game*> ret;

    game *g;
    while (getline(in, line)) {
        if (line == "Tactic") {
            startNewTactic = true;
            if (g != NULL) ret.push_back(g);
        }
        else if (startNewTactic) {
            startNewTactic = false;
            g = new game(line); // Passes in the FEN
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
