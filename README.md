# Edge Match: The Pattern Puzzle Solver

<center>
<img alt = "Vess Billion Bubbles Cola Puzzle" src="https://web.stanford.edu/~cgregg/EdgeMatch/vess-billion-bubbles-pcs1.jpg" style="width: 50%;">

Image source: <a href="http://www.robspuzzlepage.com/pattern.htm">Rob's Puzzle Page</a>
</center>

In this assignment, you will be writing code that will solve the _edge matching_ category of puzzles. In particular, your solution will be able to solve a traditional 3x3 grid. Edge matching puzzles have been around since the late 1800s, and they consist of a set of (generally square) tiles that have patterns such that two edges of a tile "match" in a recognizable pattern. For the tiles shown above, matching edges consist of a bottle top in a particular color triangle and a bottle bottom in the same color triangle. The top-right tile in the image above matches its red bottle bottom with the tile to its left's red bottle-top, and it also matches the cream bottle top with the cream bottle bottom of the tile below it. The image above happens to be a vintage puzzle for _Vess Cola_ that was a common advertising giveaway during the 1930s-1950s.

Edge-matching games are suprisingly difficult to solve by hand. Each tile can be oriented four ways, and each tile can be in any of the nine board positions. This gives a total of `9! * 4^9` combinations, or roughly _95 billion_ different positions. But, with your crack recursive backtracking skills, and thanks to the fast computation abilities of modern computers, your program will be able to completely solve these puzzles in mere milliseconds.

To complete the assignment, you will be using C++ classes, and (as mentioned above), you will be implementing a recursive backtracking algorithm to produce the puzzle solutions. Download the starter code below. You may also want to take a look at the Demo Program, available for Mac, Windows, and Linux.

<center><table>
<tr>
<td align="center"><a href="http://web.stanford.edu/~cgregg/EdgeMatch/EdgeMatch-Starter.zip"><img alt = "Starter Code" src="https://web.stanford.edu/~cgregg/EdgeMatch/download.png" style="width: 64;"><br>Starter Code</a></td>
<td align="center"><a href="https://web.stanford.edu/~cgregg/EdgeMatch/EdgeMatch-mac.zip"><img alt = "Mac Demo" src="https://web.stanford.edu/~cgregg/EdgeMatch/apple.png" style="width: 64;"><br>Mac Demo</a></td>
<td align="center"><img alt = "Windows Demo" src="https://web.stanford.edu/~cgregg/EdgeMatch/microsoft.png" style="width: 64;"><br>Windows Demo</td>
<td align="center"><img alt = "Linux Demo" src="https://web.stanford.edu/~cgregg/EdgeMatch/tux.png" style="width: 64;"><br>Linux Demo</td>
</table></center>

You will turn in the following files:

1. **`Tile.cpp`**, the code for describing a tile.
2. **`edge-match.cpp`**, the code that runs the game and solves the puzzles.

# Puzzle Overview

The Edge-Matching puzzles we will look at are 3x3 square tile puzzles where each tile can be in any of four positions on the board, and in any of the nine available positions. The user can play the game manually with the mouse, or they can solve the puzzles in one of two ways: either timed or one-at-a-time. If the game is played manually, clicking on a tile will rotate it one position clockwise, and dragging a tile on top of another tile will swap the two tiles and retain their current orientation. When the player solves the game, a message alert appears to congratulate the player.

When the user decides to find the solutions, all solutions are found at once, and an overall time to find all the solutions is shown. The user can then choose to see the solutions one at a time. Each solution is then found and displayed, and the user has the choice to save the solution in a form that can be re-loaded later.

When the program begins, the user is given a choice of which puzzle they want to play. The available puzzles have been curated from a number of sources -- some are actual photographs of vintage games, and some are computer-generated. After choosing a puzzle, the player gets to further choose a starting orientation. Some puzzles have solved orientations, and others have almost-solved starting orientations.

<center>
<td><img alt = "Program Start" src="https://web.stanford.edu/~cgregg/EdgeMatch/dogs-choice.png" style="width: 70%;"></td><br>
Game Start<p>
</center>

Once the player has chosen a puzzle, the puzzle appears on the screen and the player has the option to play the game manually. Note that some of the games have been constructed from photographs of real puzzles (the dog puzzle is an example), so the edges don't always line up exactly. Some of the puzzles (e.g., tens and turtles) have been computer-generated, and all edges should line up perfectly. 

The console window also shows the beginning layout and orientation of the tiles. We describe this notation in another section, below.

<center>
<td><img alt = "Playing Manually" src="https://web.stanford.edu/~cgregg/EdgeMatch/dogs-play.png" style="width: 100%;"></td><br>
Playing Manually<p>
</center>

Once the player is done trying to solve the puzzle manually, typing any key on the keyboard will produce another menu to begin finding solutions:

<center>
<td><img alt = "Timed Solution" src="https://web.stanford.edu/~cgregg/EdgeMatch/timed-solution.png" style="width: 100%;"></td><br>
Timing the Solution<p>
</center>

Because of symmetry considerations, there will always be at least four solutions for each game, and there could be more. All correct solutions are shown in the console (and each solved image briefly flashes on the screen), and the final orientation found remains on the screen. The timing does not include the animations.

If the player chooses to solve the puzzle one solution at a time, the player can save the solutions after each one, and then can re-load them later. The solutions are saved in text format.

<center>
<td><img alt = "Single Solution" src="https://web.stanford.edu/~cgregg/EdgeMatch/cola-solution.png" style="width: 100%;"></td><br>
Finding One Solution at a Time
</center>

# Puzzle Notation

In order to solve a puzzle algorithmically, we need a computer-friendly representation of a tile (that said, an awesome follow-on project would be to simply use [computer vision](https://en.wikipedia.org/wiki/Computer_vision) to solve puzzles!). We have chosen the format described below.

When you start a puzzle, you will see the _Beginning tiles_ shown. For example:
```
Beginning tiles: 
c a C D 0
D a c B 0
A c d B 0
d C A b 0
d b C A 0
c d B A 0
b a C D 0
A b d B 0
a B C d 0
```

Each line represents a single tile, with letters representing the sides, and the number represnting the rotational orientation.

To create a "match" in a puzzle, two tiles are needed, with each tile producing one half of the match (e.g., the top and bottom of a red-triangle cola bottle, or the head and tail of an orange-spotted dog). To represent a match, a letter is used, with the upper case varient (e.g., "A") representing one of the halves ("red-triangle cola bottle top") and the lower case variant ("a") representing the corresponding match ("red-triangle cola bottle bottom").

The orientation of a tile is based on the original image of each tile, with all tiles starting with an orientation of "0". The orientation increases as the tile is rotated clockwise, "1" would be a tile rotated clockwise once, "2" would be a tile rotated twice, and "3" would be a tile rotated three times. A tile rotated four times would be in its original position, and therefore have an orientation of "0" again.

Here is the original tile for one of the _turtle_ puzzle pieces:

<img alt = "Single Turtle" src="https://web.stanford.edu/~cgregg/EdgeMatch/singleTurtle.png" style="width: 20%;">

For a turtle, we have arbitrarily chosen `A` for blue, `B` for green, `C` for red, and `D` for yellow. We have also arbitrarily chosen the turtle head to be uppercase, and the turtle's tail to be lowercase. The following four images show the same piece rotated through its four positions. The letters are annotated on each part of a turtle on each tile, and the orientation is shown in the middle.

<img alt="single turtle annotated 0" src="https://web.stanford.edu/~cgregg/EdgeMatch/singleTurtleAnnotated.png" style="width: 20%"> ->
<img alt="single turtle annotated 1" src="https://web.stanford.edu/~cgregg/EdgeMatch/singleTurtleAnnotated1.png" style="width: 20%"> ->
<img alt="single turtle annotated 2" src="https://web.stanford.edu/~cgregg/EdgeMatch/singleTurtleAnnotated2.png" style="width: 20%"> ->
<img alt="single turtle annotated 3" src="https://web.stanford.edu/~cgregg/EdgeMatch/singleTurtleAnnotated3.png" style="width: 20%">

The following is a solved and annotated turtle puzzle. It also has the row and column annotated in the text (in <b><span style="color: #942193;">purple</span></b>).

<center>
<td><img alt = "Annotated Turtles" src="https://web.stanford.edu/~cgregg/EdgeMatch/turtle-annotated.png" style="width: 100%;"></td><br>
Annotated Turtles - the center turtle is the only one orientated in its original image form
</center>

Note that all lined-up turtles have the same uppercase and lowercase combination (A/a, B/b, C/c, or D/d). The only turtle in its original image rotation happens to be the center turtle. All others are rotated.

# Implementation Details

You will write code in the following two files. We will describe them in detail in this section.

1. **`Tile.cpp`**, the code for describing a tile.
2. **`edge-match.cpp`**, the code that runs the game and solves the puzzles.

We have provided two other classes for you:

1. **`PlayGame`**, a class that allows the user to play the game by clicking and dragging tiles. You will not have to interface with the `PlayGame` class, though you are welcome to investigate its methods.
2. **`Puzzle`**, a class that holds images and a `Grid` of tiles. You will have to interface with this in one function in `edge-match.cpp`.

Note that we have decomposed the program such that the the `Tile` class does not have information about images related to a puzzle, and simply defines a tile as described in [Puzzle Notation](#puzzle-notation). The `Puzzle` class has the (somewhat messy) image manipulation functions that are not necessary for the `Tile` class.

# `Tile.h` and `Tile.cpp`

As we described in [Puzzle Notation](#puzzle-notation), we have devised a way to describe a tile such that it can be used to solve puzzles. `Tile.h` and `Tile.cpp` are the files that define and implement a tile description in a C++ class, respectively. `Tile.h` has been commented heavily so that you know what each function is supposed to do, and we expand on that documentation here. We have written the _default_ constructor for the class for you, but you are responsible for writing the constructor that takes a `std::string` as a parameter, and you are also responsible for the five remaining functions (some are only one-liners). Although you are not required to do so, you may add _private_ functions and class variables to the class, but you _may not_ change the _public_ functions. Do not change any of the method signatures, either (i.e., don't change the parameters). In other words: someone else using the `Tile` class should be able to drop your `Tile.h` and `Tile.cpp` files into their own code and use it without any changes to their other code.

<table>
<tr><th>`Tile` method</th><th>Description</th></tr>
<tr>
    <td style="width: 40%;">`Tile()`</td>
    <td>The default constructor. We have written this for you. This creates a `Tile` instance with default values. You should not need to use the default constructor, but it is necessary so we can create a `Grid` of tiles.</td>
</tr>
<tr>
    <td>`Tile(string s)`</td>
    <td>A constructor that parses a string such as `A B a b 0` and produces a `Tile`. The orientation is optional, and if it is not present in the string (e.g., the string is simply something like `A B a b`, then you should set the orientation to 0.</td>
</tr>
<tr>
    <td>`bool isMatched(Tile &other, Connection otherLocation)`</td>
    <td>This method sees if the current `Tile` is properly matched with another `Tile`. This means that tiles are arranged such that the edge that is touching is a correct match. The location of the other tile is given relative to the current tile, using an `enum`, as defined in `Tile.h`. If you have never used an `enum` before, it is straightforward: `otherLocation` will have a value of any one of `Tile::ABOVE`, `Tile::RIGHT`, `Tile::BELOW`, or `Tile::LEFT`. You can use the values in an expression such as `if`\ `(otherLocation`\ `==`\ `Tile::ABOVE)`\ `{`. For more information about `enum`s in C++, see [here](https://en.cppreference.com/w/cpp/language/enum)</td>
</tr>
<tr>
    <td>`int getOrientation()`</td>
    <td>Returns the orientation of the tile.</td>
</tr>
<tr>
    <td>`void setOrientation(int orientation)`</td>
    <td>Sets the orientation of the tile.</td>
</tr>
<tr>
    <td>`string sidesStr() const`</td>
    <td>Returns a `string` representation of the sides. If the original tile had a representation of `A B a b 0`, then this function simply returns `ABab`. This is used by the `Puzzle` class to enter the tiles into a map with a `string` key.</td>
</tr>
<tr>
    <td>`displayTileStr()`</td>
    <td>Returns a string that is a display representation of a tile, with the correct orientation. For example, `A b a B 3` would return a string in the following form, with a newline after the "B":
```
  b
A   a
  B
```
</td>
</tr>
<tr>
    <td>`friend ostream &operator<<(ostream &out, Tile const &tile)`</td>
    <td>Overloads the `<<` operator to print out a representation of a tile, in the original form, but with the current orientation. E.g., `A`\ `B`\ `a`\ `b`\ `0`</td>
</tr>
<tr>
</tr>
<td>&nbsp;</td><td>&nbsp;</td>
<tr>
<th>`Tile` class variable</th><th>Description</th></tr>
<tr>
    <td>`int orientation`</td>
    <td>The orientation of a tile. `0` is the original image orientation, `1` is rotated clockwise 90 degrees, `2` is rotated clockwise 180 degrees, and `3` is rotated clockwise 270 degrees.</td>
</tr>
<tr>
    <td>`Vector<string> sides`</td>
    <td>A `Vector` that holds the string values (e.g., `A`). The `Vector` should hold the values in the order they appear the text representation, e.g., for `A B a b 0`, the `Vector` should have the values in the order `A` then `B` then `a` then `b`.</td>
</tr>
</table>

# `edge-match.cpp`
You have to write three functions in the `edge-match.cpp` file. There are other functions in the file that you can investigate, but you should not change any of them.

<table>
<tr><th>Function</th><th>Description</th></tr>
<tr>
    <td style="width: 40%;">`bool allMatch(Grid<Tile>& tiles`</td>
    <td>Determines if the tiles form a matching solution to the puzzle. `true` if the puzzle is solved, `false` otherwise.</td>
</tr>
<tr>
    <td>`void findAllSolutions(Vector<Tile>& tileVec, Grid<Tile>& tiles, int row,
                      int col, Vector<Grid<Tile>>& solutions)`</td>
    <td>This is the recursive puzzle solver function. The `tileVec` parameter is a `Vector` of tiles that you can use for backtracking. The `tiles` param is an initially blank `Grid` of tiles that you can populate with the tiles from `tileVec` to check whether your backtracking is headed towards a correct solution. The `row` and `col` parameters provide the current location, and the first time the function is called they will both be `0`. Finally, the `solutions` parameter is a `Vector` of `Grid` of tiles correct solutions that you should populate as you find correct solutions.</td>
</tr>
<tr>
    <td>`void displayAndSaveSolutions(Puzzle& puzzle, Vector<Grid<Tile>>& solutions)`</td>
    <td>This function loops through all of the solutions in the `solutions` `Vector`, prints and displays each one and saves the solution if the user chooses to do that. You will need to use the `puzzle` parameter, which is an instance of the `Puzzle` class, and contains images of the current puzzle. You should only need the `replaceGrid()` and `saveGrid()` functions from the `Puzzle` class.</td>
</tr>
</table>

# Development Strategy
Before you begin writing any code, check out the demo program, and make sure you understand how the program works.

The puzzle notation is important to understanding the functions in the `Tile` class, so spend some time understanding the notation. Think about what two matching tiles would look like next to each other (either vertically or horizontally).

It would be easiest to complete the `Tile` class before writing any other code. To test the code, you can uncomment the first line of `main`, which will run the `testTile()` function. The correct output for the function is as follows:
<pre class="codeblock">
t1 in display form:
  a
A   B
  b

t2 in display form:
  C
a   c
  B

t1: A a B b 3
t2: C c B a 0

orientation of t1: 3
t1 sideStr: AaBb

orientation of t2: 0
t2 sideStr: CcBa
setting t2 to have an orientation of 2
orientation of t2: 2
t2 sideStr: CcBa

t1: 
  a
A   B
  b

t2: 
  B
c   a
  C

Matches: 
if t2 is above t1: false
if t2 is below t1: true
if t2 is to the left of t1: true
if t2 is to the right of t1: false

if t1 is above t2: true
if t1 is below t2: false
if t1 is to the left of t2: false
if t1 is to the right of t2: true
</pre>

After completing the `Tile` class, work on the `allMatch` function in `edge-match.cpp`. If you load an already-completed puzzle and play manually, you can start to drag a piece and let go, and you should get a "You solved the game!" message immediately.

Next, work on the `findAllSolutions` function -- this is likely the most challenging function of the assignment.

Finally, you can complete `displayAndSaveSolutions`, which should not be too difficult.
