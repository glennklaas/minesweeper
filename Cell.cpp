#include "Global.hpp"
#include <vector>
#include "Cell.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>


// The constructor runs when a new instance is created
Cell::Cell(int x, int y) {
	this->x = x;
	this->y = y;
	reset();
}

game_state_type::game_state Cell::reveal_cell(std::vector<Cell>& grid) {
	static int stack = 0;
	// If I'm not already revealed and I'm not a mine
	if (this->is_revealed == false && !this->get_has_mine() ) {
		
		// Go ahead and reveal myself
		this->is_revealed = true;
		
		if ( this->num_neighbors == 0 ) { // Only cells that don't have neighboring mines should try to clear neighbors
			for ( int b = -1; b < 2; b++ ) {
				for ( int a = -1; a < 2; a++ ) {
					
					// Making sure we don't submit ourself or cells outside the grid
					if ( ( 0 == a && 0 == b ) || ( this->x + a < 0 || this->y + b < 0 || this->x + a > GRID_SIZE -1 || this->y + b > GRID_SIZE -1) ) {
						#ifdef DEBUGGING
						std::cout << "Depth[" << stack << "] Caller[" << x << "," << y << "] Skipping (x=" << x + a << ") (y=" << y + b << ")" << std::endl;
						#endif
					}
					else {
						#ifdef DEBUGGING
						std::cout << "Depth[" << stack << "] Caller[" << x << "," << y << "] Calling reveal on(x = " << x + a << ") (y = " << y + b << ")" << std::endl;
						#endif
						stack++;
						grid[(x + a) + GRID_SIZE * (y + b)].reveal_cell(grid);
						stack--;
					}
				}
			}
		}
	}
	else {
		
		if (this->has_mine) {
			#ifdef DEBUGGING
			std::cout << "Depth[" << stack << "] Caller[" << x << "," << y << "] Skipping is MINE" << std::endl;
			#endif
			
			this->is_revealed = true;
			return game_state_type::over_lost; // A mine was revealed, game over
		}
	}
	return game_state_type::running; // The reveal did not end this game
}

void Cell::reveal_mine(game_state_type::game_state) {
	this->is_revealed = true;
}

bool Cell::get_has_mine() {
	return this->has_mine;
}

bool Cell::get_is_revealed() {
	return this->is_revealed;
}

void Cell::reset() {
	this->has_mine = false;
	this->is_revealed = false;
	this->num_neighbors = 0;
	this->has_flag = false;
	this->has_question = false;
}

void Cell::set_has_mine(bool i_value) {
	this->has_mine = i_value;
}

int Cell::get_x() {
	return this->x;
}

int Cell::get_y() {
	return this->y;
}

void Cell::set_num_neighbors(int i_num_neighbors) {
	this->num_neighbors = i_num_neighbors;
}

int Cell::get_num_neighbors() {
	return this->num_neighbors;
}

bool Cell::get_has_flag() {
	return this->has_flag;
}

void Cell::set_has_flag(bool hasFlag) {
	this->has_flag = hasFlag;
}

bool Cell::get_has_question() {
	return this->has_question;
}

void Cell::set_has_question(bool hasQuestion) {
	this->has_question = hasQuestion;
}
