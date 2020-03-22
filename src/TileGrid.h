#pragma once

#include "Tile.h"
#include "gobjects.h"
#include "grid.h"
#include "gwindow.h"
#include "map.h"
#include "vector.h"

using namespace std;

const int TILE_PIXEL_DIM = 200; // 200px x 200px images

/* struct ImageAndTile
 * This struct keeps track of the image for a tile,
 * and both the orientation and the Tile it represents
 */
struct ImageAndTile {
    GImage* im;
    Tile tile;
};

class TileGrid {
  public:
    /* destructor (cleans up images) */
    ~TileGrid();

    /* method populate
     * Populates the TileGrid with images and tiles
     *
     * @param filename The filename for the puzzle
     * @param imageDir The directory for the images for this puzzle
     * @return true if successfully loaded puzzle, false otherwise
     */
    bool populate(string filename, string imageDir);

    /* method loadImages
     * Loads the images for the grid
     *
     * @param imageDir The directory of images to load
     */
    void loadImages(string imageDir);

    /* method drawTiles
     * Draws the puzzle on the screen
     */
    void drawTiles();

    /* method placeTile
     * Puts a tile at a row, col on the screen
     *
     * @param tile The tile to place
     * @param row The row to place the tile into
     * @param col The column to place the tile into
     */
    void placeTile(Tile& tile, int row, int col);

    /* method getGrid
     * Returns a reference to the internal grid of tiles
     *
     * @return The reference to the internal grid of tiles
     */
    Grid<Tile>& getGrid();

    /* method saveGrid
     * Saves the puzzle to a file
     *
     * @param filename The filename to save to
     * @return true if successfully saved, false otherwise
     */
    bool saveGrid(string filename);

    /* method getWindow
     * Returns a reference to the window where we draw tiles
     *
     * @return a GWindow reference
     */
    GWindow& getWindow();

    /* method getImages
     * Returns a Map of tile strings to the images
     *
     * @return A reference to the Map with tile strings as keys, and
     *         Image details as values
     */
    Map<string, ImageAndTile>& getImages();

    /* method getTileVec
     * Converts a tile grid into a simple Vector of tiles
     *
     * @return A Vector of the tiles in the grid.
     */
    Vector<Tile> getTileVec();

    /* method swapImages
     * Swaps two images in the grid (used for playing the game manually)
     *
     * @param im1: the first image and its details
     * @param im2: the second image and its details
     */
    void swapImages(ImageAndTile im1, ImageAndTile im2);

    /* method updateTile
     * Updates a tile based on the new rotation
     *
     * @param im The ImageAndTile struct
     */
    void updateTile(ImageAndTile im);

    /* method toString
     * Creates a string representation of the whole puzzle in the form
     *  A b C d 0
     *  B c D A 1
     *  ...
     *
     * @return A string representing the puzzle
     */
    string toString();

    /* method replaceGrid
     * Replaces the current grid of tiles with all new tiles
     *
     * @param newTiles The grid of new tiles
     */
    void replaceGrid(Grid<Tile> newTiles);

  private:
    Grid<Tile> tiles;
    Map<string, ImageAndTile> images;
    GWindow gw;
};
