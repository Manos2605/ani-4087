#include <cmath>
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

Quaternion quaternionDepuisAngleEtAxe(double angleDegres, Vecteur axe) {
	const double longueurAxe = std::sqrt(
		axe.x * axe.x + axe.y * axe.y + axe.z * axe.z);
	if (longueurAxe == 0.0) {
		return {1.0, 0.0, 0.0, 0.0};
	}

	axe.x /= longueurAxe;
	axe.y /= longueurAxe;
	axe.z /= longueurAxe;

	const double pi = std::acos(-1.0);
	const double angleRadians = angleDegres * pi / 180.0;
	const double demiAngle = angleRadians / 2.0;
	const double sinus = std::sin(demiAngle);

	return {
		std::cos(demiAngle),
		axe.x * sinus,
		axe.y * sinus,
		axe.z * sinus
	};
}

Vecteur tourner(const Quaternion& q, const Vecteur& point) {
	const double xx = q.x * q.x;
	const double yy = q.y * q.y;
	const double zz = q.z * q.z;
	const double xy = q.x * q.y;
	const double xz = q.x * q.z;
	const double yz = q.y * q.z;
	const double wx = q.w * q.x;
	const double wy = q.w * q.y;
	const double wz = q.w * q.z;

	return {
		(1.0 - 2.0 * (yy + zz)) * point.x + 2.0 * (xy - wz) * point.y + 2.0 * (xz + wy) * point.z,
		2.0 * (xy + wz) * point.x + (1.0 - 2.0 * (xx + zz)) * point.y + 2.0 * (yz - wx) * point.z,
		2.0 * (xz - wy) * point.x + 2.0 * (yz + wx) * point.y + (1.0 - 2.0 * (xx + yy)) * point.z
	};
}

Vecteur appliquer(const Pose& pose, const Vecteur& point) {
	Vecteur resultat = tourner(pose.rotation, point);
	resultat.x += pose.position.x;
	resultat.y += pose.position.y;
	resultat.z += pose.position.z;
	return resultat;
}

Pose inverser(const Pose& pose) {
	const Quaternion conjugue{
		pose.rotation.w,
		-pose.rotation.x,
		-pose.rotation.y,
		-pose.rotation.z
	};

	const Vecteur positionOpposee{
		-pose.position.x,
		-pose.position.y,
		-pose.position.z
	};

	return {tourner(conjugue, positionOpposee), conjugue};
}

Vecteur soustraire(const Vecteur& gauche, const Vecteur& droite) {
	return {
		gauche.x - droite.x,
		gauche.y - droite.y,
		gauche.z - droite.z
	};
}

void afficher(const char* nom, const Vecteur& point) {
	std::cout << nom << point.x << ' ' << point.y << ' ' << point.z << '\n';
}

int main() {
	Pose pose{};
	Vecteur point{};

	std::cout << "Position de la pose (x y z) : ";
	std::cin >> pose.position.x >> pose.position.y >> pose.position.z;

	double angleDegres = 0.0;
	Vecteur axe{};
	std::cout << "Angle de rotation en degres : ";
	std::cin >> angleDegres;
	std::cout << "Axe de rotation (x y z) : ";
	std::cin >> axe.x >> axe.y >> axe.z;
	pose.rotation = quaternionDepuisAngleEtAxe(angleDegres, axe);

	std::cout << "Point a transformer (x y z) : ";
	std::cin >> point.x >> point.y >> point.z;

	const Pose inverse = inverser(pose);
	const Vecteur transforme = appliquer(pose, point);
	const Vecteur retour = appliquer(inverse, transforme);
	const Vecteur ecart = soustraire(retour, point);

	afficher("Point transforme : ", transforme);
	afficher("Apres application de l'inverse : ", retour);
	afficher("Ecart avec le point de depart : ", ecart);
	std::cout << "Norme de l'ecart : "
			  << std::sqrt(ecart.x * ecart.x + ecart.y * ecart.y + ecart.z * ecart.z)
			  << '\n';
}
