void Assignment15::createBoxMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
    // The primitive built here is a box centered in the origin, with proportions respecting the texture.

    // The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
    // front face
    vDef.push_back({ {-1,-1,0.25}, {0,0,1}, {93. / 1024, 417. / 512} });  // vertex 0 - Position, Normal and uv
    vDef.push_back({ {1,-1,0.25}, {0,0,1}, {443. / 1024, 417. / 512} });  // vertex 1 - Position and Normal
    vDef.push_back({ {-1,1,0.25}, {0,0,1}, {93. / 1024, 93. / 512} });  // vertex 2 - Position and Normal
    vDef.push_back({ {1,1,0.25}, {0,0,1}, {443. / 1024, 93. / 512} });  // vertex 3 - Position and Normal
    // back face
    vDef.push_back({ {-1,-1,-0.25}, {0,0,-1}, {888. / 1024, 417. / 512} });  // vertex 4 - Position and Normal
    vDef.push_back({ {1,-1,-0.25}, {0,0,-1}, {540. / 1024, 417. / 512} });  // vertex 5 - Position and Normal
    vDef.push_back({ {-1,1,-0.25}, {0,0,-1}, {888. / 1024, 93. / 512} });  // vertex 6 - Position and Normal
    vDef.push_back({ {1,1,-0.25}, {0,0,-1}, {540. / 1024, 93. / 512} });  // vertex 7 - Position and Normal
    // left face
    vDef.push_back({ {-1,-1,-0.25}, {-1,0,0}, {0. / 1024, 417. / 512} });  // vertex 8 - Position and Normal
    vDef.push_back({ {-1,-1,0.25}, {-1,0,0}, {93. / 1024, 417. / 512} });  // vertex 9 - Position and Normal
    vDef.push_back({ {-1,1,-0.25}, {-1,0,0}, {0. / 1024, 93. / 512} });  // vertex 10 - Position and Normal
    vDef.push_back({ {-1,1,0.25}, {-1,0,0}, {93. / 1024, 93. / 512} });  // vertex 11 - Position and Normal
    // right face
    vDef.push_back({ {1,-1,-0.25}, {1,0,0}, {540. / 1024, 417. / 512} });  // vertex 12 - Position and Normal
    vDef.push_back({ {1,-1,0.25}, {1,0,0}, {443. / 1024, 417. / 512} });  // vertex 13 - Position and Normal
    vDef.push_back({ {1,1,-0.25}, {1,0,0}, {540. / 1024, 93. / 512} });  // vertex 14 - Position and Normal
    vDef.push_back({ {1,1,0.25}, {1,0,0}, {443. / 1024, 93. / 512} });  // vertex 15 - Position and Normal
    // top face
    vDef.push_back({ {-1,1,-0.25}, {0,1,0}, {93. / 1024, 0. / 512} });  // vertex 16 - Position and Normal
    vDef.push_back({ {1,1,-0.25}, {0,1,0}, {443. / 1024, 0. / 512} });  // vertex 17 - Position and Normal
    vDef.push_back({ {-1,1,0.25}, {0,1,0}, {93. / 1024, 93. / 512} });  // vertex 18 - Position and Normal
    vDef.push_back({ {1,1,0.25}, {0,1,0}, {443. / 1024, 93. / 512} });  // vertex 19 - Position and Normal
    // bottom face
    vDef.push_back({ {-1,-1,-0.25}, {0,-1,0}, {93. / 1024, 417. / 512} });  // vertex 20 - Position and Normal
    vDef.push_back({ {1,-1,-0.25}, {0,-1,0}, {443. / 1024, 417. / 512} });  // vertex 21 - Position and Normal
    vDef.push_back({ {-1,-1,0.25}, {0,-1,0}, {93. / 1024, 512. / 512} });  // vertex 22 - Position and Normal
    vDef.push_back({ {1,-1,0.25}, {0,-1,0}, {443. / 1024, 512. / 512} });  // vertex 23 - Position and Normal
    
    // The procedures also fill the array vIdx with the indices of the vertices of the triangles
    // front face
    vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);  // First triangle
    vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);  // Second triangle
    // back face
    vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(6);  // First triangle
    vIdx.push_back(5); vIdx.push_back(6); vIdx.push_back(7);  // Second triangle
    // left face
    vIdx.push_back(8); vIdx.push_back(9); vIdx.push_back(10);  // First triangle
    vIdx.push_back(9); vIdx.push_back(10); vIdx.push_back(11);  // Second triangle
    // right face
    vIdx.push_back(12); vIdx.push_back(13); vIdx.push_back(14);  // First triangle
    vIdx.push_back(13); vIdx.push_back(14); vIdx.push_back(15);  // Second triangle
    // top face
    vIdx.push_back(16); vIdx.push_back(17); vIdx.push_back(18);  // First triangle
    vIdx.push_back(17); vIdx.push_back(18); vIdx.push_back(19);  // Second triangle
    // bottom face
    vIdx.push_back(20); vIdx.push_back(21); vIdx.push_back(22);  // First triangle
    vIdx.push_back(21); vIdx.push_back(22); vIdx.push_back(23);  // Second triangle
}


#define M_PI 3.141595f
void Assignment15::createSphereMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
    // The primitive built here is a sphere of radius 1, centered at the origin, on which the Mars texture is applied seamlessly.

    const int numRings = 60;     // Number of rings (latitude lines)
    const int numSegments = 60;  // Number of segments (longitude lines)

    // Fill array vDef with the positions, normal vectors, and UV coordinates of the vertices
    for (int ring = 0; ring <= numRings; ++ring) {
        float phi = ring * M_PI / numRings;  // Latitude angle

        for (int segment = 0; segment <= numSegments; ++segment) {
            float theta = segment * 2 * M_PI / numSegments;  // Longitude angle

            // Calculate the spherical coordinates
            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            // Calculate the texture coordinates
            float u = static_cast<float>(segment) / numSegments;
            float v = static_cast<float>(ring) / numRings;

            // Add the vertex to vDef
            vDef.push_back({ {x, y, z}, {x, y, z}, {u, v} });
        }
    }

    // Fill the array vIdx with the indices of the vertices of the triangles
    for (int ring = 0; ring < numRings; ++ring) {
        for (int segment = 0; segment < numSegments; ++segment) {
            // Calculate the indices of the vertices for the current quad
            int index1 = ring * (numSegments + 1) + segment;
            int index2 = index1 + 1;
            int index3 = (ring + 1) * (numSegments + 1) + segment;
            int index4 = index3 + 1;

            // Create two triangles from the quad
            vIdx.push_back(index1);
            vIdx.push_back(index2);
            vIdx.push_back(index3);

            vIdx.push_back(index2);
            vIdx.push_back(index4);
            vIdx.push_back(index3);
        }
    }
}