// This has been adapted from the Vulkan tutorial
#include "Starter.hpp"
#include "RandomPassPosition.hpp"
#include "RandomTorchPositions.hpp"
#include "InitialPlayerPosition.hpp"
#include "gameMenu.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

// The uniform buffer objects data structures
// Remember to use the correct alignas(...) value
//        float : alignas(4)
//        vec2  : alignas(8)
//        vec3  : alignas(16)
//        vec4  : alignas(16)
//        mat3  : alignas(16)
//        mat4  : alignas(16)
static bool cKeyWasPressedLastFrame = false;
// Debug: Teleport
static int cKeyPressCount = 0;
struct menuData* menuData = nullptr; // To store the data returned by the menu

int viewModeCounter = 0;
bool isKKeyPressed = false;
bool isLKeyPressed = false;
bool isVKeyPressed = false;
bool isAnimating = false;
float animationTime = 0.0f;
const float animationDuration = 12.0f; // Duration of the entire up and down movement
const float animationHeight = 50.0f; // Height to move up

// Define a static boolean variable to keep track of the key's state
bool passObtained = false;
float passScale = 1.0f; // Initial scale
float passAnimationTime = 0.0f;
const float passAnimationDuration = 7.0f; // Duration of the animation in seconds

// GUBO: Spotlight (Torch)
struct GlobalUniformSpot {
	alignas(16) glm::vec3 lightPos;
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

// GUBO: Point (Lanterns)
struct GlobalUniformPoint {
	alignas(16) glm::vec3 lightPos[12];
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

// UBO: All textured objects
struct MeshUniformBlock {
	alignas(4) float amb;
	alignas(4) float gamma;
	alignas(16) glm::vec3 sColor;
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

// UBO: Show final screen
struct OverlayUniformBlock {
	alignas(4) float visible;
};

// GUBO: Sun
struct GlobalUniformBlock {
	alignas(16) glm::vec3 DlightDir;
	alignas(16) glm::vec3 DlightColor;
	alignas(16) glm::vec3 AmbLightColor;
	alignas(16) glm::vec3 eyePos;
};

// The vertices data structures
struct VertexMesh {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

struct VertexOverlay {
	glm::vec2 pos;
	glm::vec2 UV;
};


void renderMazeAndFloor(uint32_t currentImage, glm::mat4& View, glm::mat4& Prj, MeshUniformBlock& uboLab, DescriptorSet& DSLab, MeshUniformBlock& uboFloor, DescriptorSet DSFloor[4], MeshUniformBlock& uboTorch, DescriptorSet DSTorch[12]) {

	// Maze rendering
	glm::mat4 WorldMaze = glm::mat4(1);
	glm::mat4 WorldTorch[12];

	if (menuData->selectedMaze == 2) {
		// make the world matrix 10 times bigger
		WorldMaze = glm::scale(glm::mat4(1), glm::vec3(2.5));
	}
	else if (menuData->selectedMaze == 1) {
		// make the world matrix slightly smaller
		WorldMaze = glm::scale(glm::mat4(1), glm::vec3(0.25));
	}


	// Set the values for the uniform buffer object (UBO)
	uboLab.amb = 1.0f;
	uboLab.gamma = 180.0f;
	uboLab.sColor = glm::vec3(1.0f);
	uboLab.mvpMat = Prj * View * WorldMaze; // MVP matrix
	uboLab.mMat = WorldMaze; // Model matrix
	uboLab.nMat = glm::inverse(glm::transpose(WorldMaze)); // Normal matrix

	// Map the UBO to the GPU
	DSLab.map(currentImage, &uboLab, sizeof(uboLab), 0);

	// Floor rendering; initialize the WorldFloor matrix
	glm::mat4 WorldFloor[4];

	WorldFloor[0] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(128)), glm::vec3(-1, 0, -1));
	WorldFloor[1] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(128)), glm::vec3(0, 0, -1));
	WorldFloor[2] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(128)), glm::vec3(-1, 0, 0));
	WorldFloor[3] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(128)), glm::vec3(0, 0, 0));


	for (int i = 0; i < 4; i++) {
		uboFloor.mvpMat = Prj * View * WorldFloor[i]; // MVP matrix for the floor
		uboFloor.mMat = WorldFloor[i]; // Model matrix for the floor
		uboFloor.nMat = glm::inverse(glm::transpose(WorldFloor[i])); // Normal matrix for the floor
		uboFloor.amb = 1.0f;  // Set floor material properties
		uboFloor.gamma = 180.0f;
		uboFloor.sColor = glm::vec3(1.0f); // Set floor color
		// Map the UBO to the GPU for the floor
		DSFloor[i].map(currentImage, &uboFloor, sizeof(uboFloor), 0);
	}

	// Depending on the selected maze render torches
	if (menuData->selectedMaze == 0) {
		for (int i = 0; i < 12; i++)
		{
			WorldTorch[i] = glm::translate(glm::mat4(1), torchPosition1[i]);
		}
	}
	else if (menuData->selectedMaze == 1) {
		for (int i = 0; i < 12; i++)
		{
			WorldTorch[i] = glm::translate(glm::mat4(1), torchPosition2[i]);
		}
	}
	else {
		for (int i = 0; i < 12; i++)
		{
			WorldTorch[i] = glm::translate(glm::mat4(1), torchPosition3[i]);
		}
	}

	for (int i = 0; i < 12; i++) {
		uboTorch.mvpMat = Prj * View * WorldTorch[i]; // MVP matrix for the floor
		uboTorch.mMat = WorldTorch[i]; // Model matrix for the floor
		uboTorch.nMat = glm::inverse(glm::transpose(WorldTorch[i])); // Normal matrix for the floor
		uboTorch.amb = 1.0f;  // Set floor material properties
		uboTorch.gamma = 180.0f;
		uboTorch.sColor = glm::vec3(1.0f); // Set floor color
		// Map the UBO to the GPU for the floor
		DSTorch[i].map(currentImage, &uboTorch, sizeof(uboTorch), 0);
	}

}


// MAIN ! 
class MazeGame : public BaseProject {
public:
	glm::vec3 originalPassPosition = getRandomPassPosition(menuData->selectedMaze + 1); // Key initial position
	glm::vec3 currentPassPosition = originalPassPosition;


	int currScene = menuData->selectedLight;

	void resetApp() {
		// Reset the game state to the initial state
		gameState = 0; // back to the initial state
		currScene = menuData->selectedLight;

		// Reset other game variables as necessary
		// For example, reset player position, key obtained status, etc.
		playerPosition = getPlayerStartingPosition(menuData->selectedMaze + 1); // Reset player position to initial position
		passObtained = false; // Reset key obtained status
		viewModeCounter = 0; // Reset view mode
		cKeyWasPressedLastFrame = false;
		cKeyPressCount = 0;

		originalPassPosition = getRandomPassPosition(menuData->selectedMaze + 1); // Set to the initial position of your pass object
		currentPassPosition = originalPassPosition;

	}

protected:

	// Descriptor Layouts ["classes" of what will be passed to the shaders]
	DescriptorSetLayout DSLGubo, DSLMesh, DSLOverlay, DSLPass, DSLCharacter, DSLGuboSpot, DSLGuboPoint;

	/* Add the variable that will contain the required Descriptor Set Layout */

	// Vertex formats
	VertexDescriptor VMesh;
	VertexDescriptor VOverlay;

	/* Add the variable that will contain the required Vertex format definition */

	// Pipelines [Shader couples]
	Pipeline PMesh;
	Pipeline POverlay;
	Pipeline PPass;
	Pipeline PCharacter;
	Pipeline PSpot;
	Pipeline PPoint;

	/* Add the variable that will contain the new pipeline */

	// Models, textures and Descriptors (values assigned to the uniforms)
	// Please note that Model objects depends on the corresponding vertex structure
	Model<VertexMesh> MLab, MPass, MCharacter, MFloor, MTorch;

	/* Add the variable that will contain the model for the room */
	Model<VertexOverlay> MSplash, MEndingGame;
	DescriptorSet DSGubo, DSSplash, DSEndingGame, DSLab, DSPass, DSCharacter, DSFloor[4], DSGuboSpot, DSGuboPoint, DSTorch[12];

	/* Add the variable that will contain the Descriptor Set for the room */
	Texture TSplash, TEndingGame, TLab, TPass, TFloor[4], TTorch;

	// C++ storage for uniform variables
	MeshUniformBlock uboLab, uboPass, uboCharacter, uboFloor, uboTorch;

	/* Add the variable that will contain the Uniform Block in slot 0, set 1 of the room */
	GlobalUniformBlock gubo; // sun
	GlobalUniformSpot guboSpot; // Torch
	GlobalUniformPoint guboPoint; // Lanterns
	OverlayUniformBlock uboSplash, uboEndingGame; // start and finish

	// Other application parameters
	int gameState = 0;
	// Current aspect ratio (used by the callback that resized the window
	float Ar;
	glm::vec3 playerPosition = getPlayerStartingPosition(menuData->selectedMaze + 1); // Initial position of the player
	glm::vec3 cameraPos;
	float Yaw = glm::radians(90.f);
	float Pitch = glm::radians(0.0f);
	float Roll = glm::radians(0.0f);


	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Maze Game";
		windowResizable = GLFW_TRUE;

		switch (currScene) {
		case 0:
			initialBackgroundColor = { 0.0f, 0.6f, 0.8f, 1.0f }; // day (with sunLight)
			break;
		case 1:
			initialBackgroundColor = { 0.0f, 0.0f, 0.1f, 0.1f }; // night (with spot light)
			break;
		case 2:
			initialBackgroundColor = { 0.6f, 0.25f, 0.2f, 1.0f }; // sunset (with point light)
			break;
		}

		// Descriptor pool sizes
		/* Update the requirements for the size of the pool */
		uniformBlocksInPool = 30;
		texturesInPool = 30;
		setsInPool = 35;

		Ar = (float)windowWidth / (float)windowHeight;
	}

	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		Ar = (float)w / (float)h;
	}

	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSLMesh.init(this, {
			// this array contains the bindings:
			// first  element : the binding number
			// second element : the type of element (buffer or texture)
			//                  using the corresponding Vulkan constant
			// third  element : the pipeline stage where it will be used
			//                  using the corresponding Vulkan constant
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});

		DSLPass.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});

		DSLCharacter.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
			});

		DSLOverlay.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});
		/* Init the new Data Set Layout */

		DSLGubo.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});

		DSLGuboSpot.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});

		DSLGuboPoint.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});

		// Vertex descriptors
		VMesh.init(this, {
			// this array contains the bindings
			// first  element : the binding number
			// second element : the stride of this binging
			// third  element : whether this parameter change per vertex or per instance
			//                  using the corresponding Vulkan constant
			{0, sizeof(VertexMesh), VK_VERTEX_INPUT_RATE_VERTEX}
			}, {
				// this array contains the location
				// first  element : the binding number
				// second element : the location number
				// third  element : the offset of this element in the memory record
				// fourth element : the data type of the element
				//                  using the corresponding Vulkan constant
				// fifth  elmenet : the size in byte of the element
				// sixth  element : a constant defining the element usage
				//                   POSITION - a vec3 with the position
				//                   NORMAL   - a vec3 with the normal vector
				//                   UV       - a vec2 with a UV coordinate
				//                   COLOR    - a vec4 with a RGBA color
				//                   TANGENT  - a vec4 with the tangent vector
				//                   OTHER    - anything else
				//
				// ***************** DOUBLE CHECK ********************
				//    That the Vertex data structure you use in the "offsetoff" and
				//	in the "sizeof" in the previous array, refers to the correct one,
				//	if you have more than one vertex format!
				// ***************************************************
				{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, pos),
					   sizeof(glm::vec3), POSITION},
				{0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, norm),
					   sizeof(glm::vec3), NORMAL},
				{0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexMesh, UV),
					   sizeof(glm::vec2), UV}
			});

		VOverlay.init(this, {
				  {0, sizeof(VertexOverlay), VK_VERTEX_INPUT_RATE_VERTEX}
			}, {
			  {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, pos),
					 sizeof(glm::vec2), OTHER},
			  {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, UV),
					 sizeof(glm::vec2), UV}
			});
		/* Define the new Vertex Format */

		// Pipelines [Shader couples]
		// The second parameter is the pointer to the vertex definition
		// Third and fourth parameters are respectively the vertex and fragment shaders
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		PMesh.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFrag.spv", { &DSLGubo, &DSLMesh }); // Sun
		PPass.init(this, &VMesh, "shaders/VColorVert.spv", "shaders/VColorFrag.spv", { &DSLGubo, &DSLPass }); // Key
		PCharacter.init(this, &VMesh, "shaders/VColorVert.spv", "shaders/VColorFrag.spv", { &DSLGubo, &DSLCharacter }); // Person
		POverlay.init(this, &VOverlay, "shaders/OverlayVert.spv", "shaders/OverlayFrag.spv", { &DSLOverlay }); // Initial / Final
		POverlay.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_NONE, false);

		PSpot.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/BlinnFrag3.spv", { &DSLGuboSpot, &DSLMesh }); // Torch
		PPoint.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/BlinnFrag2.spv", { &DSLGuboPoint, &DSLMesh }); // Lanterns
		/* Create the new pipeline, using shaders "VColorVert.spv" and "VColorFrag.spv" */

		// here i create the string of the maze name and texture using the result of the menu selection
		std::string mazeName = "Models/maze" + std::to_string(menuData->selectedMaze + 1) + ".obj";
		std::string mazeTexture = "textures/maze" + std::to_string(menuData->selectedMaze + 1) + "Texture.png";


		// Models, textures and Descriptors (values assigned to the uniforms)
		// Create models
		// The second parameter is the pointer to the vertex definition for this model
		// The third parameter is the file name
		// The last is a constant specifying the file type: currently only OBJ or GLTF
		MLab.init(this, &VMesh, mazeName, OBJ);
		MFloor.init(this, &VMesh, "Models/floor.obj", OBJ);
		MPass.init(this, &VMesh, "Models/key.obj", OBJ);
		MCharacter.init(this, &VMesh, "Models/Character.obj", OBJ);
		MTorch.init(this, &VMesh, "Models/torch.mgcg", MGCG);

		// Creates a mesh with direct enumeration of vertices and indices
		MSplash.vertices = {
			{{-1.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom left
			{{-1.0f,  1.0f}, {0.0f, 1.0f}}, // Top left
			{{ 1.0f, -1.0f}, {1.0f, 0.0f}}, // Bottom right
			{{ 1.0f,  1.0f}, {1.0f, 1.0f}}  // Top right
		};
		MSplash.indices = { 0, 1, 2,    1, 2, 3 };
		MSplash.initMesh(this, &VOverlay); // Initial window

		// Creates a mesh with direct enumeration of vertices and indices
		MEndingGame.vertices = {
			{{-1.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom left
			{{-1.0f,  1.0f}, {0.0f, 1.0f}}, // Top left
			{{ 1.0f, -1.0f}, {1.0f, 0.0f}}, // Bottom right
			{{ 1.0f,  1.0f}, {1.0f, 1.0f}}  // Top right
		};
		MEndingGame.indices = { 0, 1, 2,    1, 2, 3 };
		MEndingGame.initMesh(this, &VOverlay); //Ending window

		// Create the textures
		// The second parameter is the file name
		TSplash.init(this, "textures/introMazeGame.png");
		TEndingGame.init(this, "textures/endingTextures.png");
		TLab.init(this, mazeTexture.c_str());
		TFloor[0].init(this, "textures/floorTexture.png");
		TFloor[1].init(this, "textures/floorTexture.png");
		TFloor[2].init(this, "textures/floorTexture.png");
		TFloor[3].init(this, "textures/floorTexture.png");
		TTorch.init(this, "textures/torchTexture.png");
	}

	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		PMesh.create();
		POverlay.create();
		PPass.create();
		PCharacter.create();
		PSpot.create();
		PPoint.create();
		/* Create the new pipeline */

		// Here you define the data set

		DSSplash.init(this, &DSLOverlay, {
					{0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
					{1, TEXTURE, 0, &TSplash}
			});

		DSEndingGame.init(this, &DSLOverlay, {
					{0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
					{1, TEXTURE, 0, &TEndingGame}
			});

		DSGubo.init(this, &DSLGubo, {
					{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}
			});

		DSLab.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TLab}
			});

		for (int i = 0; i < 4; i++) {
			// Initialize the floor 4 times, 4 squares
			DSFloor[i].init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TFloor[i]}
				});
		}

		for (int i = 0; i < 12; i++) {
			// Initialize the 12 torch for the selected maze
			DSTorch[i].init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TTorch}
				});
		}

		DSPass.init(this, &DSLPass, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}
			});

		DSCharacter.init(this, &DSLCharacter, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}
			});

		DSGuboSpot.init(this, &DSLGubo, {
			{0, UNIFORM, sizeof(GlobalUniformSpot), nullptr}
			});

		DSGuboPoint.init(this, &DSLGubo, {
					{0, UNIFORM, sizeof(GlobalUniformPoint), nullptr}
			});

	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup() {
		// Cleanup pipelines
		PMesh.cleanup();
		POverlay.cleanup();
		PPass.cleanup();
		PCharacter.cleanup();
		PSpot.cleanup();
		PPoint.cleanup();
		/* cleanup the new pipeline */

		// Cleanup datasets
		/* cleanup the dataset for the room */
		DSLab.cleanup();
		DSFloor[0].cleanup();
		DSFloor[1].cleanup();
		DSFloor[2].cleanup();
		DSFloor[3].cleanup();
		for (int i = 0; i < 12; i++)
		{
			DSTorch[i].cleanup();
		}
		DSPass.cleanup();
		DSCharacter.cleanup();

		DSSplash.cleanup();
		DSEndingGame.cleanup();
		DSGubo.cleanup();
		DSGuboSpot.cleanup();
		DSGuboPoint.cleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup() {
		// Cleanup textures
		TSplash.cleanup();
		TEndingGame.cleanup();
		TLab.cleanup();
		TFloor[0].cleanup();
		TFloor[1].cleanup();
		TFloor[2].cleanup();
		TFloor[3].cleanup();
		TTorch.cleanup();

		// Cleanup models
		MSplash.cleanup();
		MEndingGame.cleanup();
		MTorch.cleanup();
		MLab.cleanup();
		MFloor.cleanup();
		MPass.cleanup();
		MCharacter.cleanup();

		// Cleanup descriptor set layouts
		DSLMesh.cleanup();
		DSLOverlay.cleanup();
		/* Cleanup the new Descriptor Set Layout */

		DSLGubo.cleanup();
		DSLPass.cleanup();
		DSLCharacter.cleanup();
		DSLGuboSpot.cleanup();
		DSLGuboPoint.cleanup();

		// Destroies the pipelines
		PMesh.destroy();
		POverlay.destroy();
		PPass.destroy();
		PCharacter.destroy();
		PSpot.destroy();
		PPoint.destroy();
	}

	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures

	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {

		// Based on the light selection bind the correct GUBO
		switch (currScene) {
		case 0:
			DSGubo.bind(commandBuffer, PMesh, 0, currentImage); // SUN
			PMesh.bind(commandBuffer);
			MLab.bind(commandBuffer);
			DSLab.bind(commandBuffer, PMesh, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(MLab.indices.size()), 1, 0, 0, 0);

			MFloor.bind(commandBuffer);

			for (int i = 0; i < 4; i++) {
				DSFloor[i].bind(commandBuffer, PMesh, 1, currentImage);
				vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(MFloor.indices.size()), 1, 0, 0, 0);
			}

			break;

		case 1:
			DSGuboSpot.bind(commandBuffer, PSpot, 0, currentImage); // Torch
			PSpot.bind(commandBuffer);
			MLab.bind(commandBuffer);
			DSLab.bind(commandBuffer, PSpot, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(MLab.indices.size()), 1, 0, 0, 0);

			MFloor.bind(commandBuffer);

			for (int i = 0; i < 4; i++) {
				DSFloor[i].bind(commandBuffer, PMesh, 1, currentImage);
				vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(MFloor.indices.size()), 1, 0, 0, 0);
			}

			break;

		case 2:
			DSGuboPoint.bind(commandBuffer, PPoint, 0, currentImage); // Lantern
			PPoint.bind(commandBuffer);
			MLab.bind(commandBuffer);
			DSLab.bind(commandBuffer, PPoint, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(MLab.indices.size()), 1, 0, 0, 0);

			MFloor.bind(commandBuffer);

			for (int i = 0; i < 4; i++) {
				DSFloor[i].bind(commandBuffer, PMesh, 1, currentImage);
				vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(MFloor.indices.size()), 1, 0, 0, 0);
			}

			MTorch.bind(commandBuffer);
			for (int i = 0; i < 12; i++) {
				DSTorch[i].bind(commandBuffer, PMesh, 1, currentImage);
				vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(MTorch.indices.size()), 1, 0, 0, 0);
			}

			break;
		}
		// sets global uniforms (see below fro parameters explanation)
		DSGubo.bind(commandBuffer, PMesh, 0, currentImage);

		// binds the pipeline
		PMesh.bind(commandBuffer);
		// For a pipeline object, this command binds the corresponing pipeline to the command buffer passed in its parameter

		// the second parameter is the number of indexes to be drawn. For a Model object,
		// this can be retrieved with the .indices.size() method.

		DSGubo.bind(commandBuffer, PPass, 0, currentImage);
		PPass.bind(commandBuffer);
		MPass.bind(commandBuffer);
		DSPass.bind(commandBuffer, PPass, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MPass.indices.size()), 1, 0, 0, 0);

		DSGubo.bind(commandBuffer, PCharacter, 0, currentImage);
		PCharacter.bind(commandBuffer);
		MCharacter.bind(commandBuffer);
		DSCharacter.bind(commandBuffer, PCharacter, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MCharacter.indices.size()), 1, 0, 0, 0);

		POverlay.bind(commandBuffer);

		MSplash.bind(commandBuffer);
		DSSplash.bind(commandBuffer, POverlay, 0, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MSplash.indices.size()), 1, 0, 0, 0);

		MEndingGame.bind(commandBuffer);
		DSEndingGame.bind(commandBuffer, POverlay, 0, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MEndingGame.indices.size()), 1, 0, 0, 0);
	}

	// Here is where you update the uniforms.
	void updateUniformBuffer(uint32_t currentImage) {

		// Integration with the timers and the controllers
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);

		// Parameters
		// Camera FOV-y, Near Plane and Far Plane
		const float FOVy = glm::radians(45.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 100.0f;

		// Camera target height and distance
		const float camHeight = 1.5;
		const float camDist = 2;

		// Camera Pitch limits
		const float minPitch = glm::radians(-60.0f);
		const float maxPitch = glm::radians(60.0f);

		// Rotation and motion speed
		const float rotSpeed = glm::radians(120.0f);
		const float movSpeed = 4.0f;

		// Define rotation speed for the Pass object (degrees per second)
		const float passRotSpeed = glm::radians(90.0f); // 90 degrees per second

		// Static variable for rotation angle
		static float passRotAngle = 0.0f;

		// Update the rotation angle
		passRotAngle += passRotSpeed * deltaT;
		if (passRotAngle > glm::radians(360.0f)) {
			passRotAngle -= glm::radians(360.0f); // Wrap around at 360 degrees
		}

		glm::vec3 winningPosition = getMazeExitPosition(menuData->selectedMaze + 1); // Define winning position
		float winningRadius = 1.0f; // Define a radius for winning position proximity
		float distanceToWinning = glm::length(playerPosition - winningPosition); // Distance from character to winning position

		//matrixes initialization
		glm::mat4 Prj;
		glm::mat4 ViewPrj;
		glm::mat4 View;
		glm::mat4 World;
		glm::mat4 characterWorld;
		glm::mat4 passWorld;

		// Standard procedure to quit when the ESC key is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// Check for "L" key press
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			if (!isLKeyPressed && !isAnimating) {
				isLKeyPressed = true;
				isAnimating = true;
				animationTime = 0.0f;
			}
		}
		else {
			isLKeyPressed = false;
		}

		// Toggle camera view mode on "V" key press
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			if (!isVKeyPressed) {
				isVKeyPressed = true;
				viewModeCounter = (viewModeCounter + 1) % 3; // Cycle through 0, 1, 2
			}
		}
		else {
			isVKeyPressed = false;
		}

		if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) and (menuData->everyLightEnabled)) {
			// the user can only change the light if the selected light is fourth option "every light", otherwise the light is fixed
			if (!isKKeyPressed) {
				isKKeyPressed = true;
				currScene = (currScene + 1) % 3; // Cycle through 0, 1, 2
				RebuildPipeline();
			}
		}
		else {
			isKKeyPressed = false;
		}

		// Keyboard button for cheating
		// When 'C' is pressed once, the player is teleported to the key
		// When 'C' is pressed again, the player is teleported to the exit

		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			if (!cKeyWasPressedLastFrame) {
				cKeyPressCount++;
				if (cKeyPressCount == 1) {
					playerPosition = originalPassPosition; // Teleport to the key
				}
				else if (cKeyPressCount == 2) {
					playerPosition = winningPosition - glm::vec3(-7.0f, 0.0f, 0.0f);; // Teleport to the exit
					// Reset if needed or add additional logic for more presses
				}
			}
			cKeyWasPressedLastFrame = true;
		}
		else {
			cKeyWasPressedLastFrame = false;
		}

		// Check if the player is in the ending state and presses the "O" key
		if (gameState == 3 && glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) { // this is the key to restart the game
			// Reset the game state to the initial state
			resetApp();
		}

		// Handle the animation
		if (isAnimating) {
			animationTime += deltaT;
			if (animationTime < animationDuration / 2) {
				// Move up
				currentPassPosition.y = originalPassPosition.y + (animationHeight * (animationTime / (animationDuration / 2)));
			}
			else if (animationTime < animationDuration) {
				// Move down
				currentPassPosition.y = originalPassPosition.y + animationHeight - (animationHeight * ((animationTime - (animationDuration / 2)) / (animationDuration / 2)));
			}
			else {
				// Reset
				currentPassPosition = originalPassPosition;
				isAnimating = false;
			}
		}
		// getSixAxis() is defined in Starter.hpp in the base class.
		// It fills the float point variable passed in its first parameter with the time
		// since the last call to the procedure.
		// It fills vec3 in the second parameters, with three values in the -1,1 range corresponding
		// to motion (with left stick of the gamepad, or ASWD + RF keys on the keyboard)
		// It fills vec3 in the third parameters, with three values in the -1,1 range corresponding
		// to motion (with right stick of the gamepad, or Arrow keys + QE keys on the keyboard, or mouse)
		// If fills the last boolean variable with true if fire has been pressed:
		//          SPACE on the keyboard, A or B button on the Gamepad, Right mouse button


		// To debounce the pressing of the fire button, and start the event when the key is released
		static bool wasFire = false;
		bool handleFire = (wasFire && (!fire));
		wasFire = fire;

		glm::vec3 ux = glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
		glm::vec3 uz = glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
		playerPosition = playerPosition + movSpeed * m.x * ux * deltaT;
		playerPosition = playerPosition + movSpeed * m.y * glm::vec3(0, 1, 0) * deltaT;
		playerPosition.y = playerPosition.y < 0.0f ? 0.0f : playerPosition.y;
		playerPosition = playerPosition + movSpeed * m.z * uz * deltaT;

		//rotation
		Yaw = Yaw - rotSpeed * deltaT * r.y;
		Pitch = Pitch + rotSpeed * deltaT * r.x;
		Pitch = glm::clamp(Pitch, minPitch, maxPitch);

		Pitch = Pitch < minPitch ? minPitch :
			(Pitch > maxPitch ? maxPitch : Pitch);
		Roll = Roll - rotSpeed * deltaT * r.z;
		Roll = Roll < glm::radians(-175.0f) ? glm::radians(-175.0f) :
			(Roll > glm::radians(175.0f) ? glm::radians(175.0f) : Roll);

		// Final world matrix computaiton
		World = glm::translate(glm::mat4(1), playerPosition) * glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0, 1, 0));

		Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		glm::vec3 camPos;
		glm::vec3 camTarget;

		// Now, use viewModeCounter to determine the camera view
		if (viewModeCounter == 0) {
			// First view mode logic -> First person view
			float cameraForwardOffset = 1.0f; // Distance from the character

			// Calculate the forward direction vector based on Yaw and inverted Pitch
			glm::vec3 forwardDirection = glm::vec3(glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), -Pitch, glm::vec3(1, 0, 0)) * glm::vec4(0, 0, -1, 0));

			// Calculate camera position: Slightly ahead of the character's current position
			glm::vec3 cameraPositionOffset = forwardDirection * cameraForwardOffset;
			camPos = playerPosition + cameraPositionOffset + glm::vec3(0.0f, camHeight, 0.0f);

			// The camera looks towards where the character would look, based on current Yaw and inverted Pitch
			glm::vec3 camTarget = camPos + forwardDirection;

			// Update the view matrix
			View = glm::lookAt(camPos, camTarget, glm::vec3(0, 1, 0));

		}
		else if (viewModeCounter == 1) {
			// Second view mode logic -> third person view
			camTarget = playerPosition + glm::vec3(0, camHeight, 0);
			camPos = World * glm::vec4(0.0f, camHeight + camDist * sin(Pitch), camDist * cos(Pitch), 1.0);
			View = glm::rotate(glm::mat4(1.0f), Roll, glm::vec3(0, 0, 1)) * glm::lookAt(camPos, camTarget, glm::vec3(0, 1, 0));
		}
		else if (viewModeCounter == 2) {
			// Third view mode logic -> overhead view
			glm::vec3 overheadCameraPosition = glm::vec3(0, 100, 0); // High above the maze
			glm::vec3 overheadCameraTarget = glm::vec3(0, 0, 0); // Center of the maze
			glm::vec3 upDirection = glm::vec3(-1, 0, 0); // Up direction for top-down view
			View = glm::lookAt(overheadCameraPosition, overheadCameraTarget, upDirection);
			camPos = overheadCameraPosition; // Set camPos for the overhead view
		}
		renderMazeAndFloor(currentImage, View, Prj, uboLab, DSLab, uboFloor, DSFloor, uboTorch, DSTorch);

		float dang = Pitch + glm::radians(15.0f);
		switch (currScene) {
		case 0:
			gubo.DlightDir = glm::normalize(glm::vec3(1, 2, 3));
			gubo.DlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			gubo.AmbLightColor = glm::vec3(0.1f);
			gubo.eyePos = camPos;
			DSGubo.map(currentImage, &gubo, sizeof(gubo), 0);
			break;
		case 1:
			guboSpot.lightPos = playerPosition + glm::vec3(0, 1, 0);
			guboSpot.lightDir = glm::vec3(cos(dang) * sin(Yaw), sin(dang), cos(dang) * cos(Yaw));
			guboSpot.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			guboSpot.eyePos = camPos;
			DSGuboSpot.map(currentImage, &guboSpot, sizeof(guboSpot), 0);
			break;
		case 2:
			guboPoint.eyePos = camPos;
			guboPoint.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			if (menuData->selectedMaze == 0) {
				for (int i = 0; i < 12; i++)
				{
					guboPoint.lightPos[i] = torchPosition1[i];
				}
			}
			else if (menuData->selectedMaze == 1) {
				for (int i = 0; i < 12; i++)
				{
					guboPoint.lightPos[i] = torchPosition2[i];
				}
			}
			else {
				for (int i = 0; i < 12; i++)
				{
					guboPoint.lightPos[i] = torchPosition3[i];
				}
			}
			DSGuboPoint.map(currentImage, &guboPoint, sizeof(guboPoint), 0);
			break;
		}

		// Check if the character is close enough to the key
		float distanceToPass = glm::length(playerPosition - originalPassPosition); // Distance from character to key
		float passPickupRadius = 7.5f; // Define a radius within which the key can be picked up

		if (passObtained && distanceToWinning <= winningRadius) {

			// Optionally, you can set a flag to stop game updates or transition to a different game state
			gameState = 3; // Assuming you have different game states
		}

		if (!passObtained && distanceToPass <= passPickupRadius) {
			passObtained = true;
			passAnimationTime = 0.0f; // Start the animation
		}

		// Animate the key scaling if obtained
		if (!passObtained) {
			if (viewModeCounter == 2) { //this is the overhead view; so i need to make the key bigger in this way i can see it from the top
				// Transformations for the Pass object
				passWorld = glm::translate(glm::mat4(1), currentPassPosition)
					* glm::rotate(glm::mat4(1), passRotAngle, glm::vec3(0, 1, 0)) // Continue rotation
					* glm::scale(glm::mat4(1), glm::vec3(0.075f, 0.075f, 0.075f));
			}
			else { // these are the first and third person view; the key remains of normal size
				// Transformations for the Pass object
				passWorld = glm::translate(glm::mat4(1), currentPassPosition)
					* glm::rotate(glm::mat4(1), passRotAngle, glm::vec3(0, 1, 0)) // Continue rotation
					* glm::scale(glm::mat4(1), glm::vec3(0.015f, 0.015f, 0.015f));
			}

			uboPass.mvpMat = Prj * View * passWorld;
			uboPass.mMat = passWorld;
			uboPass.nMat = glm::inverse(glm::transpose(passWorld));
			/* map the uniform data block to the GPU */
			DSPass.map(currentImage, &uboPass, sizeof(uboPass), 0);

		}
		else {
			passAnimationTime += deltaT;
			passScale = glm::max(0.0f, 1.0f - (passAnimationTime / passAnimationDuration));
			if (passAnimationTime >= passAnimationDuration) {
				passScale = 0.0f; // Key has completely disappeared
			}
		}

		// Update the key uniform buffer only if the key is not fully scaled down
		if (passScale > 0.0f && passObtained) {
			glm::mat4 passWorld = glm::translate(glm::mat4(1.0f), originalPassPosition) // Key's position
				* glm::scale(glm::mat4(1.0f), glm::vec3(passScale * 0.015f)); // Apply dynamic scale and make the key invisible

			uboPass.mvpMat = Prj * View * passWorld;
			uboPass.mMat = passWorld;
			uboPass.nMat = glm::inverse(glm::transpose(passWorld));
			DSPass.map(currentImage, &uboPass, sizeof(uboPass), 0);
		}


		if (viewModeCounter == 2) { // this is the overhead view so i make the character bigger in order to make it more visible
			characterWorld = glm::translate(glm::mat4(1), playerPosition) *
				glm::rotate(glm::mat4(1.0f), Yaw + glm::radians(90.0f), glm::vec3(0, 1, 0)) *
				glm::scale(glm::mat4(1), glm::vec3(6.5f, 6.5f, 6.5f)); // Scale the character by a factor of 2; and the rotation is 90 degrees so in this case the character is looking forward

		}
		else { //this is the first pr third person view so the character is normal size
			characterWorld = glm::translate(glm::mat4(1), playerPosition) *
				glm::rotate(glm::mat4(1.0f), Yaw + glm::radians(90.0f), glm::vec3(0, 1, 0)) *
				glm::scale(glm::mat4(1), glm::vec3(2.0f, 2.0f, 2.0f)); // Scale the character by a factor of 2; and the rotation is 90 degrees so in this case the character is looking forward
		}

		uboCharacter.mvpMat = Prj * View * characterWorld;
		uboCharacter.mMat = World;
		uboCharacter.nMat = glm::inverse(glm::transpose(World));
		DSCharacter.map(currentImage, &uboCharacter, sizeof(uboCharacter), 0);

		uboSplash.visible = (gameState == 0) ? 1.0f : 0.0f;
		DSSplash.map(currentImage, &uboSplash, sizeof(uboSplash), 0);

		uboEndingGame.visible = (gameState == 3) ? 1.0f : 0.0f;
		DSEndingGame.map(currentImage, &uboEndingGame, sizeof(uboEndingGame), 0);

		switch (gameState) {		// main state machine implementation
		case 0: // initial state - show splash screen

			if (handleFire) {
				// Render the game menu and handle the result
				gameState = 1;	// jump to the wait key state
			}
			break;

		case 1: // wait key state
			break;

		case 3: // ending state - show end game screen
			break;
		}
	}
};


int main() {

	initializeTorchPosition();
	// call menu function
	menuData = renderGameMenu(0); // zero is the initial state of the menu; this menu is displayed only at the beginning of the first game
	bool quitGame = false;

	if (menuData->selectedMaze == -1) {
		// The user closed the window without selecting a maze so the default value is 0
		menuData->selectedMaze = 0;
	}

	if (menuData->selectedLight == -1) {
		// The user closed the window without selecting a light so the default value is 0
		menuData->selectedLight = 0;
	}	// so default values are the first maze with the sun light

	if (menuData->selectedLight == 3) {
		// The user selected the fourth option "every light" so i will force the user to start from sunLight but he can change the light during the game
		menuData->selectedLight = 0;
		menuData->everyLightEnabled = true;
	}
	MazeGame* app;

	while (quitGame == false) {
		app = new MazeGame();
		app->resetApp();
		try {
			app->run();
			delete app;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}

		menuData = renderGameMenu(3); // one is the final state of the menu; this menu is displayed only at the end of the game
		quitGame = menuData->quitTheGame;

		if (menuData->selectedMaze == -1) {
			// The user closed the window without selecting a maze so the default value is 0
			menuData->selectedMaze = 0;
		}

		if (menuData->selectedLight == -1) {
			// The user closed the window without selecting a light so the default value is 0
			menuData->selectedLight = 0;
		} // so default values are the first maze with the sun light

		if (menuData->selectedLight == 3) {
			// The user selected the fourth option "every light" so i will force the user to start from sunLight but he can change the light during the game
			menuData->selectedLight = 0;
			menuData->everyLightEnabled = true;
		}

	}

	delete menuData;
	return EXIT_SUCCESS;
}