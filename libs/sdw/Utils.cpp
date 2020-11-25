#include <algorithm>
#include <sstream>
#include "Utils.h"
#include <string>
#include <fstream>
#include <map>

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

std::vector<ModelTriangle> loadObj(std::string path, float scale) {
	std::ifstream file(path, std::ifstream::in);
	std::string line;
	
	std::vector<ModelTriangle> triangles;
	std::vector<glm::vec3> vertices;
	std::vector<TexturePoint> texturePoints;
	// std::vector<glm::vec3> vertexNormals;
	Material material;
	std::string mtlPath;

	std::map<glm::vec3*, glm::vec3> vertexNormals; //WARNING pointer here may not work :/

	while(!file.eof()) {
		std::getline(file, line);
		std::vector<std::string> tokens = split(line, ' ');

		if(tokens[0].compare("mtllib") == 0) mtlPath = tokens[1];
		else if(tokens[0].compare("usemtl") == 0) material = loadMaterial(tokens[1], mtlPath);
		else if(tokens[0].compare("v") == 0) vertices.push_back(scale * glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		else if(tokens[0].compare("vt") == 0) texturePoints.push_back(TexturePoint(stof(tokens[1]), stof(tokens[2])));
		else if(tokens[0].compare("vn") == 0) vertexNormals.insert({&(vertices[vertices.size()- 1]), glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]))});
		// else if(tokens[0].compare("vn") == 0) vertexNormals.push_back(glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		else if(tokens[0].compare("f") == 0) {
			std::array<glm::vec3, 3> trianglePoints;
			std::array<TexturePoint, 3> selectedTexturePoints;
			std::array<glm::vec3, 3> selectedVertexNormals;
			for(int i = 1; i < 4; i++) {
				std::vector<std::string> subTokens = split(tokens[i], '/');
				//TrianglePoint
				trianglePoints[i - 1] = vertices[stoi(subTokens[0]) - 1];
				//VertexNormal
				std::map<glm::vec3*, glm::vec3>::iterator it = vertexNormals.find(&(vertices[stoi(subTokens[0]) - 1]));
				if(it != vertexNormals.end()) selectedVertexNormals[i - 1] = it->second;
				//TexturePoint
				if(subTokens[1].compare("\0") != 0) {
					selectedTexturePoints[i - 1] = texturePoints[stoi(subTokens[0]) - 1];
				}
			}

			ModelTriangle triangle = ModelTriangle();
			triangle.vertices = trianglePoints;
			triangle.texturePoints = selectedTexturePoints;
			triangle.colour = material.colour;
			triangle.material = material;
			triangle.vertexNormals = selectedVertexNormals;
			glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
			glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
			triangle.normal = glm::normalize(glm::cross(e0, e1));
			
			triangles.push_back(triangle);
		}
	}
	file.close();

	return triangles;
}

Material loadMaterial(std::string mtlName, std::string path) {
	std::ifstream in(path, std::ifstream::in);
	std::string line;
	glm::vec3 colour;
	std::string texturePath;
	
    bool found = false;
    while(!found && !in.eof()) {
	    std::getline(in, line);
        std::vector<std::string> tokens = split(line, ' ');
        if(tokens[0].compare("newmtl") == 0 && tokens[1].compare(mtlName) == 0) found = true;
    }
    found = false;
	while(!in.eof() && !found) {
        std::getline(in, line);
        std::vector<std::string> tokens = split(line, ' ');
        if(tokens[0].compare("Kd") == 0) {
            colour = glm::vec3(0xFF*stof(tokens[1]), 0xFF*stof(tokens[2]), 0xFF*stof(tokens[3]));
        }
		if(tokens[0].compare("map_Kd") == 0) texturePath = tokens[1];
		if(tokens[0].compare("newmtl") == 0) found = true;
    }
	Material material = Material(Colour(colour.r, colour.g, colour.b), texturePath);
    in.close();
    return material;

}

std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues) {
	std::vector<float> values(numberOfValues);
	float interval = (to - from) / (numberOfValues - 1);
	for(int i = 0; i < numberOfValues; i++) values[i] = from + i*interval;
	return values;
}

std::vector<glm::vec3> interpolateVector(glm::vec3 from, glm::vec3 to, int numberOfValues) {
	std::vector<glm::vec3> values(numberOfValues);
	glm::vec3 intervals = (to - from) / (float)(numberOfValues - 1);
	for(int i = 0; i < numberOfValues; i++) values[i] = from + (float)i*intervals;
	return values;
}

std::vector<glm::vec2> interpolateVector(glm::vec2 from, glm::vec2 to, int numberOfValues) {
	std::vector<glm::vec2> values(numberOfValues);
	glm::vec2 intervals = (to - from) / (float)(numberOfValues - 1);
	for(int i = 0; i < numberOfValues; i++) values[i] = from + (float)i*intervals;
	return values;
}

std::vector<CanvasPoint> interpolateVector(CanvasPoint from, CanvasPoint to, int numberOfValues) {
	std::vector<CanvasPoint> values(numberOfValues);
	glm::vec3 intervals = (glm::vec3(to.x,to.y,to.depth) - glm::vec3(from.x,from.y,from.depth)) / (float)(numberOfValues);
	for(int i = 0; i < numberOfValues; i++) {
		glm::vec3 result = glm::vec3(from.x, from.y, from.depth) + (float)i*intervals;
		if(glm::isnan(result.x)) {
			std::cout << "nan intervals: " << intervals.x << " num: " << numberOfValues << " to.x: "<< to.x << " from.x: " << from.x << std::endl;
			std::cout << "from.y: " << from.y << " to.y: " << to.y << std::endl;
		}
		values[i] = CanvasPoint(result.x,result.y, result.z);
	} 		
	return values;
}

uint32_t colourPack(Colour colour, int alpha) {
	return (alpha << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
}

uint32_t colourPack(glm::vec3 colour, int alpha) {
	return (alpha << 24) + ((int)colour.r << 16) + ((int)colour.g << 8) + (int)colour.b;
}