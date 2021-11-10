﻿#pragma once
#include "noeud.hpp"
#include "gsl/gsl_assert"
template<typename T>
class Iterateur
{
public:
	//TODO: Constructeur(s).
	Iterateur(Noeud<T>* position = Noeud<T>::finListe) {
		position_ = position;
	}

	void avancer()
	{
		Expects(position_ != nullptr);
		//TODO: changez la position de l'itérateur pour le noeud suivant
		position_ = position_->suivant_;
	}
	void reculer()
	{
		//NOTE: On ne demande pas de supporter de reculer à partir de l'itérateur end().
		Expects(position_ != nullptr);
		//TODO: Changez la position de l'itérateur pour le noeud précédent
		position_ = position_->precedent_;
	}
	T& operator*()
	{
		return position_->donnee_;
	}
	//TODO: Ajouter ce qu'il manque pour que les boucles sur intervalles fonctionnent sur une ListeLiee.
	bool operator==(const Iterateur<T>& it) const = default;
private:
	Noeud<T>* position_;
	friend class ListeLiee<T>;
};
