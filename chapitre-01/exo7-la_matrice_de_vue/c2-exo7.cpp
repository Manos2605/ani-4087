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

struct Matrice {
	double valeur[4][4]{};
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

Matrice matriceDePose(const Pose& pose) {
	const Quaternion& q = pose.rotation;
	const double xx = q.x * q.x;
	const double yy = q.y * q.y;
	const double zz = q.z * q.z;
	const double xy = q.x * q.y;
	const double xz = q.x * q.z;
	const double yz = q.y * q.z;
	const double wx = q.w * q.x;
	const double wy = q.w * q.y;
	const double wz = q.w * q.z;

	return {{
		{1.0 - 2.0 * (yy + zz), 2.0 * (xy - wz), 2.0 * (xz + wy), pose.position.x},
		{2.0 * (xy + wz), 1.0 - 2.0 * (xx + zz), 2.0 * (yz - wx), pose.position.y},
		{2.0 * (xz - wy), 2.0 * (yz + wx), 1.0 - 2.0 * (xx + yy), pose.position.z},
		{0.0, 0.0, 0.0, 1.0}
	}};
}

Matrice inverserGenerale(const Matrice& matrice, bool& inversible) {
	Matrice travail = matrice;
	Matrice inverse{{
		{1.0, 0.0, 0.0, 0.0},
		{0.0, 1.0, 0.0, 0.0},
		{0.0, 0.0, 1.0, 0.0},
		{0.0, 0.0, 0.0, 1.0}
	}};
	const double epsilon = 1e-12;
	inversible = true;

	for (int colonne = 0; colonne < 4; ++colonne) {
		int pivot = colonne;
		for (int ligne = colonne + 1; ligne < 4; ++ligne) {
			if (std::abs(travail.valeur[ligne][colonne]) > std::abs(travail.valeur[pivot][colonne])) {
				pivot = ligne;
			}
		}

		if (std::abs(travail.valeur[pivot][colonne]) < epsilon) {
			inversible = false;
			return Matrice{{
				{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, 1.0}
			}};
		}

		for (int j = 0; j < 4; ++j) {
			std::swap(travail.valeur[colonne][j], travail.valeur[pivot][j]);
			std::swap(inverse.valeur[colonne][j], inverse.valeur[pivot][j]);
		}

		const double valeurPivot = travail.valeur[colonne][colonne];
		for (int j = 0; j < 4; ++j) {
			travail.valeur[colonne][j] /= valeurPivot;
			inverse.valeur[colonne][j] /= valeurPivot;
		}

		for (int ligne = 0; ligne < 4; ++ligne) {
			if (ligne == colonne) {
				continue;
			}
			const double facteur = travail.valeur[ligne][colonne];
			for (int j = 0; j < 4; ++j) {
				travail.valeur[ligne][j] -= facteur * travail.valeur[colonne][j];
				inverse.valeur[ligne][j] -= facteur * inverse.valeur[colonne][j];
			}
		}
	}

	return inverse;
}

Matrice inverseDirecte(const Pose& pose) {
	const Quaternion& q = pose.rotation;
	const Quaternion conjugue{q.w, -q.x, -q.y, -q.z};
	const Pose poseInverse{
		{-pose.position.x, -pose.position.y, -pose.position.z},
		conjugue
	};
	const Matrice rotationInverse = matriceDePose(poseInverse);
	Matrice resultat = rotationInverse;
	resultat.valeur[0][3] = rotationInverse.valeur[0][0] * -pose.position.x + rotationInverse.valeur[0][1] * -pose.position.y + rotationInverse.valeur[0][2] * -pose.position.z;
	resultat.valeur[1][3] = rotationInverse.valeur[1][0] * -pose.position.x + rotationInverse.valeur[1][1] * -pose.position.y + rotationInverse.valeur[1][2] * -pose.position.z;
	resultat.valeur[2][3] = rotationInverse.valeur[2][0] * -pose.position.x + rotationInverse.valeur[2][1] * -pose.position.y + rotationInverse.valeur[2][2] * -pose.position.z;
	return resultat;
}

double ecartMaximum(const Matrice& gauche, const Matrice& droite) {
	double maximum = 0.0;
	for (int ligne = 0; ligne < 4; ++ligne) {
		for (int colonne = 0; colonne < 4; ++colonne) {
			maximum = std::max(maximum, std::abs(
				gauche.valeur[ligne][colonne] - droite.valeur[ligne][colonne]));
		}
	}
	return maximum;
}

void afficherMatrice(const char* nom, const Matrice& matrice) {
	std::cout << nom << '\n';
	for (int ligne = 0; ligne < 4; ++ligne) {
		for (int colonne = 0; colonne < 4; ++colonne) {
			std::cout << '\t' << matrice.valeur[ligne][colonne];
		}
		std::cout << '\n';
	}
}

int main() {
	const Pose pose{
		{1.0, 2.0, 3.0},
		quaternionDepuisAngleEtAxe(35.0, {0.0, 0.0, 1.0})
	};
	const Matrice matricePose = matriceDePose(pose);
	bool inversible = false;
	const Matrice inverseGenerale = inverserGenerale(matricePose, inversible);
	const Matrice matriceInverseDirecte = inverseDirecte(pose);

	std::cout << std::fixed << std::setprecision(4);
	afficherMatrice("Inverse generale :", inverseGenerale);
	afficherMatrice("Inverse directe :", matriceInverseDirecte);
	std::cout << "Ecart maximum entre les 16 coefficients : "
			  << ecartMaximum(inverseGenerale, matriceInverseDirecte) << '\n';

	const Matrice matriceSinguliere{{
		{1.0, 2.0, 3.0, 4.0},
		{1.0, 2.0, 3.0, 4.0},
		{0.0, 1.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 1.0}
	}};
	bool degenerateInversible = false;
	const Matrice resultatSingulier = inverserGenerale(matriceSinguliere, degenerateInversible);
	std::cout << "\nPose degeneree inversible : " << (degenerateInversible ? "oui" : "non") << '\n';
	afficherMatrice("Resultat rendu pour la pose degeneree :", resultatSingulier);
}
