#pragma once
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
		position_ = position_->suivant_;
	}
	void reculer()
	{
		//NOTE: On ne demande pas de supporter de reculer à partir de l'itérateur end().
		Expects(position_ != nullptr);
		position_ = position_->precedent_;
	}

	T& operator*()
	{
		return position_->donnee_;
	}

	bool operator==(const Iterateur<T>& it) const = default;

private:
	Noeud<T>* position_;
	friend class ListeLiee<T>;
};
