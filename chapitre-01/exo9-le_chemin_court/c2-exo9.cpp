#include <cmath>
#include <iomanip>
#include <iostream>

struct Vecteur {
	double x;
	double y;
	double z;
};

struct Quaternion {
	double w;
	double x;
	double y;
	double z;
};

Quaternion conjugue(const Quaternion& quaternion) {
	return {
		quaternion.w,
		-quaternion.x,
		-quaternion.y,
		-quaternion.z
	};
}

Quaternion multiplier(const Quaternion& gauche, const Quaternion& droite) {
	return {
		gauche.w * droite.w - gauche.x * droite.x
			- gauche.y * droite.y - gauche.z * droite.z,
		gauche.w * droite.x + gauche.x * droite.w
			+ gauche.y * droite.z - gauche.z * droite.y,
		gauche.w * droite.y - gauche.x * droite.z
			+ gauche.y * droite.w + gauche.z * droite.x,
		gauche.w * droite.z + gauche.x * droite.y
			- gauche.y * droite.x + gauche.z * droite.w
	};
}

Vecteur vitesseAngulaireMoyenne(Quaternion depart, Quaternion arrivee,
							 double dt, bool cheminCourt) {
		if (cheminCourt) {
			const double produitScalaire =
				depart.w * arrivee.w + depart.x * arrivee.x
				+ depart.y * arrivee.y + depart.z * arrivee.z;
			if (produitScalaire < 0.0) {
				arrivee.w = -arrivee.w;
				arrivee.x = -arrivee.x;
				arrivee.y = -arrivee.y;
				arrivee.z = -arrivee.z;
			}
		}

		const Quaternion difference = multiplier(conjugue(depart), arrivee);
		const double longueurVecteur = std::sqrt(
			difference.x * difference.x
			+ difference.y * difference.y
			+ difference.z * difference.z);
		if (longueurVecteur == 0.0 || dt == 0.0) {
			return {0.0, 0.0, 0.0};
		}

		const double angle = 2.0 * std::atan2(longueurVecteur, difference.w);
		const double facteur = angle / (dt * longueurVecteur);
		return {
			difference.x * facteur,
			difference.y * facteur,
			difference.z * facteur
		};
}

void afficher(const char* nom, const Vecteur& vecteur) {
	std::cout << nom << vecteur.x << ' ' << vecteur.y << ' ' << vecteur.z << '\n';
}

int main() {
	const double pi = std::acos(-1.0);
	const double demiAngle = 5.0 * pi / 180.0;
	const Quaternion depart{1.0, 0.0, 0.0, 0.0};
	const Quaternion arrivee{
		-std::cos(demiAngle),
		0.0,
		0.0,
		-std::sin(demiAngle)
	};
	const double dt = 1.0;

	std::cout << std::fixed << std::setprecision(6);
	afficher("Vitesse avec chemin court : ",
		 vitesseAngulaireMoyenne(depart, arrivee, dt, true));
	afficher("Vitesse sans chemin court : ",
		 vitesseAngulaireMoyenne(depart, arrivee, dt, false));
}
