# Exercice 2 - le tableau des budgets

# Enoncer

Reprenez le tableau des cinq étapes du chapitre. Pour chacune, cherchez une source, un article ou une documentation de constructeur, qui donne une valeur mesurée. Rendez le tableau avec vos sources.

# Solution
J'ai repris le tableau du chapitre 1 et pour chaque étape j'ai cherché un vrai chiffre mesuré. J'ai regardé surtout du côté techniques d'Oculus/Meta et de Valve, et deux ou trois papiers de recherche.


| Étape | Valeur du cours | Ce que j'ai trouvé | Source |
|---|---|---|---|
| Les capteurs mesurent le mouvement | 1 à 2 ms | Une IMU (centrale inertielle) toute seule a une latence d'environ 1 ms. Ça colle avec le livre. | Blog Valve d'Abrash, "Latency – the sine qua non of AR and VR" |
| Le système transmet la mesure | 1 à 3 ms | **Pas trouvé.** Dans toutes les sources que j'ai lues, ce chiffre est mélangé avec celui des capteurs (genre "capteur + fusion = 1ms", sans dire combien pour le transport tout seul). | ... |
| Votre application décide et dessine | 5 à 11 ms | La plupart des jeux n'arrivent pas à tenir 60 Hz stable, donc ils ont souvent plus de 16 ms de latence de rendu, jugé trop lent pour la VR. Une cible de 20-25 ms est citée pour la latence totale. | Blog Valve d'Abrash + Liu et al., MobiSys 2018 |
| Le compositeur assemble | 1 à 2 ms | Un papier mesure le noyau de l'algorithme "time warp" (ATW) à environ 2,3 à 2,55 ms en moyenne, sur un système ouvert appelé ILLIXR. | arXiv:2301.10408, "Minimizing the Motion-to-Photon-delay (MPD) in VR Systems" |
| L'écran affiche la ligne | 2 à 5 ms | Deux bouts trouvés, pas exactement la même chose : réponse d'un pixel OLED < 1 ms, mais balayage complet de l'image ~17 ms à 60 Hz sans découpage, ramené à ~1 ms par bande si l'image est envoyée en plusieurs morceaux. | displaymodule.com (fiche OLED VR) + DAQRI, Medium |

## Conclusion

Sur les 5 etapes, j'ai trouvé une source correcte pour 4. L'étape "transmission de la mesure" pas trouvé.

## Liens

- Abrash, "Latency – the sine qua non of AR and VR" : http://blogs.valvesoftware.com/abrash/latency-the-sine-qua-non-of-ar-and-vr/
- Liu et al., MobiSys 2018 (à la section 2-CHALLENGES ANDLATENCYANALYSIS) : https://www.winlab.rutgers.edu/~gruteser/papers/mobisys18_low_latency_vr.pdf
- "Minimizing the Motion-to-Photon-delay (MPD) in VR Systems", arXiv:2301.10408 : https://arxiv.org/pdf/2301.10408
- displaymodule.com, fiche OLED VR : https://www.displaymodule.com/blogs/knowledge/how-to-choose-an-oled-display-for-vr
- DAQRI, "Motion to Photon Latency in Mobile AR and VR" : https://medium.com/@DAQRI/motion-to-photon-latency-in-mobile-ar-and-vr-99f82c480926