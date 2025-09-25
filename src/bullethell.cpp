#include "bullethell.h"
#include <fstream>
#include <iostream>
#include <cmath>

bool BulletHell::enabled = false;
std::vector<Bullet> BulletHell::bullets;
int BulletHell::windowWidth = 960;
int BulletHell::windowHeight = 612;
int BulletHell::attackPatternCount = 5;
float BulletHell::rest;
sf::Clock BulletHell::gameClock;
sf::Clock BulletHell::attackClock;
int BulletHell::attackPattern;
int BulletHell::godModeDeathCount = 0;
bool BulletHell::godModeEnabled = false;
int BulletHell::iFrames = 10;
float BulletHell::patternLength = 1.0f;
int BulletHell::lastAttackPattern = -1;
int BulletHell::randomSide = rand() % 4;
int BulletHell::lastRandomSide = -1;
bool BulletHell::firstAttack = true;
int BulletHell::globalAngle = 0;
sf::SoundBuffer BulletHell::buffers[4];
sf::Sound BulletHell::sounds[4];
float BulletHell::attackDelay = 1.5f;

void BulletHell::attack(float mouseX, float mouseY) {
    if (gameClock.getElapsedTime().asSeconds() < patternLength) { 
        switch (attackPattern) {
            // top carpet rain
            case 1:
                patternLength = 8.0f;
                rest = 3.0f;
                if (firstAttack || attackClock.getElapsedTime().asSeconds() > 0.4f*attackDelay) {
                    carpetTop(100,50,50,3);
                    carpetTop(100,75,70,3);
                    attackClock.restart();
                    firstAttack = false;
                    sounds[2].play();
                }
                break;
            // all spam random
            case 2:
                patternLength = 8.0f;
                rest = 4.0f;
                if (firstAttack || attackClock.getElapsedTime().asSeconds() > 0.2f*attackDelay) {
                    randomSingleAll(100,80,rand()%14+1);
                    attackClock.restart();
                    firstAttack = false;
                    sounds[1].play();
                }
                break;
            // all seeking
            case 3:
                patternLength = 8.0f;
                rest = 2.0f;
                if (firstAttack || attackClock.getElapsedTime().asSeconds() > 0.3f*attackDelay) {
                    randomSingleTargetAll(0,500,10,mouseX,mouseY);
                    attackClock.restart();
                    firstAttack = false;
                    sounds[1].play();;
                }
                break;
            // carpets with gap
            case 4:
                patternLength = 8.0f;
                rest = 3.0f;
                if (firstAttack || attackClock.getElapsedTime().asSeconds() > 0.4f*std::pow(attackDelay,0.8)) {
                    carpetGapBottom(100,50,15,6,11);
                    attackClock.restart();
                    firstAttack = false;
                    sounds[2].play();
                }
                break;
            // top cool spinner
            case 5:
                patternLength = 12.0f;
                rest = 5.0f;
                if (firstAttack || attackClock.getElapsedTime().asSeconds() > 0.07f*std::pow(attackDelay,1.5)) {
                    spinnerTop(100,0,7,0,1,2);
                    spinnerTop(150,0,11,0,1,2);
                    spinnerTop(200,100,2,0,1,2);
                    spinnerTop(200,100,2,0,-1,2);
                    globalAngle += 6;
                    attackClock.restart();
                    firstAttack = false;
                    sounds[0].play();
                }
                break;
        }
    } else if (gameClock.getElapsedTime().asSeconds() > patternLength + rest) {
        gameClock.restart();
        attackClock.restart();
        lastAttackPattern = attackPattern;
        do {
            attackPattern = rand() % attackPatternCount + 1;
        } while (attackPattern == lastAttackPattern);
        firstAttack = true;
        globalAngle = 0;
    }
}

void BulletHell::setAttackDelay(float delay) {
    attackDelay = delay;
};

void BulletHell::initializeSound() {
    buffers[0].loadFromFile("files/sounds/bullet0.wav");
    buffers[1].loadFromFile("files/sounds/bullet1.wav");
    buffers[2].loadFromFile("files/sounds/bullet2.wav");
    buffers[3].loadFromFile("files/sounds/death.wav");

    for (int i = 0; i < 3; i++) {
        sounds[i].setBuffer(buffers[i]);
        sounds[i].setVolume(5.f);
    }
    sounds[3].setBuffer(buffers[3]);
    sounds[3].setVolume(100.f);
}

void BulletHell::playDeathSound() {
    sounds[3].play();
}

void BulletHell::incrementGlobalAngle(int increment) {
    globalAngle += increment;
    if (globalAngle >= 360) globalAngle = globalAngle % 360;
}

void BulletHell::toggleGodMode() {
    godModeEnabled = !godModeEnabled;
}

bool BulletHell::godModeIsEnabled() {
    return godModeEnabled;
}

void BulletHell::incrementDeathCount() {
    godModeDeathCount++;
}

void BulletHell::decrementIFrames() {
    if (iFrames > 0) {
        iFrames--;
    }
}

bool BulletHell::isInvincible() {
    return iFrames > 0;
}

void BulletHell::resetIFrames() {
    iFrames = 10;
}

int BulletHell::getDeathCount() {
    return godModeDeathCount;
}

void BulletHell::loadWindowSize(int w, int h) {
    windowWidth = w;
    windowHeight = h;
}

void BulletHell::toggle() {
    enabled = !enabled;
}

bool BulletHell::isEnabled() {
    return BulletHell::enabled;
}

void BulletHell::spawnBullet(float x, float y, float vMag, float vAngle, float aMag, float aAngle, int color) {
    Bullet bullet(x, y, vMag, vAngle, aMag, aAngle, color);
    bullets.push_back(bullet);
}

void BulletHell::deleteOffscreenBullets() {
    for (int i = bullets.size() - 1; i >= 0; --i) {
        if (bullets[i].getX() + bullets[i].getHitboxRadius() < 0 ||
            bullets[i].getX() - bullets[i].getHitboxRadius() > windowWidth ||
            bullets[i].getY() + bullets[i].getHitboxRadius() < 0 ||
            bullets[i].getY() - bullets[i].getHitboxRadius() > windowHeight) {
            bullets.erase(bullets.begin() + i);
        }
    }
}

void BulletHell::updateBulletPos(float dt) {
    for (Bullet& bullet : bullets) {
        bullet.updatePos(dt);
    }
}

void BulletHell::restart() {
    clearBullets();
    gameClock.restart();
    attackClock.restart();
    attackPattern = rand() % attackPatternCount + 1;
    godModeDeathCount = 0;
    firstAttack = true;
    globalAngle = 0;
}

void BulletHell::clearBullets() {
    bullets.clear();
}

void BulletHell::carpetTop(int v, int a, int spacing, int color) {
    int offset = rand() % spacing;
    for (int i = 0; i < windowWidth/spacing+1; i++) {
        spawnBullet(i*spacing+offset,0,v,90,a,90,color);
    }
}

void BulletHell::carpetLeft(int v, int a, int spacing, int color) {
    int offset = rand() % spacing;
    for (int i = 0; i < windowHeight/spacing; i++) {
        spawnBullet(0,i*spacing+offset,v,0,a,0,color);
    }
}

void BulletHell::carpetBottom(int v, int a, int spacing, int color) {
    int offset = rand() % spacing;
    for (int i = 0; i < windowWidth/spacing; i++) {
        spawnBullet(i*spacing+offset,windowHeight,v,270,a,270,color);
    }
}

void BulletHell::carpetRight(int v, int a, int spacing, int color) {
    int offset = rand() % spacing;
    for (int i = 0; i < windowHeight/spacing; i++) {
        spawnBullet(windowWidth,i*spacing+offset,v,180,a,180,color);
    }
}

void BulletHell::carpetAll(int v, int a, int spacing, int color) {
    carpetTop(v,a,spacing,color);
    carpetLeft(v,a,spacing,color);
    carpetBottom(v,a,spacing,color);
    carpetRight(v,a,spacing,color);
};

void BulletHell::randomSingleTop(int v, int a, int color) {
    int offset = rand() % windowWidth;
    int angleOffset = rand() % 90 - 45;
    spawnBullet(offset,0,v,90+angleOffset,a,90+angleOffset,color);
}

void BulletHell::randomSingleLeft(int v, int a, int color) {
    int offset = rand() % windowHeight;
    int angleOffset = rand() % 90 - 45;
    spawnBullet(0,offset,v,0+angleOffset,a,0+angleOffset,color);
}

void BulletHell::randomSingleBottom(int v, int a, int color) {
    int offset = rand() % windowWidth;
    int angleOffset = rand() % 90 - 45;
    spawnBullet(offset,windowHeight,v,270+angleOffset,a,270+angleOffset,color);
}

void BulletHell::randomSingleRight(int v, int a, int color) {
    int offset = rand() % windowHeight;
    int angleOffset = rand() % 90 - 45;
    spawnBullet(windowWidth,offset,v,180+angleOffset,a,180+angleOffset,color);
}

void BulletHell::randomSingleAll(int v, int a, int color) {
    randomSingleTop(v,a,color);
    randomSingleLeft(v,a,color);
    randomSingleBottom(v,a,color);
    randomSingleRight(v,a,color);
}

void BulletHell::randomSingleTargetTop(int v, int a, int color, float targetX, float targetY) {
    int offset = rand() % windowWidth;
    float dx = targetX - offset;
    float dy = targetY;
    float angle = atan2(dy, dx) * 180.f / 3.14159265f;
    spawnBullet(offset, 0, v, angle, a, angle, color);
}

void BulletHell::randomSingleTargetLeft(int v, int a, int color, float targetX, float targetY) {
    int offset = rand() % windowHeight;
    float dx = targetX;
    float dy = targetY - offset;
    float angle = atan2(dy, dx) * 180.f / 3.14159265f;
    spawnBullet(0, offset, v, angle, a, angle, color);
}

void BulletHell::randomSingleTargetBottom(int v, int a, int color, float targetX, float targetY) {
    int offset = rand() % windowWidth;
    float dx = targetX - offset;
    float dy = targetY - windowHeight;
    float angle = atan2(dy, dx) * 180.f / 3.14159265f;
    spawnBullet(offset, windowHeight, v, angle, a, angle, color);
}

void BulletHell::randomSingleTargetRight(int v, int a, int color, float targetX, float targetY) {
    int offset = rand() % windowHeight;
    float dx = targetX - windowWidth;
    float dy = targetY - offset;
    float angle = atan2(dy, dx) * 180.f / 3.14159265f;
    spawnBullet(windowWidth, offset, v, angle, a, angle, color);
}

void BulletHell::randomSingleTargetAll(int v, int a, int color, float targetX, float targetY) {
    randomSingleTargetTop(v,a,color,targetX,targetY);
    randomSingleTargetLeft(v,a,color,targetX,targetY);
    randomSingleTargetBottom(v,a,color,targetX,targetY);
    randomSingleTargetRight(v,a,color,targetX,targetY);
}

void BulletHell::carpetGapTop(int v, int a, int spacing, int gap, int color) {
    int gapStart = rand() % (windowWidth / spacing - gap) + 1;
    for (int i = 0; i < windowWidth/spacing+1; i++) { 
        if (i >= gapStart && i < gapStart + gap) continue;
        spawnBullet(i*spacing,0,v,90,a,90,color);
    }
}

void BulletHell::carpetGapBottom(int v, int a, int spacing, int gap, int color) {
    int gapStart = rand() % (windowWidth / spacing - gap) + 1;
    for (int i = 0; i < windowWidth/spacing+1; i++) { 
        if (i >= gapStart && i < gapStart + gap) continue;
        spawnBullet(i*spacing,windowHeight,v,270,a,270,color);
    }
}

void BulletHell::spinnerTop(int v, int a, int streamCount, int startAngle, int dir, int color) {
    float angleIncrement = 360.0f / streamCount;
    for (int i = 0; i < streamCount; i++) {
        float currentAngle = startAngle + (globalAngle * dir) + (i * angleIncrement);
        spawnBullet(windowWidth/2,0,v,currentAngle,a,currentAngle,color);
    }
}

void BulletHell::spinnerTopLeft(int v, int a, int streamCount, int startAngle, int dir, int color) {
    float angleIncrement = 360.0f / streamCount;
    for (int i = 0; i < streamCount; i++) {
        float currentAngle = startAngle + (globalAngle * dir) + (i * angleIncrement);
        spawnBullet(0,0,v,currentAngle,a,currentAngle,color);
    }
}

void BulletHell::spinnerLeft(int v, int a, int streamCount, int startAngle, int dir, int color) {
    float angleIncrement = 360.0f / streamCount;
    for (int i = 0; i < streamCount; i++) {
        float currentAngle = startAngle + (globalAngle * dir) + (i * angleIncrement);
        spawnBullet(0,windowHeight/2,v,currentAngle,a,currentAngle,color);
    }
}

void BulletHell::spinnerBottomLeft(int v, int a, int streamCount, int startAngle, int dir, int color) {
    float angleIncrement = 360.0f / streamCount;
    for (int i = 0; i < streamCount; i++) {
        float currentAngle = startAngle + (globalAngle * dir) + (i * angleIncrement);
        spawnBullet(0,windowHeight,v,currentAngle,a,currentAngle,color);
    }
}

void BulletHell::spinnerBottom(int v, int a, int streamCount, int startAngle, int dir, int color) {
    float angleIncrement = 360.0f / streamCount;
    for (int i = 0; i < streamCount; i++) {
        float currentAngle = startAngle + (globalAngle * dir) + (i * angleIncrement);
        spawnBullet(windowWidth/2,windowHeight,v,currentAngle,a,currentAngle,color);
    }
}

void BulletHell::spinnerBottomRight(int v, int a, int streamCount, int startAngle, int dir, int color) {
    float angleIncrement = 360.0f / streamCount;
    for (int i = 0; i < streamCount; i++) {
        float currentAngle = startAngle + (globalAngle * dir) + (i * angleIncrement);
        spawnBullet(windowWidth,windowHeight,v,currentAngle,a,currentAngle,color);
    }
}

void BulletHell::spinnerRight(int v, int a, int streamCount, int startAngle, int dir, int color) {
    float angleIncrement = 360.0f / streamCount;
    for (int i = 0; i < streamCount; i++) {
        float currentAngle = startAngle + (globalAngle * dir) + (i * angleIncrement);
        spawnBullet(windowWidth,windowHeight/2,v,currentAngle,a,currentAngle,color);
    }
}

void BulletHell::spinnerTopRight(int v, int a, int streamCount, int startAngle, int dir, int color) {
    float angleIncrement = 360.0f / streamCount;
    for (int i = 0; i < streamCount; i++) {
        float currentAngle = startAngle + (globalAngle * dir) + (i * angleIncrement);
        spawnBullet(windowWidth,0,v,currentAngle,a,currentAngle,color);
    }
}