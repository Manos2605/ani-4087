 # Exercice 8 : le champ de vision asymétrique

 ## Énoncé

Cherchez les quatre angles du champ de vision d'un casque du commerce, pour l'oeil il gauche. Rendez-les, avec votre source.

Puis dites, en une phrase, ce qui se passerait si l'on employait à la place un champ symétrique de même surface.

 ## Solution

 J'ai choisi le **HTC Vive**. Pour l'oeil gauche, les quatre angles du champ de
 vision sont :

 | Direction | Angle |
 |---|---:|
 | Gauche | `-54,41°` |
 | Droite | `+51,35°` |
 | Bas | `-55,91°` |
 | Haut | `+55,67°` |

 On obtient donc environ :

 ```text
 Champ horizontal = 54,41° + 51,35° = 105,76°
 Champ vertical   = 55,91° + 55,67° = 111,58°
 ```

 Le champ n'est pas parfaitement symétrique : l'angle vers la gauche n'est
 pas égal à celui vers la droite, et l'angle vers le bas n'est pas égal à
 celui vers le haut. C'est normal, car l'oeil gauche n'est pas placé au centre
 de l'ensemble des deux yeux et parce que les lentilles et la projection du
 casque tiennent compte de sa position réelle.

 ## Sources

 Les valeurs du HTC Vive viennent de la base de données de géométrie des
 casques VR de Risa2000, qui rassemble les données de plusieurs casques :

 - [Dépôt GitHub HMD Geometry Database](https://github.com/Risa2000/hmdgdb)
 - [Documentation OpenVR de Valve : GetProjectionRaw](https://github.com/ValveSoftware/openvr/wiki/IVRSystem::GetProjectionRaw)

 Pour la signification des quatre angles, j'ai aussi utilisé la documentation
 de Khronos sur OpenXR. Elle définit le champ avec `angleLeft`, `angleRight`,
 `angleUp` et `angleDown`, mesurés depuis l'axe de vue de chaque oeil :

 - [OpenXR Specification - FOV angles](https://registry.khronos.org/OpenXR/specs/1.0/html/xrspec.html#XrFovf)

 ## Champ symétrique de même surface

 Si on remplaçait ce champ asymétrique par un champ symétrique de même surface,
 on garderait à peu près la même quantité de vision, mais les limites du champ
 seraient déplacées : la projection ne correspondrait plus exactement à la
 position de l'oeil et aux lentilles du casque, ce qui pourrait déformer les
 bords de l'image ou faire perdre une partie de la scène.

