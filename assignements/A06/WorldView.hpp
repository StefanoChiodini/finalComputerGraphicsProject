//this function creates the scenarion(the room)
glm::mat4 MakeViewProjectionMatrix(float Ar, float Alpha, float Beta, float Rho, glm::vec3 Pos) {
	// Creates a view projection matrix, with near plane at 0.1, and far plane at 50.0, and
	// aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
	// vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
	// defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
	// elevation (Pitch), and <Rho> the roll.
	glm::mat4 M = glm::mat4(1);
	glm::mat4 Mv =
		glm::rotate(glm::mat4(1.0), -Rho, glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1.0), -Beta, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0), -Alpha, glm::vec3(0, 1, 0)) *
		glm::translate(glm::mat4(1.0), -Pos);
	glm::mat4 Mp = glm::perspective(45.0f, Ar, 0.1f, 50.0f);
	Mp[1][1] *= -1; // flips the Y axis
	M = Mp * Mv;

	return M;
}

//this function allows to move the key around the room
glm::mat4 MakeWorldMatrix(glm::vec3 pos, glm::quat rQ, glm::vec3 size) {
	// creates and returns a World Matrix that positions the object at <pos>,
	// orients it according to <rQ>, and scales it according to the sizes
	// given in vector <size>
	glm::mat4 M = glm::mat4(1);
	M = glm::translate(glm::mat4(1.0), pos) * glm::mat4_cast(rQ) * glm::scale(glm::mat4(1.0), size);

	return M;
}