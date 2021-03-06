#include "Puzzle.h"
#include "filelib.h"
#include <iostream>
#include <sstream>

using namespace std;

Puzzle::~Puzzle() {
    for (string key : images) {
        delete images[key].im;
    }
}

bool Puzzle::populate(string filename, string imageDir) {
    // file should be in the form:
    // b c D A
    // a b A C
    // ...
    // or in the form:
    // b c D A 2
    // a b A C 0
    // ...
    Vector<Tile> tileVec;
    string line;
    ifstream fs;

    if (!openFile(fs, imageDir + "/" + filename)) {
        return false; // failure opening file
    }
    while (true) {
        getline(fs, line);
        if (fs.eof())
            break;
        tileVec.add(Tile(line));
    }
    fs.close();

    tiles.resize(int(sqrt(tileVec.size())), int(sqrt(tileVec.size())));
    gw.setCanvasSize(TILE_PIXEL_DIM * tiles.numCols(),
                     TILE_PIXEL_DIM * tiles.numRows());
    // assume count is a perfect square
    for (int r = 0; r < tiles.numRows(); r++) {
        for (int c = 0; c < tiles.numCols(); c++) {
            tiles[r][c] = tileVec[r * tiles.numCols() + c];
        }
    }

    loadImages(imageDir);
    drawTiles();
    return true;
}

void Puzzle::loadImages(string imageDir) {
    for (int r = 0; r < tiles.numRows(); r++) {
        for (int c = 0; c < tiles.numCols(); c++) {
            string sideStr = tiles[r][c].sidesStr();
            GImage *img = new GImage(imageDir + "/" + sideStr + ".png");
            // scale to 200px x 200px
            img->setSize(TILE_PIXEL_DIM, TILE_PIXEL_DIM);
            // rotate to tile rotation
            img->rotate(90 * tiles[r][c].getOrientation());
            images.put(sideStr, {img, tiles[r][c]});
        }
    }
}

void Puzzle::drawTiles() {
    // remove all tiles from gw
    // gw.clear();
    for (int r = 0; r < tiles.numRows(); r++) {
        for (int c = 0; c < tiles.numCols(); c++) {
            placeTile(tiles[r][c], r, c);
            // cout << tiles[r][c] << endl;
        }
    }
}

Grid<Tile> &Puzzle::getGrid() { return tiles; }

void Puzzle::placeTile(Tile &tile, int row, int col) {
    // rotate
    GImage *im = images[tile.sidesStr()].im;
    int curr_orientation = images[tile.sidesStr()].tile.getOrientation();
    // rotate back to normal
    im->rotate(90 * -curr_orientation);
    // rotate to new position
    im->rotate(90 * tile.getOrientation());
    images[tile.sidesStr()] = {im, tile};
    // rotation is counterclockwise about the top left corner for an image, so
    // we need to translate the coordinates appropriately. Some of the
    // translations are odd (swapping x and y, for instance)
    switch (tile.getOrientation()) {
    case 0:
        gw.add(im, col * TILE_PIXEL_DIM, row * TILE_PIXEL_DIM);
        break;
    case 1: // 90 degrees
        gw.add(im, row * TILE_PIXEL_DIM,
               -col * TILE_PIXEL_DIM - TILE_PIXEL_DIM);
        break;
    case 2: // 180 degrees
        gw.add(im, -col * TILE_PIXEL_DIM - TILE_PIXEL_DIM,
               -row * TILE_PIXEL_DIM - TILE_PIXEL_DIM);
        break;
    case 3: // 270 degrees
        gw.add(im, -row * TILE_PIXEL_DIM - TILE_PIXEL_DIM,
               col * TILE_PIXEL_DIM);
        break;
    }
}

GWindow &Puzzle::getWindow() { return gw; }

string Puzzle::toString() {
    // creates a string representing the board in the following form:
    // A b C d 0
    // B c D A 1
    // ...
    stringstream ss;
    for (int r = 0; r < tiles.numRows(); r++) {
        for (int c = 0; c < tiles.numCols(); c++) {
            ss << tiles[r][c] << endl;
        }
    }
    return ss.str();
}

bool Puzzle::saveGrid(string filename) {
    // saves grid into a file
    ofstream fs;
    if (!openFile(fs, filename)) {
        return false;
    }

    fs << toString();
    fs.close();
    return true;
}

Vector<Tile> Puzzle::getTileVec() {
    Vector<Tile> v;
    for (int r = 0; r < tiles.numRows(); r++) {
        for (int c = 0; c < tiles.numRows(); c++) {
            v.add(tiles[r][c]);
        }
    }
    return v;
}

Map<string, ImageAndTile> &Puzzle::getImages() { return images; }

void Puzzle::swapImages(ImageAndTile im1, ImageAndTile im2) {
    int t1R = 0, t1C = 0, t2R = 0, t2C = 0;
    for (int r = 0; r < tiles.numRows(); r++) {
        for (int c = 0; c < tiles.numCols(); c++) {
            if (tiles[r][c].sidesStr() == im1.tile.sidesStr()) {
                t1R = r;
                t1C = c;
            } else if (tiles[r][c].sidesStr() == im2.tile.sidesStr()) {
                t2R = r;
                t2C = c;
            }
        }
    }
    Tile temp = tiles[t1R][t1C];
    tiles[t1R][t1C] = tiles[t2R][t2C];
    tiles[t2R][t2C] = temp;
}

void Puzzle::updateTile(ImageAndTile im) {
    for (Tile &t : tiles) {
        if (t.sidesStr() == im.tile.sidesStr()) {
            t.setOrientation(im.tile.getOrientation());
            break;
        }
    }
}

void Puzzle::replaceGrid(Grid<Tile> newTiles) {
    tiles.clear();
    tiles.resize(newTiles.numRows(), newTiles.numCols());
    for (int r = 0; r < newTiles.numRows(); r++) {
        for (int c = 0; c < newTiles.numCols(); c++) {
            tiles[r][c] = newTiles[r][c];
        }
    }
    drawTiles();
}
