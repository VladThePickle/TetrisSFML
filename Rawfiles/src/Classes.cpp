// Classes.cpp
#define NOMINMAX
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <windows.h>
#include "randomness.h"
#include "Classes.h"
#include "Constants.h"

//----------------------- PIECE -----------------------------
sf::Texture piece::blocktexture;

bool piece::initResources() {
    if (!blocktexture.loadFromFile("Resources/block_texture.png")) {
        std::cerr << "Error: could not load block_texture.png\n";
        return false;
    }
    return true;
}

piece::piece()
    : x(latime / 2 - 1)
    , y(2)
    , rotation(0)
    , color(1)
    , piecetype('I')
    , blocksprite(blocktexture)
{
    blocksprite.setOrigin({
        blocktexture.getSize().x / 2.f,
        blocktexture.getSize().y / 2.f
        });
    blocksprite.setScale({ 0.15f,0.15f });
}

void piece::draw(sf::RenderTarget& target, float x, float y, int c) const {
    sf::Sprite s = blocksprite;
   s.setPosition({
       float(x * pixelsize + borderleft),
        float(y * pixelsize + bordertop)
        });
    
    s.setColor(COLORS[c]);
    target.draw(s);
}

// Setters
void piece::setx(int v) { x = v; }

void piece::sety(int v) { y = v; }

void piece::setrotation(int r) { rotation = r; }

void piece::setpiecetype(char t) { piecetype = t; }

void piece::setoffsets(const int offs[4][2]) {
    for (int i = 0;i < 4;i++)
        for (int j = 0;j < 2;j++)
            offsets[i][j] = offs[i][j];
}
void piece::setcolor(int c) { color = c; }

void piece::setBoard(board* b) { boardContext = b; }

// Getters
int piece::getx() { return x; }

int piece::gety() { return y; }

int piece::getrotation() { return rotation; }

char piece::getpiecetype() { return piecetype; }

int piece::getoffsets(int i, int j) { return offsets[i][j]; }

const int (*piece::getoffset())[2] { return offsets; }

int piece::getcolor() const { return color; }

// Actual functions
void piece::rotate() {
    if (boardContext == nullptr) return;

    boardContext->clearpiece();

    int newOffsets[4][2];
    for (int i = 0; i < 4; i++) {
        newOffsets[i][0] = offsets[i][1];
        newOffsets[i][1] = -offsets[i][0];
    }

    bool valid = true;
    for (int i = 0; i < 4; i++) {
        if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 1 || boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 4) {
            valid = false;
            break;
        }
        else if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 2)
        {
            while (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 2)
            {
                if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) != 1 && boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) != 4) {
                    x = x - 1;
                }
                else
                {
                    break;
                }
            }
        }
        else if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 3)
        {
            while (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 3)
            {
                if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) != 1 && boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) != 4) {
                    x = x + 1;
                }
                else
                {
                    break;
                }
            }
        }
    }

    if (valid) {
        for (int i = 0; i < 4; i++) {
            offsets[i][0] = newOffsets[i][0];
            offsets[i][1] = newOffsets[i][1];
        }
    }

    boardContext->update();
}

int piece::falldown() {
    if (!boardContext) return 0;
    boardContext->clearpiece();
    bool valid = true;
    for (int i = 0;i < 4;i++) {
        if (boardContext->validmove(y + offsets[i][0] + 1, x + offsets[i][1]) != 0) {
            valid = false; break;
        }
    }
    if (valid) y++;
    boardContext->update();
    return valid ? 1 : 2;
}

void piece::moveleft() {
    if (!boardContext) return;
    boardContext->clearpiece();
    bool valid = true;
    for (int i = 0;i < 4;i++) {
        if (boardContext->validmove(y + offsets[i][0], x + offsets[i][1] - 1) != 0) {
            valid = false; break;
        }
    }
    if (valid) x--;
    boardContext->update();
}

void piece::moveright() {
    if (!boardContext) return;
    boardContext->clearpiece();
    bool valid = true;
    for (int i = 0;i < 4;i++) {
        if (boardContext->validmove(y + offsets[i][0], x + offsets[i][1] + 1) != 0) {
            valid = false; break;
        }
    }
    if (valid) x++;
    boardContext->update();
}

//----------------------- BOARD -----------------------------
board::board()
    : piesaactiva(nullptr), urmatoareapiesa(nullptr), managerContext(nullptr)
{
    for (int i = 0;i < inaltime;i++)
        for (int j = 0;j < latime;j++) {
            ecran[i][j] = ' ';
            ecran_color[i][j] = 0;
        }

}

void board::setManager(gamemanager* m) { managerContext = m; }

piece* board::getpiesaactiva() { return piesaactiva; }

piece* board::geturmatoareapiesa() { return urmatoareapiesa; }

void board::setecran(char v, int y, int x) {
    if (y >= 0 && y < inaltime && x >= 0 && x < latime) 
        ecran[y][x] = v;
}

char board::getecran(int y, int x) {
    if (y >= 0 && y < inaltime && x >= 0 && x < latime) return ecran[y][x];
    return '\0';
}

void board::setCell(int r, int c, char v, int col) {
    if (r >= 0 && r < inaltime && c >= 0 && c < latime) {
        ecran[r][c] = v;
        ecran_color[r][c] = (v == cellval ? col : 0);
    }
}

void board::clearpiece() {
    if (!piesaactiva) return;
    for (int i = 0;i < 4;i++) {
        setecran(' ',
            piesaactiva->gety() + piesaactiva->getoffsets(i, 0),
            piesaactiva->getx() + piesaactiva->getoffsets(i, 1));
    }
}

void board::update() {
    if (!piesaactiva) return;
    for (int i = 0;i < 4;i++) {
        int y = piesaactiva->gety() + piesaactiva->getoffsets(i, 0);
        int x = piesaactiva->getx() + piesaactiva->getoffsets(i, 1);
        setCell(y, x, cellval, piesaactiva->getcolor());
    }
}

int board::validmove(int r, int c) {
    if (r < 0 || r >= inaltime) return 1;
    if (c < 0) return 3;
    if (c >= latime) return 2;
    return (getecran(r, c) != ' ') ? 4 : 0;
}

void board::draw(sf::RenderTarget& target) {
    for (int i = 3; i < inaltime; ++i) {
        for (int j = 0; j < latime; ++j) {
            if (ecran[i][j] == cellval) {
                sf::Sprite s(piece::blocktexture);
                s.setOrigin({
                   float(piece::blocktexture.getSize().x / 2.f),
                    float(piece::blocktexture.getSize().y / 2.f)
                    });
                s.setScale({ 0.15f, 0.15f });
                s.setPosition(
                    { float(j * pixelsize + borderleft),
                    float(i * pixelsize + bordertop)
                    });
                s.setColor(COLORS[ecran_color[i][j]]);
                target.draw(s);
            }
            else
            {
                sf::Sprite s(piece::blocktexture);
                s.setOrigin({
                   float(piece::blocktexture.getSize().x / 2.f),
                    float(piece::blocktexture.getSize().y / 2.f)
                    });
                s.setScale({ 0.15f, 0.15f });
                s.setPosition(
                    { float(j * pixelsize + borderleft),
                    float(i * pixelsize + bordertop)
                    });
                s.setColor(sf::Color(10, 10, 10));
                target.draw(s);
            }
        }
    }
}


bool board::isActivePieceCell(int y, int x) {
    if (!piesaactiva) return false;
    for (int i = 0;i < 4;i++) {
        if (piesaactiva->gety() + piesaactiva->getoffsets(i, 0) == y &&
            piesaactiva->getx() + piesaactiva->getoffsets(i, 1) == x)
            return true;
    }
    return false;
}

void board::drawNext(sf::RenderTarget& target) {
    if (!urmatoareapiesa) return;
    for (int i = 0; i < 4; ++i) {
        float py = inaltime / 2 - 1 + urmatoareapiesa->getoffsets(i, 0) + 1.2;
        float px = latime + 8 + urmatoareapiesa->getoffsets(i, 1);
        char ptype = urmatoareapiesa->getpiecetype();
        switch (ptype)
        {
            case 'I':
                px += 1;
                break;
            case 'O':
                px += 0.6f;
                py += 0.9f;
                break;
            case 'T':
                py += 1;
                break;
            case 'S':
                py += 1;
                break;
            case 'Z':
                py += 1;
                break;
            case 'J':
                px += 0.6f;
                py += 0.6f;
                break;
            case 'L':
                px += 0.6f;
                py += 0.6f;
                break;
        }
        urmatoareapiesa->draw(target, px, py, urmatoareapiesa->getcolor());
    }
}


void board::newpiece() {
    if (urmatoareapiesa != nullptr) {
        piesaactiva = urmatoareapiesa;
        urmatoareapiesa = nullptr;
    }
    else {
        piesaactiva = new piece();
        piesaactiva->setBoard(this);
        char type = getNextPiece();
        switch (type) {
        case 'I': piesaactiva->setoffsets(offsetI); piesaactiva->setcolor(1); piesaactiva->setpiecetype('I'); break;
        case 'O': piesaactiva->setoffsets(offsetO); piesaactiva->setcolor(2); piesaactiva->setpiecetype('O'); break;
        case 'T': piesaactiva->setoffsets(offsetT); piesaactiva->setcolor(3); piesaactiva->setpiecetype('T'); break;
        case 'S': piesaactiva->setoffsets(offsetS); piesaactiva->setcolor(4); piesaactiva->setpiecetype('S'); break;
        case 'Z': piesaactiva->setoffsets(offsetZ); piesaactiva->setcolor(5); piesaactiva->setpiecetype('Z'); break;
        case 'L': piesaactiva->setoffsets(offsetL); piesaactiva->setcolor(6); piesaactiva->setpiecetype('L'); break;
        case 'J': piesaactiva->setoffsets(offsetJ); piesaactiva->setcolor(7); piesaactiva->setpiecetype('J'); break;
        default: break;
        }
    }


    if (urmatoareapiesa != nullptr) {
        delete urmatoareapiesa;
        urmatoareapiesa = nullptr;
    }

    urmatoareapiesa = new piece();
    urmatoareapiesa->setBoard(this);
    char type2 = getNextPiece();
    switch (type2) {
    case 'I': urmatoareapiesa->setoffsets(offsetI);urmatoareapiesa->setcolor(1); urmatoareapiesa->setpiecetype('I'); break;
    case 'O': urmatoareapiesa->setoffsets(offsetO);urmatoareapiesa->setcolor(2); urmatoareapiesa->setpiecetype('O'); break;
    case 'T': urmatoareapiesa->setoffsets(offsetT);urmatoareapiesa->setcolor(3); urmatoareapiesa->setpiecetype('T'); break;
    case 'S': urmatoareapiesa->setoffsets(offsetS);urmatoareapiesa->setcolor(4); urmatoareapiesa->setpiecetype('S'); break;
    case 'Z': urmatoareapiesa->setoffsets(offsetZ);urmatoareapiesa->setcolor(5); urmatoareapiesa->setpiecetype('Z'); break;
    case 'L': urmatoareapiesa->setoffsets(offsetL);urmatoareapiesa->setcolor(6); urmatoareapiesa->setpiecetype('L'); break;
    case 'J': urmatoareapiesa->setoffsets(offsetJ);urmatoareapiesa->setcolor(7); urmatoareapiesa->setpiecetype('J'); break;
    default: break;
    }

    update();
}

bool board::checkclear() {
    bool hascleared = false;
    for (int i = inaltime - 1; i >= 0; i--) {
        bool full = true;

        for (int j = 0; j < latime; j++) {
            if (ecran[i][j] != cellval) {
                full = false;
                break;
            }
        }

        if (full) {
            hascleared = true;
            managerContext->setscore(managerContext->getscore() + 100);
            managerContext->checkscore();

            for (int k = i; k > 0; k--) {
                for (int j = 0; j < latime; j++) {
                    ecran[k][j] = ecran[k - 1][j];
                  ecran_color[k][j] = ecran_color[k - 1][j];
                }
            }

            Sleep(framerate);

            for (int j = 0; j < latime; j++) {
               
                ecran[0][j] = ' ';
               ecran_color[0][j] = 0;
            }
            i++;
        }
    }
    return hascleared;
}


bool board::checkloss() {
    for (int j = 0;j < latime;j++)
        if (ecran[3][j] == cellval) return true;
    return false;
}

void board::deletepiesaactiva() {
    delete piesaactiva;
    piesaactiva = nullptr;
}

board::~board() {
    delete piesaactiva;
    delete urmatoareapiesa;
}

static sf::Texture borderTex, nextTex;
static sf::Texture stscreenbg, bg, lsscreenbg;

//------------------------ GAMEMANAGER ----------------
gamemanager::gamemanager()
    : score(0), level(1), running(true), laberson(nullptr), w(nullptr),
    sprites{ sf::Sprite(borderTex), sf::Sprite(nextTex) },
    texts{ sf::Text(font), sf::Text(font), sf::Text(font), sf::Text(font), sf::Text(font), sf::Text(font), sf::Text(font) },
    screens{ sf::Sprite(stscreenbg), sf::Sprite(bg), sf::Sprite(lsscreenbg) }
{

    auto desktop = sf::VideoMode::getDesktopMode();
    w = new sf::RenderWindow(
        desktop, "tetris",
        sf::Style::Close, sf::State::Fullscreen,
        sf::ContextSettings{}
    );

    sf::View view(sf::FloatRect({ {0.f, 0.f}, {vwidth, vheight} }));
    view = getLetterboxView(view, desktop.size.x, desktop.size.y);
    w->setView(view);

    LoadFont(font, "Resources/PressStart2P-Regular.ttf");


    if (!stscreenbg.loadFromFile("Resources/startscreenbg.png"))
        std::cerr << "startscreen.png load failed\n";
    screens[0] = sf::Sprite(stscreenbg);
    if (!lsscreenbg.loadFromFile("Resources/lscreen.png"))
        std::cerr << "lscreen.png load failed";
    screens[2] = sf::Sprite(lsscreenbg);

    if (!borderTex.loadFromFile("Resources/border.png"))
        std::cerr << "border.png load failed\n";
    if (!nextTex.loadFromFile("Resources/nextpieceborder.png"))
        std::cerr << "nextpieceborder.png load failed\n";

    sprites[0] = sf::Sprite(borderTex);
    sprites[0].setPosition({ 800.f, 516.f });
    sprites[0].setOrigin({ 170.f, 360.f });
    sprites[0].setScale({ 1.5f, 1.5f });

    sprites[1] = sf::Sprite(nextTex);
    sprites[1].setPosition({ 1500.f, 600.f });
    sprites[1].setOrigin({ 120.f, 120.f });

    texts[0].setString("Level: " + std::to_string(level));
    texts[1].setString("Score: " + std::to_string(score));
    texts[2].setString("Next:");
    texts[3].setString("Press Up Arrow To Start");
    texts[4].setString("laberson 2025");
    texts[5].setString("Press Up Arrow To Get Back To The Main Menu");
    texts[6].setString("Press Down Arrow To Quit The Game (skill issue)");

    texts[0].setOrigin(texts[0].getGlobalBounds().size / 2.0f);
    texts[1].setOrigin(texts[1].getGlobalBounds().size / 2.0f);
    texts[2].setOrigin(texts[2].getGlobalBounds().size / 2.0f);
    texts[3].setOrigin(texts[3].getGlobalBounds().size / 2.0f);
    texts[4].setOrigin(texts[4].getGlobalBounds().size / 2.0f);
    texts[5].setOrigin(texts[5].getGlobalBounds().size / 2.0f);
    texts[6].setOrigin(texts[6].getGlobalBounds().size / 2.0f);

    texts[0].setPosition({ 1485.0f, 200.0f });
    texts[1].setPosition({ 1485.0f, 300.0f });
    texts[2].setPosition({ 1500.0f, 450.0f });
    texts[3].setPosition({ w->getSize().x / 2.0f - 200, w->getSize().y / 2.0f + 300 });
    texts[4].setPosition({ w->getSize().x / 2.0f - 80, w->getSize().y / 2.0f + 462 });
    texts[4].setScale({ 0.75f, 0.75f });
    texts[5].setPosition({ w->getSize().x / 2.0f, w->getSize().y / 2.0f + 100});
    texts[6].setPosition({ w->getSize().x / 2.0f, w->getSize().y / 2.0f + 200 });
    texts[5].setScale({ 0.7f, 0.7f });
    texts[6].setScale({ 0.7f, 0.7f });


    screens[0].setOrigin(screens[0].getGlobalBounds().size / 2.0f);
    screens[0].setPosition({ w->getSize().x / 2.0f, w->getSize().y / 2.0f });
    screens[0].setScale({ 0.7f, 0.75f });
    
    screens[2].setOrigin(screens[2].getGlobalBounds().size / 2.0f);
    screens[2].setPosition({ w->getSize().x / 2.0f, w->getSize().y / 2.0f });
    screens[2].setScale({ 0.7f, 0.75f });
}

void gamemanager::mainloop() {
    static unsigned long lastUpdate = 0;
    static int coyoteCount = 0;

    if (laberson == nullptr) {   
        laberson = new board;
        laberson->setManager(this);
        laberson->newpiece();

        lastUpdate = GetTickCount();
    }

    while (auto ev = getwindow().pollEvent()) {

        if (ev->is<sf::Event::Closed>())
        {
            getwindow().close();
          
            running = false;
            return;
        }
        else if (auto* kp = ev->getIf<sf::Event::KeyPressed>())
        {
            if (kp->code == sf::Keyboard::Key::Escape)
            {
                getwindow().close();
                running = false;  
                return;
            }
            else if (kp->code == sf::Keyboard::Key::Up)
            {
                if (laberson->getpiesaactiva())
                    laberson->getpiesaactiva()->rotate();
            }
            else if (kp->code == sf::Keyboard::Key::Left)
            {
                if (laberson->getpiesaactiva())
                    laberson->getpiesaactiva()->moveleft();
            }
            else if (kp->code == sf::Keyboard::Key::Right)
            {
                if (laberson->getpiesaactiva())
                    laberson->getpiesaactiva()->moveright();
            }
        }
    }
        unsigned long now = GetTickCount();
        int speedMultiplier = (GetAsyncKeyState(VK_DOWN) & 0x8000) ? 5 : 1;
        double delay = (20 - 2 * level) * framerate;
        if (delay < framerate * 2) delay = framerate * 2; 

        if (now - lastUpdate >= delay / speedMultiplier) {
            if (laberson->getpiesaactiva()->falldown() == 2) {
                if (++coyoteCount > coyoteticks) {
                    laberson->deletepiesaactiva();
                    if (laberson->checkloss()) {
                        deleteboard();
                        score = 0;
                        level = 1;
                        ag = 1;
                        texts[0].setString("Level: " + std::to_string(level));
                        texts[1].setString("Score: " + std::to_string(score));
                        running = false;
                        return;  
                    }
                    if (laberson->checkclear())
                    {
                        texts[0].setString("Level: " + std::to_string(level));
                        texts[1].setString("Score: " + std::to_string(score));
                    }
                    laberson->newpiece();
                    coyoteCount = 0;
                }
            }
            else {
                coyoteCount = 0;
            }
            lastUpdate = now;
        }

        w->clear();

        laberson->draw(*w);

        laberson->drawNext(*w);

        w->draw(sprites[0]);
        w->draw(sprites[1]);
        w->draw(texts[0]);
        w->draw(texts[1]);
        w->draw(texts[2]);

        w->display();
    }

sf::View gamemanager::getLetterboxView(sf::View view, int windowWidth, int windowHeight)
{
    float windowRatio = windowWidth / static_cast<float>(windowHeight);
    float viewRatio = view.getSize().x / view.getSize().y;
    float sizeX = 1.0f;
    float sizeY = 1.0f;
    float posX = 0.0f;
    float posY = 0.0f;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    if (horizontalSpacing) {
        sizeX = viewRatio / windowRatio;
        posX = (1.0f - sizeX) / 2.0f;
    }
    else {
        sizeY = windowRatio / viewRatio;
        posY = (1.0f - sizeY) / 2.0f;
    }

    view.setViewport(sf::FloatRect({ { posX, posY}, {sizeX, sizeY } }));
    return view;
}



void gamemanager::setscore(int s) { score = s; }

void gamemanager::LoadFont(sf::Font& font, std::string str)
{
    if (!font.openFromFile(str))
    {
        std::cerr << "ERROR: Couldn't load font";
    }
}

int  gamemanager::getscore()  { return score; }

int  gamemanager::getlevel()  { return level; }

bool gamemanager::getrunning()  { return running; }

void gamemanager::checkscore() {
    if (score % nextscore == 0) ++level;
}

void gamemanager::deleteboard() {
    delete laberson; laberson = nullptr;
}

gamemanager::~gamemanager() {
    delete laberson;
    delete w;
}

sf::RenderWindow& gamemanager::getwindow() {
    return *w;
}

board* gamemanager::getboard() {
    return laberson;
}

sf::Font gamemanager::getfont()
{
    return font;
}



void gamemanager::losescreen()
{
    bool run = true;
    while (run && getwindow().isOpen())
    {
        while (auto ev = getwindow().pollEvent()) {

            if (ev->is<sf::Event::Closed>())
            {
                getwindow().close();
                run = false;
                running = false;
            }
            else if (auto* kp = ev->getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Escape)
                {
                    getwindow().close();
                    run = false;
                    running = false;
                }
                else if (kp->code == sf::Keyboard::Key::Up)
                {
                    run = false;
                }
                else if (kp->code == sf::Keyboard::Key::Down)
                {
                    getwindow().close();
                    run = false;
                    running = false;
                }
            }
        }

        w->clear();
        w->draw(screens[2]);
        w->draw(texts[5]);
        w->draw(texts[6]);
        w->display();
    }
    w->clear();
}

void gamemanager::startscreen()
{
   
    bool run = true;
    while (run && getwindow().isOpen())
    {
        while (auto ev = getwindow().pollEvent()) {

            if (ev->is<sf::Event::Closed>())
            {
                getwindow().close();
                run = false;
                running = false;
            }
            else if (auto* kp = ev->getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Escape)
                {
                    getwindow().close();
                    run = false;
                    running = false;
                }
                else if (kp->code == sf::Keyboard::Key::Up)
                {
                    run = false;
                }
               
            }
        }
       
        w->clear();
        w->draw(screens[0]);
        w->draw(texts[3]);
        w->draw(texts[4]);
        w->display();
    }
    running = true;
    w->clear();
}