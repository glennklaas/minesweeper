#pragma once
#include "Global.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class Cell
{
	int x, y;
	bool has_mine;
	bool is_revealed;
	int num_neighbors;
	bool has_flag;
	bool has_question;


public:
	Cell(int x, int y);
	void reset();
	bool get_has_mine();
	bool get_is_revealed();
	void set_has_mine(bool i_value);
	game_state_type::game_state reveal_cell(std::vector<Cell>& cells);
	void reveal_mine(game_state_type::game_state);
	void set_num_neighbors(int i_num_neighbors);
	int  get_num_neighbors();
	bool get_has_flag();
	void set_has_flag(bool hasFlag);
	bool get_has_question();
	void set_has_question(bool hasQuestion);
	int get_x();
	int get_y();

};
