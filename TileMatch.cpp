#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "timer.h"
#include <iostream>

#include "PlayGame.h"
#include "Puzzle.h"
#include "Tile.h"

using namespace std;

void init();
bool allMatch(Grid<Tile> &tiles);
void populateGrid(Grid<Tile> &tiles, Vector<Tile> &tileVec);
Vector<Grid<Tile>> solvePuzzle(Puzzle &puzzle);
void findAllSolutions(Vector<Tile> &tileVec, Grid<Tile> &tiles, int row,
                      int col, Vector<Grid<Tile>> &solutions);
void solvePuzzle(Vector<Tile> &tileVec, int row, int col, Puzzle &puzzle,
                 Vector<Grid<Tile>> &solutions, bool timeIt);
void testTile();
bool loadPuzzle(Puzzle &puzzle);
void manualPlay(Puzzle &puzzle, bool &donePlayingManually);
void solveAndTimePuzzle(Puzzle &puzzle, Vector<Grid<Tile>> &solutions);
void displayAndSaveSolutions(Puzzle &puzzle, Vector<Grid<Tile>> &solutions);

#include <QCoreApplication>
#include <QDir>
int main() {
    // NOTE: this solution requires you to manually put the puzzles folder
    // into the TileMatchDemo.app/Contents/Resources folder.
    // If you want to deploy the app, you also need to run
    // ~/Qt/5.15.2/clang_64/bin/macdeployqt TileMatchDemo.app
    // if (runSimpleTests(NO_TESTS)) {
    /*
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }*/

    // cout << QDir::currentPath().toStdString() << endl;
    // cout << QCoreApplication::applicationDirPath().toStdString() << endl;
    setConsoleWindowTitle("Tile Match");
    setConsoleSize(600, 600);
    cout << "Welcome to the tile match game!" << endl;
    init();

    return 0;
}

/* function allMatch
 * Determines if the puzzle is solved or not
 *
 * @param tiles A Grid of Tile instances
 * @return true if the puzzle is solved, false if it is not
 */
bool allMatch(Grid<Tile> &tiles) {
    // only need to check 12 positions, as the rest are overlapping
    int rows = tiles.numRows();
    int cols = tiles.numCols();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // no need to check above or to the left

            // right
            if (tiles.inBounds(r, c + 1)) {
                if (!tiles[r][c].isMatched(tiles[r][c + 1], Tile::RIGHT)) {
                    return false;
                }
            }
            // below
            if (tiles.inBounds(r + 1, c)) {
                if (!tiles[r][c].isMatched(tiles[r + 1][c], Tile::BELOW)) {
                    return false;
                }
            }
        }
    }
    return true;
}

/* function solvePuzzle
 * Produces all puzzle solutions. Uses recursive findAllSolutions function
 *
 * @param puzzle A Puzzle object, representing a puzzle
 * @param timeIt true if the puzzle solution is being timed, false if not
 * @return a vector of solved puzzle grids
 */
Vector<Grid<Tile>> solvePuzzle(Puzzle &puzzle) {
    // recursively populate tiles and check solution
    Vector<Grid<Tile>> solutions;
    Vector<Tile> tileVec = puzzle.getTileVec();
    Grid<Tile> gridOfTiles(3, 3);
    findAllSolutions(tileVec, gridOfTiles, 0, 0, solutions);
    return solutions;
}

/* function findAllSolutions (recursive)
 * Recursively finds all puzzle solutions
 *
 * @param tileVec Vector of tiles, used for populating a grid for backtracking
 * @param tiles Grid of tiles, used to hold a puzzle of tiles, and for checking
 * solutions
 * @param row The current row we are analyzing
 * @param col The current column we are analyzing
 * @param solutions A Vector of tile grids that will hold all solutions
 */
void findAllSolutions(Vector<Tile> &tileVec, Grid<Tile> &tiles, int row,
                      int col, Vector<Grid<Tile>> &solutions) {
    if (tileVec.size() == 0) {
        // found potential solution
        bool matched = allMatch(tiles);
        if (matched) {
            solutions.add(tiles);
        }
    } else {
        // for each tile in the vector, remove the tile, and put it at the
        // current position
        int newCol = col + 1;
        int newRow = row;
        if (newCol == tiles.numCols()) {
            newCol = 0;
            newRow++;
        }
        for (int i = 0; i < tileVec.size(); i++) {
            for (int orientation = 0; orientation < 4; orientation++) {
                // add to grid
                Tile t = tileVec[i];
                Tile origTile = tiles[row][col];
                int origOrientation = t.getOrientation();
                t.setOrientation(orientation);
                tileVec.remove(i);
                tiles[row][col] = t;

                // recurse otherwise because we will never find a match
                bool canRecurse = true;

                if (tiles.inBounds(row - 1, col) &&
                    !tiles[row][col].isMatched(tiles[row - 1][col],
                                               Tile::ABOVE)) {
                    canRecurse = false;
                }

                if (tiles.inBounds(row, col - 1) &&
                    !tiles[row][col].isMatched(tiles[row][col - 1],
                                               Tile::LEFT)) {
                    canRecurse = false;
                }
                if (canRecurse) {
                    findAllSolutions(tileVec, tiles, newRow, newCol, solutions);
                }

                // re-orient and replace
                t.setOrientation(origOrientation);
                tileVec.insert(i, t);
                tiles[row][col] = origTile;
            }
        }
    }
}

/* function displayAndSaveSolutions
 * This function displays one solution at a time and asks the user
 * if they want to save the solution. If the user does want to
 * save the solution, calls the saveGrid() function of puzzle with the
 * user supplied filename.
 *
 * @param puzzle A Puzzle object
 * @param solutions The vector of all solutions
 */
void displayAndSaveSolutions(Puzzle &puzzle, Vector<Grid<Tile>> &solutions) {
    for (Grid<Tile> tiles : solutions) {
        puzzle.replaceGrid(tiles);
        cout << puzzle.toString() << endl;

        string filename =
            getLine("Please type a file name to save, or <enter> to "
                    "continue without saving.");
        if (filename != "") {
            if (puzzle.saveGrid(filename)) {
                cout << "'" << filename << "'"
                     << " saved." << endl;
            } else {
                cout << "Could not open file for saving." << endl;
            }
        }
    }
}

/* Feel free to investigate the functions below, but you should not modify them
 */

/* function init
 * Initializes the board, and runs the program loop.
 */
void init() {
    while (1) {
        bool donePlayingManually = false;
        Puzzle puzzle;
        GConsoleWindow::instance()->requestFocus();
        if (!loadPuzzle(puzzle)) {
            cout << "Could not load puzzle." << endl;
            return;
        }

        cout << "Beginning tiles: " << endl;
        cout << puzzle.toString() << endl;
        manualPlay(puzzle, donePlayingManually);

        while (!donePlayingManually) {
            std::this_thread::yield();
        }
        GConsoleWindow::instance()->requestFocus();
        Vector<Grid<Tile>> solutions;
        solveAndTimePuzzle(puzzle, solutions);
        for (Grid<Tile> tiles : solutions) {
            puzzle.replaceGrid(tiles);
            cout << puzzle.toString() << endl;
        }

        string seeAllSolutions =
            getLine("Would you like to see all solutions? (Y/n)? ");
        if (seeAllSolutions == "" || tolower(seeAllSolutions[0] == 'y')) {
            displayAndSaveSolutions(puzzle, solutions);
        }

        string playAgain = getLine("Would you like to play again (Y/n)? ");
        if (playAgain != "" && tolower(playAgain[0]) == 'n')
            break;
        puzzle.getWindow().close();
    }

    cout << "Thank you for plaing the tile match game!" << endl;
}

/* function solveAndTimePuzzle
 * Times how long it takes to find solutions, and prints out all solutions
 *
 * @param puzzle The Puzzle object
 * @param solutions A vector of all solutions
 */
void solveAndTimePuzzle(Puzzle &puzzle, Vector<Grid<Tile>> &solutions) {
    Timer t;
    getLine("Press <enter> to start searching for solutions.");
    cout << endl;

    t.start();

    solutions = solvePuzzle(puzzle);

    t.stop();
    cout << "Time: " << t.elapsed() << "ms" << endl;
    cout << "Found " << solutions.size() << " solutions:" << endl;
}

/* function loadPuzzle
 * Finds all puzzles and lets the user choose one.
 *
 * @param puzzle The Puzzle object
 * @return True if a puzzle was successfully loaded, false otherwise
 */
bool loadPuzzle(Puzzle &puzzle) {
    // list puzzle directories
    int i = 0;

    Vector<string> files;
    Vector<string> allowableFiles;

    string appDir = QCoreApplication::applicationDirPath().toStdString();

    listDirectory(appDir + "/../Resources/puzzles", files);

    for (string f : files) {
        if (isDirectory(appDir + "/../Resources/puzzles/" + f)) {
            allowableFiles.add(f);
            i++;
            cout << i << ". " << f << endl;
        }
    }

    int response = -1;
    while (response < 1 || response > files.size()) {
        response = getInteger("Choose a number: ", "Please enter a number.");
    }
    cout << endl;
    string puzzleDir =
        appDir + "/../Resources/puzzles/" + allowableFiles[response - 1];

    listDirectory(puzzleDir, files);
    cout << "Please choose the number of the file you would like to load:"
         << endl
         << endl;

    i = 0;
    allowableFiles.clear();
    for (string f : files) {
        if (matchFilenamePattern(f, "*.txt")) {
            allowableFiles.add(f);
            i++;
            cout << i << ". " << f << endl;
        }
    }

    response = -1;
    while (response < 1 || response > allowableFiles.size()) {
        response = getInteger("Choose a number:", "Please enter a number.");
    }

    string filename = allowableFiles[response - 1];

    if (!puzzle.populate(filename, puzzleDir)) {
        return false;
    }
    puzzle.getWindow().setLocation(0, 0);
    setConsoleLocation(
        puzzle.getWindow().getX() + puzzle.getWindow().getWidth(), 0);
    cout << endl;
    return true;
}

/* function manualPlay
 * Asks the user if they want to play manually, and then creates a PlayGame
 * instance to play the game.
 *
 * @param puzzle The Puzzle object
 */
void manualPlay(Puzzle &puzzle, bool &donePlayingManually) {
    string response =
        getLine("Would you like to play the game manually (y/N)? ");
    if (response != "" && toupper(response[0]) == 'Y') {
        cout
            << "Have fun! Use the mouse to move pieces, and click on a piece to"
            << endl;
        cout << "rotate the piece 90 degrees clockwise." << endl;
        cout << "Press any key on the keyboard to quit manual play mode."
             << endl;
        PlayGame(puzzle, allMatch, donePlayingManually);
    } else {
        donePlayingManually = true;
    }
}
