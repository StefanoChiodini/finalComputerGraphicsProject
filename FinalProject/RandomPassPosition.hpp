// in this file i will calculate the random position of the pass; there are some different hardcodedpositions; depending on the maze i will choose a dictionary.
// every dictionary has 10 different positions; i will choose one of them randomly.

#ifndef RANDOMPASSPOSITION_HPP
#define RANDOMPASSPOSITION_HPP

#include <cstdlib> // For srand() and rand()
#include <ctime>   // For time()
#include <map>
#include <glm/glm.hpp>

// Create a dictionary with int keys and glm::vec3 values
std::map<int, glm::vec3> maze1KeyCoordinates;
std::map<int, glm::vec3> maze2KeyCoordinates;
std::map<int, glm::vec3> maze3KeyCoordinates;


// Initialize the dictionary with some hardcoded coordinates
void initializeCoordinates() {
    // Coordinates for maze 1
    maze1KeyCoordinates[1] = glm::vec3(21.1332, 5.35717, -37.2648);
    maze1KeyCoordinates[2] = glm::vec3(21.0749, 5.35717, 16.0006);
    maze1KeyCoordinates[3] = glm::vec3(-8.51145, 5.35717, -0.616729);
    maze1KeyCoordinates[4] = glm::vec3(-21.4684, 5.35717, -16.0267);
    maze1KeyCoordinates[5] = glm::vec3(-37.3851, 5.35717, 20.2177);

    // Coordinates for maze 2
    maze2KeyCoordinates[1] = glm::vec3(10.8378, 0, 10.2204);
    maze2KeyCoordinates[2] = glm::vec3(4.26218, 0, -10.3494);
    maze2KeyCoordinates[3] = glm::vec3(-1.51123, 0, -2.6596);
    maze2KeyCoordinates[4] = glm::vec3(-12.3179, 0, 1.00438);
    maze2KeyCoordinates[5] = glm::vec3(-5.32916, 0, 11.6213);

    // Coordinates for maze 3
    maze3KeyCoordinates[1] = glm::vec3(19.3372, 0, 6.19042);
    maze3KeyCoordinates[2] = glm::vec3(-1.82148, 0, 14.905);
    maze3KeyCoordinates[3] = glm::vec3(-14.0044, 0, -23.1403);
    maze3KeyCoordinates[4] = glm::vec3(-6.49174, 0, 6.33185);
    maze3KeyCoordinates[5] = glm::vec3(-14.8171, 0, 6.40876);
}

// Get a random position from the dictionary
glm::vec3 getRandomPassPosition(int mazeNumber) {
    // Initialize the random seed once
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(NULL)));
        initialized = true;
        initializeCoordinates();
    }

    std::map<int, glm::vec3>* selectedMazeCoordinates;

    // Select the appropriate dictionary based on the maze number
    switch (mazeNumber) {
    case 1:
        selectedMazeCoordinates = &maze1KeyCoordinates;
        break;
    case 2:
        selectedMazeCoordinates = &maze2KeyCoordinates;
        break;
    case 3:
        selectedMazeCoordinates = &maze3KeyCoordinates;
        break;
    default:
        throw std::invalid_argument("Invalid maze number");
    }

    // Get a random key from the selected dictionary
    int randomKey = rand() % selectedMazeCoordinates->size() + 1;

    // Get the value for the random key
    return (*selectedMazeCoordinates)[randomKey];
}

#endif