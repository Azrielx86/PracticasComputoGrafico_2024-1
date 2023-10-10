//
// Created by edgar on 10/9/2023.
//

#ifndef PRACTICA07_LIGHTCOLLECTION_H
#define PRACTICA07_LIGHTCOLLECTION_H

#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <iostream>
#include <type_traits>
#include <vector>

/**
 * Contiene una colección de luces, con métodos para encender/apagar el arreglo.
 * Esto es para no tener que crear distintos arreglos, ni condiciones en el main,
 * el arreglo se haría automaticamente al encender o apagar una luz.
 * En cuanto a rendimiento, como solo se actualiza cada que cambia el estado de una luz,
 * y el maximo de iteraciones sera 8 (o 16), supongo que no afectaria mucho,
 * o eso espero...
 * @tparam T Clase tipo Light, PointLight o SpotLight
 * @author Azrielx86 (Edgar Chalico)
 */
template <typename T>
class LightCollection
{
	static_assert(std::is_same<Light, T>::value || std::is_same<PointLight, T>::value || std::is_same<SpotLight, T>::value, "Must be Light-like type");
	using LightPair = std::pair<T, bool>;

  public:
	/**
	 * Constructor del arreglo, construye los pares T - bool
	 * y los agrega a un nuevo vector
	 * @param inLightVector Vector de luces (Light, PointLight o SpotLight)
	 */
	explicit LightCollection(const std::vector<T> &inLightVector)
	{
		for (const auto &light : inLightVector)
			lightsVector.push_back(std::make_pair(std::ref(light), true));
		currentCount = inLightVector.size();
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
		tmp.clear();
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
		return light.first;
	}

  private:
	std::vector<LightPair> lightsVector;
	T *lightArray;
	int currentCount;
	std::vector<T> tmp;
};

/**
 * Constructor de un arreglo de luces, puede ser util para no declarar
 * un arreglo externo de luces, e ingresarlas directamente mediante la
 * funcion addLight.
 * @tparam T Clase tipo Light, PointLight o SpotLight
 */
template <typename T>
class LightCollectionBuilder
{
	static_assert(std::is_same<Light, T>::value || std::is_same<PointLight, T>::value || std::is_same<SpotLight, T>::value, "Must be Light-like type");

  public:
	/**
	 * Constructor para un constructor (valga la redundancia) de
	 * una colección de luces.
	 * @param lightCount Conteo de las luces que se van a agregar,
	 * por defecto es 8 (El maximo numero de luces en OpenGL).
	 * (Creo que no es necesario, lo quirare despues...)
	 */
	explicit LightCollectionBuilder<T>(int lightCount)
	{
		maxLightCount = lightCount;
		this->lightCount = 0;
	}

	/**
	 * Agrega una luz a la coleccion.
	 * @param light Luz tipo Light, SpotLight o PointLight
	 * @return Referencia al objeto constructor.
	 */
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

	/**
	 * Agrega un conjunto de luces a la coleccion.
	 * @param lights Luces del tipo Light, SpotLight o PointLight.
	 * @param count Longitud del arreglo.
	 * @return Referencia al objecto constructor.
	 */
	LightCollectionBuilder<T> &addFromArray(T *lights, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			if (this->lightCount < maxLightCount)
				this->lightVector.push_back(lights[i]);
			else
				std::cout << "[WARNING] Se intentaron agregar más de "
				          << maxLightCount
				          << " al arreglo, se ignora la inserción.\n";
		}
		return *this;
	}

	/**
	 * Construye la colección de luces.
	 * @return Nueva colección de luces.
	 */
	LightCollection<T> build()
	{
		auto collection = LightCollection<T>(this->lightVector);
		collection.updateArray();
		return collection;
	}

  private:
	int maxLightCount = 8;
	int lightCount{};
	std::vector<T> lightVector;
};

#endif // PRACTICA07_LIGHTCOLLECTION_H
