//
// Created by edgar on 10/17/2023.
//

#include "Animation.h"
Animation::Animation()
{
	current = nullptr;
	currentIndex = 0;
}

Animation &Animation::addCondition(const std::function<bool(float)> &function)
{
	this->conditions.push_back(function);
	return *this;
}

void Animation::prepare()
{
	current = &conditions.front();
	currentIndex = 0;
}

void Animation::update(float deltaTime)
{
	if (current == nullptr) return;
	if ((*current)(deltaTime))
	{
		currentIndex++;
		if (currentIndex >= (int) conditions.size()) currentIndex = 0;
		current = &conditions[currentIndex];
	}
}
