#include <iostream>

#include "board.h"
#include "stdio.h"
#include "string.h"
#include <vector>

using namespace std;

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

board::board(string fen) {

    int currentLetter = 0;
    int currentRow = 0;

    vector<char> validPieces = {'N','n','P','p','R','r','K','k','Q','q','B','b'};
    vector<piece> toPieces = {{true, KNIGHT}, {false, KNIGHT}, {true, PAWN}, {false, PAWN}, {true, ROOK}, {false, ROOK},
                             {true, KING}, {false, KING}, {true, QUEEN}, {false, QUEEN}, {true, BISHOP}, {false, BISHOP}};

//    for (int i = 0; i < fen.length(); i++) {
//        if (fen[i] == '/') {
//            currentRow++;
//            currentLetter = 0;
//        }
//        if (std::find(validPieces.begin(), validPieces.end(), fen[i]) != validPieces.end()) {
//            // It is a valid piece
//            _board[currentLetter][currentRow] = toPieces.at(std::find(validPieces.begin(), validPieces.end(), fen[i]) - validPieces.begin());
//        }
//        if ()
//    }
}

board::piece board::MakeMove(move m) {
    // This method is assumed to be called with a valid move...
    piece copy = _board[m.d1][m.d2];
    _board[m.d1][m.d2] = _board[m.s1][m.s2];
    _board[m.s1][m.s2].piece = EMPTY;
    return copy;
}

bool board::inBounds(int let, int num) {
    return 0 <= let && let < 8 && num >= 0 && num < 8;
}

std::set<board::move> board::PossibleMoves() {
    set<board::move> moves;
    for (int let = 0; let < 8; let++) {
        for (int num = 0; num < 8; num++) {
            piece p = _board[let][num];
            set<board::move> piecemoves;
            if (p.piece == PAWN) piecemoves = pawnMove(let, num, p.iswhite);
            if (p.piece == KNIGHT) piecemoves = knightMove(let, num, p.iswhite);
            if (p.piece == BISHOP) piecemoves = bishopMove(let, num, p.iswhite);
            if (p.piece == KING) piecemoves = kingMove(let, num, p.iswhite, p.iswhite ? canWhiteCastle : canBlackCastle);
            if (p.piece == QUEEN) piecemoves = queenMove(let, num, p.iswhite);
            if (p.piece == ROOK) piecemoves = rookMove(let, num, p.iswhite);
        }
    }
    return {};
}

std::set<board::move> board::pawnMove(int let, int num, bool isWhite) {
    int wd = isWhite ? 1 : -1; // pieces move forward when white and "backward" when black
    set<board::move> ret;
    if (inBounds(let, num + wd) && _board[let][num+wd].piece == EMPTY) ret.insert({let,num,let,num+wd,{isWhite, PAWN}, false});
    // pawn moving up by 2 if it is on the second rank (which is 1 zero indexed)
    if (((num == 1 && isWhite) || (num == 6 && !isWhite)) &&
            _board[let][num+2 * wd].piece == EMPTY && _board[let][num+2*wd].piece == EMPTY)
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
    if (inBounds(let + 1, num + 2) &&
            (_board[let+1][num+2].piece == EMPTY || (_board[let+1][num+2].piece != EMPTY && isWhite != _board[let+1][num+2].iswhite)))
        ret.insert({let,num,let+1,num+2,{isWhite, KNIGHT}, false});

    if (inBounds(let - 1, num + 2) &&
            (_board[let-1][num+2].piece == EMPTY || (_board[let-1][num+2].piece != EMPTY && isWhite != _board[let-1][num+2].iswhite)))
        ret.insert({let,num,let-1,num+2,{isWhite, KNIGHT}, false});

    if (inBounds(let + 1, num - 2) &&
            (_board[let+1][num-2].piece == EMPTY || (_board[let+1][num-2].piece != EMPTY && isWhite != _board[let+1][num-2].iswhite)))
        ret.insert({let,num,let+1,num-2,{isWhite, KNIGHT}, false});

    if (inBounds(let - 1, num - 2) &&
            (_board[let-1][num-2].piece == EMPTY || (_board[let-1][num-2].piece != EMPTY && isWhite != _board[let-1][num-2].iswhite)))
        ret.insert({let,num,let-1,num-2,{isWhite, KNIGHT}, false});

    // second set of 4

    if (inBounds(let + 2, num + 1) &&
            (_board[let+2][num+1].piece == EMPTY || (_board[let+2][num+1].piece != EMPTY && isWhite != _board[let+2][num+1].iswhite)))
        ret.insert({let,num,let+2,num+1,{isWhite, KNIGHT}, false});

    if (inBounds(let - 2, num + 1) &&
            (_board[let-2][num+1].piece == EMPTY || (_board[let-2][num+1].piece != EMPTY && isWhite != _board[let-2][num+1].iswhite)))
        ret.insert({let,num,let-2,num+1,{isWhite, KNIGHT}, false});

    if (inBounds(let + 2, num - 1) &&
            (_board[let+2][num-1].piece == EMPTY || (_board[let+2][num-1].piece != EMPTY && isWhite != _board[let+2][num-1].iswhite)))
        ret.insert({let,num,let+2,num-1,{isWhite, KNIGHT}, false});

    if (inBounds(let - 2, num - 1) &&
            (_board[let-2][num-1].piece == EMPTY || (_board[let-2][num-1].piece != EMPTY && isWhite != _board[let-2][num-1].iswhite)))
        ret.insert({let,num,let-2,num-1,{isWhite, KNIGHT}, false});


    return ret;
}

std::set<board::move> board::bishopMove(int let, int num, bool isWhite) {
    set<board::move> ret;
    return ret;
}

std::set<board::move> board::kingMove(int let, int num, bool isWhite, bool canCastle) {
    set<board::move> ret;
    for (int plusl = -1; plusl <= 1; plusl++) {
        for (int plusn = -1; plusn <= 1; plusn++) {
            if (inBounds(let+plusl, num + plusn) && ((_board[let+plusl][num+plusn].piece == EMPTY) || (_board[let+plusl][num+plusn].iswhite != isWhite)))
                ret.insert({let, num, let+plusl, num+plusn, {isWhite, KING}, false});
        }
    }

    // TODO: Add prevention from castling through check

    if (canCastle && isWhite && let == 4 && num == 0 && _board[let+1][num].piece == EMPTY &&
            _board[let+2][num].piece == EMPTY && _board[let+3][num].piece == ROOK && _board[let+1][num].iswhite) {
        // Then, we can castle kingside
        ret.insert({let, num, let+3, num, {isWhite, KING}, true});
    }

    if (canCastle && isWhite && let == 4 && num == 0 && _board[let-1][num].piece == EMPTY &&
            _board[let-2][num].piece == EMPTY && num == 0 && _board[let-3][num].piece == EMPTY
            && _board[let-4][num].piece == ROOK && _board[let-4][num].iswhite && canCastle) {
        // Then, we can castle queenside
        ret.insert({let, num, let-4, num, {isWhite, KING}, true});
    }

    return ret;
}

std::set<board::move> board::queenMove(int let, int num, bool isWhite) {
    set<board::move> ret;
    return ret;
}

std::set<board::move> board::rookMove(int let, int num, bool isWhite) {
    set<board::move> ret;
    return ret;
}

board::piece board::get(int row, int col) {
    return _board[row][col];
}

void board::ToString() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            string val = typeToStr[_board[i][j].piece];
            if (_board[i][j].iswhite) cout << val[0];
            else {
                cout << toupper(val[0]);
            }
        }
        cout << endl;
    }
}
