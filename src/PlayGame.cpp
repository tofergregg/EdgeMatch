#include "PlayGame.h"

using namespace std;

PlayGame::PlayGame(Puzzle& tg, bool (*allMatch)(Grid<Tile>&),
                   bool& donePlayingManually) {
    mouseDown = false;
    currIm = nullptr;
    this->allMatch = allMatch;
    playGameManually(tg, donePlayingManually);
    while (!donePlayingManually) {
        std::this_thread::yield();
    }
}

void PlayGame::playGameManually(Puzzle& tg, bool& donePlayingManually) {
    tg.getWindow().requestFocus();
    tg.getWindow().setMouseListener(
        [&tg, &donePlayingManually, this](GEvent e) {
            if (!handleMouseEvent(tg, e)) {
                tg.getWindow().removeKeyListener();
                tg.getWindow().removeMouseListener();
                donePlayingManually = true;
            }
        });

    tg.getWindow().setKeyListener([&donePlayingManually, &tg](GEvent e) {
        e.ignore();
        tg.getWindow().removeKeyListener();
        tg.getWindow().removeMouseListener();
        GConsoleWindow::instance()->requestFocus();
        donePlayingManually = true;
    });
}

bool PlayGame::handleMouseEvent(Puzzle& tg, GMouseEvent e) {
    EventType etype = e.getEventType();
    if (etype == MOUSE_PRESSED) {
        mousePressed(e);
    } else if (etype == MOUSE_DRAGGED) {
        mouseDragged(e, tg);
    } else if (etype == MOUSE_RELEASED) {
        mouseReleased(e, tg);
        // check to see if game is solved
        Grid<Tile>& tiles = tg.getGrid();
        if (allMatch(tiles)) {
            cout << "Solution: " << endl;
            cout << tg.toString() << endl;
            GOptionPane::ConfirmResult res = GOptionPane::showConfirmDialog(
                "You solved the game!\n"
                "Would you like to keep playing?");
            if (res == GOptionPane::CONFIRM_YES) {
                return true;
            } else {
                return false;
            }
        }
    }
    return true;
}

void PlayGame::mousePressed(GMouseEvent e) {
    // cout << "mouse pressed" << endl;
    origX = e.getX();
    origY = e.getY();
    mouseDown = true;
}

void PlayGame::mouseReleased(GMouseEvent e, Puzzle& tg) {
    // cout << "mouse released" << endl;
    mouseDown = false;
    if (currIm) {
        // we must have been dragging
        finishDrag(tg);
    } else {
        // the user clicked
        rotateImage(e, tg);
    }
}

void PlayGame::mouseDragged(GMouseEvent e, Puzzle& tg) {
    // cout << "mouse dragged" << endl;
    GImage* im = nullptr;
    if (currIm) {
        im = currIm;
    } else {
        // find image that was clicked
        Map<string, ImageAndTile> images = tg.getImages();
        for (string s : images) {
            GImage* possibleIm = images[s].im;
            if (fixedContains(possibleIm, images[s].tile.getOrientation(),
                              {e.getX(), e.getY()})) {
                // cout << "Found image!" << endl;
                currIm = possibleIm;
                im = possibleIm;
                currImOrientation = images[s].tile.getOrientation();
                origImCenterLoc = fixedGetCenterLocation(im, currImOrientation);
                // origImCenterLoc = im->getCenterLocation();
                break;
            }
        }
    }
    if (currIm) {
        switch (currImOrientation) {
        case 0:
            im->move(e.getX() - origX, e.getY() - origY);
            break;
        case 1:
            im->move(e.getY() - origY, origX - e.getX());
            break;
        case 2:
            im->move(origX - e.getX(), origY - e.getY());
            break;
        case 3:
            im->move(origY - e.getY(), e.getX() - origX);
            break;
        }
        im->sendToFront();
        origX = e.getX();
        origY = e.getY();
    }
}

void PlayGame::finishDrag(Puzzle& tg) {
    GImage* closestImg = nullptr;
    int closestOrientation = 0;
    double closestDist = 0.0;
    Map<string, ImageAndTile>& images = tg.getImages();
    for (string s : images) {
        double distToImage;
        GImage* possibleIm = images[s].im;
        if (possibleIm == currIm) {
            distToImage =
                dist(fixedGetCenterLocation(currIm, currImOrientation),
                     origImCenterLoc);
            // distToImage = dist(currIm->getCenterLocation(),origImCenterLoc);
        } else {
            distToImage =
                dist(fixedGetCenterLocation(currIm, currImOrientation),
                     fixedGetCenterLocation(possibleIm,
                                            images[s].tile.getOrientation()));
            // distToImage =
            // dist(currIm->getCenterLocation(),possibleIm->getCenterLocation());
        }
        if (closestImg) {
            if (closestDist > distToImage) {
                closestDist = distToImage;
                closestImg = possibleIm;
                closestOrientation = images[s].tile.getOrientation();
            }
        } else {
            closestDist = distToImage;
            closestImg = possibleIm;
            closestOrientation = images[s].tile.getOrientation();
        }
    }
    if (currIm == closestImg) {
        // currIm->setCenterLocation(origImCenterLoc);
        fixedSetCenterLocation(currIm, currImOrientation, origImCenterLoc);
    } else {
        fixedSetCenterLocation(
            currIm, currImOrientation,
            fixedGetCenterLocation(closestImg, closestOrientation));
        fixedSetCenterLocation(closestImg, closestOrientation, origImCenterLoc);
        // swap images in Puzzle and the Grid<Tile> &
        // GPoint otherCenter = closestImg->getCenterLocation();
        // closestImg->setCenterLocation(origImCenterLoc);
        // currIm->setCenterLocation(otherCenter);
        for (string sCurr : images) {
            if (images[sCurr].im == currIm) {
                for (string sClosest : images) {
                    if (images[sClosest].im == closestImg) {
                        tg.swapImages(images[sClosest], images[sCurr]);
                    }
                }
            }
        }
    }
    currIm = nullptr;
}

void PlayGame::rotateImage(GMouseEvent e, Puzzle& tg) {
    // the user clicked
    // rotate 90 degrees clockwise
    // find image that was clicked
    Map<string, ImageAndTile>& images = tg.getImages();
    for (string s : images) {
        GImage* im = images[s].im;
        if (fixedContains(im, images[s].tile.getOrientation(),
                          {e.getX(), e.getY()})) {
            // rotate back to 0
            GPoint currCenterLoc = im->getCenterLocation();
            im->rotate(90 * -images[s].tile.getOrientation());
            // now rotate 90 degrees clockwise from original
            im->rotate(90 * ((images[s].tile.getOrientation() + 1) % 4));
            images[s].tile.setOrientation(
                (images[s].tile.getOrientation() + 1) % 4);
            //           fixedSetCenterLocation(im,images[s].tile.getOrientation(),currCenterLoc);
            im->setCenterLocation(currCenterLoc.getY(), -currCenterLoc.getX());
            tg.updateTile(images[s]);
            break;
        }
    }
}

GPoint PlayGame::getCenter(GRectangle rect) {
    return {(rect.getX() + rect.getX() + rect.getWidth()) / 2,
            (rect.getY() + rect.getY() + rect.getHeight() / 2)};
}

double PlayGame::dist(GPoint a, GPoint b) {
    return sqrt((a.getX() - b.getX()) * ((a.getX() - b.getX())) +
                (a.getY() - b.getY()) * ((a.getY() - b.getY())));
}

bool PlayGame::fixedContains(GImage* im, int orientation, GPoint loc) {
    // for some reason, the contains method in GImage doesn't work for rotated
    // images.
    switch (orientation) {
    case 1:
        loc = {loc.getY(), -loc.getX()};
        break;
    case 2:
        loc = {-loc.getX(), -loc.getY()};
        break;
    case 3:
        loc = {-loc.getY(), loc.getX()};
        break;
    }
    return im->contains(loc);
}

GPoint PlayGame::fixedGetCenterLocation(GImage* im, int orientation) {
    GPoint loc = im->getCenterLocation();

    switch (orientation) {
    case 1:
        loc = {-loc.getY(), loc.getX()};
        break;
    case 2:
        loc = {-loc.getX(), -loc.getY()};
        break;
    case 3:
        loc = {loc.getY(), -loc.getX()};
        break;
    }
    return loc;
}

void PlayGame::fixedSetCenterLocation(GImage* im, int orientation, GPoint loc) {
    switch (orientation) {
    case 1:
        loc = {loc.getY(), -loc.getX()};
        break;
    case 2:
        loc = {-loc.getX(), -loc.getY()};
        break;
    case 3:
        loc = {-loc.getY(), loc.getX()};
        break;
    }
    im->setCenterLocation(loc);
}
