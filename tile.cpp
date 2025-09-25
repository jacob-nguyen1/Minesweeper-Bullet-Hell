#include "tile.h"
#include <iostream>

sf::Texture Tile::mineTexture;
sf::Texture Tile::tileHiddenTexture;
sf::Texture Tile::tileRevealedTexture;
sf::Texture Tile::num1;
sf::Texture Tile::num2;
sf::Texture Tile::num3;
sf::Texture Tile::num4;
sf::Texture Tile::num5;
sf::Texture Tile::num6;
sf::Texture Tile::num7;
sf::Texture Tile::num8;
sf::Texture Tile::flagTexture;

void Tile::loadTextures() {
    mineTexture.loadFromFile("files/images/mine.png");
    tileHiddenTexture.loadFromFile("files/images/tile_hidden.png");
    tileRevealedTexture.loadFromFile("files/images/tile_revealed.png");
    num1.loadFromFile("files/images/number_1.png");
    num2.loadFromFile("files/images/number_2.png");
    num3.loadFromFile("files/images/number_3.png");
    num4.loadFromFile("files/images/number_4.png");
    num5.loadFromFile("files/images/number_5.png");
    num6.loadFromFile("files/images/number_6.png");
    num7.loadFromFile("files/images/number_7.png");
    num8.loadFromFile("files/images/number_8.png");
    flagTexture.loadFromFile("files/images/flag.png");
}

void Tile::updateRevealedSprite() {
    if (isRevealed || tilePaused) {
        revealedSprite.setTexture(tileRevealedTexture);
    } else {
        revealedSprite.setTexture(tileHiddenTexture);
    }
    revealedSprite.setPosition(x*32, y*32);
}

sf::Sprite& Tile::getRevealedSprite() {
    updateRevealedSprite();
    return revealedSprite;
}

void Tile::updateSprite() {
    if (tilePaused) {
        sprite.setTexture(tileRevealedTexture);
    } else if (isFlagged) {
        sprite.setTexture(flagTexture);
    } else if (isMine) {
        sprite.setTexture(mineTexture);
    } else if (mineCount == 0) {
        sprite.setTexture(tileRevealedTexture);
    } else {
        sf::Texture* numTextures[8] = {&num1, &num2, &num3, &num4, &num5, &num6, &num7, &num8};
        sprite.setTexture(*numTextures[mineCount - 1]);
    }
    sprite.setPosition(x*32, y*32);
}

void Tile::togglestupid() {
    stupid = !stupid;
}

bool Tile::stupidindicator() {
    return stupid;
}

sf::Sprite& Tile::getstupid() {
    
    return stupidsprite;
}

sf::Sprite& Tile::getSprite() {
    updateSprite();
    return sprite;
}

void Tile::becomeMine() {
    isMine = true;
}

void Tile::becomeNotMine() {
    isMine = false;
}

bool Tile::getIsMine() {
    return isMine;
}

void Tile::reveal() {
    isRevealed = true;
}

void Tile::unReveal() {
    isRevealed = false;
}


void Tile::addNeighbor(Tile* tile) {
    neighbors.push_back(tile);
}

std::vector<Tile*> Tile::getNeighbors() {
    return neighbors;
}

void Tile::incrementMineCount() {
    mineCount++;
}

bool Tile::getIsRevealed() {
    return isRevealed;
}

bool Tile::getMineCount() {
    return mineCount;
}

int Tile::getX() {
    return x;
}

int Tile::getY() {
    return y;
}

void Tile::flag() {
    isFlagged = true;
}

bool Tile::getIsFlagged() {
    return isFlagged;
}

void Tile::unFlag() {
    isFlagged = false;
}

void Tile::reset() {
    neighbors.clear();
    mineCount = 0;
    unFlag();
}

void Tile::togglePaused() {
    tilePaused = !tilePaused;
}