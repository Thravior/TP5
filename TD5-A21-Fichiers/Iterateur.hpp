/******************************************************************************
* Programme qui creer la classe Iterateur. Cette classe permet d'obtenir un
* iterateur sur notre liste liee. Elle nous permettera de parcourir la liste en
* avancant ou en reculant dedans.
* \file   Iterateur.hpp
* \author Laurie Bedard-Cote (2086165) et Mathias Gagnon (2115246)
* \date  12 novembre 2021
* \cree  9 novembre 2021
******************************************************************************/

#pragma once
#include "noeud.hpp"
#include "gsl/gsl_assert"
template<typename T>
class Iterateur
{
public:
	Iterateur(Noeud<T>* position = Noeud<T>::finListe) 
	{
		position_ = position;
	}

	void avancer()
	{
		Expects(position_ != nullptr);
		position_ = position_->suivant_;
	}

	void reculer()
	{
		//NOTE: On ne demande pas de supporter de reculer à partir de l'itérateur end().
		Expects(position_ != nullptr);
		position_ = position_->precedent_;
	}

	T& operator*()		{return position_->donnee_; }

	bool operator==(const Iterateur<T>& it) const = default;

	bool operator++()	{return position_ = position_->suivant_;}


private:
	Noeud<T>* position_;
	friend class ListeLiee<T>;
};
