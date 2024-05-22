#include "FileReader.h"

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace T1WD;


void FileReader::Init() {
}
void FileReader::CleanUp() {
}

void FileReader::GenerateMeshFromFBXFile(const char* location, BasicMesh& mesh) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(location, aiProcess_Triangulate | aiProcess_FlipUVs);
    // process all the node's meshes (if any)

    if (scene->mRootNode->mChildren[0]->mNumMeshes < 1) {
        std::cout << "GenerateMeshFromFBXFile() Has failed because \"" << location << "\" Does not have atleast 1 mesh to get." << std::endl;
        return;
    }

    //aiNode* node = 

    aiMesh* loaded_mesh = scene->mMeshes[0];
    //Vertices
    //float* vertices = new float[loaded_mesh->mNumVertices * 3];
    std::vector<float> vertices;
    unsigned int* indices = new unsigned int[loaded_mesh->mNumFaces * 3];

    //float scale = loaded_mesh->
    for (size_t i = 0; i < loaded_mesh->mNumVertices; i++) {
        //vertices[i * 3] = loaded_mesh->mVertices[i * 3].x;
        //vertices[i * 3 + 1] = loaded_mesh->mVertices[i * 3].y;
        //vertices[i * 3 + 2] = loaded_mesh->mVertices[i * 3].z;
    }
    int vertex_index = 0;
    for (size_t i = 0; i < loaded_mesh->mNumFaces; i++) {
        if (loaded_mesh->mFaces[i].mNumIndices != 3)continue;
        for (int j = 0; j < 3; j++) {
            aiVector3D vertex = loaded_mesh->mVertices[loaded_mesh->mFaces[i].mIndices[j]];
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            indices[i * 3 + j] = vertex_index;
            vertex_index++;

        }

        //indices[i * 3 + 1] = loaded_mesh->mFaces[i].mIndices[1];
        //indices[i * 3 + 2] = loaded_mesh->mFaces[i].mIndices[2];
       // std::cout << indices[i * 3] << " " << indices[i * 3 + 1] << " " << indices[i * 3 + 2] << std::endl;
    }

    mesh.Create(3, vertices.data(), loaded_mesh->mNumVertices * 3, indices, loaded_mesh->mNumFaces * 3);
}