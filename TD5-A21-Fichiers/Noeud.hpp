#pragma once
#include <gsl/pointers>
template<typename T> class ListeLiee;
template<typename T> class Iterateur;
template<typename T>
class Noeud
{
public:
	//TODO: Constructeur(s).
	Noeud() {}
	Noeud(const T& donnee) {
		donnee_ = donnee;
	}

private:
	//TODO: Attributs d'un noeud.
	friend class ListeLiee<T>;
	friend class Iterateur<T>;
	T donnee_;
	Noeud* precedent_ = finListe;
	Noeud* suivant_ = finListe;
	inline static constexpr Noeud* finListe = nullptr;
};
