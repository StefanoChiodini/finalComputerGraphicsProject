void SetupProjectionMatrices(Assignment04 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
	glm::mat4 R;
	glm::mat4 R1;
	glm::mat4 R2;
	glm::mat4 P_ort; // orthographic projection
	glm::mat4 S1;
	float w = 10;
	float n = -50;
	float f = 50;

	// Isometric projection
	S = glm::mat4(1);

	//Port = glm::ortho(l, r, b, t, n, f);
	P_ort = glm::ortho(-w, w, -w/Ar, w/Ar, n, f); // creation of orthographic projection
	R1 = glm::rotate(glm::mat4(1), glm::radians(-35.26f), glm::vec3(1, 0, 0));
	R2 = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0, 1, 0));
	S1 = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1)); // now i flip the y axis to match the vulkan convention

	S = P_ort * R1 * R2 * S1;
	A->Matrix(1, S); // sets the matrix corresponding to piece 1


	// Dimetric
	S = glm::mat4(1);

	P_ort = glm::ortho(-w, w, -w/Ar, w/Ar, n, f); // creation of orthographic projection
	R1 = glm::rotate(glm::mat4(1), glm::radians(-25.0f), glm::vec3(1, 0, 0)); // arbitrary rotation a around the x-axis
	R2 = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0, 1, 0));
	S1 = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1)); // now i flip the y axis to match the vulkan convention

	S = P_ort * R1 * R2 * S1;
	A->Matrix(2, S); // sets the matrix corresponding to piece 2


	// Trimetric
	S = glm::mat4(1);
	P_ort = glm::ortho(-w, w, -w / Ar, w / Ar, n, f); // creation of orthographic projection
	S1 = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1)); // now i flip the y axis to match the vulkan convention
    // arbitrary rotation a around the x-axis
	R1 = glm::rotate(glm::mat4(1), glm::radians(-20.0f), glm::vec3(1, 0, 0));
	// an arbitrary rotation b around the y - axis
	R2 = glm::rotate(glm::mat4(1), glm::radians(-32.5f), glm::vec3(0, 1, 0));
	S = P_ort * R1 * R2 * S1;


	A->Matrix(3, S); // sets the matrix corresponding to piece 3


	// Cabinet
	S = glm::mat4(1);
	P_ort = glm::ortho(-w, w, -w / Ar, w / Ar, n, f); // creation of orthographic projection
	R = glm::mat4(1, 0, 0, 0,
					0, 1, 0, 0,
					-0.5f * cos(45), -0.5f * sin(45), 1, 0,
					0, 0, 0, 1);// this matrix is transposed respect to the one written in the slides
	// now i flip the y axis to match the vulkan convention
	S1 = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1));
	S = S1 * P_ort * R;
	A->Matrix(4, S); // sets the matrix corresponding to piece 4
}