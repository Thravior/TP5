/******************************************************************************
* Programme creer la classe ListeLiee. Elle nous permet de verifier si la liste
* est vide et elle nous donne sa taille. Nous pouvons ensuite trouver le debut 
* et la fin de cette liste (c'est une liste double). Finalement, les methodes 
* de la classe nous permettront d'ajouter un item en fin de liste, d'ajouter un
* item apres un iterateur et d'effacer un item.
* \file   ListeLiee.hpp
* \author Laurie Bedard-Cote (2086165) et Mathias Gagnon (2115246)
* \date  12 novembre 2021
* \cree  9 novembre 2021
******************************************************************************/

#pragma once
#include "iterateur.hpp"
#include "gsl/gsl_assert"
template<typename T> class Iterateur;
template<typename T>
class ListeLiee
{
public:
	ListeLiee() = default;

	~ListeLiee()
	{
		if (!estVide()){
			while (tete_->suivant_ != nullptr) {
				Noeud<T>* temp = tete_;
				tete_ = tete_->suivant_;
				delete temp;
				temp = nullptr;
			}
			delete tete_;
			tete_ = nullptr;
		}
	}

	bool	 estVide() const  { return taille_ == 0; }
	unsigned size()	   const  { return taille_; }
	
	//NOTE: to_address permet que ce même code fonctionne que vous utilisiez des pointeurs bruts ou intelligents (ça prend le pointeur brut associé au pointeur intelligent, s'il est intelligent).
	Iterateur<T> begin()  { return {to_address(tete_)}; }
	Iterateur<T> end()    { return {to_address(queue_->suivant_)}; }

	// Ajoute à la fin de la liste.
	void push_back(const T& item)
	{
		Noeud<T>* nouveau = new Noeud<T>(item);
		if (estVide()) 
			tete_ = nouveau;

		else {
			queue_->suivant_ = nouveau;
			nouveau->precedent_ = queue_;
		}
		queue_ = nouveau;
		taille_++;
	}

	// Insère avant la position de l'itérateur.
	Iterateur<T> insert(Iterateur<T> it, const T& item)
	{
		if (it.position_ == Noeud<T>::finListe) {
			push_back(item);
			return Iterateur(queue_);
		}
		else {
			Noeud<T>* nouveau = new Noeud<T>(item);
			nouveau->suivant_ = it.position_;
			nouveau->precedent_ = nouveau->suivant_->precedent_;

			nouveau->suivant_->precedent_ = nouveau;
			nouveau->precedent_->suivant_ = nouveau;
			taille_++;
			return Iterateur(nouveau);
		}
	}

	// Enlève l'élément à la position it et retourne un itérateur vers le suivant.
	Iterateur<T> erase(Iterateur<T> it)
	{
		Noeud<T>* avant = it.position_->precedent_;
		Noeud<T>* apres = it.position_->suivant_;

		if (avant == Noeud<T>::finListe) {
			tete_ = apres;
		}
		else {
			avant->suivant_ = apres;
		}

		apres->precedent_ = avant;
		delete it.position_;
		taille_--;
		return Iterateur(apres);
	}

private:
	gsl::owner<Noeud<T>*> tete_;
	Noeud<T>*			  queue_;
	unsigned			  taille_;
	friend class Iterateur<T>;
};
