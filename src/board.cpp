#include "board.h"
#include "stdio.h"
#include "string.h"

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
    _board[0][6] = {false, ROOK};
    _board[1][6] = {false, KNIGHT};
    _board[2][6] = {false, BISHOP};
    _board[3][6] = {false, QUEEN};
    _board[4][6] = {false, KING};
    _board[5][6] = {false, BISHOP};
    _board[6][6] = {false, KNIGHT};
    _board[7][6] = {false, ROOK};

    for (int j = 1; j <= 6; j++) for (int i = 0; i < 8; i++) _board[i][j] = {true, EMPTY};
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

std::set<board::move> board::PossibleMoves() {

    for (int let = 0; let < 8; let++) {
        for (int num = 0; num < 8; num++) {

        }
    }
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
