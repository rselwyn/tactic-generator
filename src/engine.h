#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

class engine {
    public:

        struct moveoption {
            double evaluation;
            board::move m;
        };

        static double evalb(board* b);

        static moveoption bestmove(board* b);
        // Minimax with alpha-beta pruning
        static double minimax(board* b, int depth, bool isMax, double alpha, double beta);

    private:

};

#endif // ENGINE_H
