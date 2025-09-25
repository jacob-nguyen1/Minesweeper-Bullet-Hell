#include "timer.h"

void Timer::startTimer() {
    paused = false;
    startTime = std::chrono::high_resolution_clock::now();
    pauseDuration = std::chrono::seconds(0);
}
void Timer::pauseTimer() {
    if (paused) {
        return;
    }
    pauseTime = std::chrono::high_resolution_clock::now();
    paused = true;
}
void Timer::resumeTimer() {
    if (!paused) {
        return;
    }
    auto resumeTime = std::chrono::high_resolution_clock::now();
    pauseDuration += resumeTime - pauseTime;
    paused = false;
}
void Timer::resetTimer() {
    paused = true;
    pauseDuration = std::chrono::seconds(0);
    startTime = std::chrono::high_resolution_clock::now();
}
int Timer::getElapsedSeconds() {
    if (paused) {
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(pauseTime - startTime - pauseDuration);
        return elapsedTime.count();
    } else {
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - startTime - pauseDuration);
        return elapsedTime.count();
    }
}
int Timer::getElapsedMinutes() {
    return getElapsedSeconds() / 60;
}