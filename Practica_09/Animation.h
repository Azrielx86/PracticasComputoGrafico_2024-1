//
// Created by edgar on 10/17/2023.
//

#ifndef PRACTICA08_ANIMATION_H
#define PRACTICA08_ANIMATION_H

#include <functional>
#include <vector>

class Animation
{
  public:
	explicit Animation(bool loop = true);
	Animation &addCondition(const std::function<bool(float)>& function);
	void prepare();
	void update(float deltaTime);

  private:
	int currentIndex;
	std::function<bool(float)>* current;
	std::vector<std::function<bool(float)>> conditions;
};

#endif // PRACTICA08_ANIMATION_H
