#include "bullet.h"
#include "bullethell.h"

sf::Texture Bullet::bulletTexture;
float Bullet::bulletSpeed = 0.8f;
bool Bullet::gradescope = false;

void Bullet::loadBulletTexture() {
    if(gradescope) {
        bulletTexture.loadFromFile("files/images/mine.png");
    }else{
        bulletTexture.loadFromFile("files/images/bullets.png");
    }
}

void Bullet::updatePos(float dt) {
    x += vx * dt * bulletSpeed;
    y += vy * dt * bulletSpeed;
    vx += ax * dt * bulletSpeed;
    vy += ay * dt * bulletSpeed;

    bulletSprite.setPosition(x,y);
}

void Bullet::setBulletSpeed(float speed) {
    bulletSpeed = speed;
}

bool Bullet::checkCollision(const sf::Vector2f& prevMousePos, const sf::Vector2f& currMousePos) {
    if (prevMousePos == currMousePos) {
        return ((x-currMousePos.x)*(x-currMousePos.x) + (y-currMousePos.y)*(y-currMousePos.y)) 
               <= (hitboxRadius * hitboxRadius);
    }
    sf::Vector2f bulletToPrev(x - prevMousePos.x, y - prevMousePos.y);
    sf::Vector2f dir = currMousePos - prevMousePos;
    float projection = (bulletToPrev.x*dir.x + bulletToPrev.y*dir.y) / (dir.x*dir.x + dir.y*dir.y);
    sf::Vector2f closestPoint;
    if (projection <= 0) {
        closestPoint = prevMousePos;
    } 
    else if (projection >= 1) {
        closestPoint = currMousePos;
    }
    else {
        closestPoint = prevMousePos + projection * dir;
    }
    
    return ((x-closestPoint.x)*(x-closestPoint.x)+(y-closestPoint.y)*(y-closestPoint.y)) < (hitboxRadius * hitboxRadius);
}

bool Bullet::gradescopequestionmark() {return gradescope;}