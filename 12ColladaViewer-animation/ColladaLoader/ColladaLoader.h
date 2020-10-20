#pragma once
#include <unordered_map>
#include <queue>
#include <iostream>
#include <map>

#include <glm\glm.hpp>
#include "..\tinyxml\tinyxml.h"

struct S {
	int jointId;
	float weight;


	S(int jointId, float weight) : jointId(jointId), weight(weight) {}

	bool operator<(const struct S& other) const {
		return weight < other.weight;
	}
};

struct VertexSkinData {
public:
	void addJointEffect(int jointId, float weight) {
		pq.push(S(jointId, weight));
	}

	void fill() {
		while (!pq.empty()) {
			jointIds.push_back(pq.top().jointId);
			weights.push_back(pq.top().weight);
			pq.pop();
		}
	}

	void limitJointNumber(int max) {
		fillEmptyWeights(max);
	}
	std::priority_queue<S> pq;
	std::vector<int> jointIds;
	std::vector<float> weights;

private:

	void fillEmptyWeights(int max) {

		while (jointIds.size() < max) {
			jointIds.push_back(0);
			weights.push_back(0.0);
		}
	}
};

struct JointData {
	int index;
	std::string nameId;
	glm::mat4 bindLocalTransform;
	std::vector<JointData> children;

	JointData(int _index, std::string _nameId, glm::mat4 _bindLocalTransform) {
		index = _index;
		nameId = _nameId;
		bindLocalTransform = _bindLocalTransform;
	}

	JointData() {}

	void addChild(JointData child) {
		children.push_back(child);
	}
};

struct SkeletonData {

	int jointCount;
	JointData headJoint;

	SkeletonData(int _jointCount, JointData _headJoint) {
		jointCount = _jointCount;
		headJoint = _headJoint;
	}

	SkeletonData() {}
};

struct Joint;

class ColladaLoader {

public:
	ColladaLoader(const std::string & _path);
	~ColladaLoader();

	void loadData(std::vector<glm::vec3> &positions, 
				  std::vector<glm::vec2> &texCoords, 
				  std::vector<glm::vec3> &normals, 
				  std::vector<glm::uvec4> &jointIds, 
				  std::vector<glm::vec4> &jointWeights, 
				  std::vector<unsigned int> &indices,
				  std::vector<std::string> &jointsList);

	void loadVisualScene(std::vector<std::string> &jointsList);
	void createJoints(Joint &joint);

	TiXmlDocument doc;

private:
	const std::string& path;
	

	std::vector<float> m_vertexBuffer;
	std::vector<unsigned int> m_indexBuffer;

	std::map<int, std::vector<int>> m_vertexCache;
	int addVertex(int hash, const float *pVertex, int numberOfBytes);

	void loadGeometry(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &normals, std::vector<unsigned int> &indices);
	void loadController(std::vector<glm::uvec4> &jointIds, std::vector<glm::vec4> &jointWeights, std::vector<std::string> &jointsList, std::vector<VertexSkinData> &skinningData);
	void createIndexBuffer(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &normals, std::vector<glm::uvec4> &jointIds, std::vector<glm::vec4> &jointWeights, std::vector<unsigned int> &indices, std::vector<VertexSkinData> &skinningData);
	
	JointData search(TiXmlNode* node, std::vector<std::string> &jointsList);
	Joint createJoints(JointData data, glm::mat4 parentBindTransform);

	SkeletonData skeletonData;

	short inputNodes, offsetPositions, offsetNormals, offsetTexcoords, offsetColors;
	int prim_count, num_indices, pos_count, norm_count, tex_count, numberOfTriangles;
	bool hasPositions, hasNormals, hasTexCoords, hasColors;
};