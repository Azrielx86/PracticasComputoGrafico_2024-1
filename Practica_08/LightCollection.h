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

/**
 * Contiene una colección de luces, con métodos para encender/apagar el arreglo
 * @tparam T Clase tipo Light, PointLight o SpotLight
 * @author Azrielx86 (Edgar Chalico)
 */
template <typename T>
class LightColletction
{
	static_assert(std::is_same<Light, T>::value || std::is_same<PointLight, T>::value || std::is_same<SpotLight, T>::value, "Must be Light-like type");
	using LightPair = std::pair<T, bool>;

  public:
	/**
	 * Constructor del arreglo, construye los pares T - bool
	 * y los agrega a un nuevo vector
	 * @param lVecIn Vector de luces (Light, PointLight o SpotLight)
	 */
	[[maybe_unused]] explicit LightColletction(const std::vector<T> &lVecIn)
	{
		for (const auto &light : lVecIn)
//			lightsVector.push_back(LightPair(light, true));
			lightsVector.push_back(std::make_pair(std::ref(light), true));
		currentCount = lVecIn.size();
	}

	/**
	 * Actualiza el estado de una luz, y actualiza el arreglo.
	 * @param idx Indice de la luz que se quiere encender o apagar.
	 * @param newState Estado de encendido o apagado.
	 */
	void toggleLight(int idx, bool newState)
	{
		bool isTurnedOn = lightsVector.at(idx).second;
		if (newState == isTurnedOn) return;
		lightsVector.at(idx).second = !isTurnedOn;
		updateArray();
	}

	/**
	 * @return Arreglo unicamente con luces encendidas.
	 */
	T *getLightArray()
	{
		updateArray();
		return lightArray;
	}

	/**
	 * @return Conteo de luces encendidas.
	 */
	int getCurrentCount() { return currentCount; }

	/**
	 * Actualiza el arreglo cuando hay cambios en las luces,
	 * para enviar a OpenGL únicamente las luces encendidas.
	 */
	void updateArray()
	{
		currentCount = 0;
		for (const LightPair &p : lightsVector)
		{
			if (p.second)
			{
				tmp.push_back(p.first);
				currentCount++;
			}
		}
		lightArray = tmp.data();
	}

	/**
	 * Retorna una luz del arreglo, si esta está encendida
	 * @param index
	 * @return
	 */
	T &operator[](int index) 
	{
		auto &light = lightsVector.at(index);
		//		if (!light.second)
		return light.first;
		//		return &lightArray[index];
	}

  private:
	std::vector<LightPair> lightsVector;
	T *lightArray;
	int currentCount;
	std::vector<T> tmp;
};

template <typename T>
class LightCollectionBuilder
{
	static_assert(std::is_same<Light, T>::value || std::is_same<PointLight, T>::value || std::is_same<SpotLight, T>::value, "Must be Light-like type");

  public:
	explicit LightCollectionBuilder<T>(int lightCount)
	{
		this->maxLightCount = lightCount;
		this->lightCount = 0;
	}

	LightCollectionBuilder<T> &addLight(const T &light)
	{
		if (this->lightCount < maxLightCount)
			this->lightVector.push_back(light);
		else
			std::cout << "[WARNING] Se intentaron agregar más de "
			          << maxLightCount
			          << " al arreglo, se ignora la inserción.\n";
		return *this;
	}

	LightColletction<T> build()
	{
		LightColletction<T> collection(this->lightVector);
		collection.updateArray();
		return collection;
	}

  private:
	int lightCount{};
	int maxLightCount{};
	std::vector<T> lightVector;
};

#endif // PRACTICA07_LIGHTCOLLECTION_H
