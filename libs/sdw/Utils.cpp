#include <algorithm>
#include <sstream>
#include "Utils.h"

std::vector<std::string> split(const std::string &line, char delimiter) {
	auto haystack = line;
	std::vector<std::string> tokens;
	size_t pos;
	while ((pos = haystack.find(delimiter)) != std::string::npos) {
		tokens.push_back(haystack.substr(0, pos));
		haystack.erase(0, pos + 1);
	}
	// Push the remaining chars onto the vector
	tokens.push_back(haystack);
	return tokens;
}

std::vector<ModelTriangle> loadObj(string path, float scale) {
	std::ifstream file(path, std::ifstream::in);
	std::string line;
	
	std::vector<ModelTriangle> triangles;
	std::vector<glm::vec3> vertices;
	std::vector<TexturePoint> texturePoints;
	std::vector<glm::vec3> vertexNormals;
	Material material;
	std::string mtlPath;


	while(!file.eof()) {
		std::getline(file, line);
		std::vector<std::string> tokens = split(line, ' ');

		if(tokens[0].compare("mtllib") == 0) mtlPath = tokens[1];
		else if(tokens[0].compare("usemtl") == 0) material = loadMaterial(tokens[1], mtlPath);
		else if(tokens[0].compare("v") == 0) vertices.push_back(scale * glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		else if(tokens[0].compare("vt") == 0) texturePoints.push_back(TexturePoint(stof(tokens[1]), stof(tokens[2])));
		else if(tokens[0].compare("vn") == 0) textureNormals.push_back(glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		else if(tokens[0].compare("f") == 0) {
			std::array<glm::vec3, 3> trianglePoints;
			std::array<TexturePoint, 3> selectedTexturePoints;
			std::array>glm::vec3, 3> selectedVertexNormals;
			for(int i = 1; i < 4; i++) {
				std::vector<std::string> subTokens = split(tokens[i], '/');
				//TrianglePoint
				trianglePoints[i - 1] = vertices[stoi(subTokens[0]) - 1];
				//VertexNormal
				selectedVertexNormals[i - 1] = vertexNormals[stoi(subTokens[0]) - 1];
				//TexturePoint
				if(subTokens[1].compare("\0") != 0) {
					selectedTexturePoints[i - 1] = texturePoints[stoi(subTokens[1]) - 1];
				}
			}

			ModelTriangle triangle = ModelTriangle();
			triangle.vertices = trianglePoints;
			triangle.texturePoints = selectedTexturePoints;
			triangle.colour = material.colour;
			triangle.material = material;
			triangle.vertexNormals = selectedVertexNormals; //////Need to check this lol
			glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
			glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
			triangle.normal = glm::normalize(glm::cross(e0, e1));
			
			pairs.push_back(std::pair<ModelTriangle,Material>(triangle,material));
		}
}

Material loadMtl(string mtlName, string path) {
	std::ifstream in(mtlPath, std::ifstream::in);
	std::string line;
	glm::vec3 colour;
	std::string texturePath;
	
    bool found = false;
    while(!found && !in.eof()) {
	    std::getline(in, line);
        std::vector<std::string> tokens = split(line, ' ');
        if(tokens[0].compare("newmtl") == 0 && tokens[1].compare(mtlName) == 0) {
            found = true;
        }
    }
    found = false;
	while(!in.eof() && !found) {
        std::getline(in, line);
        std::vector<std::string> tokens = split(line, ' ');
        if(tokens[0].compare("Kd") == 0) {
            colour = glm::vec3(0xFF*stof(tokens[1]), 0xFF*stof(tokens[2]), 0xFF*stof(tokens[3]));
        }
		if(tokens[0].compare("map_Kd") == 0) {
			texturePath = tokens[1];
		}
		if(tokens[0].compare("newmtl") == 0) {
			found = true;
		}
    }
	Material material = Material(Colour(colour.r, colour.g, colour.b), texturePath);
    in.close();
    return material;

}