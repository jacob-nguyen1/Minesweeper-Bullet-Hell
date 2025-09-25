#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "bullet.h"
class BulletHell {
    private:
        static bool enabled;
        static std::vector<Bullet> bullets;
        static int windowWidth;
        static int windowHeight;
        static sf::Clock gameClock;
        static sf::Clock attackClock;
        static int attackPatternCount;
        static int attackPattern;
        static float rest;
        static bool godModeEnabled;
        static int godModeDeathCount;
        static int iFrames;
        static float patternLength;
        static int lastAttackPattern;
        static int randomSide;
        static int lastRandomSide;
        static bool firstAttack;
        static int globalAngle;
        static sf::SoundBuffer buffers[4];
        static sf::Sound sounds[4];
        static float attackDelay;
    public:
        static void toggle();
        static bool isEnabled();
        static void spawnBullet(float x, float y, float vMag, float vAngle, float aMag, float aAngle, int color);
        static void deleteOffscreenBullets();
        static std::vector<Bullet>& getBullets() {return bullets;}
        static void updateBulletPos(float dt);
        static void clearBullets();
        static void restart();
        static void attack(float mouseX, float mouseY);
        static void loadWindowSize(int w, int h);
        static void toggleGodMode();
        static bool godModeIsEnabled();
        static void incrementDeathCount();
        static int getDeathCount();
        static void decrementIFrames();
        static bool isInvincible();
        static void resetIFrames();
        static void incrementGlobalAngle(int a);
        static void initializeSound();
        static void playDeathSound();
        static void setAttackDelay(float da);

        // ATTACKS
        static void carpetTop(int v, int a, int spacing, int color);
        static void carpetLeft(int v, int a, int spacing, int color);
        static void carpetBottom(int v, int a, int spacing, int color);
        static void carpetRight(int v, int a, int spacing, int color);
        static void carpetAll(int v, int a, int spacing, int color);

        static void randomSingleTop(int v, int a, int color);
        static void randomSingleLeft(int v, int a, int color);
        static void randomSingleBottom(int v, int a, int color);
        static void randomSingleRight(int v, int a, int color);
        static void randomSingleAll(int v, int a, int color);

        static void randomSingleTargetTop(int v, int a, int color, float targetX, float targetY);
        static void randomSingleTargetLeft(int v, int a, int color, float targetX, float targetY);
        static void randomSingleTargetBottom(int v, int a, int color, float targetX, float targetY);
        static void randomSingleTargetRight(int v, int a, int color, float targetX, float targetY);
        static void randomSingleTargetAll(int v, int a, int color, float targetX, float targetY);

        static void carpetGapTop(int v, int a, int spacing, int gap, int color);
        static void carpetGapBottom(int v, int a, int spacing, int gap, int color);

        static void spinnerTop(int v, int a, int streamCount, int startAngle, int dir, int color);
        static void spinnerTopLeft(int v, int a, int streamCount, int startAngle, int dir, int color);
        static void spinnerLeft(int v, int a, int streamCount, int startAngle, int dir, int color);
        static void spinnerBottomLeft(int v, int a, int streamCount, int startAngle, int dir, int color);
        static void spinnerBottom(int v, int a, int streamCount, int startAngle, int dir, int color);
        static void spinnerBottomRight(int v, int a, int streamCount, int startAngle, int dir, int color);
        static void spinnerRight(int v, int a, int streamCount, int startAngle, int dir, int color);
        static void spinnerTopRight(int v, int a, int streamCount, int startAngle, int dir, int color);
};