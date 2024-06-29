#include <GLFW/glfw3.h>

struct menuData {
    bool closed; // To track if the menu is closed
    int selectedMaze; // To store the selected maze
    int selectedLight; // To store the selected light option
    bool everyLightEnabled; // To track if the user can change light during the game
    bool quitTheGame; // To track if the user wants to quit the game
};

// Function prototypes (as per your existing file)
menuData* renderGameMenu(int gameState);

void mainLoops(GLFWwindow* window, int gameState);