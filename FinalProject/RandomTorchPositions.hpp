// in this file i will write some random positions of some torches; there are some different hardcodedpositions; depending on the maze i will choose a dictionary.
// once selected the dictionary, i will insert all the torches in the maze, in the positions written inside the dictionary.
#include <glm/glm.hpp>

#ifndef RANDOMTORCHPOSITIONS_HPP
#define RANDOMTORCHPOSITIONS_HPP

glm::vec3 torchPosition1[12];
glm::vec3 torchPosition2[12];
glm::vec3 torchPosition3[12];

void initializeTorchPosition() {

	// Labirinto 1 luci
	torchPosition1[0] = glm::vec3(39.5, 9.5, -0.146264);
	torchPosition1[1] = glm::vec3(33.4252, 8.41648, 14.5645);
	torchPosition1[2] = glm::vec3(16, 8.41648, -1.2546);
	torchPosition1[3] = glm::vec3(28.6832, 8.41648, -17.1862);
	torchPosition1[4] = glm::vec3(26.5967, 8.41648, -38.3707);
	torchPosition1[5] = glm::vec3(19.2539, 8.41648, -21.3459);
	torchPosition1[6] = glm::vec3(3.44414, 8.41648, -34.2745);
	torchPosition1[7] = glm::vec3(10.9536, 8.41648, -26.1043);
	torchPosition1[8] = glm::vec3(7.97971, 8.41648, -12.8518);
	torchPosition1[9] = glm::vec3(16.4741, 8.41648, 6.72083);
	torchPosition1[10] = glm::vec3(-16.8132, 8.41648, 6.89392);
	torchPosition1[11] = glm::vec3(-15.6691, 8.41648, -1.28796);

	// Labirinto 2 Luci
	torchPosition2[0] = glm::vec3(11.094, 2.78552, -0.362499);
	torchPosition2[1] = glm::vec3(5.48342, 2.78552, 7.328);
	torchPosition2[2] = glm::vec3(9.56556, 2.99048, -10.8873);
	torchPosition2[3] = glm::vec3(10.9391, 2.78552, 9.25317);
	torchPosition2[4] = glm::vec3(-6.16518, 2.78552, 7.84024);
	torchPosition2[5] = glm::vec3(-9.87154, 2.78552, 9.56594);
	torchPosition2[6] = glm::vec3(-1.32261, 2.78552, 3.95251);
	torchPosition2[7] = glm::vec3(-2.65385, 2.78552, 1.88288);
	torchPosition2[8] = glm::vec3(7.72963, 2.78552, -8.85465);
	torchPosition2[9] = glm::vec3(-5.73847, 2.78552, -12.3971);
	torchPosition2[10] = glm::vec3(-7.91691, 2.78552, 2.01044);
	torchPosition2[11] = glm::vec3(-8.81441, 2.78552, -8.77266);

	// Labirinto 3 luci
	torchPosition3[0] = glm::vec3(4.23734, 6.27828, 8.74588);
	torchPosition3[1] = glm::vec3(19.9074, 6.27828, 9.01652);
	torchPosition3[2] = glm::vec3(16.5159, 6.27828, -16.1645);
	torchPosition3[3] = glm::vec3(5.35123, 6.27828, -7.90683);
	torchPosition3[4] = glm::vec3(-0.640561, 6.27828, 4.51482);
	torchPosition3[5] = glm::vec3(-14.2102, 6.27828, -7.76685);
	torchPosition3[6] = glm::vec3(-19.5405, 6.27828, 8.83247);
	torchPosition3[7] = glm::vec3(-11.5006, 6.27828, 13.1638);
	torchPosition3[8] = glm::vec3(-20.8257, 6.27828, -3.99824);
	torchPosition3[9] = glm::vec3(-13.5317, 6.27828, -20.1855);
	torchPosition3[10] = glm::vec3(-1.3132, 6.27828, -7.85135);
	torchPosition3[11] = glm::vec3(24.145, 6.27828, -24.5881);

}
#endif