#include <iostream>

#include "board.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "random.h"
#include <vector>
#include <deque>

using namespace std;

long board::ZOBRIST_TABLE[8][8][12]; // Contains bitstrings to hash with
long board::ZOBRIST_WHITE_MOVE;
long board::ZOBRIST_BLACK_MOVE;

board::board()
{
    // White Pieces
    for (int i = 0; i < 8; i++) _board[i][1] = {true, PAWN};
    _board[0][0] = {true, ROOK};
    _board[1][0] = {true, KNIGHT};
    _board[2][0] = {true, BISHOP};
    _board[3][0] = {true, QUEEN};
    _board[4][0] = {true, KING};
    _board[5][0] = {true, BISHOP};
    _board[6][0] = {true, KNIGHT};
    _board[7][0] = {true, ROOK};

    // Black Pieces
    for (int i = 0; i < 8; i++) _board[i][6] = {false, PAWN};
    _board[0][7] = {false, ROOK};
    _board[1][7] = {false, KNIGHT};
    _board[2][7] = {false, BISHOP};
    _board[3][7] = {false, QUEEN};
    _board[4][7] = {false, KING};
    _board[5][7] = {false, BISHOP};
    _board[6][7] = {false, KNIGHT};
    _board[7][7] = {false, ROOK};

    for (int j = 2; j < 6; j++) for (int i = 0; i < 8; i++) _board[i][j] = {true, EMPTY};
}

board::board(string fen, bool isWhiteToPlay) {
    sideToMove = !isWhiteToPlay;

    int currentLetter = -1;
    int currentRow = 7;

    vector<char> validPieces = {'N','n','P','p','R','r','K','k','Q','q','B','b'};
    vector<piece> toPieces = {{true, KNIGHT}, {false, KNIGHT}, {true, PAWN}, {false, PAWN}, {true, ROOK}, {false, ROOK},
                             {true, KING}, {false, KING}, {true, QUEEN}, {false, QUEEN}, {true, BISHOP}, {false, BISHOP}};

    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) _board[i][j] = {true, EMPTY};

    for (int i = 0; i < fen.length(); i++) {
        if (fen[i] == '/') {
            currentRow--;
            currentLetter = -1;
        }
        if (std::find(validPieces.begin(), validPieces.end(), fen[i]) != validPieces.end()) {
            // It is a valid piece
            currentLetter++;
//            cout << ((char) ('a'+currentLetter)) << (currentRow+1) << std::endl;
            _board[currentLetter][currentRow] = toPieces.at(std::find(validPieces.begin(), validPieces.end(), fen[i]) - validPieces.begin());
        }
        else {
            if (isdigit(fen[i])) {
                currentLetter+= (fen[i] - '0');
            }
        }
    }
}

board::~board() {

}

board::piece board::MakeMove(move m) {
    // This method is assumed to be called with a valid move...
    piece copy = _board[m.d1][m.d2];
    _board[m.d1][m.d2] = _board[m.s1][m.s2];
    _board[m.s1][m.s2].piece = EMPTY;
    sideToMove = !sideToMove;
    if (m.isCastle) {
        if (m.d1 > m.s1) {
            // Castle Kingside
            _board[5][m.d2] = _board[7][m.d2];
            _board[7][m.d2].piece = EMPTY;
        }
        else {
            // Castle Queenside
            _board[3][m.d2] = _board[0][m.d2];
            _board[0][m.d2].piece = EMPTY;
        }
    }
    performedMovesStack.push(std::make_pair(m, copy));
    return copy;
}

board::piece board::MakeMove(std::string m) {
    // This method is assumed to be called with a valid move...
    piece copy = _board[m[0]-'a'][m[1]-'1'];
    move mo = {m[0]-'a', m[1]-'1', m[2]-'a', m[3]-'1', copy, (copy.piece == KING && m[2]-m[0] == 2)};
    return MakeMove(mo);
}

void board::UndoLast() {
    if (performedMovesStack.empty()) {
        cerr << "ERROR: Attempting to undo a non-existant move." << endl;
        return;
    }

    std::pair<move, piece> lastMove = performedMovesStack.top();
    performedMovesStack.pop();
    sideToMove = !sideToMove;
    _board[lastMove.first.s1][lastMove.first.s2] = lastMove.first.p;
    _board[lastMove.first.d1][lastMove.first.d2] = lastMove.second;

    if (lastMove.first.isCastle) {
        if (lastMove.first.d1 > lastMove.first.s1) {
            // Castle Kingside
            _board[7][lastMove.first.d2] = _board[5][lastMove.first.d2];
            _board[5][lastMove.first.d2].piece = EMPTY;
        }
        else {
            // Castle Queenside
            _board[0][lastMove.first.d2] = _board[3][lastMove.first.d2];
            _board[3][lastMove.first.d2].piece = EMPTY;
        }
    }
}


bool board::inBounds(int let, int num) {
    return 0 <= let && let < 8 && num >= 0 && num < 8;
}

std::vector<board::move> board::PossibleMoves() {
    return PossibleMovesWithColor(true);
}

// Include king moves parameter makes it so that we can get all of the moves with the king moves
// not included.
std::vector<board::move> board::PossibleMovesWithColor(bool includeKingMoves) {
    deque<board::move> moves;
    vector<board::move> final;
    for (int num = 0; num < 8; num++) {
        for (int let = 0; let < 8; let++) {
            piece p = _board[let][num];
            if (p.iswhite != !sideToMove) continue;
            set<board::move> piecemoves;
            if (p.piece == PAWN) piecemoves = pawnMove(let, num, p.iswhite);
            if (p.piece == KNIGHT) piecemoves = knightMove(let, num, p.iswhite);
            if (p.piece == BISHOP) piecemoves = bishopMove(let, num, p.iswhite);
            if (p.piece == KING && includeKingMoves) piecemoves = kingMove(let, num, p.iswhite, p.iswhite ? canWhiteCastle : canBlackCastle);
            if (p.piece == QUEEN) piecemoves = queenMove(let, num, p.iswhite);
            if (p.piece == ROOK) piecemoves = rookMove(let, num, p.iswhite);

            // Optimization Notes:
            // By the nature of alpha-beta search, it is advantageous to encounter moves that are stronger faster.  This reordering
            // sketchily hangs on the assumption that random pawn moves tend to be worse than full piece moves
            if (p.piece == PAWN) {
                for (move m : piecemoves) moves.push_back(m);
            }
            else {
                for (move m : piecemoves) moves.push_front(m);
            }
        }
    }

    final = {moves.begin(), moves.end()};

    return final;
}

std::set<board::move> board::pawnMove(int let, int num, bool isWhite) {
    int wd = isWhite ? 1 : -1; // pieces move forward when white and "backward" when black
    set<board::move> ret;
    if (inBounds(let, num + wd) && _board[let][num+wd].piece == EMPTY) ret.insert({let,num,let,num+wd,{isWhite, PAWN}, false});

    // pawn moving up by 2 if it is on the second rank (which is 1 zero indexed)
    if (((num == 1 && isWhite) || (num == 6 && !isWhite)) &&
            _board[let][num+ 2*wd].piece == EMPTY && _board[let][num+1*wd].piece == EMPTY)
        ret.insert({let,num,let,num+2*wd,{isWhite, PAWN}, false});

    // capture piece to the right
    if (inBounds(let+1, num+wd) && _board[let+1][num+wd].piece != EMPTY && _board[let+1][num+wd].iswhite != isWhite)
        ret.insert({let,num,let+1,num+wd,{isWhite, PAWN}, false});

    // capture piece to the left
    if (inBounds(let-1, num+wd) && _board[let-1][num+wd].piece != EMPTY && _board[let-1][num+wd].iswhite != isWhite)
        ret.insert({let,num,let-1,num+wd,{isWhite, PAWN}, false});

    return ret;
}

std::set<board::move> board::knightMove(int let, int num, bool isWhite) {
    set<board::move> ret;

    std::vector<std::pair<int, int>> diffs = {{1,2}, {1, -2}, {-1, 2}, {-1, -2}, {2,1}, {2,-1}, {-2, 1}, {-2, -1}};
    for (std::pair<int, int> diff : diffs) {
        if (inBounds(let + diff.first, num + diff.second) &&
                (_board[let+diff.first][num+diff.second].piece == EMPTY || (_board[let+diff.first][num+diff.second].piece != EMPTY && isWhite != _board[let+diff.first][num+diff.second].iswhite))) {
            ret.insert({let,num,let+diff.first,num+diff.second,{isWhite, KNIGHT}, false});
        }
    }

    return ret;
}

std::set<board::move> board::bishopMove(int let, int num, bool isWhite) {
    // Very similar to the code for the rook moves, except that we are now going up and side by 1 instead of just in one direction
    set<board::move> ret;
    // NOTE:
    // Diagonal that looks like / and in the downward direction

    for (int dcol = 1; dcol < 8; dcol++) {
        // Horizontal Moves Left
        if (inBounds(let-dcol, num-dcol) && (_board[let-dcol][num-dcol].piece == EMPTY || (_board[let-dcol][num-dcol].piece != EMPTY && _board[let-dcol][num-dcol].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let-dcol, num-dcol, {isWhite, BISHOP}, false});
            if (_board[let-dcol][num-dcol].iswhite != isWhite && _board[let-dcol][num-dcol].piece != EMPTY) {
                // If it is an opposing piece, treat it as an obstruction and stop iterating
                break;
            }
        }
        else {
            break; // if we hit an obstruction, stop trying to gen moves
        }
    }

    // Diagonal that looks like / and in the upward direction
    for (int dcol = 1; dcol < 8; dcol++) {
        if (inBounds(let+dcol, num+dcol) && (_board[let+dcol][num+dcol].piece == EMPTY || (_board[let+dcol][num+dcol].piece != EMPTY && _board[let+dcol][num+dcol].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let+dcol, num+dcol, {isWhite, BISHOP}, false});
            if (_board[let+dcol][num+dcol].piece != EMPTY && _board[let+dcol][num+dcol].iswhite != isWhite) {
                // If it is an opposing piece, treat it as an obstruction and stop iterating
                break;
            }
        }
        else {
            break; // if we hit an obstruction, stop trying to gen moves
        }
    }

    // NOTE:
    // Diagonal that looks like \ in the upward direction

    for (int drow = 1; drow < 8; drow++) {
        if (inBounds(let-drow, num+drow) && (_board[let-drow][num+drow].piece == EMPTY || (_board[let-drow][num+drow].piece != EMPTY && _board[let-drow][num+drow].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let-drow, num+drow, {isWhite, BISHOP}, false});
            if (_board[let-drow][num+drow].piece != EMPTY && _board[let-drow][num+drow].iswhite != isWhite) {
                // If it is an opposing piece, treat it as an obstruction and stop iterating
                break;
            }
        }
        else {
            break; // if we hit an obstruction, stop trying to gen moves
        }
    }

    // downward direction on the line \

    for (int drow = 1; drow < 8; drow++) {
        if (inBounds(let+drow, num-drow) && (_board[let+drow][num-drow].piece == EMPTY || (_board[let+drow][num-drow].piece != EMPTY && _board[let+drow][num-drow].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let+drow, num-drow, {isWhite, BISHOP}, false});

            if (_board[let+drow][num-drow].piece != EMPTY && _board[let+drow][num-drow].iswhite != isWhite) {
                // If it is an opposing piece, treat it as an obstruction and stop iterating
                break;
            }
        }
        else {
            break; // if we hit an obstruction, stop trying to gen moves
        }
    }

    return ret;
}

// Note: This method generates raw moves and does not check for the move to be in bounds
std::set<board::move> board::kingMove(int let, int num, bool isWhite, bool canCastle) {
    set<board::move> ret;
    for (int plusl = -1; plusl <= 1; plusl++) {
        for (int plusn = -1; plusn <= 1; plusn++) {
            if (inBounds(let+plusl, num + plusn) && ((_board[let+plusl][num+plusn].piece == EMPTY) || (_board[let+plusl][num+plusn].iswhite != isWhite)))
                ret.insert({let, num, let+plusl, num+plusn, {isWhite, KING}, false});
        }
    }

    if (canCastle && let == 4 && (num == 0 || num == 7) && _board[let+1][num].piece == EMPTY &&
            _board[let+2][num].piece == EMPTY && _board[let+3][num].piece == ROOK && _board[let+3][num].iswhite == isWhite) {
        // Then, we can castle kingside
        ret.insert({let, num, let+2, num, {isWhite, KING}, true});
    }

    if (canCastle && let == 4 && (num == 0 || num == 7) && _board[let-1][num].piece == EMPTY &&
            _board[let-2][num].piece == EMPTY && num == 0 && _board[let-3][num].piece == EMPTY
            && _board[let-4][num].piece == ROOK && _board[let-4][num].iswhite == isWhite && canCastle) {
        // Then, we can castle queenside
        ret.insert({let, num, let-2, num, {isWhite, KING}, true});
    }



    return ret;
}

std::set<board::move> board::queenMove(int let, int num, bool isWhite) {
    set<board::move> ret;
    // We can "cheat" and compose all of the queen moves as a combination of the rook moves and bishop moves
    set<board::move> sideSide = rookMove(let, num, isWhite);
    set<board::move> diag = bishopMove(let, num, isWhite);
    for (board::move m : sideSide) {
        // We need to modify the piece to make it a queen now
        m.p.piece = QUEEN;
        ret.insert(m);
    }

    for (board::move m : diag) {
        m.p.piece = QUEEN;
        ret.insert(m);
    }

    return ret;
}

std::set<board::move> board::rookMove(int let, int num, bool isWhite) {
    set<board::move> ret;
    for (int dcol = 1; dcol < 8; dcol++) {
        // Horizontal Moves Left
        if (inBounds(let-dcol, num) && (_board[let-dcol][num].piece == EMPTY || (_board[let-dcol][num].piece != EMPTY && _board[let-dcol][num].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let-dcol, num, {isWhite, ROOK}, false});

            if (_board[let-dcol][num].piece != EMPTY
                    && _board[let-dcol][num].iswhite != isWhite) {
                // If it is an opposing piece, treat it as an obstruction and stop iterating
                break;
            }
        }
        else {
            break; // if we hit an obstruction, stop trying to gen moves
        }
    }

    for (int dcol = 1; dcol < 8; dcol++) {
        // Horizontal Moves Right
        if (inBounds(let+dcol, num) && (_board[let+dcol][num].piece == EMPTY || (_board[let+dcol][num].piece != EMPTY && _board[let+dcol][num].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let+dcol, num, {isWhite, ROOK}, false});

            if (_board[let+dcol][num].piece != EMPTY &&
                    _board[let+dcol][num].iswhite != isWhite) {
                // If it is an opposing piece, treat it as an obstruction and stop iterating
                break;
            }
        }
        else {
            break; // if we hit an obstruction, stop trying to gen moves
        }
    }

    for (int drow = 1; drow < 8; drow++) {
        // Row Moves
        if (inBounds(let, num-drow) && (_board[let][num-drow].piece == EMPTY || (_board[let][num-drow].piece != EMPTY && _board[let][num-drow].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let, num-drow, {isWhite, ROOK}, false});

            if (_board[let][num-drow].piece != EMPTY &&
                    _board[let][num-drow].iswhite != isWhite) {
                // If it is an opposing piece, treat it as an obstruction and stop iterating
                break;
            }
        }
        else {
            break; // if we hit an obstruction, stop trying to gen moves
        }
    }

    for (int drow = 1; drow < 8; drow++) {
        // Row Moves
        if (inBounds(let, num+drow) && (_board[let][num+drow].piece == EMPTY || (_board[let][num+drow].piece != EMPTY && _board[let][num+drow].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let, num+drow, {isWhite, ROOK}, false});

            if (_board[let][num+drow].piece != EMPTY &&
                    _board[let][num+drow].iswhite != isWhite) {
                // If it is an opposing piece, treat it as an obstruction and stop iterating
                break;
            }
        }
        else {
            break; // if we hit an obstruction, stop trying to gen moves
        }
    }

    return ret;
}

board::piece board::get(int row, int col) {
    return _board[row][col];
}

bool board::confirmNoKingProblems(bool isWhite, board::move mo) {
    return true;
    // Checks if the king would be "hanging" after a move
    MakeMove(mo);

    int let = 0; int num = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j <8; j++) {
            if (_board[i][j].piece == KING && _board[i][j].iswhite == isWhite) {
                let = i;
                num = j;
            }
        }
    }

    std::vector<move> options = PossibleMovesWithColor(false);
    for (move m : options) {
        if (m.d1 == let && m.d2 == num) {
            return false; // a.k.a there is a problem
        }
    }

    UndoLast();
    return true;
}

void board::ToString() {
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            string val = typeToStr[_board[i][7-j].piece];
            if (_board[i][7-j].iswhite) cout << val[0];
            else {
                cout << ((char) toupper(val[0]));
            }
        }
        cout << endl;
    }
}

long board::generateRandomBitString() {
    // Source: https://stackoverflow.com/questions/21096015/how-to-generate-64-bit-random-numbers
    // Note: C++ does not make it incredibly easy to generate large random bitstrings, so this will have
    // to do.
    return ((long long) rand() << 32) | rand();
}

void board::InitializeHashTable() {
    map<board::piece, int> toPieces = {{{false, board::PAWN},1}, {{false, board::ROOK},2}, {{false, board::KNIGHT},3}, {{false, board::BISHOP},4}, {{false, board::QUEEN},5}, {{false, board::KING},6},
                                       {{false, board::KNIGHT},7}, {{false, board::KNIGHT},8}, {{false, board::KNIGHT},9}, {{false, board::KNIGHT},10}, {{false, board::KNIGHT},11}, {{false, board::KNIGHT},12}};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 12; k++) board::ZOBRIST_TABLE[i][j][k] = board::generateRandomBitString();
        }
    }

    board::ZOBRIST_WHITE_MOVE = board::generateRandomBitString();
    board::ZOBRIST_BLACK_MOVE = board::generateRandomBitString();
}

std::string board::ToHashString() {
    string s = "                                                                ";
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            string val = typeToStr[_board[i][7-j].piece];
            if (_board[i][7-j].iswhite) s[8*j + i] = val[0];
            else {
                s[8*j + i] = toupper(val[0]);
            }
        }
    }
    return s;
}

long board::ZobristKey() {
//     Uses the Zobrist Algorithm to Hash
//     https://en.wikipedia.org/wiki/Zobrist_hashing
        map<board::piece, int> toPieces = {{{false, board::PAWN},1}, {{false, board::ROOK},2}, {{false, board::KNIGHT},3}, {{false, board::BISHOP},4}, {{false, board::QUEEN},5}, {{false, board::KING},6},
                                           {{false, board::KNIGHT},7}, {{false, board::KNIGHT},8}, {{false, board::KNIGHT},9}, {{false, board::KNIGHT},10}, {{false, board::KNIGHT},11}, {{false, board::KNIGHT},12}};

        long h = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int p = toPieces[_board[i][j]];
                long pkey = board::ZOBRIST_TABLE[i][j][p];
                h = (h ^ pkey);
            }
        }

        if (!this->sideToMove) {
            h = (h ^ board::ZOBRIST_WHITE_MOVE);
        }
        else {
            h = (h ^ board::ZOBRIST_BLACK_MOVE);
        }

        return h;
}

int board::movesMade() {
    return this->performedMovesStack.size();
}
