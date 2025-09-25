#include "board.h"
#include <iostream>

void Board::fillGrid() {
    for (int i = 0; i < colCount; i++) {
        grid.push_back(std::vector<Tile>());
        for (int j = 0; j < rowCount; j++) {
            Tile tile(i, j);
            grid[i].push_back(tile);
        }
    }
}

void Board::placeMines() {
    int currentMineCount = mineCount;
    while (currentMineCount > 0) {
        int c = rand() % colCount;
        int r = rand() % rowCount;
        if (!grid[c][r].getIsMine()) {
            grid[c][r].becomeMine();
            currentMineCount--;
        }
    }
}

std::vector<std::vector<Tile>>& Board::getGrid() {
    return grid;
}

void Board::revealTile(int x, int y) {
    if (grid[x][y].getIsRevealed() || grid[x][y].getIsFlagged()) return;
    grid[x][y].reveal();
    if (grid[x][y].getIsMine()) {
        return;
    } else if (grid[x][y].getMineCount() == 0) {
        for(Tile* neighbor : grid[x][y].getNeighbors()) {
            revealTile(neighbor->getX(), neighbor->getY());
        }
    }
}

void Board::flagTile(int x, int y) {
    grid[x][y].flag();
}

void Board::unFlagTile(int x, int y) {
    grid[x][y].unFlag();
}

void Board::revealAll() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            grid[i][j].reveal();
        }
    }
}

void Board::unrevealAll() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            grid[i][j].unReveal();
            if (grid[i][j].stupidindicator()) {
                grid[i][j].togglestupid();
            }
        }
    }
}

void Board::fillTileNeighbors() {
    int directions[8][2] = {
        {-1,-1},{-1,0},{-1,1},
        {0,-1},        {0,1},
        {1,-1}, {1,0}, {1,1}
    };
    
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            for (int k = 0; k < 8; k++) {
                int c = i + directions[k][0];
                int r = j + directions[k][1];
                if (c >= 0 && c < colCount && r >= 0 && r < rowCount) {
                    grid[i][j].addNeighbor(&grid[c][r]);
                }
            }
        }
    }
}

void Board::setCounts() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            for(Tile* neighbor : grid[i][j].getNeighbors()) {
                if (neighbor->getIsMine()) {
                    grid[i][j].incrementMineCount();
                }
            }
        }
    }
}

void Board::toggleDebug() {
    debug = !debug;
}

bool Board::getDebug() {
    return debug;
}

void Board::toggleGameOver() {
    gameOver = !gameOver;
}
bool Board::getGameOver() {
    return gameOver;
}

void Board::clearMines() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            if (grid[i][j].getIsMine()) {
                grid[i][j].becomeNotMine();
            }
        }
    }
}

void Board::restart() {
    clearMines();
    placeMines();
    deleteNeighbors();
    fillTileNeighbors();
    setCounts();
    unrevealAll();
}

void Board::deleteNeighbors() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            grid[i][j].reset();
        }
    }
}

bool Board::checkWin() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            if (!grid[i][j].getIsRevealed() && !grid[i][j].getIsMine()) {
                return false;
            }
        }
    }
    return true;
}

void Board::flagAllMines() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            if (grid[i][j].getIsMine() && !grid[i][j].getIsFlagged()) {
                grid[i][j].flag();
            }
        }
    }
};

void Board::revealAllMines() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            if (grid[i][j].getIsMine()) {
                grid[i][j].reveal();
                if (grid[i][j].getIsFlagged()) {
                    grid[i][j].unFlag();
                    grid[i][j].togglestupid();
                }
            }
        }
    }
}

void Board::togglePaused() {
    paused = !paused;
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            grid[i][j].togglePaused();
        }
    }
};

bool Board::getPaused() {
    return paused;
}