#include <algorithm>
#include "Grid.hpp"
#include "Global.hpp"
#include <SFML/Graphics.hpp>
#include "Cell.hpp"
#include <iostream>
#include <string>

Grid::Grid(int nbr_mines) {

	// Load the game textures into the map for later retrieval
	load_textures();
	
	first_cell_revealed = false;
	this->mines_covered = 0;

	//state = game_state_type::running;

	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			Cell cell(x,y);
			cells.push_back(cell);
		}
	}
	
}

game_state_type::game_state Grid::reveal_cell(int x, int y) {
	
	if (first_cell_revealed == false) {
		
		// Create a temporary blank grid
		std::vector<Cell> mine_vector;
		for (int tmp_y = 0; tmp_y < GRID_SIZE; tmp_y++) {
			for (int tmp_x = 0; tmp_x < GRID_SIZE; tmp_x++) {
				Cell cell(tmp_x, tmp_y);
				mine_vector.push_back(cell);
			}
		}

		// Remove the first cell and it's neighbors -- has to be done highest to lowest
		for (int off_y = y + 1; off_y >= y - 1; off_y--) {
			for (int off_x = x + 1; off_x >= x - 1; off_x--) {
				std::cout << "  x,y:" << off_x << "," << off_y << std::endl;
				if (
					x + off_x >= 0 && y + off_y >= 0 && 
					x + off_x < GRID_SIZE  && y + off_y < GRID_SIZE
					
					) {
					std::cout << "removing:" << off_x + GRID_SIZE * off_y << std::endl;
					mine_vector.erase(
						mine_vector.begin() + off_x + GRID_SIZE * off_y
					);
				}
			}
			
		}

		std::random_shuffle(mine_vector.begin(), mine_vector.end());
		for (int z = 0; z < NUM_MINES; z++) {
			cells[mine_vector.back().get_x() + GRID_SIZE * mine_vector.back().get_y()].set_has_mine(true);
			mine_vector.pop_back();
		}


		// Calculate neighbors
		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				int tmp_neighbor_count = 0;
				for (int y_off = -1; y_off <= 1; y_off++) {
					for (int x_off = -1; x_off <= 1; x_off++) {
						if (!cells[x + GRID_SIZE * y].get_has_mine()) {
							if (x + x_off >= 0 && x + x_off < GRID_SIZE && y + y_off >= 0 && y + y_off < GRID_SIZE) {
								if (cells[(x + x_off) + GRID_SIZE * (y + y_off)].get_has_mine()) {
									tmp_neighbor_count++;
								}
							}
						}
					}
				}
				cells[x + GRID_SIZE * y].set_num_neighbors(tmp_neighbor_count);
			}

			// We have setup the board after the first
			first_cell_revealed = true;
		}
	}
	
	// Tell the cell that was clicked to get in here to reveal itself
	return cells[x + GRID_SIZE * y].reveal_cell(cells);
	
}

void Grid::load_textures() {
	
	// Retrieve individual textures that need to be manually named
	sf::Texture texture_unrevealed, texture_revealed, texture_flag, texture_question,
				texture_mine, texture_minered, texture_smile_up;

	texture_unrevealed.loadFromFile("./minesweeper-sprites.png",sf::IntRect(0, 51, 16, 16) );
	textures.insert(std::pair<std::string, sf::Texture>("unrevealed",texture_unrevealed));

	texture_revealed.loadFromFile("./minesweeper-sprites.png",sf::IntRect(17, 51, 16, 16) );
	textures.insert(std::pair<std::string, sf::Texture>("revealed",texture_revealed));

	texture_flag.loadFromFile("./minesweeper-sprites.png",sf::IntRect(34, 51, 16, 16) );
	textures.insert(std::pair<std::string, sf::Texture>("flag",texture_flag));

	texture_question.loadFromFile("./minesweeper-sprites.png",sf::IntRect(51, 51, 16, 16) );
	textures.insert(std::pair<std::string, sf::Texture>("question",texture_question));

	texture_mine.loadFromFile("./minesweeper-sprites.png",sf::IntRect(85, 51, 16, 16) );
	textures.insert(std::pair<std::string, sf::Texture>("mine",texture_mine));
	texture_minered.loadFromFile("./minesweeper-sprites.png",sf::IntRect(102, 51, 16, 16) );
	textures.insert(std::pair<std::string, sf::Texture>("mine_red",texture_minered));

	texture_smile_up.loadFromFile("./minesweeper-sprites.png",sf::IntRect(0, 24, 26, 26) );
	textures.insert(std::pair<std::string, sf::Texture>("smile_up",texture_smile_up));

	texture_smile_up.loadFromFile("./minesweeper-sprites.png",sf::IntRect(27, 24, 51-27, 49-24) );
	textures.insert(std::pair<std::string, sf::Texture>("smile_down",texture_smile_up));

	// Load neighbor numbers and use their value as the key - suffix N is for neighbor
	for( int i = 0; i < 8; i++ ) {
		sf::Texture curTexture;
		curTexture.loadFromFile("./minesweeper-sprites.png",sf::IntRect((i * 16) + i, 68, 16, 16));
		textures.insert(std::pair<std::string, sf::Texture>(std::to_string(i+1) + "N",curTexture));
	}

	// 7 Segment display for count down and timer - suffix D is for display
	for( int i = 0; i < 10; i++ ) {
		sf::Texture curTexture;
		curTexture.loadFromFile("./minesweeper-sprites.png",sf::IntRect((i * 13) + i, 0, 13, 23));
		textures.insert(std::pair<std::string, sf::Texture>(std::to_string(i+1) + "D",curTexture));
	}
	
}

void Grid::draw(sf::RenderWindow& i_mainWindow) {

	// Draw the scoreboard
	// 	Scoreboard rect
	sf::RectangleShape score_shape(sf::Vector2f(WINDOW_WIDTH, SCOREBOARD_HEIGHT-1));
	i_mainWindow.draw(score_shape);

	//	Scoreboard guy
	sf::RectangleShape emoji_shape(sf::Vector2f(100, 100));
	emoji_shape.setOrigin(sf::Vector2f(26,26));
	emoji_shape.setPosition(WINDOW_WIDTH/2 - 26 ,SCOREBOARD_HEIGHT/2 - 26);
	emoji_shape.setTexture( &textures.find("smile_up")->second );
	i_mainWindow.draw(emoji_shape);

	// Draw the grid
	sf::RectangleShape cell_shape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
						
			cell_shape.setPosition(static_cast<float>(CELL_SIZE * x), static_cast<float>(CELL_SIZE * y + SCOREBOARD_HEIGHT));
			
			if (cells[x + GRID_SIZE * y].get_is_revealed()) {
				if (cells[x + GRID_SIZE * y].get_has_mine()) {
					cell_shape.setTexture( &textures.find("mine_red")->second );
				}
				else {
					cell_shape.setTexture( &textures.find("revealed")->second );
				}
			} else {
				if (cells[x + GRID_SIZE * y].get_has_flag()) {
					cell_shape.setTexture( &textures.find("flag")->second );
				} else if (cells[x + GRID_SIZE * y].get_has_question()) { 
					cell_shape.setTexture( &textures.find("question")->second );
				}else {
					cell_shape.setTexture( &textures.find("unrevealed")->second );
				}
			}
			
			#ifdef DEBUGGING
			// Debug only - indicate where mines are
			if (cells[x + GRID_SIZE * y].get_has_mine() && !cells[x + GRID_SIZE * y].get_is_revealed())
				cell_shape.setFillColor(sf::Color(175, 175, 175));
			#endif
			
			// Print number of neighbors
			if (cells[x + GRID_SIZE * y].get_num_neighbors() > 0) {
				
				std::string key = std::to_string(cells[x + GRID_SIZE * y].get_num_neighbors()) + "N";
				cell_shape.setTexture( &textures.find(key.c_str())->second);
			}

			// Draw the cell itself
			i_mainWindow.draw(cell_shape);
			 
		}
	}
}

void Grid::toggle_flag(int i_x, int i_y) {
	if(!cells[i_x + GRID_SIZE * i_y].get_is_revealed()) {
		if (cells[i_x + GRID_SIZE * i_y].get_has_flag()) { // Flag -> Question
			cells[i_x + GRID_SIZE * i_y].set_has_flag(false);
			cells[i_x + GRID_SIZE * i_y].set_has_question(true);
			mines_covered -= 1;
		}else if (cells[i_x + GRID_SIZE * i_y].get_has_question()) { // Question - > Revealed
			cells[i_x + GRID_SIZE * i_y].set_has_flag(false);
			cells[i_x + GRID_SIZE * i_y].set_has_question(false);
		}else {
			cells[i_x + GRID_SIZE * i_y].set_has_flag(true); // Revealed -> Flag
			mines_covered += 1;
		}
	}
}

void Grid::reveal_all_mines(game_state_type::game_state state) {
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			if (cells[x + GRID_SIZE * y].get_has_mine() ) {
				cells[x + GRID_SIZE * y].reveal_mine(state);
			}
		}
	}
}

void Grid::process_scoreboard_click(float x, float y) {

	if(x){

	
	}
}

int Grid::get_mines_covered() {

	return this->mines_covered;
}

void Grid::set_mines_covered(int nbr_mines) {

	this->mines_covered = nbr_mines;
}
