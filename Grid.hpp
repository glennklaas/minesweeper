#pragma once
#include "Grid.hpp"
#include "Cell.hpp"
#include "Global.hpp"
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class Grid
{	
	std::vector<Cell> cells;
	std::map<std::string, sf::Texture> textures;
	int mines_covered;

public:
	Grid(int i_num_mines);
	void draw(sf::RenderWindow& i_mainWindow);
	game_state_type::game_state reveal_cell(int i_x, int i_y);
	void toggle_flag(int i_x, int i_y);
	void reveal_all_mines(game_state_type::game_state);
	int get_mines_covered();
	void set_mines_covered(int nbr_mines);

	// TODO: Support the 
	void reset_game(void);
	void process_scoreboard_click(float x, float y);

private:
	bool first_cell_revealed;
	void load_textures();
};