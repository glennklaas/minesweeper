#include "Global.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "Cell.hpp"

int main() {
    
    // Create a window for the app and set frame rate
    sf::RenderWindow mainWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT + SCOREBOARD_HEIGHT), "MineSweeper");
    mainWindow.setFramerateLimit(60);

    // Create a grid
    Grid grid(NUM_MINES);

    // Game loop
    while(mainWindow.isOpen()) {
        
        // Capture the location of the mouse pointer relative to window for this frame
        sf::Vector2i position = sf::Mouse::getPosition(mainWindow);
        position.y -= SCOREBOARD_HEIGHT;  // Adjust for the height of the scoreboard

        #ifdef DEBUGGING
        // For debugging purposes, write the X,Y coordinates to the console
        if(position.x >= 0 && position.y >=0 && position.x <= GRID_SIZE * CELL_SIZE && position.y <= GRID_SIZE * CELL_SIZE) {
            //TODO: need to change CELL_SIZE.  This doesn't work after window resize.
            std::cout << " XCELL:" << (position.x) / CELL_SIZE << " YCELL:" << (position.y) / CELL_SIZE << std::endl;  
        }
        #endif

        // Handle events
        sf::Event event;
        while(mainWindow.pollEvent(event)) {
            
            //Close the game
            if(event.type == sf::Event::Closed)
                mainWindow.close();
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
                mainWindow.close();
            
            //Reset the game and start from the beginning
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                grid = NULL;
                Grid grid(NUM_MINES);
            }

            if(event.type == sf::Event::MouseWheelScrolled){
                std::cout << "Wheel moved" << std::endl;
            }
            
            // Left button pressed - Reveal a cell
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) { // left click
                if(position.x >= 0  && position.y >= 0  && position.x <= GRID_SIZE * CELL_SIZE && position.y <= GRID_SIZE * CELL_SIZE) { // within the grid
                    switch(grid.reveal_cell((position.x) / CELL_SIZE, (position.y) / CELL_SIZE)) {
                        case game_state_type::over_lost:
                            std::cout << "Game lost" << std::endl;
                            grid.reveal_all_mines(game_state_type::over_lost);
                            break;
                        case game_state_type::over_won:
                            std::cout << "Game won" << std::endl;
                            break;
                        case game_state_type::running:
                            //std::cout << "Game running" << std::endl;
                            break;
                        default:
                            std::cout << "game_state_type not found!!!!!!!!!!!!" << std::endl;
                            break;
                    }
                } else {
                    if (position.x >= 0 && position.y >= -SCOREBOARD_HEIGHT && position.x <= GRID_SIZE * CELL_SIZE && position.y <= SCOREBOARD_HEIGHT * CELL_SIZE) {
                        
                        // Reset button was pressed in scoreboard region
                        std::cout << "Clicked in scoreboard." << std::endl;
                        grid.reset_game();

                    }
                } 
            }

            // Right button pressed - Set or clear a flag
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                if (position.x >= 0 && position.y >= 0 && position.x <= GRID_SIZE * CELL_SIZE && position.y <= SCOREBOARD_HEIGHT * CELL_SIZE) // within the 
                    grid.toggle_flag((position.x) / CELL_SIZE, (position.y) / CELL_SIZE);
                    std::cout << NUM_MINES - grid.get_mines_covered() << std::endl;
            }

        }
        
        // Update window with frame
        mainWindow.clear();
        grid.draw(mainWindow);       
        mainWindow.display();
    }

    return 0;
}