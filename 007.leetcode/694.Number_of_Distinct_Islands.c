#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int
numDistinctIslands (int **grid, int gridSize, int *gridColSize)
{
}

/*
// BFS
int numDistinctIslands(int row, int col, grid[row][col]) {
    int offsets[] = {0, 1, 0, -1, 0};
    unordered_set<string> islands;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j]) {
                grid[i][j] = 0;
                string island;
                queue<pair<int, int>> todo;
                todo.push({i, j});
                while (!todo.empty()) {
                    pair<int, int> p = todo.front();
                    todo.pop();
                    for (int k = 0; k < 4; k++) {
                        int r = p.first + offsets[k], c = p.second + offsets[k + 1];
                        if (r >= 0 && r < m && c >= 0 && c < n && grid[r][c]) {
                            grid[r][c] = 0;
                            todo.push({r, c});
                            island += to_string(r - i) + to_string(c - j);
                        }
                    }
                }
                islands.insert(island);
            }
        }
    }
    return islands.size();
}

// DFS
int numDistinctIslands(int row, int col, grid[row][col]) {
    unordered_set<string> islands;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j]) {
                string island;
                distinct(grid, i, j, i, j, island);
                islands.insert(island);
            }
        }
    }
    return islands.size();
}
void distinct(vector<vector<int>>& grid, int i, int j, int r, int c, string& island) {
    int m = grid.size(), n = grid[0].size();
    if (r >= 0 && r < m && c >= 0 && c < n && grid[r][c]) {
        grid[r][c] = 0;
        island += to_string(r - i) + to_string(c - j);
        distinct(grid, i, j, r - 1, c, island);
        distinct(grid, i, j, r + 1, c, island);
        distinct(grid, i, j, r, c - 1, island);
        distinct(grid, i, j, r, c + 1, island);
    }
}
*/