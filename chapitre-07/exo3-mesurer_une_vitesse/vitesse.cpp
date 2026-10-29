#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkMouseEvent.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkClock.h"

#include "NKRHI/Core/NkDeviceFactory.h"
#include "NKRHI/Core/NkGraphicsApi.h"

#include "NKRenderer/NkRenderer.h"
#include "NKRenderer/Core/NkCamera.h"
#include "NKRenderer/Core/NkRenderGraph.h"
#include "NKRenderer/Core/NkTextureLibrary.h"
#include "NKRenderer/Materials/NkMaterial.h"
#include "NKRenderer/Materials/NkMaterialCollection.h"
#include "NKRenderer/Mesh/NkMeshSystem.h"
#include "NKRenderer/Tools/Offscreen/NkOffscreenTarget.h"
#include "NKRenderer/Tools/Render2D/NkRender2D.h"
#include "NKRenderer/Tools/Render3D/NkRender3D.h"

#include "NKMath/NkQuat.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;
using namespace nkentseu::renderer;

namespace {

    // Dimensions de la salle en mètres.
    const float32 kPiece = 4.00f;
    const float32 kPlafond = 2.50f;
    const float32 kMur = 0.10f;

    const float32 kPorteL = 0.90f;
    const float32 kPorteH = 2.00f;

    const float32 kFenL = 1.20f;
    const float32 kFenH = 1.00f;
    const float32 kFenY = 1.40f;

    const float32 kTableL = 1.20f;
    const float32 kTableP = 0.80f;
    const float32 kTableH = 0.75f;

    const float32 kDemi = kPiece * 0.5f;

    const float32 kPi = 3.14159265358979323846f;
    const float32 kSourisRad = 0.0022f;
    const uint32 kEchauffement = 30;
    const uint32 kMesures = 20;

    using Horloge = std::chrono::steady_clock;

    // Lit une valeur flottante dans une variable d'environnement.
    float32 LireFloat(const char *nom, float32 defaut) {
        const char *valeur = std::getenv(nom);

        if (valeur == nullptr || valeur[0] == '\0') {
            return defaut;
        }

        return static_cast<float32>(std::atof(valeur));
    }

    // Lit une valeur entière dans une variable d'environnement.
    uint64 LireEntier(const char *nom, uint64 defaut) {
        const char *valeur = std::getenv(nom);

        if (valeur == nullptr || valeur[0] == '\0') {
            return defaut;
        }

        return static_cast<uint64>(
            std::strtoull(valeur, nullptr, 10)
        );
    }

    // Dessine un pavé dans la salle.
    void Pave(
        NkRender3D *r3d,
        NkMeshSystem *meshes,
        const NkVec3f &centre,
        const NkVec3f &taille,
        const NkVec3f &couleur,
        float32 rugosite = 1.0f,
        NkMaterial *material = nullptr
    ) {
        NkDrawCall3D dc;

        dc.mesh = meshes->GetCube();

        dc.transform =
            NkMat4f::Translate(centre) *
            NkMat4f::Scale(taille);

        dc.aabb = {
            {
                centre.x - taille.x * 0.5f,
                centre.y - taille.y * 0.5f,
                centre.z - taille.z * 0.5f
            },
            {
                centre.x + taille.x * 0.5f,
                centre.y + taille.y * 0.5f,
                centre.z + taille.z * 0.5f
            }
        };

        dc.tint = couleur;
        dc.metallic = 0.0f;
        dc.roughness = rugosite;

        if (material != nullptr) {
            dc.material = material->GetInstHandle();
        }

        r3d->Submit(dc);
    }

    // Un objet de la salle.
    struct Piece {
        NkVec3f centre;
        NkVec3f taille;
        NkVec3f couleur;
        float32 rugosite;
        bool materiauMur;
    };

    // Construit la salle complète.
    void SoumettreLaSalle(
        NkRender3D *r3d,
        NkMeshSystem *meshes,
        NkMaterial *murMaterial
    ) {
        const NkVec3f grisMur = {0.72f, 0.70f, 0.66f};
        const NkVec3f grisSol = {0.38f, 0.36f, 0.34f};
        const NkVec3f bois = {0.55f, 0.38f, 0.22f};
        const NkVec3f vitre = {0.75f, 0.85f, 0.92f};

        const float32 yMur = kPlafond * 0.5f;
        const float32 bord = kDemi + kMur * 0.5f;

        const float32 tx = 0.60f;
        const float32 tz = 0.70f;
        const float32 ep = 0.05f;

        // Sol.
        NkDrawCall3D sol;

        sol.mesh = meshes->GetPlane();
        sol.transform = NkMat4f::Scale(
            {kPiece, 1.0f, kPiece}
        );

        sol.aabb = {
            {-kDemi, 0.0f, -kDemi},
            {kDemi, 0.0f, kDemi}
        };

        sol.castShadow = false;
        sol.tint = grisSol;
        sol.metallic = 0.0f;
        sol.roughness = 1.0f;

        r3d->Submit(sol);

        // Murs, plafond, porte, fenêtres, plateau et cube.
        const Piece pieces[] = {
            // Les quatre murs et le plafond.
            {
                {0.0f, yMur, -bord},
                {kPiece, kPlafond, kMur},
                grisMur, 1.0f, true
            },
            {
                {0.0f, yMur, bord},
                {kPiece, kPlafond, kMur},
                grisMur, 1.0f, false
            },
            {
                {-bord, yMur, 0.0f},
                {kMur, kPlafond, kPiece},
                grisMur, 1.0f, false
            },
            {
                {bord, yMur, 0.0f},
                {kMur, kPlafond, kPiece},
                grisMur, 1.0f, false
            },
            {
                {0.0f, kPlafond + kMur * 0.5f, 0.0f},
                {kPiece, kMur, kPiece},
                grisMur, 1.0f, false
            },

            // Porte.
            {
                {-0.70f, kPorteH * 0.5f, -kDemi + 0.02f},
                {kPorteL, kPorteH, 0.04f},
                bois, 0.6f, false
            },

            // Fenêtres.
            {
                {-kDemi + 0.02f, kFenY, -0.90f},
                {0.04f, kFenH, kFenL},
                vitre, 0.15f, false
            },
            {
                {-kDemi + 0.02f, kFenY, 0.90f},
                {0.04f, kFenH, kFenL},
                vitre, 0.15f, false
            },

            // Plateau de table.
            {
                {tx, kTableH - ep * 0.5f, tz},
                {kTableL, ep, kTableP},
                bois, 0.5f, false
            },

            // Cube de référence.
            {
                {-1.10f, 0.50f, 1.20f},
                {1.0f, 1.0f, 1.0f},
                {0.85f, 0.80f, 0.30f},
                0.8f, false
            }
        };

        for (const Piece &p : pieces) {
            Pave(
                r3d,
                meshes,
                p.centre,
                p.taille,
                p.couleur,
                p.rugosite,
                p.materiauMur ? murMaterial : nullptr
            );
        }

        // Les quatre pieds de la table.
        const float32 pied = 0.06f;
        const float32 hp = kTableH - ep;

        for (int i = 0; i < 4; ++i) {
            const float32 dx =
                ((i & 1) ? 1.0f : -1.0f) *
                (kTableL * 0.5f - pied);

            const float32 dz =
                ((i & 2) ? 1.0f : -1.0f) *
                (kTableP * 0.5f - pied);

            Pave(
                r3d,
                meshes,
                {tx + dx, hp * 0.5f, tz + dz},
                {pied, hp, pied},
                bois,
                0.6f
            );
        }
    }

    // Calcule la vitesse angulaire entre deux orientations horodatées.
    NkVec3f VitesseAngulaire(
        const NkQuatf &from,
        const NkQuatf &to,
        float32 dtSeconds
    ) {
        if (dtSeconds <= 0.0f) {
            return NkVec3f(0.0f, 0.0f, 0.0f);
        }

        NkQuatf delta =
            (to * from.Conjugate()).Normalized();

        // On retient le chemin de rotation le plus court.
        if (delta.w < 0.0f) {
            delta = NkQuatf(
                -delta.x,
                -delta.y,
                -delta.z,
                -delta.w
            );
        }

        const float32 angleRad = delta.Angle().Rad();

        if (angleRad <= 1e-6f) {
            return NkVec3f(0.0f, 0.0f, 0.0f);
        }

        return delta.Axis() * (angleRad / dtSeconds);
    }

} // namespace

int nkmain(const NkEntryState &state) {
    (void)state;

    // MASALLE_TOUR = vitesse du lacet en degrés par seconde.
    // Une valeur nulle permet de commander la tête à la souris.
    const float32 tourDeg = LireFloat("MASALLE_TOUR", 0.0f);

    // Nombre maximal d'images avant l'arrêt automatique.
    const uint64 imageDeSortie = LireEntier("MASALLE_EXIT", 0);

    const bool tourProgramme = std::fabs(tourDeg) > 1e-6f;

    std::printf(
        "serie %s\n",
        tourProgramme ? "programme" : "souris"
    );

    // Création de la fenêtre.
    NkWindowConfig cfg;
    cfg.title = "Mesure de la vitesse angulaire";
    cfg.width = 1280;
    cfg.height = 720;

    NkWindow window;

    if (!window.Create(cfg)) {
        logger.Error("La fenetre n'a pas ete creee");
        return -1;
    }

    // Création du périphérique graphique.
    NkDeviceInitInfo devInit{};
    devInit.surface = window.GetSurfaceDesc();
    devInit.width = window.GetSize().x;
    devInit.height = window.GetSize().y;

    NkIDevice *device =
        NkDeviceFactory::CreateAutoDetect(devInit);

    if (device == nullptr) {
        logger.Error("Echec de creation du device");
        return -2;
    }

    // Renderer de la fenêtre.
    NkRendererConfig cfgEcran =
        NkRendererConfig::For2D(
            devInit.api,
            devInit.width,
            devInit.height
        );

    NkRenderer *rEcran =
        NkRenderer::Create(device, cfgEcran);

    if (rEcran == nullptr || !rEcran->Initialize()) {
        logger.Error("Echec de creation du renderer");

        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Cible de rendu.
    NkOffscreenDesc od;
    od.width = devInit.width;
    od.height = devInit.height;
    od.hasDepth = true;
    od.hdr = false;
    od.colorFmt = NkGPUFormat::NK_RGBA8_UNORM;
    od.readable = true;
    od.readback = true;
    od.name = "Render_Cible";

    NkOffscreenTarget *cible =
        rEcran->CreateOffscreen(od);

    if (cible == nullptr) {
        logger.Error("Echec de creation de la cible");

        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Renderer 3D.
    NkRendererConfig cfgScene =
        NkRendererConfig::ForGame(
            devInit.api,
            devInit.width,
            devInit.height
        );

    cfgScene.Disable(NK_SS_RENDER2D);
    cfgScene.Disable(NK_SS_TEXT);
    cfgScene.Disable(NK_SS_OVERLAY);
    cfgScene.Enable(NK_SS_OFFSCREEN);

    NkRenderer *renderer =
        NkRenderer::Create(device, cfgScene);

    if (renderer == nullptr || !renderer->Initialize()) {
        logger.Error("Echec de creation du renderer 3D");

        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Matériau du mur.
    NkMaterial *murMaterial = NkMaterial::Create(
        renderer->GetMaterials(),
        NkMaterialType::NK_PBR_METALLIC
    );

    if (murMaterial == nullptr) {
        logger.Error("Echec de creation du materiau du mur");

        NkRenderer::Destroy(renderer);
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    renderer->SetFinalColorTarget(
        rEcran->GetTextures()->GetRHIHandle(
            cible->GetColorResult()
        )
    );

    // Variables de suivi de la tête.
    float32 lacetRad = 0.0f;
    float32 tangageRad = 0.0f;

    // Déplacements bruts de souris accumulés entre deux images.
    int64 sourisX = 0;
    int64 sourisY = 0;

    // Orientation et instant de l'image précédente.
    bool premierePose = true;
    NkQuatf orientationPrecedente;
    Horloge::time_point instantPrecedent;

    uint64 numeroImage = 0;
    uint32 nombreMesures = 0;

    // Temps de référence pour le tour programmé.
    const Horloge::time_point debut = Horloge::now();

    auto &eventSystem = NkEvents();
    bool running = true;

    while (running) {
        // Collecte de tous les événements.
        NkEvent *event = nullptr;

        while (eventSystem.PollEvent(event)) {
            if (event->As<NkWindowCloseEvent>() != nullptr) {
                running = false;
                continue;
            }

            // Les déplacements sont accumulés puis consommés une fois
            // par image, pas directement dans la pompe d'événements.
            if (!tourProgramme) {
                if (auto *mouvement = event->As<NkMouseRawEvent>()) {
                    sourisX += mouvement->GetDeltaX();
                    sourisY += mouvement->GetDeltaY();
                }
            }
        }

        if (!running) {
            break;
        }

        if (!rEcran->BeginFrame()) {
            logger.Error("BeginFrame a refuse l'image");
            continue;
        }

        ++numeroImage;

        NkICommandBuffer *cmd = rEcran->GetCmd();
        NkRender3D *r3d = renderer->GetRender3D();

        renderer->FlushGraphRebuilds();
        r3d->ResetFrame();

        if (auto *mc = renderer->GetMaterialCollection()) {
            mc->Upload();
        }

        const Horloge::time_point maintenant = Horloge::now();

        const float32 tempsDepuisDebut =
            std::chrono::duration<float>(
                maintenant - debut
            ).count();

        // Le tour programmé dépend du temps écoulé, et non du nombre
        // d'images calculées.
        if (tourProgramme) {
            lacetRad =
                tourDeg * (kPi / 180.0f) *
                tempsDepuisDebut;
        } else {
            // Appliquer le déplacement accumulé une seule fois.
            lacetRad -=
                static_cast<float32>(sourisX) *
                kSourisRad;

            tangageRad -=
                static_cast<float32>(sourisY) *
                kSourisRad;

            // Éviter de retourner complètement la tête.
            tangageRad = std::max(
                -1.50f,
                std::min(1.50f, tangageRad)
            );

            // L'accumulateur est vidé après consommation.
            sourisX = 0;
            sourisY = 0;
        }

        // Orientation : rotation autour de Y, puis autour de X.
        const NkMat4f rotation =
            NkMat4f::RotationY(
                NkAngle::FromRad(lacetRad)
            ) *
            NkMat4f::RotationX(
                NkAngle::FromRad(tangageRad)
            );

        const NkQuatf orientation =
            NkQuatf(rotation).Normalized();

        // Mesure de la vitesse angulaire entre deux poses.
        const float32 dtSeconds =
            premierePose
                ? 0.0f
                : std::chrono::duration<float>(
                      maintenant - instantPrecedent
                  ).count();

        if (!premierePose && nombreMesures < kMesures) {
            const NkVec3f vitesseRad =
                VitesseAngulaire(
                    orientationPrecedente,
                    orientation,
                    dtSeconds
                );

            const float32 facteurDeg = 180.0f / kPi;

            const NkVec3f vitesseDeg = {
                vitesseRad.x * facteurDeg,
                vitesseRad.y * facteurDeg,
                vitesseRad.z * facteurDeg
            };

            // Les 30 premières images servent à stabiliser le démarrage.
            if (numeroImage > kEchauffement) {
                ++nombreMesures;

                std::printf(
                    "image %llu : dt = %.3f ms, "
                    "vitesse = (%.3f, %.3f, %.3f) deg/s\n",
                    static_cast<unsigned long long>(numeroImage),
                    static_cast<double>(dtSeconds * 1000.0f),
                    static_cast<double>(vitesseDeg.x),
                    static_cast<double>(vitesseDeg.y),
                    static_cast<double>(vitesseDeg.z)
                );
            }
        }

        orientationPrecedente = orientation;
        instantPrecedent = maintenant;
        premierePose = false;

        // Caméra placée à hauteur d'yeux et orientée selon la tête.
        NkCamera3DData cd;
        cd.position = {0.30f, 1.70f, 1.20f};

        cd.target = cd.position +
            orientation * NkVec3f(0.0f, 0.0f, -1.0f);

        cd.up = {0.0f, 1.0f, 0.0f};
        cd.fovY = 70.0f;
        cd.aspect =
            static_cast<float32>(devInit.width) /
            static_cast<float32>(devInit.height);

        cd.nearPlane = 0.05f;
        cd.farPlane = 100.0f;

        // Lumière.
        NkSceneContext sctx;
        sctx.camera = NkCamera3D(cd);
        sctx.time = tempsDepuisDebut;

        NkLightDesc soleil;
        soleil.type = NkLightType::NK_DIRECTIONAL;
        soleil.direction = {-0.5f, -1.0f, -0.35f};
        soleil.color = {1.0f, 0.96f, 0.88f};
        soleil.intensity = 3.0f;
        soleil.castShadow = false;

        sctx.lights.PushBack(soleil);

        r3d->SetIBLStrength(0.35f);
        r3d->BeginScene(sctx);

        // Dessin de la salle complète.
        SoumettreLaSalle(
            r3d,
            renderer->GetMeshSystem(),
            murMaterial
        );

        // Exécution du rendu.
        if (auto *graph = renderer->GetRenderGraph()) {
            graph->Execute(cmd);
        }

        if (auto *r2d = rEcran->GetRender2D()) {
            r2d->Begin(
                cmd,
                devInit.width,
                devInit.height
            );

            r2d->DrawImage(
                cible->GetColorResult(),
                {
                    0.0f,
                    0.0f,
                    static_cast<float32>(devInit.width),
                    static_cast<float32>(devInit.height)
                }
            );

            r2d->End();
        }

        rEcran->Present();
        rEcran->EndFrame();

        // Arrêt automatique facultatif.
        if (imageDeSortie > 0 &&
            numeroImage >= imageDeSortie) {
            running = false;
        }
    }

    // Libération des ressources.
    NkMaterial::Destroy(murMaterial);
    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);

    return 0;
}