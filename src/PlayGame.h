#pragma once

#include "TileGrid.h"
#include "vector.h"

#include "gconsolewindow.h"
#include "gevents.h"
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
    PlayGame(TileGrid& tg, bool (*allMatch)(Grid<Tile>&),
             bool& donePlayingManually);

  private:
    double origX, origY;
    GPoint origImCenterLoc;
    bool mouseDown = false;
    GImage* currIm = nullptr;
    int currImOrientation;
    bool (*allMatch)(Grid<Tile>&);

    bool handleMouseEvent(TileGrid& tg, GMouseEvent e);
    void mousePressed(GMouseEvent e);
    void mouseReleased(GMouseEvent e, TileGrid& tg);
    void finishDrag(TileGrid& tg);
    void mouseDragged(GMouseEvent e, TileGrid& tg);
    void rotateImage(GMouseEvent e, TileGrid& tg);
    GPoint getCenter(GRectangle rect);
    double dist(GPoint a, GPoint b);
    bool fixedContains(GImage* im, int rotation, GPoint loc);
    GPoint fixedGetCenterLocation(GImage* im, int orientation);
    void fixedSetCenterLocation(GImage* im, int orientation, GPoint loc);
    void playGameManually(TileGrid& tg, bool& donePlayingManually);
};
