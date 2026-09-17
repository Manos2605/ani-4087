#include <iomanip>
#include <iostream>

struct Dimensions {
	double longueur;
	double largeur;
	double hauteur;
};

Dimensions multiplier(const Dimensions& dimensions, double facteur) {
	return {
		dimensions.longueur * facteur,
		dimensions.largeur * facteur,
		dimensions.hauteur * facteur
	};
}

void afficher(const char* nom, const Dimensions& dimensions) {
	std::cout << nom << " : "
			  << dimensions.longueur << " m x "
			  << dimensions.largeur << " m x "
			  << dimensions.hauteur << " m\n";
}

int main() {
	const Dimensions salle{4.0, 3.0, 2.5};
	const Dimensions table{1.20, 0.75, 0.75};
	const Dimensions chaise{0.45, 0.45, 0.90};
	const Dimensions porte{0.90, 0.10, 2.00};
	const Dimensions fenetre{1.20, 0.10, 1.00};
	const Dimensions meuble{1.00, 0.45, 1.80};
	double facteur = 0.0;

	std::cout << "Facteur d'echelle : ";
	std::cin >> facteur;

	std::cout << std::fixed << std::setprecision(2);
	afficher("Salle", multiplier(salle, facteur));
	afficher("Table", multiplier(table, facteur));
	afficher("Chaise", multiplier(chaise, facteur));
	afficher("Porte", multiplier(porte, facteur));
	afficher("Fenetre", multiplier(fenetre, facteur));
	afficher("Meuble", multiplier(meuble, facteur));
}
