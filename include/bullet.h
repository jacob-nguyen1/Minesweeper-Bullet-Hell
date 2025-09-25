#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Bullet {
    private:
        static sf::Texture bulletTexture;
        sf::Sprite bulletSprite;
        static float bulletSpeed;
        static bool gradescope;

        float hitboxRadius;
        float x;
        float y;
        float vx;
        float vy;
        float ax;
        float ay;
    public:
        Bullet(float x, float y, float vMag, float vAngle, float aMag, float aAngle, int color) : 
            hitboxRadius(10.0), x(x), y(y),
            vx(vMag * std::cos(vAngle * 3.14159f / 180.0f)),
            vy(vMag * std::sin(vAngle * 3.14159f / 180.0f)),
            ax(aMag * std::cos(aAngle * 3.14159f / 180.0f)),
            ay(aMag * std::sin(aAngle * 3.14159f / 180.0f))  {
            bulletSprite.setTexture(bulletTexture);
            if(!gradescope) {
                bulletSprite.setTextureRect(sf::IntRect(color*16, 0, 16, 16));
                bulletSprite.setScale(1.5f, 1.5f);
                bulletSprite.setOrigin(8.0f, 8.0f);
            } else {
                bulletSprite.setOrigin(16.0f, 16.0f);
            }
            
            bulletSprite.setPosition(x, y);
        };

        static void loadBulletTexture();
        static bool gradescopequestionmark();

        void updatePos(float dt);
        static void setBulletSpeed(float speed);
        bool checkCollision(const sf::Vector2f& prevMousePos, const sf::Vector2f& currMousePos);

        sf::Sprite getSprite() const {return bulletSprite;}
        float getX() const {return x;}
        float getY() const {return y;}
        float getHitboxRadius() const {return hitboxRadius;}

        
};