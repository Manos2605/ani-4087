#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>

struct Quaternion {
	double w;
	double x;
	double y;
	double z;
};

Quaternion multiplier(const Quaternion& gauche, const Quaternion& droite) {
	return {
		gauche.w * droite.w - gauche.x * droite.x - gauche.y * droite.y - gauche.z * droite.z,
		gauche.w * droite.x + gauche.x * droite.w + gauche.y * droite.z - gauche.z * droite.y,
		gauche.w * droite.y - gauche.x * droite.z + gauche.y * droite.w + gauche.z * droite.x,
		gauche.w * droite.z + gauche.x * droite.y - gauche.y * droite.x + gauche.z * droite.w
	};
}

Quaternion normaliser(const Quaternion& quaternion) {
	const double longueur = std::sqrt(
		quaternion.w * quaternion.w + quaternion.x * quaternion.x
		+ quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	return {
		quaternion.w / longueur,
		quaternion.x / longueur,
		quaternion.y / longueur,
		quaternion.z / longueur
	};
}

Quaternion rotationAutourDeZ(double angle) {
	return {std::cos(angle / 2.0), 0.0, 0.0, std::sin(angle / 2.0)};
}

double erreurAngulaire(const Quaternion& premier, const Quaternion& second) {
	double produit = premier.w * second.w + premier.x * second.x
		+ premier.y * second.y + premier.z * second.z;
	produit = std::abs(produit);
	if (produit > 1.0) {
		produit = 1.0;
	}
	return 2.0 * std::acos(produit) * 180.0 / std::acos(-1.0);
}

Quaternion vraiePosePasAPas(double vitesseInitiale, double acceleration,
							double duree, double pas) {
	Quaternion pose{1.0, 0.0, 0.0, 0.0};
	for (double temps = 0.0; temps < duree - 1e-12; temps += pas) {
		const double tempsRestant = duree - temps;
		const double dt = tempsRestant < pas ? tempsRestant : pas;
		const double milieu = temps + dt / 2.0;
		const double vitesseInstantanee = vitesseInitiale + acceleration * milieu;
		pose = normaliser(multiplier(
				pose, rotationAutourDeZ(vitesseInstantanee * dt)));
	}
	return pose;
}

Quaternion extrapoler(double vitesseAngulaire, double duree) {
	return rotationAutourDeZ(vitesseAngulaire * duree);
}

int main() {
	const double pi = std::acos(-1.0);
	const double vitesseInitiale = 180.0 * pi / 180.0;
	const double acceleration = 180.0 * pi / 180.0;
	const double pasSimulation = 0.001;

	std::ofstream donnees("exo12_erreur.csv");
	donnees << "duree_ms,erreur_degres\n";

	std::ofstream image("exo12_courbe.svg");
	image << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"900\" height=\"520\" viewBox=\"0 0 900 520\">\n";
	image << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
	image << "<text x=\"450\" y=\"32\" text-anchor=\"middle\" font-size=\"22\">Erreur d'extrapolation</text>\n";
	image << "<text x=\"450\" y=\"505\" text-anchor=\"middle\" font-size=\"16\">Duree (ms)</text>\n";
	image << "<text x=\"18\" y=\"260\" text-anchor=\"middle\" font-size=\"16\" transform=\"rotate(-90 18 260)\">Erreur (degres)</text>\n";
	image << "<line x1=\"70\" y1=\"450\" x2=\"850\" y2=\"450\" stroke=\"black\"/>\n";
	image << "<line x1=\"70\" y1=\"60\" x2=\"70\" y2=\"450\" stroke=\"black\"/>\n";
	image << "<line x1=\"70\" y1=\"411\" x2=\"850\" y2=\"411\" stroke=\"#cccccc\"/>\n";
	image << "<line x1=\"70\" y1=\"255\" x2=\"850\" y2=\"255\" stroke=\"#cccccc\"/>\n";
	image << "<line x1=\"70\" y1=\"60\" x2=\"850\" y2=\"60\" stroke=\"#cccccc\"/>\n";
	image << "<text x=\"58\" y=\"455\" text-anchor=\"end\" font-size=\"14\">0</text>\n";
	image << "<text x=\"58\" y=\"260\" text-anchor=\"end\" font-size=\"14\">45</text>\n";
	image << "<text x=\"58\" y=\"65\" text-anchor=\"end\" font-size=\"14\">90</text>\n";
	image << "<text x=\"70\" y=\"470\" text-anchor=\"middle\" font-size=\"14\">0</text>\n";
	image << "<text x=\"460\" y=\"470\" text-anchor=\"middle\" font-size=\"14\">500</text>\n";
	image << "<text x=\"850\" y=\"470\" text-anchor=\"middle\" font-size=\"14\">1000</text>\n";
	image << "<polyline fill=\"none\" stroke=\"#d14\" stroke-width=\"3\" points=\"";

	for (int dureeMs = 10; dureeMs <= 1000; dureeMs += 10) {
		const double duree = dureeMs / 1000.0;
		const Quaternion extrapolee = extrapoler(vitesseInitiale, duree);
		const Quaternion vraie = vraiePosePasAPas(
			vitesseInitiale, acceleration, duree, pasSimulation);
		const double erreur = erreurAngulaire(extrapolee, vraie);
		donnees << dureeMs << ',' << std::setprecision(12) << erreur << '\n';
		const double x = 70.0 + 780.0 * dureeMs / 1000.0;
		const double y = 450.0 - 390.0 * erreur / 90.0;
		image << x << ',' << y << ' ';
	}
	image << "\"/>\n</svg>\n";
	std::cout << "Donnees enregistrees dans exo12_erreur.csv\n";
	std::cout << "Image enregistree dans exo12_courbe.svg\n";
}
