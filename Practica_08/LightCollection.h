//
// Created by edgar on 10/9/2023.
//

#ifndef PRACTICA07_LIGHTCOLLECTION_H
#define PRACTICA07_LIGHTCOLLECTION_H

#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename T>
class LightColletction
{
	static_assert(std::is_same<Light, T>::value || std::is_same<PointLight, T>::value || std::is_same<SpotLight, T>::value, "Must be Light-like type");
	using LightPair = std::pair<T, bool>;

  public:
	explicit LightColletction(const std::vector<T> &lVecIn)
	{
		for (const auto &light : lVecIn)
			lightsVector.push_back(LightPair(light, true));
		currentCount = lVecIn.size();
	}

  private:
	std::vector<LightPair> lightsVector;

  public:
	T *getLightArray() const
	{
		return lightArray;
	}

	int getCurrentCount() const
	{
		return currentCount;
	}

  private:
	T             *lightArray;
	int            currentCount;
	std::vector<T> tmp;

  public:
	void updateArray()
	{
		currentCount = 0;
		std::for_each(lightsVector.begin(), lightsVector.end(),
		              [this](LightPair p) -> void
		              {
			              if (p.second)
			              {
				              tmp.push_back(p.first);
				              currentCount++;
			              }
		              });
		lightArray = &tmp[0];
	}

	void toggleLight(int idx, bool currentState)
	{
		bool isTurnedOn             = lightsVector.at(idx).second;
		lightsVector.at(idx).second = !isTurnedOn;
		updateArray();
	}
	
	
};

template <typename T>
class LightCollectionBuilder
{
	static_assert(std::is_same<Light, T>::value || std::is_same<PointLight, T>::value || std::is_same<SpotLight, T>::value, "Must be Light-like type");

  public:
	explicit LightCollectionBuilder<T>(int lightCount)
	{
		this->maxLightCount = lightCount;
		this->lightCount    = 0;
	}

	LightCollectionBuilder<SpotLight> &addLight(const T &light)
	{
		if (this->lightCount < maxLightCount)
			this->lightVector.push_back(light);
		else
			std::cout << "[WARNING] Se intentaron agregar más de "
			          << maxLightCount
			          << " al arreglo, se ignora la inserción.\n";
		return *this;
	}

	LightColletction<T> *build()
	{
		// TODO : Validación que arrSize == maxLights
		auto collection = new LightColletction<T>(this->lightVector);
		collection->updateArray();
		return collection;
	}

  private:
	int            lightCount{};
	int            maxLightCount{};
	std::vector<T> lightVector;
};

#endif // PRACTICA07_LIGHTCOLLECTION_H
