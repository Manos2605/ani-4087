#include <chrono>
#include <iostream>

int main() {
    // On mesure exactement 1 000 images et on vérifie la limite de 11 ms.
    const int nombreImages = 1000;
    const double limit = 11.0;
    const int pixelCount = 1920 * 1080;

    // Ce tableau représente les pixels de l'écran.
    int* pixels = new int[pixelCount];
    double longestFrame = 0.0;
    int framesOverLimit = 0;

    for (int frame = 0; frame < nombreImages; ++frame) {
        // On commence à mesurer le temps nécessaire pour dessiner une image.
        const auto start = std::chrono::steady_clock::now();

        // On efface tous les pixels de l'écran.
        for (int pixel = 0; pixel < pixelCount; ++pixel) {
            pixels[pixel] = 0;
        }

        // On arrête le chronomètre et on convertit le temps en millisecondes.
        const auto end = std::chrono::steady_clock::now();
        const double frameTime = std::chrono::duration<double, std::milli>(end - start).count();

        // On garde le temps de l'image la plus lente.
        if (frameTime > longestFrame) {
            longestFrame = frameTime;
        }

        // On compte les images qui dépassent la limite de 11 ms.
        if (frameTime > limit) {
            ++framesOverLimit;
        }
    }

    std::cout << "Plus longue image : " << longestFrame << " ms\n";
    std::cout << "Images au-dessus de 11 ms : " << framesOverLimit << " sur " << nombreImages << "\n";

    // On libère la mémoire utilisée par le tableau de pixels.
    delete[] pixels;
    return 0;
}
