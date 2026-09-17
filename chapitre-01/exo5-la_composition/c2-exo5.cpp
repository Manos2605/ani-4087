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
	const double demiAngle = angleDegres * pi / 360.0;
	const double sinus = std::sin(demiAngle);

	return {
		std::cos(demiAngle),
		axe.x * sinus,
		axe.y * sinus,
		axe.z * sinus
	};
}

Quaternion multiplier(const Quaternion& gauche, const Quaternion& droite) {
	return {
		gauche.w * droite.w - gauche.x * droite.x - gauche.y * droite.y - gauche.z * droite.z,
		gauche.w * droite.x + gauche.x * droite.w + gauche.y * droite.z - gauche.z * droite.y,
		gauche.w * droite.y - gauche.x * droite.z + gauche.y * droite.w + gauche.z * droite.x,
		gauche.w * droite.z + gauche.x * droite.y - gauche.y * droite.x + gauche.z * droite.w
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

Vecteur ajouter(const Vecteur& gauche, const Vecteur& droite) {
	return {gauche.x + droite.x, gauche.y + droite.y, gauche.z + droite.z};
}

Vecteur appliquer(const Pose& pose, const Vecteur& point) {
	return ajouter(tourner(pose.rotation, point), pose.position);
}

Pose composer(const Pose& parent, const Pose& enfant) {
	return {
		ajouter(tourner(parent.rotation, enfant.position), parent.position),
		multiplier(parent.rotation, enfant.rotation)
	};
}

Vecteur soustraire(const Vecteur& gauche, const Vecteur& droite) {
	return {gauche.x - droite.x, gauche.y - droite.y, gauche.z - droite.z};
}

void afficher(const char* nom, const Vecteur& point) {
	std::cout << nom << point.x << ' ' << point.y << ' ' << point.z << '\n';
}

int main() {
	const Pose parent{
		{1.0, 2.0, 0.0},
		quaternionDepuisAngleEtAxe(90.0, {0.0, 0.0, 1.0})
	};
	const Pose enfant{
		{2.0, 0.0, 0.0},
		quaternionDepuisAngleEtAxe(90.0, {0.0, 0.0, 1.0})
	};
	const Vecteur point{1.0, 0.0, 0.0};

	const Pose composee = composer(parent, enfant);
	const Vecteur pointCompose = appliquer(composee, point);
	const Vecteur pointEnchaine = appliquer(parent, appliquer(enfant, point));
	const Vecteur ecart = soustraire(pointCompose, pointEnchaine);

	afficher("Point avec la pose composee : ", pointCompose);
	afficher("Point avec les poses enchainees : ", pointEnchaine);
	afficher("Ecart : ", ecart);
	std::cout << "Norme de l'ecart : " << std::sqrt(ecart.x * ecart.x + ecart.y * ecart.y + ecart.z * ecart.z) << std::endl;
}
