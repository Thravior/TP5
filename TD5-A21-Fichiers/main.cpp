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

struct MaComparaison{
	bool operator() (const string& nom1, const string& nom2) const {
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

	//{ Solutionnaire du TD4:
	vector<Heros> heros = lireFichier<Heros>("heros.bin");
	vector<Vilain> vilains = lireFichier<Vilain>("vilains.bin");
	vector<unique_ptr<Personnage>> peronnages;  // Doit être des pointeurs pour le polymorphisme, l'énoncé ne force pas les unique_ptr.

	#if 1 //TODO: Vous n'avez pas à conserver ces affichages pour le TD5, ils sont pour le solutionnaire du TD4:
	cout << separateurSections << "Heros:" << endl;
	for (auto& h : heros) {
		cout << separateurElements;
		h.changerCouleur(cout, 0);
		h.afficher(cout);
	}

	cout << separateurSections << "Vilains:" << endl;
	for (auto& v : vilains) {
		cout << separateurElements;
		v.changerCouleur(cout, 0);
		v.afficher(cout);
	}

	for (auto& h : heros)
		peronnages.push_back(make_unique<Heros>(h));

	for (auto& v : vilains)
		peronnages.push_back(make_unique<Vilain>(v));

	peronnages.push_back(make_unique<VilainHeros>(vilains[1], heros[2]));

	cout << separateurSections << "Personnages:" << endl;
	for (auto& p : peronnages) {
		cout << separateurElements;
		p->changerCouleur(cout, 0);
		p->afficher(cout);
	}
	cout << separateurSections << "Un autre vilain heros (exemple de l'énoncé du TD):" << endl;
	VilainHeros kefkaCrono(vilains[2], heros[0]);
	kefkaCrono.changerCouleur(cout,1);
	kefkaCrono.afficher(cout);

	#endif
	cout << separateurSections;
	//}

	ListeLiee<Heros> ll = ListeLiee<Heros>();
	//TODO: Transférez les héros du vecteur heros dans une ListeLiee.
	for (auto h : heros) {
		ll.push_back(h);
	}
	//TODO: Créez un itérateur sur la liste liée à la position du héros Alucard
	// Servez-vous de la fonction trouverParNom définie plus haut
	auto iterAlucard = trouverParNom(ll, "Alucard");
	iterAlucard.operator*().afficher(cout);
	//TODO: Servez-vous de l'itérateur créé précédemment pour trouver l'héroine Aya Brea,
	// en sachant qu'elle se trouve plus loin dans la liste.
	while (iterAlucard != trouverParNom(ll, "Aya Brea")) {
		iterAlucard.avancer();
	}
	iterAlucard.operator*().afficher(cout);

	cout << endl << separateurSections << endl;

	//TODO: Ajouter un hero bidon à la liste avant Aya Brea en vous servant de l'itérateur.
	//TODO: Assurez-vous que la taille de la liste est correcte après l'ajout.
	Heros idiot = Heros("Guy", "Vie", "Mort");
	cout << "Taille avant ajout: " << ll.size() << endl;
	ll.insert(iterAlucard, idiot);
	cout << "Taille apres ajout: " << ll.size() << endl << endl;
	cout << endl << separateurSections << endl;

	//TODO: Reculez votre itérateur jusqu'au héros Mario et effacez-le en utilisant l'itérateur, puis affichez le héros suivant dans la liste (devrait êter "Naked Snake/John").
	while (iterAlucard != trouverParNom(ll, "Mario")) {
		iterAlucard.reculer();
	}

	iterAlucard = ll.erase(iterAlucard);
	iterAlucard.operator*().afficher(cout);

	//TODO: Assurez-vous que la taille de la liste est correcte après le retrait.
	cout << "Taille apres erase: " << ll.size() << endl;
	//TODO: Effacez le premier élément de la liste.
	ll.erase(ll.begin());
	
	//TODO: Affichez votre liste de héros en utilisant un itérateur. La liste débute
	// avec le héros Randi et n'a pas Mario.
	// Servez-vous des methodes begin et end de la liste...
	cout << endl << separateurSections << endl;
	for (Iterateur iter = ll.begin(); iter != ll.end(); iter.avancer()) {
		iter.operator*().afficher(cout);
		cout << separateurElements << endl;
	}
	cout << endl << separateurSections << endl;
	//TODO: Refaite le même affichage mais en utilisant une simple boucle "for" sur intervalle.
	auto iter = ll.begin();
	for ([[maybe_unused]] unsigned i = 0; i < ll.size(); ++i) {
		iter.operator*().afficher(cout);
		cout << separateurElements << endl;
		iter.avancer();
	}

	cout << endl << separateurSections << endl;
	//TODO: Utilisez un conteneur pour avoir les héros en ordre alphabétique (voir point 2 de l'énoncé).
	map<string,Heros,MaComparaison> mapHeros;
	for (auto h : heros) {
		mapHeros.insert(pair<string,Heros>(h.getNom(),h));
	}
	mapHeros["Alucard"].afficher(cout);
	// liste lie trouve l'element en tmeps lineaire O(n); mapHero trouve l'element en temps constant selon le nom,  de par le choix de cle O(1) temps lineaire pour tout autre critere
	//TODO: Assurez-vous de n'avoir aucune ligne non couverte dans les classes pour la liste liée.  Il peut y avoir des lignes non couvertes dans les personnages...

}
