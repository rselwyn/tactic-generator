#include <iostream>
#include "console.h"
#include "display.h"
#include "board.h"
#include "simpio.h"

using namespace std;

int main() {
    display d;
    board *b = new board;
    d.ShowBoard(b);
    while(true) {
        string s = getLine();
        cout << s << endl;
    }
    return 0;
}
