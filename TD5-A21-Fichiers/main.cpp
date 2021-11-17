/******************************************************************************
* Programme qui teste la liste liee en ajoutant un element, en faisant une
* recherche, en avancant et reculant dans la liste, et en supprimant un
* element de la liste. Finalement, nous avons choisi une map comme
* conteneur pour trier nos elements en ordre alphabetique. Notre reponse
* a la question 2.3 du TP se retrouve a la fin du fichier.
* \file   main.cpp
* \author Laurie Bedard-Cote (2086165) et Mathias Gagnon (2115246)
* \date  12 novembre 2021
* \cree  9 novembre 2021
******************************************************************************/

#include "Personnage.hpp"
#include "Heros.hpp"
#include "Vilain.hpp"
#include "VilainHeros.hpp"
#include "ListeLiee.hpp"
#include <fstream>
#include <vector>
#include <functional>
#include "cppitertools/range.hpp"
#include "bibliotheque_cours.hpp"
#include <map>
using namespace std;
using namespace iter;

using UInt8  = uint8_t;
using UInt16 = uint16_t;

struct MaComparaison
{
	bool operator() (const string& nom1, const string& nom2) const 
	{
		return nom1 < nom2;
	}
};

UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}

UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read(reinterpret_cast<char*>(&texte[0]), streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

template <typename T>
vector<T> lireFichier(istream& fichier)
{
	vector<T> elements;
	for ([[maybe_unused]] int i : range(lireUint16(fichier)))
		elements.push_back(T(fichier));
	return elements;
}

ifstream ouvrirLectureFichierBinaire(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	return fichier;
}

// Permet d'avoir une référence non-const à un objet temporaire.
template <typename T> T& temporaireModifiable(T&& objet) { return objet; }

template <typename T>
vector<T> lireFichier(const string& nomFichier)
{
	return lireFichier<T>(temporaireModifiable(
		ouvrirLectureFichierBinaire(nomFichier)));
}

template <typename T>
Iterateur<T> trouverParNom(ListeLiee<T>& liste, const string& nom)
{
	Iterateur<T> fin = liste.end();
	for (Iterateur<T> pos = liste.begin(); pos != fin; pos.avancer()) {
		if ((*pos).getNom() == nom)
			return pos;
	}
	return fin;
}

int main()
{
	#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi();
	#pragma endregion
	
	// Trait de separation
	static const string trait =
		"═════════════════════════════════════════════════════════════════════════";

	static const string separateurSections = "\033[95m" + trait + "\033[0m\n";
	static const string separateurElements = "\033[33m" + trait + "\033[0m\n";

	
	vector<Heros>				   heros   = lireFichier<Heros>("heros.bin");
	vector<Vilain>				   vilains = lireFichier<Vilain>("vilains.bin");
	vector<unique_ptr<Personnage>> peronnages;  // Doit être des pointeurs pour le polymorphisme, l'énoncé ne force pas les unique_ptr.

	/****************************************************************************/
	/*********************************** TP 5 ***********************************/
	/****************************************************************************/

	ListeLiee<Heros> ll = ListeLiee<Heros>();
	
	//TODO: Transférez les héros du vecteur heros dans une ListeLiee.
	for (auto h : heros) 
		ll.push_back(h);
	
	//TODO: Créez un itérateur sur la liste liée à la position du héros Alucard
	// Servez-vous de la fonction trouverParNom définie plus haut
	cout << "Creation de l'iterateur a la position du hero Alucard : " << endl;
	
	auto iterateur = trouverParNom(ll, "Alucard");
	iterateur.operator*().afficher(cout);
	
	cout << endl << separateurSections << endl;

	//TODO: Servez-vous de l'itérateur créé précédemment pour trouver l'héroine Aya Brea,
	// en sachant qu'elle se trouve plus loin dans la liste.
	cout << endl << "Avancer jusqu'a Aya Brea : " << endl;
	
	while (iterateur != trouverParNom(ll, "Aya Brea")) {
		iterateur.avancer();
	}
	iterateur.operator*().afficher(cout);

	cout << endl << separateurSections << endl;

	//TODO: Ajouter un hero bidon à la liste avant Aya Brea en vous servant de l'itérateur.
	//TODO: Assurez-vous que la taille de la liste est correcte après l'ajout.
	cout << "Ajout d'un hero bidon avant Aya Brea : " << endl;
	
	Heros heroBidon = Heros("Guy", "Vie", "Mort");
	cout << "Taille avant ajout: " << ll.size() << endl;
	ll.insert(iterateur, heroBidon);
	
	cout << "Taille apres ajout: " << ll.size() << endl << endl;
	cout << endl << separateurSections << endl;

	//TODO: Reculez votre itérateur jusqu'au héros Mario et effacez-le en utilisant l'itérateur, puis affichez le héros suivant dans la liste (devrait êter "Naked Snake/John").
	while (iterateur != trouverParNom(ll, "Mario")) {
		iterateur.reculer();
	}

	cout << endl <<  "Une fois l'hero Mario supprime, affichage du hero suivant : " << endl;
	
	iterateur = ll.erase(iterateur);
	iterateur.operator*().afficher(cout);

	//TODO: Assurez-vous que la taille de la liste est correcte après le retrait.
	cout << "Taille apres erase: " << ll.size() << endl;
	
	//TODO: Effacez le premier élément de la liste.
	ll.erase(ll.begin());
	
	cout << endl << separateurSections << endl;

	//TODO: Affichez votre liste de héros en utilisant un itérateur. La liste débute
	// avec le héros Randi et n'a pas Mario.
	// Servez-vous des methodes begin et end de la liste...
	cout << "AFFICHAGE 1 : " << endl;
	for (Iterateur iter = ll.begin(); iter != ll.end(); iter.avancer()) {
		iter.operator*().afficher(cout);
		cout << separateurElements << endl;
	}
	cout << endl << separateurSections << endl;

	//TODO: Refaite le même affichage mais en utilisant une simple boucle "for" sur intervalle.
	cout << endl << separateurSections << endl;
	cout << "AFFICHAGE 2 : " << endl;
	auto iter = ll.begin();
	for ([[maybe_unused]] unsigned i = 0; i < ll.size(); ++i) {
		iter.operator*().afficher(cout);
		cout << separateurElements << endl;
		iter.avancer();
	}
	cout << endl << separateurSections << endl;

	/****************************************************************************/
	/***************************** PARTIE 2 *************************************/
	/****************************************************************************/
	//Utilisez un conteneur pour avoir les héros en ordre alphabétique
	// map classe les cles par ordre alphabetique

	map<string,Heros> mapHeros;
	for (auto h : heros) {
		mapHeros.insert(pair<string,Heros>(h.getNom(),h));
	}
	mapHeros["Alucard"].afficher(cout);

	cout << endl << separateurSections << endl;

	/* REPONSE A LA QUESTION 2.3 */
	/* Sachant que le temps pour rechercher un hero par son nom avec la liste */
	/* lie est lineaire O(1), et qu'avec une map le temps est constant O(n),  */
	/* on peut affimer qu'il est plus rapide de faire cette recherche avec	  */
	/* une map (soit le conteneur). Notons tout de meme que si nous effectuons*/
	/* une recherche sur un autre element, le temps de recherche pour la map  */
	/* serait lineaire comme la liste lie.									  */

}
