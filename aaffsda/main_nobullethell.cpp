#include "board.h"
#include "tile.h"
#include "UI.h"
#include "extra.h"
#include "timer.h"
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>


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
        welcomeWindow.display();
    }

    sf::RenderWindow gameWindow(sf::VideoMode(colCount*32, rowCount*32+100), "Game Window", sf::Style::Close);
    
    Tile::loadTextures();
    UI::loadTextures();
    UI::setupSprites(colCount, rowCount);
    UI::setFaceSprite(0);
    UI::setPausePlaySprite(0);
    UI::setCounterSprite(displayedMineCount);
    UI::setTimerSprite(7, 27);

    Board board(colCount, rowCount, mineCount);
    board.fillGrid();
    board.placeMines();
    board.fillTileNeighbors();
    board.setCounts();
    //board.revealAll();

    Timer timer;

    timer.startTimer();
    while(gameWindow.isOpen()) {
        UI::setCounterSprite(displayedMineCount);
        UI::setTimerSprite(timer.getElapsedMinutes(), timer.getElapsedSeconds());
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
                                    if (!board.getGrid()[event.mouseButton.x/32][event.mouseButton.y/32].getIsRevealed()) {
                                        if (board.getGrid()[event.mouseButton.x/32][event.mouseButton.y/32].getIsFlagged()) {
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
                                if (!board.getGrid()[event.mouseButton.x/32][event.mouseButton.y/32].getIsFlagged()) {
                                    board.revealTile(event.mouseButton.x/32,event.mouseButton.y/32);
                                    if (board.getGrid()[event.mouseButton.x/32][event.mouseButton.y/32].getIsMine()) {
                                        board.toggleGameOver();
                                        UI::setFaceSprite(2);
                                        board.revealAllMines();
                                        timer.pauseTimer();
                                    } else if (board.checkWin()) {
                                        board.toggleGameOver();
                                        UI::setFaceSprite(1);
                                        board.flagAllMines();
                                        timer.pauseTimer();
                                        for (int i = 0; i < colCount; i++) {
                                            for (int j = 0; j < rowCount; j++) {
                                                gameWindow.draw(board.getGrid()[i][j].getRevealedSprite());
                                                if (board.getGrid()[i][j].getIsRevealed() || board.getGrid()[i][j].getIsFlagged()) {
                                                    gameWindow.draw(board.getGrid()[i][j].getSprite());
                                                }
                                            }
                                        }
                                        for (int k = 0; k < UI::getSprites().size(); k++) {
                                            gameWindow.draw(UI::getSprites()[k]);
                                        }
                                        gameWindow.display();
                                        int playerMinutes = timer.getElapsedMinutes();
                                        int playerSeconds = timer.getElapsedSeconds();
                                        std::ifstream leaderboardFile("files/leaderboard.txt");
                                        for (int i = 0; i < 5; i++) {
                                            std::getline(leaderboardFile, leaderboard[i]);
                                            if (!leaderboard[i].empty() && leaderboard[i].back() == '*') leaderboard[i].pop_back();
                                        }
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
                        // pause button
                        } else if (event.mouseButton.x > colCount*32-240 && event.mouseButton.x < colCount*32-240+64 && event.mouseButton.y > 32*(rowCount+0.5) && event.mouseButton.y < 32*(rowCount+0.5)+64) {
                            if (!board.getGameOver()) {
                                board.togglePaused();
                                if (board.getPaused()) {
                                    UI::setPausePlaySprite(1);
                                    timer.pauseTimer();
                                } else {
                                    UI::setPausePlaySprite(0);
                                    timer.resumeTimer();

                                }
                            }
                        // leaderboard button
                        } else if (event.mouseButton.x > colCount*32-176 && event.mouseButton.x < colCount*32-176+64 && event.mouseButton.y > 32*(rowCount+0.5) && event.mouseButton.y < 32*(rowCount+0.5)+64) {
                            bool pauseFlag = false;
                            if(!board.getPaused()) {
                                board.togglePaused();
                                timer.pauseTimer();
                                pauseFlag = true;
                            }
                            for (int i = 0; i < colCount; i++) {
                                for (int j = 0; j < rowCount; j++) {
                                    gameWindow.draw(board.getGrid()[i][j].getRevealedSprite());
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
                gameWindow.draw(board.getGrid()[i][j].getRevealedSprite());
                if (board.getGrid()[i][j].getIsRevealed() || board.getGrid()[i][j].getIsFlagged()) {
                    gameWindow.draw(board.getGrid()[i][j].getSprite());
                }
                if (board.getDebug()) {
                    if (board.getGrid()[i][j].getIsMine()) {
                        gameWindow.draw(board.getGrid()[i][j].getSprite());
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
        gameWindow.display();
    }
    return 0;
}