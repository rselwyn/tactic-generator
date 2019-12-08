#include "game.h"
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include "evalconst.h"
#include "dirent.h"
#include "engine.h"

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
            engine::moveoption op = engine::bestmove(b);
            evaluation.push_back(op.evaluation);
        }
        std::cout << "Evaluated move " << moveNumber  << std::endl;
    }
}

void game::gotoMove(int move, display& d) {
    std::cout << "GOTO " << move << std::endl;
    for (int i = b->movesMade(); i > 0; i--) {
        b->UndoLast();
    }
    std::cout << "End" << std::endl;
    for (int i = 0; i < move; i++) {
        b->MakeMove(this->moveOrder[i]);
    }
}

void game::csvEvaluation() {
    for (double d : evaluation) {
        std::cout << d << ", " << std::endl;
    }
}

int game::ScanForTactic() {
    if (evaluation.size() == 0) return -1; // If there is no data, we can't find any tactics.
    std::vector<double> first_pass = evaluation;

    for (int i = 2; i < first_pass.size(); i++) {
        first_pass[i] = (3*first_pass[i] + 2 * first_pass[i-1] + first_pass[i-2]) / 6.0;

        // Normalize all evaluations by the standard value of a pawn (in chess,
        // positions are typically given in terms of who is better and by how many pawns)
        first_pass[i] /= PAWN_VALUE;

        // The pawn values seem a little off in my evaluation, but they are relatively speaking correct
        first_pass[i] /= 1.05;
    }

    for (int i = 1; i < first_pass.size(); i++) {
        if (std::abs(first_pass[i] - first_pass[i-1]) > RAPID_TRANSITION_EVAL) {
            return i;
        }
    }

    return 0;
}
