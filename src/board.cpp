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

board::~board() {

}

board::piece board::MakeMove(move m) {
    // This method is assumed to be called with a valid move...
    piece copy = _board[m.d1][m.d2];
    _board[m.d1][m.d2] = _board[m.s1][m.s2];
    _board[m.s1][m.s2].piece = EMPTY;
    sideToMove = !sideToMove;
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
//            if (p.piece == PAWN) piecemoves = pawnMove(let, num, p.iswhite);
//            if (p.piece == KNIGHT) piecemoves = knightMove(let, num, p.iswhite);
//            if (p.piece == BISHOP) piecemoves = bishopMove(let, num, p.iswhite);
            if (p.piece == KING) piecemoves = kingMove(let, num, p.iswhite, p.iswhite ? canWhiteCastle : canBlackCastle);
//            if (p.piece == QUEEN) piecemoves = queenMove(let, num, p.iswhite);
//            if (p.piece == ROOK) piecemoves = rookMove(let, num, p.iswhite);
            moves.insert(piecemoves.begin(), piecemoves.end());
        }
    }
    return moves;
}

std::set<board::move> board::pawnMove(int let, int num, bool isWhite) {
    int wd = isWhite ? 1 : -1; // pieces move forward when white and "backward" when black
    set<board::move> ret;
    if (inBounds(let, num + wd) && _board[let][num+wd].piece == EMPTY) ret.insert({let,num,let,num+wd,{isWhite, PAWN}, false});

    cout << "pawN" << endl;
    cout << ((num == 1 && isWhite) || (num == 6 && !isWhite));
    cout << (_board[let][num+ 2*wd].piece == EMPTY && _board[let][num+1*wd].piece == EMPTY) << endl;
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
            if (_board[let-dcol][num-dcol].iswhite != isWhite) {
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
        // Horizontal Moves Left
        if (inBounds(let+dcol, num+dcol) && (_board[let+dcol][num+dcol].piece == EMPTY || (_board[let+dcol][num+dcol].piece != EMPTY && _board[let+dcol][num+dcol].iswhite != isWhite))) {
            // Check if the piece is in bounds, and that the square is either empty or an opposing piece
            ret.insert({let, num, let+dcol, num+dcol, {isWhite, BISHOP}, false});
            if (_board[let+dcol][num+dcol].iswhite != isWhite) {
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
            if (_board[let-drow][num+drow].iswhite != isWhite) {
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

            if (_board[let+drow][num-drow].iswhite != isWhite) {
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

            if (_board[let-dcol][num].iswhite != isWhite) {
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

            if (_board[let+dcol][num].iswhite != isWhite) {
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

            if (_board[let][num-drow].iswhite != isWhite) {
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

            if (_board[let][num+drow].iswhite != isWhite) {
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
