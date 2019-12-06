#ifndef EVALCONST_H
#define EVALCONST_H

// NOTE: Because my engine is majority a tactical engine, I don't need to have as much
// positional evaluation code.  See https://github.com/official-stockfish/Stockfish/blob/master/src/evaluate.cpp
// for more evaluation code.

// Constants Source: https://github.com/official-stockfish/Stockfish/blob/master/src/types.h#L182
const double PAWN_VALUE = 260;
const double KNIGHT_VALUE = 781;
const double BISHOP_VALUE = 825;
const double QUEEN_VALUE = 2538;
const double ROOK_VALUE = 1276;
const double KING_VALUE = 100000.0; // Effectively Infinity

const int NOMINAL_MAX_DEPTH = 6;

// Constants Sourced from the #1 chess engine in the world
// which can be found at https://github.com/official-stockfish/Stockfish/blob/master/src/psqt.cpp#L41
// These arrays are symmetric around the vertical line in the middle of the board
const double KNIGHT_AT_POSITION_VALUES[8][4] = {{-175,-92,-74,-73},
                                                {-77,-41,-27,-15},
                                                {-61,-17,6,12},
                                                {-35,8,40,49},
                                                {-34,13,44,51},
                                                {-9,22,58,53},
                                                {-67,-27,4,37},
                                                {-201,-83,-56,-26}};

const double BISHOP_AT_POSITION_VALUES[8][4] = {{-53,-5,-8,-23},
                                                {-15,8,19,4},
                                                {-7,21,-5,17},
                                                {-5,11,25,39},
                                                {-12,29,22,31},
                                                {-16,6,1,11},
                                                {-17,-14,5,0},
                                                {-48,1,-14,-23}};

const double ROOK_AT_POSITION_VALUES[8][4] = {{-31,-20,-14,-5},
                                              {-21,-13,-8,6},
                                              {-25,-11,-1,3},
                                              {-13,-5,-4,-6},
                                              {-27,-15,-4,3},
                                              {-22,-2,6,12},
                                              {-2,12,16,18},
                                              {-17,-19,-1,9}};

const double QUEEN_AT_POSITION_VALUES[8][4] = {{3,-5,-5,4},
                                               {-3,5,8,12},
                                               {-3,6,13,7},
                                               {4,5,9,8},
                                               {0,14,12,5},
                                               {-4,10,6,8},
                                               {-5,6,10,8},
                                               {-2,-2,1,-2}};

const double KING_AT_POSITION_VALUES[8][4] = {{271,327,270,192},
                                              {278,303,230,174},
                                              {195,258,169,120},
                                              {164,190,138,98},
                                              {154,179,105,70},
                                              {123,145,81,31},
                                              {88,120,65,33},
                                              {59,89,45,-1}};

const double PAWN_AT_POSITION_VALUES[8][4] = {{0,0,0,0},
                                              {13,26,26, -52},
                                              {13,-13,-26,0},
                                              {0,0,0,52},
                                              {13,13,26,65},
                                              {26,26,52,78},
                                              {130,130,130,130},
                                              {0,0,0,0}};

#endif // EVALCONST_H
