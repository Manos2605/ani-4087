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

Pose composer(const Pose& parent, const Pose& enfant) {
	return {
		ajouter(tourner(parent.rotation, enfant.position), parent.position),
		multiplier(parent.rotation, enfant.rotation)
	};
}

void afficher(const char* nom, const Vecteur& point) {
	std::cout << nom << point.x << ' ' << point.y << ' ' << point.z << '\n';
}

int main() {
	const double longueurBras = 0.30;
	const double longueurAvantBras = 0.30;
	const Quaternion aucuneRotation = quaternionDepuisAngleEtAxe(
		0.0, {0.0, 0.0, 1.0});

	const Pose epaule{
		{0.0, 0.0, 0.0},
		aucuneRotation
	};
	const Pose coude{
		{longueurBras, 0.0, 0.0},
		aucuneRotation
	};
	const Pose main{
		{longueurAvantBras, 0.0, 0.0},
		aucuneRotation
	};

	const Pose coudeMonde = composer(epaule, coude);
	const Pose mainMonde = composer(coudeMonde, main);
	const Vecteur mainAvantRotation = mainMonde.position;

	const Pose epauleTournee{
		{0.0, 0.0, 0.0},
		quaternionDepuisAngleEtAxe(90.0, {0.0, 0.0, 1.0})
	};
	const Pose coudeApresRotation = composer(epauleTournee, coude);
	const Pose mainApresRotation = composer(coudeApresRotation, main);

	std::cout << std::fixed << std::setprecision(4);
	afficher("Coude dans le monde : ", coudeMonde.position);
	afficher("Main avant rotation de l'epaule : ", mainAvantRotation);
	afficher("Main apres rotation de l'epaule : ", mainApresRotation.position);
}
