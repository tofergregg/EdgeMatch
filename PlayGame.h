#pragma once

#include "Puzzle.h"
#include "vector.h"

#include "gconsolewindow.h"
// #include "gevents.h"
#include "gobjects.h"
#include "goptionpane.h"
#include "gthread.h"
#include "gwindow.h"
#include "strlib.h"
#include "vector.h"
#include <cctype>

using namespace std;

class PlayGame {
  public:
    PlayGame(Puzzle &puzzle, bool (*allMatch)(Grid<Tile> &),
             bool &donePlayingManually);

  private:
    double origX, origY;
    GPoint origImCenterLoc;
    bool mouseDown = false;
    GImage *currIm = nullptr;
    int currImOrientation;
    bool (*allMatch)(Grid<Tile> &);

    bool handleMouseEvent(Puzzle &puzzle, GMouseEvent e);
    void mousePressed(GMouseEvent e);
    void mouseReleased(GMouseEvent e, Puzzle &puzzle);
    void finishDrag(Puzzle &puzzle);
    void mouseDragged(GMouseEvent e, Puzzle &puzzle);
    void rotateImage(GMouseEvent e, Puzzle &puzzle);
    GPoint getCenter(GRectangle rect);
    double dist(GPoint a, GPoint b);
    bool fixedContains(GImage *im, int rotation, GPoint loc);
    GPoint fixedGetCenterLocation(GImage *im, int orientation);
    void fixedSetCenterLocation(GImage *im, int orientation, GPoint loc);
    void playGameManually(Puzzle &puzzle, bool &donePlayingManually);
};
