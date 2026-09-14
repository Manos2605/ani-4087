#include <chrono>
#include <iomanip>
#include <iostream>

int main() {
    // On mesure 1 000 images avec un budget de 11 ms par image.
    const int nombreImages = 1000;
    const double budgetImage = 11.0;
    const int nombrePixels = 1920 * 1080;

    // Le tableau représente les pixels de l'écran.
    // volatile empêche le compilateur de supprimer les écritures mesurées.
    volatile int* pixels = new volatile int[nombrePixels];
    double plusLongueImage = 0.0;
    double sommeRendu = 0.0;

    for (int image = 0; image < nombreImages; ++image) {
        // On chronomètre uniquement le rendu, sans inclure les calculs suivants.
        const auto debut = std::chrono::steady_clock::now();

        // Effacer tous les pixels simule le rendu d'une image.
        for (int pixel = 0; pixel < nombrePixels; ++pixel) {
            pixels[pixel] = 0;
        }

        const auto fin = std::chrono::steady_clock::now();
        // Conversion de la durée mesurée en millisecondes.
        const double dureeRendu = std::chrono::duration<double, std::milli>(fin - debut).count();

        // On conserve la moyenne et la durée de l'image la plus lente.
        sommeRendu += dureeRendu;
        if (dureeRendu > plusLongueImage) {
            plusLongueImage = dureeRendu;
        }
    }

    // On estime le coût de deux rendus et le temps qui resterait pour la logique.
    const double renduMoyen = sommeRendu / nombreImages;
    const double doubleRendu = 2.0 * plusLongueImage;
    const double tempsRestant = budgetImage - doubleRendu;

    std::cout << "Rendu moyen : " << renduMoyen << " ms\n";
    std::cout << "Plus longue image : " << plusLongueImage << " ms\n";
    std::cout << "Rendu deux fois : " << doubleRendu << " ms\n";
    std::cout << "Temps restant sur 11 ms : " << tempsRestant << " ms\n";

    // Libération du tableau de pixels.
    delete[] pixels;
    return 0;
}
