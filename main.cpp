#include "board.h"
#include "tile.h"
#include "UI.h"
#include "extra.h"
#include "timer.h"
#include "bullet.h"
#include "bullethell.h"
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cctype>


int main() {
    srand(time(0));
    std::ifstream config("files/config.cfg");
    std::string leaderboard[5];
    int colCount, rowCount, mineCount;
    config >> colCount >> rowCount >> mineCount;
    int width = colCount*32;
    int height = rowCount*32+100;
    int displayedMineCount = mineCount;
    std::string name;
    bool godModePrompted = false;
    bool godModeTextFlag = false;
    int difficulty = 1; // 0 easy 1 normal 2 hard 3 impossible
    
    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    sf::Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    setText(welcomeText, width/2,height/2 - 150);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text welcomePrompt("Enter your name:", font, 20);
    setText(welcomePrompt, width/2,height/2 - 75);
    welcomePrompt.setStyle(sf::Text::Bold);

    sf::Text welcomeInput("|", font, 18);
    setText(welcomeInput, width/2,height/2 - 45);
    welcomeInput.setStyle(sf::Text::Bold);
    welcomeInput.setFillColor(sf::Color::Yellow);

    sf::Text welcomeBulletHellText("BULLET HELL MODE", font, 20);
    welcomeBulletHellText.setPosition(40,5);
    welcomeBulletHellText.setStyle(sf::Text::Bold);
    sf::RectangleShape bulletHellButton;
    bulletHellButton.setSize(sf::Vector2f(30.0f,30.0f));
    bulletHellButton.setPosition(5,5);
    bulletHellButton.setFillColor(sf::Color::Red);

    sf::Text welcomeGodModeText("GOD MODE", font, 20);
    welcomeGodModeText.setPosition(40,40);
    welcomeGodModeText.setStyle(sf::Text::Bold);
    sf::RectangleShape godModeButton;
    godModeButton.setSize(sf::Vector2f(30.0f,30.0f));
    godModeButton.setPosition(5,40);
    godModeButton.setFillColor(sf::Color::Red);

    sf::Text difficultyText("Difficulty:", font, 20);
    difficultyText.setPosition(5,75);
    difficultyText.setStyle(sf::Text::Bold);

    sf::Text easyText("Easy", font, 20);
    easyText.setPosition(40,110);
    easyText.setStyle(sf::Text::Bold);
    sf::RectangleShape easyButton;
    easyButton.setSize(sf::Vector2f(30.0f,30.0f));
    easyButton.setPosition(5,110);
    easyButton.setFillColor(sf::Color::Red);
    sf::Text normalText("Normal", font, 20);
    normalText.setPosition(40,145);
    normalText.setStyle(sf::Text::Bold);
    sf::RectangleShape normalButton;
    normalButton.setSize(sf::Vector2f(30.0f,30.0f));
    normalButton.setPosition(5,145);
    normalButton.setFillColor(sf::Color::Green);
    sf::Text hardText("Hard", font, 20);
    hardText.setPosition(40,180);
    hardText.setStyle(sf::Text::Bold);
    sf::RectangleShape hardButton;
    hardButton.setSize(sf::Vector2f(30.0f,30.0f));
    hardButton.setPosition(5,180);
    hardButton.setFillColor(sf::Color::Red);
    sf::Text impossibleText("IMPOSSIBLE", font, 20);
    impossibleText.setPosition(40,215);
    impossibleText.setStyle(sf::Text::Bold);
    sf::RectangleShape impossibleButton;
    impossibleButton.setSize(sf::Vector2f(30.0f,30.0f));
    impossibleButton.setPosition(5,215);
    impossibleButton.setFillColor(sf::Color::Red);

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while (welcomeWindow.pollEvent(event)) {
            switch (event.type)
            {
                case sf::Event::Closed:
                    welcomeWindow.close();
                    return 0;
                case sf::Event::TextEntered:
                    // backspace
                    if (event.text.unicode == 8 && !name.empty()) {
                        name.pop_back();
                        welcomeInput.setString(name + "|");
                        welcomeInput.setOrigin(welcomeInput.getLocalBounds().width / 2, welcomeInput.getLocalBounds().height / 2);
                    } else if (((event.text.unicode >= 65 && event.text.unicode <= 90) || (event.text.unicode >= 97 && event.text.unicode <= 122)) && name.length() < 10) {
                        if (event.text.unicode <= 90 && !name.empty()) {
                            name += static_cast<char>(event.text.unicode + 32);
                        } else if (event.text.unicode >= 97 && name.empty()) { 
                            name += static_cast<char>(event.text.unicode - 32);
                        } else {
                            name += static_cast<char>(event.text.unicode);
                        }
                        welcomeInput.setString(name + "|");
                        welcomeInput.setOrigin(welcomeInput.getLocalBounds().width / 2, welcomeInput.getLocalBounds().height / 2);
                    // enter
                    } else if (event.text.unicode == 13) {
                        if (!name.empty()) {
                            welcomeWindow.close();
                        }
                        if (BulletHell::isEnabled()) {
                            BulletHell::restart();
                        }
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (event.mouseButton.x > 5 && event.mouseButton.x < 35 && event.mouseButton.y > 5 && event.mouseButton.y < 35) {
                            BulletHell::toggle();
                            switch (BulletHell::isEnabled()) {
                                case true:
                                    bulletHellButton.setFillColor(sf::Color::Green);
                                    break;
                                case false:
                                    bulletHellButton.setFillColor(sf::Color::Red);
                                    break;
                            }
                        } else if (event.mouseButton.x > 5 && event.mouseButton.x < 35 && event.mouseButton.y > 40 && event.mouseButton.y < 70) {
                            BulletHell::toggleGodMode();
                            switch (BulletHell::godModeIsEnabled()) {
                                case true:
                                    godModeButton.setFillColor(sf::Color::Green);
                                    godModeTextFlag = true;
                                    break;
                                case false:
                                    godModeButton.setFillColor(sf::Color::Red);
                                    godModeTextFlag = false;
                                    break;
                            }
                        } else if (event.mouseButton.x > 5 && event.mouseButton.x < 35 && event.mouseButton.y > 110 && event.mouseButton.y < 140) {
                            BulletHell::setAttackDelay(2.0f);
                            Bullet::setBulletSpeed(0.6f);
                            easyButton.setFillColor(sf::Color::Green);
                            normalButton.setFillColor(sf::Color::Red);
                            hardButton.setFillColor(sf::Color::Red);
                            impossibleButton.setFillColor(sf::Color::Red);
                        } else if (event.mouseButton.x > 5 && event.mouseButton.x < 35 && event.mouseButton.y > 145 && event.mouseButton.y < 175) {
                            BulletHell::setAttackDelay(1.5f);
                            Bullet::setBulletSpeed(0.8f);
                            easyButton.setFillColor(sf::Color::Red);
                            normalButton.setFillColor(sf::Color::Green);
                            hardButton.setFillColor(sf::Color::Red);
                            impossibleButton.setFillColor(sf::Color::Red);
                        } else if (event.mouseButton.x > 5 && event.mouseButton.x < 35 && event.mouseButton.y > 180 && event.mouseButton.y < 210) {
                            BulletHell::setAttackDelay(1.0f);
                            Bullet::setBulletSpeed(1.0f);
                            easyButton.setFillColor(sf::Color::Red);
                            normalButton.setFillColor(sf::Color::Red);
                            hardButton.setFillColor(sf::Color::Green);
                            impossibleButton.setFillColor(sf::Color::Red);
                        } else if (event.mouseButton.x > 5 && event.mouseButton.x < 35 && event.mouseButton.y > 215 && event.mouseButton.y < 245) {
                            BulletHell::setAttackDelay(0.5f);
                            Bullet::setBulletSpeed(1.5f);
                            easyButton.setFillColor(sf::Color::Red);
                            normalButton.setFillColor(sf::Color::Red);
                            hardButton.setFillColor(sf::Color::Red);
                            impossibleButton.setFillColor(sf::Color::Green);
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcomeText);
        welcomeWindow.draw(welcomePrompt);
        welcomeWindow.draw(welcomeInput);
        welcomeWindow.draw(bulletHellButton);
        welcomeWindow.draw(welcomeBulletHellText);
        if (BulletHell::isEnabled()) {
            welcomeWindow.draw(welcomeGodModeText);
            welcomeWindow.draw(godModeButton);
            welcomeWindow.draw(difficultyText);
            welcomeWindow.draw(easyText);
            welcomeWindow.draw(easyButton);
            welcomeWindow.draw(normalText);
            welcomeWindow.draw(normalButton);
            welcomeWindow.draw(hardText);
            welcomeWindow.draw(hardButton);
            welcomeWindow.draw(impossibleText);
            welcomeWindow.draw(impossibleButton);
        }
        welcomeWindow.display();
    }

    if (BulletHell::isEnabled()) {
        Bullet::loadBulletTexture();
        BulletHell::loadWindowSize(width,height);
        BulletHell::initializeSound();
    }

    //sf::RenderWindow gameWindow(sf::VideoMode::getDesktopMode(), "Minesweeper", sf::Style::None);
    sf::RenderWindow gameWindow(sf::VideoMode(colCount*32, rowCount*32+100), "Game Window", sf::Style::Close);
    gameWindow.setVerticalSyncEnabled(true);
    gameWindow.setFramerateLimit(60);
    
    Tile::loadTextures();
    UI::loadTextures();
    UI::setupSprites(colCount, rowCount);
    UI::setFaceSprite(0);
    UI::setPausePlaySprite(1);
    UI::setCounterSprite(displayedMineCount);
    UI::setTimerSprite(7, 27);

    Board board(colCount, rowCount, mineCount);
    board.fillGrid();
    board.placeMines();
    board.fillTileNeighbors();
    board.setCounts();
    //board.revealAll();

    Timer timer;
    sf::Clock clock;

    timer.startTimer();

    std::vector<std::vector<Tile>>& grid = board.getGrid();
    std::vector<Bullet>& bullets = BulletHell::getBullets();

    sf::Texture deathTexture;
    if (Bullet::gradescopequestionmark()) {
        deathTexture.loadFromFile("files/images/face_lose.png");
    } else {
        deathTexture.loadFromFile("files/images/death.png");
    }
    sf::Sprite deathSprite(deathTexture);
    deathSprite.setOrigin(16,16);
    if (Bullet::gradescopequestionmark()) {
        deathSprite.setScale(0.5f,0.5f);
        deathSprite.setOrigin(32,32);
    }
    sf::Vector2f prevMousePos = sf::Vector2f(sf::Mouse::getPosition(gameWindow));
    sf::Text deathCountText("Death count: 0", font, 20);
    deathCountText.setStyle(sf::Text::Bold);
    deathCountText.setFillColor(sf::Color::Black);
    deathCountText.setPosition(120,32*(rowCount+0.5)+16);
    sf::Text godModePrompt("Enable Godmode?", font, 20);
    godModePrompt.setStyle(sf::Text::Bold);
    godModePrompt.setFillColor(sf::Color::Black);
    godModePrompt.setPosition(120,32*(rowCount+0.5));
    sf::RectangleShape godModePromptButton;
    godModePromptButton.setSize(sf::Vector2f(30.0f,30.0f));
    godModePromptButton.setPosition(200,32*(rowCount+0.5)+32);
    godModePromptButton.setFillColor(sf::Color::Red);
    while(gameWindow.isOpen()) {
        float dt = clock.restart().asSeconds();
        UI::setCounterSprite(displayedMineCount);
        if (board.getGameOver() && board.checkWin()) {
            UI::setCounterSprite(0);
        }
        UI::setTimerSprite(timer.getElapsedMinutes(), timer.getElapsedSeconds() % 60);
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            switch (event.type)
            {
                case sf::Event::Closed:
                    gameWindow.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Right:
                            if (!board.getGameOver() && !board.getPaused()) {
                                if (event.mouseButton.y < rowCount*32) {
                                    if (!grid[event.mouseButton.x/32][event.mouseButton.y/32].getIsRevealed()) {
                                        if (grid[event.mouseButton.x/32][event.mouseButton.y/32].getIsFlagged()) {
                                            board.unFlagTile(event.mouseButton.x/32,event.mouseButton.y/32);
                                            displayedMineCount++;
                                        } else {
                                            board.flagTile(event.mouseButton.x/32,event.mouseButton.y/32);
                                            displayedMineCount--;
                                        }
                                    }
                                }
                            }
                            break;
                        case sf::Mouse::Left:
                        if (!board.getGameOver() && !board.getPaused()) {
                            if (event.mouseButton.y < rowCount*32) {
                                if (!grid[event.mouseButton.x/32][event.mouseButton.y/32].getIsFlagged()) {
                                    board.revealTile(event.mouseButton.x/32,event.mouseButton.y/32);
                                    if (grid[event.mouseButton.x/32][event.mouseButton.y/32].getIsMine()) {
                                        board.toggleGameOver();
                                        UI::setFaceSprite(2);
                                        board.revealAllMines();
                                        timer.pauseTimer();
                                        deathSprite.setPosition(-100,-100);
                                    } else if (board.checkWin()) {
                                        UI::setCounterSprite(0);
                                        board.toggleGameOver();
                                        UI::setFaceSprite(1);
                                        board.flagAllMines();
                                        timer.pauseTimer();
                                        for (int i = 0; i < colCount; i++) {
                                            for (int j = 0; j < rowCount; j++) {
                                                gameWindow.draw(grid[i][j].getRevealedSprite());
                                                if (grid[i][j].getIsRevealed() || grid[i][j].getIsFlagged()) {
                                                    gameWindow.draw(grid[i][j].getSprite());
                                                }
                                            }
                                        }
                                        for (int k = 0; k < UI::getSprites().size(); k++) {
                                            gameWindow.draw(UI::getSprites()[k]);
                                        }
                                        gameWindow.display();
                                        int playerMinutes = timer.getElapsedMinutes();
                                        int playerSeconds = timer.getElapsedSeconds() % 60;
                                        std::ifstream leaderboardFile("files/leaderboard.txt");
                                        for (int i = 0; i < 5; i++) {
                                            std::getline(leaderboardFile, leaderboard[i]);
                                            if (!leaderboard[i].empty() && leaderboard[i].back() == '*') leaderboard[i].pop_back();
                                        }
                                        
                                        bool bruh = false;
                                        for (int i = 0; i < 5; i++) {
                                            if (playerMinutes < std::stoi(leaderboard[i].substr(0,2)) || (playerMinutes == std::stoi(leaderboard[i].substr(0,2)) && playerSeconds < std::stoi(leaderboard[i].substr(3,2)))) {
                                                std::ofstream leaderboardWrite("files/leaderboard.txt", std::ios::out);
                                                for (int j = 4; j > i; j--) {
                                                    leaderboard[j] = leaderboard[j-1];
                                                }
                                                leaderboard[i] = "";
                                                if (playerMinutes < 10) leaderboard[i] += '0';
                                                leaderboard[i] += std::to_string(playerMinutes) + ':';
                                                if (playerSeconds < 10) leaderboard[i] += '0';
                                                leaderboard[i] += std::to_string(playerSeconds) + ", " + name;
                                                for (int i = 0; i < 5; i++) {
                                                    leaderboardWrite << leaderboard[i];
                                                    if (i < 4) leaderboardWrite << std::endl;
                                                }
                                                leaderboard[i] += '*';
                                                break;
                                            }
                                        }
                                        gameWindow.setActive(false);
                                        sf::RenderWindow leaderboardWindow(sf::VideoMode(colCount*16, rowCount*16+50), "Leaderboard Window", sf::Style::Close);
                                        leaderboardWindow.requestFocus();
            
                                        sf::Text leaderboardText("LEADERBOARD", font, 20);
                                        setText(leaderboardText, width/4,height/4 - 120);
                                        leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
            
                                        std::string l;
                                        for (int i = 0; i < 5; i++) {
                                            l += std::to_string(i+1) + ".\t" + leaderboard[i].substr(0,5) + '\t' + leaderboard[i].substr(7) + "\n\n";
                                        }
                                        sf::Text leaderboardContent(l, font, 18);
                                        setText(leaderboardContent, width/4,height/4 + 20);
                                        leaderboardContent.setStyle(sf::Text::Bold);
            
                                        while (leaderboardWindow.isOpen()) {
                                            sf::Event event;
                                            while (leaderboardWindow.pollEvent(event)) {
                                                switch (event.type)
                                                {
                                                    case sf::Event::Closed:
                                                        leaderboardWindow.close();
                                                        gameWindow.setActive(true);
                                                        break;
                                                    default:
                                                        break;
                                                }
                                            }
                                            leaderboardWindow.clear(sf::Color::Blue);
                                            leaderboardWindow.draw(leaderboardText);
                                            leaderboardWindow.draw(leaderboardContent);
                                            leaderboardWindow.display();
                                        }
                                        sf::Event flush; while (gameWindow.pollEvent(flush)) {}
                                        break;
                                    }
                                }
                            }
                        }
                        // debug
                        if (event.mouseButton.x > colCount*32-304 && event.mouseButton.x < colCount*32-304+64 && event.mouseButton.y > 32*(rowCount+0.5) && event.mouseButton.y < 32*(rowCount+0.5)+64) {
                            if (!board.getGameOver() && !board.getPaused() && !board.checkWin()) {
                                board.toggleDebug();
                            }
                            
                        // face button
                        } else if (event.mouseButton.x > colCount/2.0*32-32 && event.mouseButton.x < colCount/2.0*32-32+64 && event.mouseButton.y > 32*(rowCount+0.5) && event.mouseButton.y < 32*(rowCount+0.5)+64) {
                            if (board.getGameOver()) {
                                board.toggleGameOver();
                            }
                            board.restart();
                            UI::setFaceSprite(0);
                            displayedMineCount = mineCount;
                            timer.resetTimer();
                            timer.startTimer();
                            UI::setCounterSprite(displayedMineCount);
                            if (BulletHell::isEnabled()) {
                                BulletHell::restart();
                            }
                            if (godModePrompted) {
                                godModeTextFlag = true;
                            }
                            
                        // pause button
                        } else if (event.mouseButton.x > colCount*32-240 && event.mouseButton.x < colCount*32-240+64 && event.mouseButton.y > 32*(rowCount+0.5) && event.mouseButton.y < 32*(rowCount+0.5)+64) {
                            if (!board.getGameOver() && !BulletHell::isEnabled()) {
                                board.togglePaused();
                                if (board.getPaused()) {
                                    UI::setPausePlaySprite(0);
                                    timer.pauseTimer();
                                } else {
                                    UI::setPausePlaySprite(1);
                                    timer.resumeTimer();

                                }
                            }
                        // leaderboard button
                        } else if (event.mouseButton.x > colCount*32-176 && event.mouseButton.x < colCount*32-176+64 && event.mouseButton.y > 32*(rowCount+0.5) && event.mouseButton.y < 32*(rowCount+0.5)+64) {
                            if (!BulletHell::isEnabled()) {
                                bool pauseFlag = false;
                                if(!board.getPaused()) {
                                    board.togglePaused();
                                    timer.pauseTimer();
                                    pauseFlag = true;
                                }
                                for (int i = 0; i < colCount; i++) {
                                    for (int j = 0; j < rowCount; j++) {
                                        gameWindow.draw(grid[i][j].getRevealedSprite());
                                    }
                                }
                                gameWindow.display();
                                std::ifstream leaderboardFile("files/leaderboard.txt");
                                for (int i = 0; i < 5 && leaderboardFile; i++) {
                                    std::getline(leaderboardFile, leaderboard[i]);
                                }
                                sf::RenderWindow leaderboardWindow(sf::VideoMode(colCount*16, rowCount*16+50), "Leaderboard Window", sf::Style::Close);
    
                                sf::Text leaderboardText("LEADERBOARD", font, 20);
                                setText(leaderboardText, width/4,height/4 - 120);
                                leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    
                                std::string l;
                                for (int i = 0; i < 5; i++) {
                                    l += std::to_string(i+1) + ".\t" + leaderboard[i].substr(0,5) + '\t' + leaderboard[i].substr(7) + "\n\n";
                                }
                                sf::Text leaderboardContent(l, font, 18);
                                setText(leaderboardContent, width/4,height/4 + 20);
                                leaderboardContent.setStyle(sf::Text::Bold);
    
                                leaderboardWindow.requestFocus();
                                while (leaderboardWindow.isOpen()) {
                                    sf::Event event;
                                    while (leaderboardWindow.pollEvent(event)) {
                                        switch (event.type)
                                        {
                                            case sf::Event::Closed:
                                                leaderboardWindow.close();
                                                break;
                                            default:
                                                break;
                                        }
                                    }
                                    leaderboardWindow.clear(sf::Color::Blue);
                                    leaderboardWindow.draw(leaderboardText);
                                    leaderboardWindow.draw(leaderboardContent);
                                    leaderboardWindow.display();
                                }
                                sf::Event flush;
                                while (gameWindow.pollEvent(flush)) {}
                                if (pauseFlag) {
                                    board.togglePaused();
                                    gameWindow.display();
                                }
                                timer.resumeTimer();
                                gameWindow.setActive(true);
                            }
                        // godmode prompt 200,32*(rowCount+0.5)+32
                        } else if (board.getGameOver() && BulletHell::isEnabled() && !godModeTextFlag && event.mouseButton.x > 200 && event.mouseButton.x < 230 && event.mouseButton.y > 32*(rowCount+0.5)+32 && event.mouseButton.y < 32*(rowCount+0.5)+62) {
                            BulletHell::toggleGodMode();
                            switch (BulletHell::godModeIsEnabled()) {
                                case true:
                                    godModePromptButton.setFillColor(sf::Color::Green);
                                    godModePrompted = true;
                                    break;
                                case false:
                                    godModePromptButton.setFillColor(sf::Color::Red);
                                    godModePrompted = false;
                                    break;
                            }
                        }
                        break;
                    }
                    break;
                default:
                    break;
            }
        }

        gameWindow.clear(sf::Color::White);
        
        for (int i = 0; i < colCount; i++) {
            for (int j = 0; j < rowCount; j++) {
                gameWindow.draw(grid[i][j].getRevealedSprite());
                if (board.getGameOver() && grid[i][j].stupidindicator()) {
                    gameWindow.draw(grid[i][j].getstupid());
                }
                if (grid[i][j].getIsRevealed() || grid[i][j].getIsFlagged()) {
                    gameWindow.draw(grid[i][j].getSprite());
                }
                if (board.getDebug()) {
                    if (grid[i][j].getIsMine()) {
                        gameWindow.draw(grid[i][j].getSprite());
                    }
                }
            }
        }
        for (int k = 0; k < UI::getSprites().size(); k++) {
            gameWindow.draw(UI::getSprites()[k]);
        }
        if (displayedMineCount < 0) {
            gameWindow.draw(UI::getNegativeSign());
        }
        if (BulletHell::isEnabled() && !board.getGameOver()) {
            sf::Vector2f currMousePos = sf::Vector2f(sf::Mouse::getPosition(gameWindow));
            BulletHell::attack(currMousePos.x, currMousePos.y);
            BulletHell::updateBulletPos(dt);
            BulletHell::decrementIFrames();
            for (Bullet& bullet : bullets) {
                if (bullet.checkCollision(prevMousePos, currMousePos)) {
                    if (BulletHell::godModeIsEnabled()) {
                        if (!BulletHell::isInvincible()) {
                            BulletHell::incrementDeathCount();
                            deathSprite.setPosition(currMousePos.x, currMousePos.y);
                            gameWindow.draw(deathSprite);
                            BulletHell::resetIFrames();
                            BulletHell::playDeathSound();
                            break;
                        }
                    } else {
                        board.toggleGameOver();
                        UI::setFaceSprite(2);
                        board.revealAllMines();
                        timer.pauseTimer();
                        deathSprite.setPosition(currMousePos.x, currMousePos.y);
                        BulletHell::playDeathSound();
                        break;
                    }
                }
            }
            prevMousePos = currMousePos;
            // cursor out of bounds
            if (currMousePos.x < 0 || currMousePos.x >= width || currMousePos.y < 0 || currMousePos.y >= height) {
                if (BulletHell::godModeIsEnabled()) {
                    if (!BulletHell::isInvincible()) {
                        BulletHell::incrementDeathCount();
                        deathSprite.setPosition(currMousePos.x, currMousePos.y);
                        BulletHell::resetIFrames();
                        BulletHell::playDeathSound();
                    }
                } else {
                    board.toggleGameOver();
                    UI::setFaceSprite(2);
                    board.revealAllMines();
                    timer.pauseTimer();
                    deathSprite.setPosition(currMousePos.x, currMousePos.y);
                    BulletHell::playDeathSound();
                }
            }
            // delete offscreen bullets
            BulletHell::deleteOffscreenBullets();
            
        }
        if (BulletHell::isEnabled() && BulletHell::godModeIsEnabled() && godModeTextFlag) {
            deathCountText.setString("Death count: " + std::to_string(BulletHell::getDeathCount()));
            gameWindow.draw(deathCountText);
        }
        if (board.getGameOver() && BulletHell::isEnabled() && !godModeTextFlag) {
            gameWindow.draw(godModePrompt);
            gameWindow.draw(godModePromptButton);
        }
        for (int k = 0; k < bullets.size(); k++) {
            gameWindow.draw(bullets[k].getSprite());
        }
        if (BulletHell::isEnabled() && board.getGameOver()) {
            gameWindow.draw(deathSprite);
        }
        gameWindow.display();
    }
    return 0;
}