#include <iostream>
#include "console.h"
#include "grid.h"
#include "filelib.h"
#include "strlib.h"
#include "simpio.h"
#include "timer.h"

#include "PlayGame.h"
#include "Tile.h"
#include "TileGrid.h"

using namespace std;

void init();
bool allMatch(Grid<Tile> &tiles);
void populateGrid(Grid<Tile> &tiles, Vector<Tile> &tileVec);
Vector<Grid<Tile>> solvePuzzle(TileGrid &tg, bool timeIt = false);
void solvePuzzle(Vector<Tile> &tileVec, int row, int col, TileGrid &tg,
                 Vector<Grid<Tile>> &solutions, bool timeIt);
bool loadPuzzle(TileGrid &tg);
void manualPlay(TileGrid &tg, bool &donePlayingManually);
bool solvePuzzleSetup(TileGrid &tg);

// #include <QDir>

int main() {
    // QDir().setCurrent("EdgeMatch.app/Contents/Resources");
    setConsoleWindowTitle("Tile Match");
    setConsoleSize(600, 600);
    cout << "Welcome to the tile match game!" << endl;
    init();

    return 0;
}

/* function allMatch
 * @param tiles A Grid of Tile instances
 * @return true if the puzzle is solved, false if it is not
 */
bool allMatch(Grid<Tile> &tiles) {
    // only need to check 12 positions, as the rest are overlapping
    int rows = tiles.numRows();
    int cols = tiles.numCols();

    for (int r=0; r < rows; r++) {
        for (int c=0; c < cols; c++) {
            // no need to check above or to the left

            // right
            if (tiles.inBounds(r,c+1)) {
                if (!tiles[r][c].isMatched(tiles[r][c+1], Tile::RIGHT)) {
                    return false;
                }
            }
            // below
            if (tiles.inBounds(r+1,c)) {
                if (!tiles[r][c].isMatched(tiles[r+1][c], Tile::BELOW)) {
                    return false;
                }
            }
        }
    }
    return true;
}

/* function solvePuzzle
 * @param tg A TileGrid object, representing a puzzle
 * @param timeIt true if the puzzle solution is being timed, false if not
 * @return a vector of solved puzzle grids
 */
Vector<Grid<Tile>> solvePuzzle(TileGrid &tg, bool timeIt) {
    // recursively populate tiles and check solution
    Vector<Grid<Tile>> solutions;
    Vector<Tile> tileVec = tg.getTileVec();
    solvePuzzle(tileVec, 0, 0, tg, solutions, timeIt);
    return solutions;
}

/* function solvePuzzle (recursive)
 * @param elements Vector of tiles, used for populating a grid for backtracking
 * @param row The current row we are analyzing
 * @param col The current column we are analyzing
 * @param tg A reference to a TileGrid instance
 * @param solutions A Vector of tile grids that will hold all solutions
 * @param timeIt true if the puzzle solution is being timed, false if not
 */
void solvePuzzle(Vector<Tile> &tileVec, int row, int col, TileGrid &tg, Vector<Grid<Tile>> &solutions, bool timeIt) {
    Grid<Tile> &tiles = tg.getGrid();
    if (tileVec.size() == 0) {
        // found potential solution
        if (!timeIt) {
            tg.drawTiles();
        }

        bool matched = allMatch(tiles);
        if (matched) {
            if (!timeIt) {
                cout << "Found a match:" << endl;
                cout << tg.toString() << endl << endl;
            }

            solutions.add(tiles);

            if (!timeIt) {
                string filename = getLine("Please type a file name to save, or <enter> to continue without saving.");
                if (filename != "") {
                    if (tg.saveGrid(filename)) {
                        cout << "'" << filename << "'" << " saved.";
                    } else {
                        cout << "Could not open file for saving.";
                    }
                }
                cout << endl;
            }
        }
    } else {
        // for each tile in the vector, remove the tile, and put it at the current position
        int newCol = col + 1;
        int newRow = row;
        if (newCol == tiles.numCols()) {
            newCol = 0;
            newRow++;
        }
        for (int i=0; i < tileVec.size(); i++) {
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

                if (tiles.inBounds(row-1,col) && !tiles[row][col].isMatched(tiles[row-1][col], Tile::ABOVE)) {
                    canRecurse = false;
                }

                if (tiles.inBounds(row,col-1) && !tiles[row][col].isMatched(tiles[row][col-1], Tile::LEFT)) {
                    canRecurse = false;
                }
                if (canRecurse) {
                    solvePuzzle(tileVec, newRow, newCol, tg, solutions, timeIt);
                }

                // re-orient and replace
                t.setOrientation(origOrientation);
                tileVec.insert(i,t);
                tiles[row][col] = origTile;
            }
        }
    }
}

/* Feel free to investigate the functions below, but you should not modify them */
void init() {
    while (1) {
        bool donePlayingManually = false;
        TileGrid tg;
        if (!loadPuzzle(tg)) {
        cout << "Could not load puzzle." << endl;
        return;
        }

        cout << "Beginning tiles: " << endl;
        cout << tg.toString() << endl;
        manualPlay(tg, donePlayingManually);

        while (!donePlayingManually) {
            std::this_thread::yield();
        }
        GConsoleWindow::instance()->requestFocus();
        if (!solvePuzzleSetup(tg)) break;
    }

    cout << "Thank you for plaing the tile match game!" << endl;
}

bool solvePuzzleSetup(TileGrid &tg) {
    bool timeIt = false;
    Timer t;
    string response = getLine("Would you like to time your solution (y/N)? ");
    if (response != "" && toupper(response[0]) == 'Y') {
        timeIt = true;
    }
    getLine("Press <enter> to start searching for solutions.");
    cout << endl;

    if (timeIt) {
        t.start();
    }

    Vector<Grid<Tile>> solutions = solvePuzzle(tg, timeIt);

    if (timeIt) {
        t.stop();
        cout << "Time: " << t.elapsed() << "ms" << endl;
    }

    cout << "Found " << solutions.size() << " solutions:" << endl;
    for (Grid<Tile> tiles : solutions) {
        tg.replaceGrid(tiles);
        cout << tg.toString() << endl;
    }
    string playAgain = getLine("Would you like to play again (Y/n)? ");
    if (playAgain != "" && tolower(playAgain[0]) == 'n') return false;
    tg.getWindow().close();
    return true;
}

bool loadPuzzle(TileGrid &tg) {
    // list puzzle directories
    int i = 0;

    Vector<string>files;
    Vector<string>allowableFiles;

    listDirectory("puzzles",files);

    for (string f : files) {
        if (isDirectory("puzzles/" + f)) {
            allowableFiles.add(f);
            i++;
            cout << i << ". " << f << endl;
        }
    }

    int response = -1;
    while (response < 1 || response > files.size()) {
        response = getInteger("Choose a number: ",
                              "Please enter a number.");
    }
    cout << endl;
    string puzzleDir = "puzzles/" + allowableFiles[response-1];

    listDirectory(puzzleDir,files);
    cout << "Please choose the number of the file you would like to load:" << endl << endl;

    i = 0;
    allowableFiles.clear();
    for (string f : files) {
        if (matchFilenamePattern(f,"*.txt")) {
            allowableFiles.add(f);
            i++;
            cout << i << ". " << f << endl;
        }
    }

    response = -1;
    while (response < 1 || response > allowableFiles.size()) {
        response = getInteger("Choose a number:",
                              "Please enter a number.");
    }

    string filename = allowableFiles[response-1];

    if (!tg.populate(filename, puzzleDir)) {
        return false;
    }
    tg.getWindow().setLocation(0,0);
    setConsoleLocation(tg.getWindow().getX() + tg.getWindow().getWidth(),0);
    cout << endl;
    return true;
}

void manualPlay(TileGrid &tg, bool &donePlayingManually) {
    string response = getLine("Would you like to play the game manually (y/N)? ");
    if (response != "" && toupper(response[0]) == 'Y')  {
        cout << "Have fun! Use the mouse to move pieces, and click on a piece to" << endl;
        cout << "rotate the piece 90 degrees clockwise." << endl;
        cout << "Press any key on the keyboard to quit manual play mode." << endl;
        PlayGame(tg,allMatch,donePlayingManually);
    } else {
        donePlayingManually = true;
    }
}
