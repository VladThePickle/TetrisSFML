#pragma once
#include <vector>

const int framerate = 42;
const int inaltime = 23;
const int vwidth = 1920;
const int vheight = 1080;
const int latime = 10;
const double tickspeed = 7;
const int nextscore = 500;
const int coyoteticks = 1;

const char cellval = '#';
const float pixelsize = 320 * 0.15;
const float borderleft = 605.75 -20.5;
const float bordertop = -32;
static const sf::Color COLORS[8] = {
    sf::Color::Transparent,
    sf::Color(50, 230, 230),    // I 
    sf::Color(255, 240, 80),    // O
    sf::Color(190, 80, 190),    // T 
    sf::Color(80, 240, 80),     // S 
    sf::Color(240, 70, 70),     // Z 
    sf::Color(80, 80, 255),     // J 
    sf::Color(255, 140, 40)     // L 
};
// FORME
// I

const int offsetI[4][2] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};

// O

const int offsetO[4][2] = { {0,0}, {0, 1}, {1, 0}, {1, 1} };

// T 

const int offsetT[4][2] = { {0,0}, {0,1}, {0, 2}, {1,1} };

// S

const int offsetS[4][2] = { {0, 1}, {0, 2}, {1, 0}, {1, 1} };

// Z 

const int offsetZ[4][2] = {{0, 0}, {0, 1}, {1,1}, {1,2}};

// J 

const int offsetL[4][2] = { {0,0}, {1,0}, {2,0}, {2,1} };

// L 

const int offsetJ[4][2] = { {0,1}, {1,1}, {2, 0}, {2,1} };