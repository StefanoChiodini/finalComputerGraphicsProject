#define _USE_MATH_DEFINES
#include <cmath>

# define M_PI 3.14159265358979323846  /* pi */

void Assignment14::createCubeMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
    vDef.push_back({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} });	// vertex 0 - Position and Normal
    vDef.push_back({ {1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} });	// vertex 1 - Position and Normal
    vDef.push_back({ {-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} });	// vertex 2 - Position and Normal
    vDef.push_back({ {-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} });	// vertex 3 - Position and Normal
    // back face
    vDef.push_back({ {1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f} });	// vertex 4 - Position and Normal
    vDef.push_back({ {1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f} });	// vertex 5 - Position and Normal
    vDef.push_back({ {-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f} });	// vertex 6 - Position and Normal
    vDef.push_back({ {-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f} });	// vertex 7 - Position and Normal
    // left face
    vDef.push_back({ {-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} });	// vertex 8 - Position and Normal
    vDef.push_back({ {-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} });	// vertex 9 - Position and Normal
    vDef.push_back({ {-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f} });	// vertex 10 - Position and Normal
    vDef.push_back({ {-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f} });	// vertex 11 - Position and Normal
    // right face
    vDef.push_back({ {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f} });	// vertex 12 - Position and Normal
    vDef.push_back({ {1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f} });	// vertex 13 - Position and Normal
    vDef.push_back({ {1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f} });	// vertex 14 - Position and Normal
    vDef.push_back({ {1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f} });	// vertex 15 - Position and Normal
    // top face
    vDef.push_back({ {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} });	// vertex 16 - Position and Normal
    vDef.push_back({ {1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f} });	// vertex 17 - Position and Normal
    vDef.push_back({ {-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f} });	// vertex 18 - Position and Normal
    vDef.push_back({ {-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} });	// vertex 19 - Position and Normal
    // bottom face
    vDef.push_back({ {1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} });	// vertex 20 - Position and Normal
    vDef.push_back({ {1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f} });	// vertex 21 - Position and Normal
    vDef.push_back({ {-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f} });	// vertex 22 - Position and Normal
    vDef.push_back({ {-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} });	// vertex 23 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    // front face
    vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);
    vIdx.push_back(0); vIdx.push_back(2); vIdx.push_back(3);
    // back face
    vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(6);
    vIdx.push_back(4); vIdx.push_back(6); vIdx.push_back(7);
    // left face
    vIdx.push_back(8); vIdx.push_back(9); vIdx.push_back(10);
    vIdx.push_back(8); vIdx.push_back(10); vIdx.push_back(11);
    // right face
    vIdx.push_back(12); vIdx.push_back(13); vIdx.push_back(14);
    vIdx.push_back(12); vIdx.push_back(14); vIdx.push_back(15);
    // top face
    vIdx.push_back(16); vIdx.push_back(17); vIdx.push_back(18);
    vIdx.push_back(16); vIdx.push_back(18); vIdx.push_back(19);
    // bottom face
    vIdx.push_back(20); vIdx.push_back(21); vIdx.push_back(22);
    vIdx.push_back(20); vIdx.push_back(22); vIdx.push_back(23);
    
}


void Assignment14::createFunctionMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
    const float minX = -3.0f;
    const float maxX = 3.0f;
    const float minZ = -3.0f;
    const float maxZ = 3.0f;
    const int numSegmentsX = 32;
    const int numSegmentsZ = 32;

    float segmentSizeX = (maxX - minX) / numSegmentsX;
    float segmentSizeZ = (maxZ - minZ) / numSegmentsZ;

    // Generate vertices
    for (int i = 0; i <= numSegmentsX; i++) {
        float x = minX + i * segmentSizeX;

        for (int j = 0; j <= numSegmentsZ; j++) {
            float z = minZ + j * segmentSizeZ;
            float y = sin(x) * cos(z);

            // Vertex
            vDef.push_back({ {x, y, z}, {0.0f, 0.0f, 0.0f} });
        }
    }

    // Generate indices for triangles
    for (int i = 0; i < numSegmentsX; i++) {
        for (int j = 0; j < numSegmentsZ; j++) {
            int currentIdx = i * (numSegmentsZ + 1) + j;

            vIdx.push_back(currentIdx);
            vIdx.push_back(currentIdx + 1);
            vIdx.push_back(currentIdx + numSegmentsZ + 1);

            vIdx.push_back(currentIdx + 1);
            vIdx.push_back(currentIdx + numSegmentsZ + 2);
            vIdx.push_back(currentIdx + numSegmentsZ + 1);
        }
    }

    // Calculate vertex normals
    for (size_t i = 0; i < vIdx.size(); i += 3) {
        uint32_t v0 = vIdx[i];
        uint32_t v1 = vIdx[i + 1];
        uint32_t v2 = vIdx[i + 2];

        glm::vec3 p0 = vDef[v0].pos;
        glm::vec3 p1 = vDef[v1].pos;
        glm::vec3 p2 = vDef[v2].pos;

        glm::vec3 faceNormal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

        // Add face normal to vertex normals
        vDef[v0].norm += faceNormal;
        vDef[v1].norm += faceNormal;
        vDef[v2].norm += faceNormal;
    }

    // Normalize vertex normals
    for (auto& vertex : vDef) {
        vertex.norm = normalize(vertex.norm);
    }
}

void Assignment14::createCylinderMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {

    int nv1 = 100, nv2 = 100;
    float x, y, z;
    glm::vec3 normal;
    for (int i = 0; i < nv1; i++) {
        for (int j = 0; j < nv2; j++) {
            x = cos(2 * M_PI * i / (nv1 - 1));
            y = -1.0f + 2.0f * j / (nv2 - 1);
            z = sin(2 * M_PI * i / (nv1 - 1));
            normal = glm::normalize(glm::vec3{ x, 0, z });
            vDef.push_back({ {x, y, z}, normal });	
        }
    }
    // push the center of the top and bottom faces
    vDef.push_back({ {0, -1.0f, 0}, glm::vec3{0, -1, 0} });	
    vDef.push_back({ {0, 1.0f, 0}, glm::vec3{0, 1, 0} });	

    for (int i = 0; i < nv1; i++) {
        x = cos(2 * M_PI * i / (nv1 - 1));
        z = sin(2 * M_PI * i / (nv1 - 1));
        vDef.push_back({ {x, -1.0f, z}, glm::vec3{0, -1, 0} });	
        vDef.push_back({ {x, 1.0f, z}, glm::vec3{0, 1, 0} });	
    }

    for (int i = 0; i < nv1 - 1; i++) {
        for (int j = 0; j < nv2 - 1; j++) {
            vIdx.push_back(i * nv2 + j); vIdx.push_back(i * nv2 + j + 1); vIdx.push_back((i + 1) * nv2 + j);
            vIdx.push_back(i * nv2 + j + 1); vIdx.push_back((i + 1) * nv2 + j + 1); vIdx.push_back((i + 1) * nv2 + j);
        }
    }

    for (int i = 0; i < nv1 - 1; i++) {
        vIdx.push_back(nv1 * nv2); vIdx.push_back(nv1 * nv2 + 2 * i + 2); vIdx.push_back(nv1 * nv2 + 2 * i + 4);
        vIdx.push_back(nv1 * nv2 + 1); vIdx.push_back(nv1 * nv2 + 2 * i + 3); vIdx.push_back(nv1 * nv2 + 2 * i + 5);
    }
    vIdx.push_back(nv1 * nv2); vIdx.push_back(nv1 * nv2 + 2 * nv1); vIdx.push_back(nv1 * nv2 + 2);
    vIdx.push_back(nv1 * nv2 + 1); vIdx.push_back(nv1 * nv2 + 2 * nv1 + 1); vIdx.push_back(nv1 * nv2 + 3);


}
