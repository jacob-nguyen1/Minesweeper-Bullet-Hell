#pragma once
#include <chrono>

class Timer {
    private:
        std::chrono::high_resolution_clock::time_point startTime;
        std::chrono::high_resolution_clock::time_point pauseTime;
        std::chrono::high_resolution_clock::duration pauseDuration;
        bool paused;
    public:
    Timer() : paused(true), pauseDuration(0) {}

        void startTimer();
        void pauseTimer();
        void resumeTimer();
        void resetTimer();
        int getElapsedSeconds();
        int getElapsedMinutes();
};