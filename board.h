#pragma once
#include <vector>
#include <tile.h>

class Board {
    private:
        std::vector<std::vector<Tile>> grid;
        int rowCount;
        int colCount;
        int mineCount;
        bool debug;
        bool gameOver;
        bool paused;
    public:
        Board(int colCount, int rowCount, int mineCount) : rowCount(rowCount), colCount(colCount), mineCount(mineCount), debug(false), gameOver(false), paused(false) {};
        void fillGrid();
        void placeMines();
        std::vector<std::vector<Tile>>& getGrid();
        void revealTile(int x, int y);
        void revealAll();
        void unrevealAll();
        void fillTileNeighbors();
        void setCounts();
        void flagTile(int x, int y);
        void unFlagTile(int x, int y);
        void toggleDebug();
        bool getDebug();
        void toggleGameOver();
        bool getGameOver();
        void clearMines();
        void restart();
        void deleteNeighbors();
        bool checkWin();
        void flagAllMines();
        void revealAllMines();
        void togglePaused();
        bool getPaused();
};
