the TIMES-TABLES
================

the TIMES-TABLES is a little game to practise the times-tables.
It is written in c++. It uses openGL & openAL. It is not intented to be of serious educational usage, but if someone 
experiences an improvement in their multiplication-skills, then I will be delighted with finding out, what you mean by that.
The game is written in 'artist'-mode. That means that, at first, I did not have a clue about how the game would look & play.
Everything is made on the fly and very little was planned. So a lot of code might not be very efficient or messy, but it 
works, so I'm happy with the end-result. The focus was not on the code, but on the outcome :-)
There might be improvements in the future. But since this is a hobby-project I don't know when.

some more:
The game is inpired by the MSX-1, the 8bit homecomputer of the 80's. The idea of the game came when I made an opengl-demo
to print the characterset of the MSX, made from 3D-cubes. Before that i had found an old audio cassette from 1986. With an old
walkman I recorded its contents with audacity and normalized it, to be fed back(with nails & crocodile-clamps) to the audio-input of the original MSX1 that we
owned in the 80's. I managed to load all basic-programs that were on the cassette. One of it was a small basic-program to learn the times-tables, made by my father.


USAGE:
--------
* press L to cycle through languages
* press ESC to go back. Also from gameplay. (WARNING you will NOT be asked if sure or not!)
* press F to toggle fullscreen.
* press ENTER to confirm answer
* press SPACE to START/SELECT/PROCEED


Gameplay:
choose gamemode:
At the title-menu-screen, first choose the game mode with RIGHT/LEFT/UP/DOWN-keys.
Game modes are:
* Successive: choose a table to start from. After 10 (correct or incorrect) answers you warp to the next table. For example:
if you choose to start from table 5, you will proceed to the table of 6.
* Single: practice a single table, choose with UP/DOWN. after 5 correct answers, you 'warp', and the difficulty increases
* random: You will be asked randomly, all the tables from 1 to 10.. after 5 correct answers, you 'warp', and the difficulty increases.

.. after confirming with SPACE, you can set the difficulty, with UP&DOWN-keys, choose the speed at which the cyphers are falling.
press space to start playing.
First the question will fly into place, after that the cyphers start falling, at a speed depending on the difficulty-value
(displayed as 'speed': in the top). You have to type quickly, and confirm the answer with ENTER, otherwise the cyphers will
fall into the all-destructing laser-beam.. and you will loose a life.
When an answer is icorrect, points will be given. The amount of points gained with each correct answer, is based on difficulty.
The game has no end. The goal is to gather as much pints as possible.
Good Luck & Have fun!


Known bugs:
* when ESC is pressed during warp to next level, the game goes back to title but the stars remain in 'hyperdrivemode'. I like it 
so I decided to keep it, but this 'feature' might dissapear in the future
* The text does not auto-scale, especially the title might be cropped in certain languages or window-dimensions.

warning:
Maybe the warp-effect for reaching next level, is too epileptic for some. I have only tested the game on my own children
and none of them became ill.


INSTALLATION:
------------

Binaries
downdoad the binary releases here:
http:\\sdfasdfasdf

BUILD:
linux:
first you need these dependencies:
- libxcursor
- libxineram
- pulseaudio

on ubuntu:
`sudo apt update; sudo apt install libxinerama libxcursor libpulseaudio`
.. and maybe som eother things I have to find out
on fedora/centos:
`coming soon...`


`git clone sdfasfdsa --recurse-submodules`
`git submodule update --init --recursive`
`cd timestables`
`mkdir build`
`cd build`
`cmake ../`
`make` (or `make -j4` to build faster)

for debugging(when changing the code yourself), instead of `cmake ../`, use:
`cmake -DCMAKE_BUILD_TYPE=Debug ../`. This way you can make use the IMGui-api for testing stuff.

Run the game by typing:
`./tables`

In case of audioproblems(error-messages and/or not hearing any..), start the game with:
`padsp ./tables`

windows:
`git clone sdfasfdsa --recurse-submodules`
with cmake you can generate a solution, and load it with VisualStudio. I will add the solution later.


Made by KG-Soft 2020
DISCLAIMER: if something or someone gets damaged, then I am not responsible.

