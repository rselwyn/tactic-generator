#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"
#include "hashmap.h"

class engine {
    public:
        static double kConsidered;

        struct moveoption {
            double evaluation;
            board::move m;
            double evalSecondBest;
        };

        static double evalb(board* b);
        static void* dispatch_minimax(void *_i);
        static moveoption bestmove(board* b, int MAX_DEPTH);
        // Minimax with alpha-beta pruning
        static double minimax(board* b, int depth, bool isMax, double alpha, double beta, std::unordered_map<long, std::pair<double, int>> *transpose);

    private:

};

#endif // ENGINE_H
