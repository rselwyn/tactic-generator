Generating Chess Tactics and Quantifying Perceived Difficulty
------------------------------------------------------------

My project is broken down into three primary components: the chess engine, the tactic finder, and the difficulty evaluator.  One important thing to note: when running my program in QT Creator.  PLEASE BUILD THE PROJECT FOR RELEASE TO ENSURE IT RUNS AS FAST AS POSSIBLE.


1.  To be able to do pretty much anything with chess, there must be an engine to evaluate any position.  From scratch, I wrote not only the chess engine, but also the model class for the chess board.  The chess board class supports nearly all of the rules of chess (it does not support en-passant unfortunately), it can calculate all of the possible moves at its current position, and it is capable of doing and undoing moves.  

Developing the engine was particularly challenging, especially given my concerns for its speed.  The engine implements the minimax algorithm, and it uses alpha-beta pruning to try to decrease the size of the search tree.  On top of that, I try to do a little bit to reorder the search tree so that the potentially better moves are considered first so that more subtrees can be removed (via a beta cutoff).  On top of the alpha-beta pruning, I also spent a lot of time optimizing the engine using C++ multithreading from the <pthread.h> header file.  Combining this with alpha-beta pruning was also difficult (because alpha-beta pruning does not typically interface nicely with multithreading).  That being said, I devised what I think is a pretty neat solution:

At any given point in time, there are only so many threads that can be run on your computer.  Additionally, alpha-beta pruning is only good once the alpha and beta values start to get tighter.  So, instead of trying to process each possible move in a separate thread, I process the first few candidate moves, wait for them to finish, update the global alpha-beta values (each of the recently finished threads had their own alpha-beta values, but now I update them globally), and then process the next candidate moves, this time with the updated alpha-beta values.  

The last thing that I implemented for the engine was a transposition table, which is effectively like memoization for a chess board.  Unlike a lot of the memoization that we did in class, using transposition tables is a little bit difficult because you have to hash the chess board somehow.  To hash the chess board, I implemented the Zobrist algorithm.  While I have left the code for the TT in the engine, it is not utilized because I was not able to figure out the mutex locks for the unordered_map to run in a multithreaded manner.

A word about performance: In evalconstants.h, you can modify the depth of the engine and the number of evaluation threads.  On my 2-core Mac, the most comfortable settings are probably to run at 4 evaluation threads and a game analysis depth of 6.  The game analysis can be pushed to a depth of 7, but move generation will take a little longer.  On a faster computer, you can push up the evaluation threads or the depth, but I wouldn't move the depth past 7 because the eval tree gets too big.

2. The second thing that I implemented was a tactic scanner and finder.  To start, chess games are typically serialized into what are called pgn files, which have all the moves, information about where the game was played, and more.  Parsing these files in C++ would be very difficult, but luckily, the Python chess module provides a pretty easy way to parse the pgn.  After parsing the pgn in python, I serialize the data in a much more simple format (see the epgn files in the resource folder).  The parsing code is located in tacticgen.py (which you should be able to run if you run the command "pip install python-chess").  

Once the files are parsed, they can be moved into the res/processed folder.  These files are then read in by game.h, processed, and presented to the user.   

3. 
