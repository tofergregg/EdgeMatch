#include "Tile.h"
#include "strlib.h"
#include "vector.h"
#include <cctype>

using namespace std;

Tile::Tile() {
    sides.add("X");
    sides.add("X");
    sides.add("x");
    sides.add("x");
    orientation = 0; // top faces up
}

Tile::Tile(string s) {
    // reads in an entire string in the form "A B a b 0"
    // where the end digit is the orientation, and is optional
    Vector<string> parts = stringSplit(s, " ");
    if (parts.size() == 5) {
        // we have the optional orientation
        orientation = stringToInteger(parts[4]);
        // remove the orientation from the Vector
        parts.removeBack();
    } else {
        orientation = 0;
    }
    for (string part : parts) {
        sides.add(part);
    }
}

int Tile::getOrientation() { return orientation; }

void Tile::setOrientation(int orientation) { this->orientation = orientation; }

string Tile::sidesStr() {
    string ss;
    for (string s : sides) {
        ss += s;
    }
    return ss;
}

bool Tile::isMatched(Tile& other, Connection otherLocation) {
    string thisSide, otherSide;

    if (otherLocation == Tile::ABOVE) {
        thisSide = sides[(4 - orientation) % 4];
        if (other.orientation % 2 == 0) {
            otherSide = other.sides[abs(other.orientation - 2)];
        } else {
            otherSide = other.sides[other.orientation];
        }
    } else if (otherLocation == Tile::RIGHT) {
        if (orientation % 2 == 0) {
            thisSide = sides[orientation + 1];
        } else {
            thisSide = sides[orientation - 1];
        }
        otherSide = other.sides[3 - other.orientation];
    } else if (otherLocation == Tile::LEFT) {
        thisSide = sides[3 - orientation];
        if (other.orientation % 2 == 0) {
            otherSide = other.sides[other.orientation + 1];
        } else {
            otherSide = other.sides[other.orientation - 1];
        }
    } else {
        if (orientation % 2 == 0) {
            thisSide = sides[abs(orientation - 2)];
        } else {
            thisSide = sides[orientation];
        }
        otherSide = other.sides[(4 - other.orientation) % 4];
    }
    if ((isupper(thisSide[0]) && islower(otherSide[0])) ||
        (islower(thisSide[0]) && isupper(otherSide[0]))) {
        return tolower(thisSide[0]) == tolower(otherSide[0]);
    } else {
        return false;
    }
}

ostream& operator<<(ostream& out, Tile const& tile) {
    // prints as follows:
    // A B C D 0
    // Which means that with an orientation of 0, the piece
    // would look like this:
    //  A
    // C B
    //  D
    //
    // If we had this:
    // A B C D 3
    // The piece would look like this, because it has been
    // rotated clockwise by 270 degrees:
    //  B
    // A C
    //  D
    //
    for (string s : tile.sides) {
        out << s << " ";
    }
    out << tile.orientation;
    return out;
}
