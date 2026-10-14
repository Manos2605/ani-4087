#include "NKImage/Core/NkImage.h"
#include <iostream>

int nkmain(const NkEntryState &state) {

    nkentseu::NkImage image;
    bool charge = image.Load("murs/nexistepas.png");
    std::cout << "charge : " << charge << ", valide : " << image.IsValid() << ", largeur : " << image.Width() << ", hauteur : " << image.Height() << '\n';

}