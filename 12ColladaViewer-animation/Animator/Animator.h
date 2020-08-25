#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <memory>
#include <unordered_map>
#include "Animation.h"

class AnimatedModel;
class Joint;

class Animator{

public:
	Animator(AnimatedModel* model);

	void startAnimation(const std::string& animationName);
	void addAnimation(const std::string &filename, const std::string &rootJoinName);
	void Update(double elapsedTime);

private:
	AnimatedModel* _model;

	std::vector<std::shared_ptr<Animation>> _animations;
	std::shared_ptr<Animation> _currentAnimation;
	double _animationTime;
	
	std::unordered_map<std::string, glm::mat4> calculateCurrentAnimationPose();

};

#endif