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

struct Pose {
	Vecteur position;
	Quaternion rotation;
};

Vecteur ajouter(const Vecteur& gauche, const Vecteur& droite) {
	return {gauche.x + droite.x, gauche.y + droite.y, gauche.z + droite.z};
}

Vecteur multiplier(const Vecteur& vecteur, double scalaire) {
	return {vecteur.x * scalaire, vecteur.y * scalaire, vecteur.z * scalaire};
}

Quaternion multiplier(const Quaternion& gauche, const Quaternion& droite) {
	return {
		gauche.w * droite.w - gauche.x * droite.x - gauche.y * droite.y - gauche.z * droite.z,
		gauche.w * droite.x + gauche.x * droite.w + gauche.y * droite.z - gauche.z * droite.y,
		gauche.w * droite.y - gauche.x * droite.z + gauche.y * droite.w + gauche.z * droite.x,
		gauche.w * droite.z + gauche.x * droite.y - gauche.y * droite.x + gauche.z * droite.w
	};
}

Quaternion normaliser(const Quaternion& quaternion) {
	const double longueur = std::sqrt( quaternion.w * quaternion.w + quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	if (longueur == 0.0) {
		return {1.0, 0.0, 0.0, 0.0};
	}
	return {
		quaternion.w / longueur,
		quaternion.x / longueur,
		quaternion.y / longueur,
		quaternion.z / longueur
	};
}

Pose extrapoler(const Pose& pose, const Vecteur& vitesseLineaire, const Vecteur& vitesseAngulaire, double dt) {
		Pose resultat = pose;
		resultat.position = ajouter(pose.position, multiplier(vitesseLineaire, dt));

		const double vitesseRotation = std::sqrt(vitesseAngulaire.x * vitesseAngulaire.x + vitesseAngulaire.y * vitesseAngulaire.y + vitesseAngulaire.z * vitesseAngulaire.z);
		if (vitesseRotation == 0.0) {
			return resultat;
		}

		const double angle = vitesseRotation * dt;
		const Vecteur axe{
			vitesseAngulaire.x / vitesseRotation,
			vitesseAngulaire.y / vitesseRotation,
			vitesseAngulaire.z / vitesseRotation
		};
		const double demiAngle = angle / 2.0;
		const double sinus = std::sin(demiAngle);
		const Quaternion increment{
			std::cos(demiAngle),
			axe.x * sinus,
			axe.y * sinus,
			axe.z * sinus
		};

		resultat.rotation = normaliser(multiplier(pose.rotation, increment));
		return resultat;
}

void afficher(const char* nom, const Vecteur& vecteur) {
	std::cout << nom << vecteur.x << ' ' << vecteur.y << ' ' << vecteur.z << '\n';
}

void afficher(const char* nom, const Quaternion& quaternion) {
	std::cout << nom << quaternion.w << ' ' << quaternion.x << ' ' << quaternion.y << ' ' << quaternion.z << '\n';
}

int main() {
	Pose pose{};
	Vecteur vitesseLineaire{};
	Vecteur vitesseAngulaire{};
	double dt = 0.0;

	std::cout << "Position de la pose (x y z) : ";
	std::cin >> pose.position.x >> pose.position.y >> pose.position.z;
	std::cout << "Quaternion unitaire de la pose (w x y z) : ";
	std::cin >> pose.rotation.w >> pose.rotation.x
			 >> pose.rotation.y >> pose.rotation.z;

	std::cout << "Vitesse lineaire (m/s, x y z) : ";
	std::cin >> vitesseLineaire.x >> vitesseLineaire.y >> vitesseLineaire.z;
	std::cout << "Vitesse angulaire (rad/s, x y z) : ";
	std::cin >> vitesseAngulaire.x >> vitesseAngulaire.y >> vitesseAngulaire.z;
	std::cout << "Duree dt en secondes : ";
	std::cin >> dt;

	const Pose extrapolee = extrapoler(pose, vitesseLineaire, vitesseAngulaire, dt);

	std::cout << std::fixed << std::setprecision(4);
	afficher("Position extrapolee : ", extrapolee.position);
	afficher("Quaternion extrapole : ", extrapolee.rotation);
}
