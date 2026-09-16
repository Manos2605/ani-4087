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

Vecteur appliquer(const Pose& pose, const Vecteur& point) {
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

	Vecteur resultat{
		(1.0 - 2.0 * (yy + zz)) * point.x + 2.0 * (xy - wz) * point.y + 2.0 * (xz + wy) * point.z,
		2.0 * (xy + wz) * point.x + (1.0 - 2.0 * (xx + zz)) * point.y + 2.0 * (yz - wx) * point.z,
		2.0 * (xz - wy) * point.x + 2.0 * (yz + wx) * point.y + (1.0 - 2.0 * (xx + yy)) * point.z
	};

	resultat.x += pose.position.x;
	resultat.y += pose.position.y;
	resultat.z += pose.position.z;
	return resultat;
}

int main() {
	Pose pose{};
	Vecteur point{};

	std::cerr << "Position de la pose (x y z) : ";
	std::cin >> pose.position.x >> pose.position.y >> pose.position.z;

	std::cerr << "Quaternion de la pose (w x y z) : ";
	std::cin >> pose.rotation.w >> pose.rotation.x >> pose.rotation.y >> pose.rotation.z;

	std::cerr << "Point a transformer (x y z) : ";
	std::cin >> point.x >> point.y >> point.z;

	const Vecteur resultat = appliquer(pose, point);
	std::cerr << "Point transforme : ";
	std::cout << std::fixed << std::setprecision(4)
			  << resultat.x << ' ' << resultat.y << ' ' << resultat.z << '\n';
}
