#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class UI {
    private:
        static sf::Sprite debugSprite;
        static sf::Sprite pausePlaySprite;
        static sf::Sprite leaderboardSprite;
        static sf::Sprite faceSprite;
        static sf::Sprite counterSprite[3];
        static sf::Sprite timerSprite[4];
        static sf::Sprite negativeSign;

        static sf::Texture debugTexture;
        static sf::Texture pauseTexture;
        static sf::Texture playTexture;
        static sf::Texture leaderboardTexture;
        static sf::Texture faceHappyTexture;
        static sf::Texture faceLoseTexture;
        static sf::Texture faceWinTexture;
        static sf::Texture digitsTexture;
    public:
        static void loadTextures();
        static void setFaceSprite(int n);
        static void setPausePlaySprite(int n);
        static void setupSprites(int colCount, int rowCount);
        static std::vector<sf::Sprite> getSprites();
        static void setCounterSprite(int count);
        static void setTimerSprite(int m, int s);
        static sf::Sprite getNegativeSign();
};