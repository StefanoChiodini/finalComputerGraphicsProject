void Assignment08::createMazeMesh(int row, int col, char** maze) {

	//here i will create all vertexes (column by column)
	for (int r = 0; r <= row; r++) {
		for (int c = 0; c <= col; c++) {
			vPos.push_back(r); vPos.push_back(0); vPos.push_back(c);
			vPos.push_back(r); vPos.push_back(1); vPos.push_back(c);
		}
	}

	// floor -> here i render the floor as a plane; this is a big plane that covers the whole maze
	vIdx.push_back(0); vIdx.push_back(col * 2); vIdx.push_back((row + 1) * col * 2);
	vIdx.push_back(col * 2); vIdx.push_back((row + 1) * col * 2); vIdx.push_back((row + 1) * (col + 1) * 2 - 2);

	for (int r = 0; r < row; r++) {
		for (int c = 0; c < col; c++) {
			if (maze[r][c] == '#') {

				// Cube exists, create its mesh

				// Check if there is a neighboring cube to the left
				bool hasLeftNeighbor = (c == 0 || maze[r][c - 1] != '#');

				// Check if there is a neighboring cube to the right
				bool hasRightNeighbor = (c == col - 1 || maze[r][c + 1] != '#');

				// Check if there is a neighboring cube in front
				bool hasFrontNeighbor = (r == 0 || maze[r - 1][c] != '#');

				// Check if there is a neighboring cube in back
				bool hasBackNeighbor = (r == row - 1 || maze[r + 1][c] != '#');

				// Generate mesh for cube, skipping walls that are redundant due to adjacent cubes
				int twoTimesColumnsNum = (col + 1) * 2;
				int firstVertexOfRow = r * twoTimesColumnsNum; // index of the first row vertex
				int selectedColumn = firstVertexOfRow + c * 2; // To select the correct column. c * 2 because we have 2 vertexes per column


				if (hasFrontNeighbor) {
					vIdx.push_back(selectedColumn); vIdx.push_back(selectedColumn + 1); vIdx.push_back(selectedColumn + 2); // +0, +1, +2
					vIdx.push_back(selectedColumn + 2); vIdx.push_back(selectedColumn + 2 + 1); vIdx.push_back(selectedColumn + 1); // +2, +3, +1
				}

				if (hasBackNeighbor) {
					vIdx.push_back(selectedColumn + twoTimesColumnsNum); vIdx.push_back(selectedColumn + 1 + twoTimesColumnsNum); vIdx.push_back(selectedColumn + 2 + twoTimesColumnsNum); // +0, +1, +2
					vIdx.push_back(selectedColumn + 2 + twoTimesColumnsNum); vIdx.push_back(selectedColumn + 2 + 1 + twoTimesColumnsNum); vIdx.push_back(selectedColumn + 1 + twoTimesColumnsNum); // +2, +3, +1
				}

				if (hasLeftNeighbor) {
					vIdx.push_back(selectedColumn); vIdx.push_back(selectedColumn + twoTimesColumnsNum); vIdx.push_back(selectedColumn + 1); // +0, +0, +1
					vIdx.push_back(selectedColumn + 1); vIdx.push_back(selectedColumn + twoTimesColumnsNum + 1); vIdx.push_back(selectedColumn + twoTimesColumnsNum); // +1, +1, +0
				}

				if (hasRightNeighbor) {
					vIdx.push_back(selectedColumn + 2); vIdx.push_back(selectedColumn + twoTimesColumnsNum + 2); vIdx.push_back(selectedColumn + 1 + 2);
					vIdx.push_back(selectedColumn + 1 + 2); vIdx.push_back(selectedColumn + twoTimesColumnsNum + 1 + 2); vIdx.push_back(selectedColumn + twoTimesColumnsNum + 2);
				}

				// roofTop
				vIdx.push_back(selectedColumn + 1); vIdx.push_back(selectedColumn + 1 + twoTimesColumnsNum); vIdx.push_back(selectedColumn + 1 + 2);
				vIdx.push_back(selectedColumn + 1 + 2); vIdx.push_back(selectedColumn + 1 + twoTimesColumnsNum + 2); vIdx.push_back(selectedColumn + 1 + twoTimesColumnsNum);
			}
		}
	}
}