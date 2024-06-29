void SetupProjectionMatrices(Assignment05 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
	
	float n = 0.1f;
	float f = 50.0f;
	float a = Ar;

	float t = n * tan(glm::radians(45.0f/2));
	float b = -t;
	float r = a * t;
	float l = -r;
	
	// Fov-y = 60 deg -> this i theta
	S = glm::mat4(1);

	glm::mat4 P_ort = glm::perspective(glm::radians(60.0f), a, n, f); 
	P_ort[1][1] *= -1; // without this lane the image is flipped
	S = P_ort;

	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Fov-y = 105 deg
	S = glm::mat4(1);

	P_ort = glm::perspective(glm::radians(105.0f), a, n, f); 
	P_ort[1][1] *= -1; // without this lane the image is flipped
	S = P_ort;
	
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Fov-y = 25 deg
	S = glm::mat4(1);

	P_ort = glm::perspective(glm::radians(25.0f), a, n, f);
	P_ort[1][1] *= -1; // without this lane the image is flipped
	S = P_ort;

	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Right View, Fov-y = 45 deg
	S = glm::mat4(1);
	P_ort = glm::frustum(0.0f, 2*r, b, t, n, f);
	P_ort[1][1] *= -1; // without this lane the image is flipped
	S = P_ort;
	A->Matrix(4, S); // sets the matrix corresponding to piece 4
}