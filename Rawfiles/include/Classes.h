#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Constants.h"

class board;
class gamemanager;

class piece {
private:
    int x, y, rotation, color;
    char piecetype;
    int offsets[4][2];
    board* boardContext;
    sf::Sprite blocksprite;
public:
    static sf::Texture blocktexture;

    piece();

    //sfml functions
    static bool initResources();
    void draw(sf::RenderTarget& target, float x, float y, int c) const;

    //seters
    void setx(int); void sety(int); void setrotation(int);
    void setpiecetype(char); void setoffsets(const int[4][2]);
    void setcolor(int); void setBoard(board*);

    //getters
    int getx(); int gety(); int getrotation();
    char getpiecetype(); int getoffsets(int, int);
    const int (*getoffset())[2]; int getcolor() const;

    //Actual functions
    void rotate(); int falldown(); void moveleft(); void moveright();
};

class board {
private:
    char ecran[inaltime][latime];
    int ecran_color[inaltime][latime];
    piece* piesaactiva;
    piece* urmatoareapiesa;
    gamemanager* managerContext;
public:
    board();

    //setters
    void setecran(char, int, int); void setCell(int, int, char, int);
    void setManager(gamemanager*);


    //getters
    char getecran(int, int);
    piece* getpiesaactiva(); piece* geturmatoareapiesa();

    //Actual functions
    void drawNext(sf::RenderTarget& target);
    void draw(sf::RenderTarget& target);
    void printecran(); void printurmatoare();
    void newpiece(); int validmove(int, int);
    void clearpiece(); void update();
    bool isActivePieceCell(int, int);
    bool checkclear(); bool checkloss();
    
    void deletepiesaactiva();
    ~board();
};

class gamemanager {
private:
    int score{ 0 }, level{ 1 };
    double ag{ 1 };
    bool running{ true };
    board* laberson{ nullptr };
    sf::RenderWindow* w;
    sf::Font font;
    sf::Sprite sprites[2];
    sf::Sprite screens[3];
    sf::Text texts[7];
public:
    gamemanager();

    //setters
    void setscore(int); void LoadFont(sf::Font& font, std::string str);

    //getters
    sf::RenderWindow& getwindow();
    int getscore();
    int getlevel(); bool getrunning();
    board* getboard(); sf::Font getfont();

    //Actual functions
    void mainloop();
    sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight);
    void checkscore(); void deleteboard();

    //Screens
    void losescreen(); void startscreen();

    ~gamemanager();
};
