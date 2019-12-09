#include "engine.h"
#include "evalconst.h"
#include "board.h"
#include <iostream>
#include <vector>
#include <chrono>
#include "hashmap.h"
#include <exception>
#include "stdio.h"
#include "simpio.h"

// Multithreaded action
#include <pthread.h>


#include "stdio.h"

// Citations: In constructing this class, I referenced a few sources on the implementation of a Chess AI and on the Minimax Algorithm (with A/B pruning)
// https://medium.freecodecamp.com/simple-chess-ai-step-by-step-1d55a9266977
// https://en.wikipedia.org/wiki/Alpha–beta_pruning#Core_idea
// https://en.wikipedia.org/wiki/Minimax

double engine::kConsidered = 0;

// Positive evaluation is an advantage for white
double engine::evalb(board *b) {

    double white = 0;
    double black = 0;

    for (int let = 0; let < 8; let++) {
        for (int num = 0; num < 8; num++) {
            board::piece p = b->get(let, num);
            if (p.iswhite) {
                if (p.piece == board::PAWN) white += PAWN_VALUE + PAWN_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::KNIGHT) white += KNIGHT_VALUE + KNIGHT_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::BISHOP) white += BISHOP_VALUE + BISHOP_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::ROOK) white += ROOK_VALUE + ROOK_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::QUEEN) white += QUEEN_VALUE + QUEEN_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::KING) white += KING_VALUE + KING_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
            }
            else {
                // The constant arrays are white-oriented, so we must convert the black coordinates so that we get the right value.
                if (p.piece == board::PAWN) black += PAWN_VALUE + PAWN_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::KNIGHT) black += KNIGHT_VALUE + KNIGHT_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::BISHOP) black += BISHOP_VALUE + BISHOP_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::ROOK) black += ROOK_VALUE + ROOK_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::QUEEN) black += QUEEN_VALUE + QUEEN_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::KING) black += KING_VALUE + KING_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
            }
        }
    }


    return white - black;
}


engine::moveoption engine::bestmove(board* b) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<board::move> moves = b->PossibleMoves();
    bool whiteMove = b->sideToMove;
    double topValue = whiteMove ? 10000000 : -10000000;
    double secondBest = whiteMove ? 10000000 : -10000000;

    double alpha = -1000000;
    double beta = 1000000;

    board::move topMove;

    pthread_t eval_threads[moves.size()];
    double extreme_value[moves.size()]; // max or min
    board::minimax_args* copies[moves.size()];
    std::unordered_map<long, std::pair<double, int>> *table = new std::unordered_map<long, std::pair<double, int>>;
    // evaluate first n, get results back, and feed those as alphas for the new things
    for (int range = 0; range < moves.size(); range+=EVAL_THREADS) {
        for (int i = 0; i < EVAL_THREADS; i++) {
            int index = (range + i);
            if (index >= moves.size()) break;

            board *copy = new board;
            *copy = *b;
            copy->MakeMove(moves[index]);
            board::minimax_args *args = new board::minimax_args;
            args->b = copy;
            args->depth = NOMINAL_MAX_DEPTH - 1;
            args->isMax = b->sideToMove;
            args->alpha = alpha;
            args->beta = beta;
            args->writeVal = &extreme_value[index];
            args->tt = table;
            copies[index] = args;
            pthread_create(&eval_threads[index], NULL, dispatch_minimax, (void*) args);
        }
        for (int i = 0; i < EVAL_THREADS; i++) {
            int index = (range + i);
            if (index >= moves.size()) break;
            pthread_join(eval_threads[index], NULL);
            if (!b->sideToMove) {
                alpha = std::max(alpha, extreme_value[index]);

            }
            else {
                beta = std::min(beta, extreme_value[index]);
            }
        }
    }


    for (int i = 0; i < moves.size(); i++) {
        pthread_join(eval_threads[i], NULL);   
    }

    std::cout << std::endl;
    for (int i = 0; i < moves.size(); i++) {
        double eval = extreme_value[i];
        if ((eval < topValue && whiteMove)
                || (eval > topValue && !whiteMove)) {
            secondBest = topValue;
            topValue = eval;
            topMove = moves[i];
        }
        pthread_cancel(eval_threads[i]);
        delete copies[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: "
             << duration.count() << " microseconds" << std::endl;

    return {topValue, topMove, secondBest};
}

void* engine::dispatch_minimax(void *_i) {
    struct board::minimax_args *args = (struct board::minimax_args*) _i;
    double value = minimax(args->b, args->depth, args->isMax, args->alpha, args->beta, args->tt);
    *(args->writeVal) = value;

    return NULL;
}

/**
 * This is the primary minimax function that the program uses.  It implements the basic minimax algorithm, in
 * addition to alpha-beta subtree pruning.  I have also built up almost the entire framework for transposition tables,
 * but was unable to utilize my implementation because I was not able to get the multithreaded access to the map to work
 * (there are issues with thread safety which require mutex locks).  The transposition table, which is effectively memoization (that we did in class),
 * is backed by the Zobrist hashing algrothim, which is implemented in board.cpp (to efficiently hash a board state).
 *
 * On top of this, I have built a heuristic into the movgen code (in board.cpp as well) which in theory should improve
 * the number of search branches that can be removed with the alpha-beta pruning.
 */
double engine::minimax(board* b, int depth, bool isMax, double alpha, double beta, std::unordered_map<long, std::pair<double, int>> *transpose) {

    engine::kConsidered += 1.0;
    if (depth == 0) {
        return evalb(b);
    }

    if (isMax) {
        double bestValue = -10000000;
        std::vector<board::move> moves = b->PossibleMoves();
        for (board::move option : moves) {
            b->MakeMove(option);
//            std::cout << key << std::endl;
            double mm = minimax(b, depth-1, !isMax, alpha, beta, transpose);

            bestValue = std::max(bestValue, mm);
            alpha = std::max(alpha, bestValue);
            b->UndoLast();

            if (alpha > beta) {
                break;
            }
        }
        return bestValue;
    }
    else {
        double bestValue = 10000000;
        std::vector<board::move> moves = b->PossibleMoves();
        for (board::move option : moves) {
            b->MakeMove(option);
            double mm = minimax(b, depth-1, !isMax, alpha, beta, transpose);

            bestValue = std::min(bestValue, mm);
            beta = std::min(beta, bestValue);
            b->UndoLast();
            if (beta < alpha) {
                break;
            }
        }
        return bestValue;
    }
}
