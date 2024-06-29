// the owl pieces are in this order: 4,1
//									 5,2
//									 6,3

void SetupMatrices(Assignment03 *A) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
	glm::mat4 M;
	glm::mat4 T;
	glm::mat4 R;

	// To solve the puzzle: scale 2x proportionally, centered in (2,0,-3)
	S = glm::mat4(1); // identity matrix 4x4 -> this will be the matrix used for the transformation
	T = glm::translate(glm::mat4(1), glm::vec3(2, 0, -3));
	M = glm::scale(glm::mat4(1), glm::vec3(2, 2, 2));
	S = T * M * glm::inverse(T);
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// To solve the puzzle: scale 3x on an axis oriented 45 degrees between the x and z axis and passing through point (1,0,0)
	S = glm::mat4(1);
	M = glm::scale(glm::mat4(1), glm::vec3(3, 1, 1));
	R = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, 1, 0));
	T = glm::translate(glm::mat4(1), glm::vec3(1, 0, 0));
	S = T * R * M * glm::inverse(R) * glm::inverse(T);
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// To solve the puzzle: find it yourself! Hint: compose a rotation around an arbitrary point with a translation
	S = glm::mat4(1);
	T = glm::translate(glm::mat4(1), glm::vec3(0.3, 0, 3.70));
	R = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, 1, 0));
	S = T * R * glm::inverse(T);
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// To solve the puzzle: rotate 60 degree around the y axis, centered in (-1,0,-2)
	S = glm::mat4(1);
	R = glm::rotate(glm::mat4(1), glm::radians(60.0f), glm::vec3(0, 1, 0));
	T = glm::translate(glm::mat4(1), glm::vec3(-1, 0, -2));
	S = T * R * glm::inverse(T);
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

	// To solve the puzzle: rotate -90 degree around an arbitrary axis passing in (-1,0,0). The x axis can be aligned to this arbitrary direction with a rotation of -45 around the y axis.	
	S = glm::mat4(1);
	R = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0, 1, 0));
	M = glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	T = glm::translate(glm::mat4(1), glm::vec3(-1, 0, 0));
	S = T * R * M * glm::inverse(R) * glm::inverse(T);
	A->Matrix(5, S); // sets the matrix corresponding to piece 5

	// To solve the puzzle: find it yourself! Hint: compose a non-proportional scaling that halves the object along one of the main axes, centered in an arbitrary point, with a translation
	S = glm::mat4(1);
	T = glm::translate(glm::mat4(1), glm::vec3(1, 0, 0));
	M = glm::scale(glm::mat4(1), glm::vec3(0.5, 1, 1));
	S = T * M * glm::inverse(T);
	A->Matrix(6, S); // sets the matrix corresponding to piece 6

}