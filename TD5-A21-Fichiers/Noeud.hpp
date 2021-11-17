/******************************************************************************
* Programme qui construit la classe Noeud qui sera utile pour former la classe 
* Iterateur, pour finalement forme la classe nous permettant de faire une liste 
* liee.
* \file   Noeud.hpp
* \author Laurie Bedard-Cote (2086165) et Mathias Gagnon (2115246)
* \date  12 novembre 2021
* \cree  9 novembre 2021
******************************************************************************/


#pragma once
#include <gsl/pointers>
template<typename T> class ListeLiee;
template<typename T> class Iterateur;
template<typename T>

class Noeud
{
public:
	Noeud() {}
	Noeud(const T& donnee)
	{
		donnee_ = donnee;
	}

private:
	friend class ListeLiee<T>;
	friend class Iterateur<T>;

	T donnee_;
	Noeud* precedent_						= finListe;
	Noeud* suivant_							= finListe;
	inline static constexpr Noeud* finListe = nullptr;
};
