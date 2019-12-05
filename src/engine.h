#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

const int NOMINAL_MAX_DEPTH = 10;


class engine {
    public:

        struct moveoption {
            double evaluation;
            board::move m;
        };

        engine();
        double evalb(board* b);

        moveoption bestmove(board* b);
        // Minimax with alpha-beta pruning
        double minimax(board* b, int depth, bool isMax, double alpha, double beta);

    private:

};

#endif // ENGINE_H
