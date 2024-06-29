// this file contains and returns the initial position of the player for each maze.

#ifndef INITIALPLAYERPOSITION_HPP
#define INITIALPLAYERPOSITION_HPP

#include <map>
#include <glm/glm.hpp>


glm::vec3 getPlayerStartingPosition(int selectedMaze) {
	// this function returns the initial position of the player for each maze. (the player will spawn at the entrance of the maze)
	if (selectedMaze == 1) {
		return glm::vec3(50, 5, 0);
	}
	else if (selectedMaze == 2) {
		return glm::vec3(-19.2577, 0, -8.63351);
	}
	else if (selectedMaze == 3) {
		return glm::vec3(2.02956, 0, 32.4812);
	}
}

glm::vec3 getMazeExitPosition(int selectedMaze) {
	// this function returns the exit position of the maze for each maze. (if the player reaches this position, the game will end)
	if (selectedMaze == 1) {
		return glm::vec3(-40, 5, 0);
	}
	else if (selectedMaze == 2) {
		return glm::vec3(11.6164, 0, 8.09081);
	}
	else if (selectedMaze == 3) {
		return glm::vec3(-1.65614, 0, -23.4206);
	}
}

#endif
