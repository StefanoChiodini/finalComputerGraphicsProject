void GameLogic(Assignment07* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World) {
    // The procedure must implement the game logic  to move the character in third person.
    // Input:
    // <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
    // <float Ar> Aspect ratio of the current window (for the prj Matrix)
    // Output:
    // <glm::mat4 &ViewPrj> the view-prj matrix of the camera
    // <glm::mat4 &World> the world matrix of the object
    // Parameters
    // Camera FOV-y, Near Plane and Far Plane
    const float FOVy = glm::radians(45.0f);
    const float nearPlane = 0.1f;
    const float farPlane = 100.f;

    // Player starting point
    const glm::vec3 StartingPosition = glm::vec3(-41.50, 0.0, -19.5);

    // Camera target height and distance
    const float camHeight = 0.25;
    const float camDist = 1.5;
    // Camera Pitch limits
    const float minPitch = glm::radians(-8.75f);
    const float maxPitch = glm::radians(60.0f);
    // Rotation and motion speed
    const float ROT_SPEED = glm::radians(120.0f);
    const float MOVE_SPEED = 2.0f;

    // Integration with the timers and the controllers
    // returns:
    // <float deltaT> the time passed since the last frame
    // <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
    // <glm::vec3 r> the state of the rotation axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
    // <bool fire> if the user has pressed a fire button (not required in this assginment)
    float deltaT;
    glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
    bool fire = false;
    A->getSixAxis(deltaT, m, r, fire);

    // Game Logic implementation
    // Current Player Position - statc variables make sure thattheri value remain unchanged in subsequent calls to the procedure
    static glm::vec3 Pos = StartingPosition;

    //perspective matrix
    glm::mat4 prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);

    glm::vec3 cameraPos = Pos + glm::vec3(0.0f, camHeight, camDist);

    glm::vec3 cameraDir = glm::normalize(Pos - cameraPos);

    glm::vec3 cameraUp = glm::vec3(0.0f, -1.0f, 0.0f);

    // Compute view matrix using Look-at model
    glm::mat4 view = glm::lookAt(cameraPos, Pos, cameraUp);
    // To be done in the assignment
    ViewPrj = prj * view;

    static float yaw = 0.0f;
    static float pitch = 0.0f;
    static float roll = 0.0f;
    static glm::vec3 target = glm::vec3(0.0f, 0.25f, 1.5f);

    glm::vec3 ux = glm::vec3(glm::rotate(glm::mat4(1), yaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1));
    glm::vec3 uy = glm::vec3(0, 1, 0);
    glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), yaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));

    // Update yaw and pitch
    yaw += ROT_SPEED * r.y * deltaT;
    pitch += ROT_SPEED * r.x * deltaT;
    roll += ROT_SPEED * r.z * deltaT;
    Pos += -ux * MOVE_SPEED * m.x * deltaT;
    Pos += -uy * MOVE_SPEED * m.y * deltaT; 
    Pos += uz * MOVE_SPEED * m.z * deltaT;

    // Clamp pitch
    pitch = glm::clamp(pitch, -8.7f, 60.0f);

    // Compute world matrix using walk model
    glm::mat4 rotx, roty, rotz;
    roty = glm::rotate(glm::mat4(1), yaw, glm::vec3(0, 1, 0));
    rotx = glm::rotate(glm::mat4(1), pitch, glm::vec3(1, 0, 0));
    rotz = glm::rotate(glm::mat4(1), roll, glm::vec3(0, 0, 1));
    World = glm::translate(glm::mat4(1.0f), Pos) * rotx * roty * rotz;
}