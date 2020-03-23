#pragma once

#include "vector.h"

using namespace std;

class Tile {
  public:
    /* enum Connection
     * This enum represents the placement of a tile when used in the isMatched()
     * function. ABOVE means that the other tile is located directly above this
     * tile, RIGHT means that the other tile is located to the right of this
     * tile, BELOW means that the other tile is located below this tile, LEFT
     * means that the other tile is located to the left of this tile
     */
    enum Connection {
        ABOVE,
        RIGHT,
        BELOW,
        LEFT,
    };

    /* method Tile (default)
     * The default constructor. Constructs a Tile with default but useless
     * values.
     */
    Tile();

    /* method Tile (overloaded)
     * @param s A string representing a tile
     *
     * A constructor that uses a string to populate the orientation and sides
     * vector. The string must be in the form "A B a b 0" or "A B a b". If the
     * orientation is not included in the string, the orientation should be set
     * to 0.
     */
    Tile(string s);

    /* method isMatched
     * @param other The other tile to compare to this one.
     * @param otherLocation The location of the other tile relative to this
     * tile.
     *
     * @return true if the tiles are in a matching configuration, false if they
     * are not matched.
     *
     * The isMatched function compares the two tiles in their current
     * orientation and location relative to each other. If the tiles form a
     * matching configuration, return true, otherwise, return false.
     */
    bool isMatched(Tile& other, Connection otherLocation);

    /* method getOrientation
     * @return The orientation of the tile
     */
    int getOrientation();

    /* method setOrientation
     * @param orientation The orientation: 0 = original,
     * 									   1=rotated right
     * 90 degrees, 2 = rotated right 180 degrees 3 = rotated right 270 degrees
     */
    void setOrientation(int orientation);
    string sidesStr();

    friend ostream& operator<<(ostream& out, Tile const& tile);

  private:
    /* class variable orientation
     * The orientation of the tile
     */
    int orientation;

    /* class variable sides
     * A vector of strings representing the sides of the tile
     * Kept in order. For an input string of "A B a b 0", sides
     * should hold the values "A", "B", "a", "b" in order.
     */
    Vector<string> sides;
};
