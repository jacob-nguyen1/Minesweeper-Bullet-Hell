#include "UI.h"
#include <iostream>

sf::Sprite UI::debugSprite;
sf::Sprite UI::pausePlaySprite;
sf::Sprite UI::leaderboardSprite;
sf::Sprite UI::faceSprite;
sf::Sprite UI::counterSprite[3];
sf::Sprite UI::timerSprite[4];
sf::Sprite UI::negativeSign;

sf::Texture UI::debugTexture;
sf::Texture UI::pauseTexture;
sf::Texture UI::playTexture;
sf::Texture UI::leaderboardTexture;
sf::Texture UI::faceHappyTexture;
sf::Texture UI::faceLoseTexture;
sf::Texture UI::faceWinTexture;
sf::Texture UI::digitsTexture;

void UI::loadTextures() {
    debugTexture.loadFromFile("files/images/debug.png");
    pauseTexture.loadFromFile("files/images/pause.png");
    playTexture.loadFromFile("files/images/play.png");
    leaderboardTexture.loadFromFile("files/images/leaderboard.png");
    faceHappyTexture.loadFromFile("files/images/face_happy.png");
    faceLoseTexture.loadFromFile("files/images/face_lose.png");
    faceWinTexture.loadFromFile("files/images/face_win.png");
    digitsTexture.loadFromFile("files/images/digits.png");
}

void UI::setFaceSprite(int n) {
    switch (n) {
        case 0:
            faceSprite.setTexture(faceHappyTexture);
            break;
        case 1:
            faceSprite.setTexture(faceWinTexture);
            break;
        case 2:
            faceSprite.setTexture(faceLoseTexture);
            break;
        default:
            std::cerr << "Invalid face";
            break;
    }
}

void UI::setPausePlaySprite(int n) {
    switch (n) {
        case 0:
            pausePlaySprite.setTexture(playTexture);
            break;
        case 1:
            pausePlaySprite.setTexture(pauseTexture);
            break;
        default:
            std::cerr << "Invalid pause/play";
            break;
    }
}

void UI::setCounterSprite(int count) {
    if (count > 999) count = 999;
    if (count < -99) count = -99;

    for (int i = 0; i < 3; i++) {
        counterSprite[i].setTexture(digitsTexture);
    }
    
    count = abs(count);
    counterSprite[0].setTextureRect(sf::IntRect(21*(count/100), 0, 21, 32));
    counterSprite[1].setTextureRect(sf::IntRect(21*(count/10%10), 0, 21, 32));
    counterSprite[2].setTextureRect(sf::IntRect(21*(count%10), 0, 21, 32));
}
void UI::setTimerSprite(int m, int s) {
    for (int i = 0; i < 4; i++) {
        timerSprite[i].setTexture(digitsTexture);
    }

    timerSprite[0].setTextureRect(sf::IntRect(21*(m/10), 0, 21, 32));
    timerSprite[1].setTextureRect(sf::IntRect(21*(m%10), 0, 21, 32));
    timerSprite[2].setTextureRect(sf::IntRect(21*(s/10), 0, 21, 32));
    timerSprite[3].setTextureRect(sf::IntRect(21*(s%10), 0, 21, 32));
}

void UI::setupSprites(int colCount, int rowCount) {
    debugSprite.setTexture(debugTexture);
    leaderboardSprite.setTexture(leaderboardTexture);
    negativeSign.setTexture(digitsTexture);
    negativeSign.setTextureRect(sf::IntRect(210, 0, 21, 32));

    debugSprite.setPosition(colCount*32-304, 32*(rowCount+0.5));
    pausePlaySprite.setPosition(colCount*32-240, 32*(rowCount+0.5));
    leaderboardSprite.setPosition(colCount*32-176, 32*(rowCount+0.5));
    faceSprite.setPosition(colCount/2.0*32-32, 32*(rowCount+0.5));
    negativeSign.setPosition(12,32*(rowCount+0.5)+16);

    counterSprite[0].setPosition(33,32*(rowCount+0.5)+16);
    counterSprite[1].setPosition(33+21,32*(rowCount+0.5)+16);
    counterSprite[2].setPosition(33+21+21,32*(rowCount+0.5)+16);

    timerSprite[0].setPosition(colCount*32-97,32*(rowCount+0.5)+16);
    timerSprite[1].setPosition(colCount*32-97+21,32*(rowCount+0.5)+16);
    timerSprite[2].setPosition(colCount*32-54,32*(rowCount+0.5)+16);
    timerSprite[3].setPosition(colCount*32-54+21,32*(rowCount+0.5)+16);
}

std::vector<sf::Sprite> UI::getSprites() {
    return {debugSprite, pausePlaySprite, leaderboardSprite, faceSprite,
        counterSprite[0], counterSprite[1], counterSprite[2],
        timerSprite[0], timerSprite[1], timerSprite[2], timerSprite[3]
    };
}

sf::Sprite UI::getNegativeSign() {
    return negativeSign;
}