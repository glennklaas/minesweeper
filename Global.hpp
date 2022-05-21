#pragma once

// Game Parameters
#define GRID_SIZE 16
#define CELL_SIZE 70
#define WINDOW_WIDTH GRID_SIZE * CELL_SIZE
#define WINDOW_HEIGHT WINDOW_WIDTH
#define SCOREBOARD_HEIGHT 150
#define NUM_MINES 16

//#define DEBUGGING

namespace game_state_type {
	enum game_state { running, over_won, over_lost };
}

