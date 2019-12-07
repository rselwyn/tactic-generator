import os
# TODO: Write instructions on how to install the chess module
import chess
import chess.pgn

# Program turns PGN files into files that can easily be read by the c++ software
raw = os.listdir("raw/")
for file_ext in raw:
    f = open("raw/" + file_ext, "r")
    games = []
    while True:
        pgn = chess.pgn.read_game(f)
        if pgn == None:
            break
        turn = "w" if pgn.board().turn == chess.WHITE else "b"
        games.append([turn + str(pgn.board().board_fen())] + list(pgn.main_line()))

    txt = [[str(i) for i in board] for board in games]
    fout = open("processed/"+file_ext.replace("pgn","epgn"), "w")

    for g in txt:
        fout.write("TACTIC\n")
        for i in g:
            fout.write(i+"\n")
    fout.close()
