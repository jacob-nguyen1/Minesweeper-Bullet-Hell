#pragma once
#include <SFML/Graphics.hpp>

class Tile {
    private:
        static sf::Texture mineTexture;
        static sf::Texture tileHiddenTexture;
        static sf::Texture tileRevealedTexture;
        static sf::Texture num1;
        static sf::Texture num2;
        static sf::Texture num3;
        static sf::Texture num4;
        static sf::Texture num5;
        static sf::Texture num6;
        static sf::Texture num7;
        static sf::Texture num8;
        static sf::Texture flagTexture;

        bool isMine;
        bool isRevealed;
        bool isFlagged;
        int mineCount;
        int x;
        int y;
        std::vector<Tile*> neighbors;
        sf::Sprite revealedSprite;
        sf::Sprite stupidsprite;
        sf::Sprite sprite;
        bool tilePaused;
        bool stupid;
    public:
        Tile(int x, int y) : x(x), y(y), isMine(false), isRevealed(false), mineCount(0), isFlagged(false), tilePaused(false) {
            stupidsprite.setTexture(flagTexture);
            stupidsprite.setPosition(x * 32, y * 32);
        };
        static void loadTextures();
        void updateRevealedSprite();
        sf::Sprite& getRevealedSprite();
        void updateSprite();
        sf::Sprite& getSprite();
        bool getIsMine();
        void becomeMine();
        void becomeNotMine();
        void reveal();
        void unReveal();
        void addNeighbor(Tile* tile);
        std::vector<Tile*> getNeighbors();
        void incrementMineCount();
        bool getIsRevealed();
        bool getMineCount();
        int getX();
        int getY();
        void flag();
        bool getIsFlagged();
        void unFlag();
        void reset();
        void togglePaused();
        sf::Sprite& getstupid();
        void togglestupid();
        bool stupidindicator();
};

