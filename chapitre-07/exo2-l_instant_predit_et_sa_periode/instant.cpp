#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkClock.h"
#include "NKTime/NkChrono.h"
#include "NKImage/Core/NkImage.h"

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

#include "NKMath/NkAngle.h"
#include "NKMath/NkQuat.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <iomanip>

using namespace nkentseu;
using namespace nkentseu::renderer;

namespace {

    // Dimensions de la salle en mètres.
    const float32 kPiece = 4.00f;
    const float32 kPlafond = 2.50f;
    const float32 kMur = 0.10f;

    // Dimensions de la porte.
    const float32 kPorteL = 0.90f;
    const float32 kPorteH = 2.00f;

    // Dimensions des fenêtres.
    const float32 kFenL = 1.20f;
    const float32 kFenH = 1.00f;
    const float32 kFenY = 1.40f;

    // Dimensions de la table.
    const float32 kTableL = 1.20f;
    const float32 kTableP = 0.80f;
    const float32 kTableH = 0.75f;

    const float32 kDemi = kPiece * 0.5f;

    constexpr uint64 kImagesIgnorees = 30;
    constexpr uint64 kNombreEcarts = 600;
    constexpr uint64 kNombreInstantsAffiches = 20;

    // Lit une variable d'environnement entière.
    uint64 EnvU64(const char *nom, uint64 valeurDefaut) {
        const char *valeur = std::getenv(nom);

        if (valeur == nullptr || valeur[0] == '\0') {
            return valeurDefaut;
        }

        return static_cast<uint64>(
            std::strtoull(valeur, nullptr, 10)
        );
    }

    // Lit la cadence demandée.
    double EnvHz() {
        const char *valeur = std::getenv("MASALLE_HZ");

        if (valeur == nullptr || valeur[0] == '\0') {
            return 72.0;
        }

        const double hz = std::strtod(valeur, nullptr);

        return hz > 0.0 ? hz : 72.0;
    }

    // Dessine un objet avec ses dimensions et sa position.
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

    // Soumet tous les objets de la salle au renderer 3D.
    void SoumettreLaSalle(
        NkRender3D *r3d,
        NkMeshSystem *meshes,
        NkMaterial *murMaterial
    ) {
        const NkVec3f grisMur = {0.72f, 0.70f, 0.66f};
        const NkVec3f grisSol = {0.38f, 0.36f, 0.34f};
        const NkVec3f bois = {0.55f, 0.38f, 0.22f};
        const NkVec3f vitre = {0.75f, 0.85f, 0.92f};

        // Sol.
        NkDrawCall3D sol;
        sol.mesh = meshes->GetPlane();
        sol.transform = NkMat4f::Scale({kPiece, 1.0f, kPiece});
        sol.aabb = {
            {-kDemi, 0.0f, -kDemi},
            {kDemi, 0.0f, kDemi}
        };
        sol.castShadow = false;
        sol.tint = grisSol;
        sol.metallic = 0.0f;
        sol.roughness = 1.0f;

        r3d->Submit(sol);

        // Quatre murs.
        const float32 yMur = kPlafond * 0.5f;
        const float32 bord = kDemi + kMur * 0.5f;

        Pave(
            r3d, meshes,
            {0.0f, yMur, -bord},
            {kPiece, kPlafond, kMur},
            grisMur, 1.0f, murMaterial
        );

        Pave(
            r3d, meshes,
            {0.0f, yMur, bord},
            {kPiece, kPlafond, kMur},
            grisMur
        );

        Pave(
            r3d, meshes,
            {-bord, yMur, 0.0f},
            {kMur, kPlafond, kPiece},
            grisMur
        );

        Pave(
            r3d, meshes,
            {bord, yMur, 0.0f},
            {kMur, kPlafond, kPiece},
            grisMur
        );

        // Plafond.
        Pave(
            r3d, meshes,
            {0.0f, kPlafond + kMur * 0.5f, 0.0f},
            {kPiece, kMur, kPiece},
            grisMur
        );

        // Porte sur le mur du fond.
        Pave(
            r3d, meshes,
            {-0.70f, kPorteH * 0.5f, -kDemi + 0.02f},
            {kPorteL, kPorteH, 0.04f},
            bois, 0.6f
        );

        // Deux fenêtres sur le mur de gauche.
        Pave(
            r3d, meshes,
            {-kDemi + 0.02f, kFenY, -0.90f},
            {0.04f, kFenH, kFenL},
            vitre, 0.15f
        );

        Pave(
            r3d, meshes,
            {-kDemi + 0.02f, kFenY, 0.90f},
            {0.04f, kFenH, kFenL},
            vitre, 0.15f
        );

        // Plateau de la table.
        const float32 tx = 0.60f;
        const float32 tz = 0.70f;
        const float32 ep = 0.05f;

        Pave(
            r3d, meshes,
            {tx, kTableH - ep * 0.5f, tz},
            {kTableL, ep, kTableP},
            bois, 0.5f
        );

        // Quatre pieds de la table.
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
                r3d, meshes,
                {tx + dx, hp * 0.5f, tz + dz},
                {pied, hp, pied},
                bois, 0.6f
            );
        }

        // Cube d'un mètre servant de référence.
        Pave(
            r3d, meshes,
            {-1.10f, 0.50f, 1.20f},
            {1.0f, 1.0f, 1.0f},
            {0.85f, 0.80f, 0.30f},
            0.8f
        );
    }

} // namespace

int nkmain(const NkEntryState &state) {
    (void)state;

    // Paramètres de mesure.
    const double hz = EnvHz();
    const double periodeNs = 1'000'000'000.0 / hz;
    const double periodeMs = periodeNs / 1'000'000.0;

    const uint64 limiteImages = EnvU64("MASALLE_EXIT", 700);

    // Fichier contenant les mesures.
    std::ofstream fichier("instants.txt", std::ios::app);

    if (!fichier.is_open()) {
        std::printf("Impossible d'ouvrir instants.txt\n");
        return -1;
    }

    fichier << std::fixed << std::setprecision(3);

    // Variables de mesure.
    double sommeEcartsMs = 0.0;
    uint64 nombreEcarts = 0;
    uint64 imageNumber = 0;

    double instantDepartNs = 0.0;
    double preditPrecedentNs = 0.0;

    bool chronometreDemarre = false;
    bool predictionPrecedenteValide = false;

    // Création de la fenêtre.
    NkWindowConfig cfg;
    cfg.title = "Chapitre 7 - Exo 2 : Instant prédit";
    cfg.width = 1280;
    cfg.height = 720;

    NkWindow window;

    if (!window.Create(cfg)) {
        logger.Error("La fenêtre n'a pas été créée");
        return -1;
    }

    // Création du périphérique graphique.
    NkDeviceInitInfo devInit{};
    devInit.surface = window.GetSurfaceDesc();
    devInit.width = window.GetSize().x;
    devInit.height = window.GetSize().y;

    NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);

    if (device == nullptr) {
        logger.Error("Échec de création du device");
        return -2;
    }

    // Renderer de la fenêtre.
    NkRendererConfig cfgEcran =
        NkRendererConfig::For2D(
            devInit.api,
            devInit.width,
            devInit.height
        );

    NkRenderer *rEcran = NkRenderer::Create(device, cfgEcran);

    if (rEcran == nullptr || !rEcran->Initialize()) {
        logger.Error("Échec de création du renderer de l'écran");
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Cible hors écran.
    NkOffscreenDesc od;
    od.width = devInit.width;
    od.height = devInit.height;
    od.hasDepth = true;
    od.hdr = false;
    od.colorFmt = NkGPUFormat::NK_RGBA8_UNORM;
    od.readable = true;
    od.readback = true;
    od.name = "Render_Cible";

    NkOffscreenTarget *cible = rEcran->CreateOffscreen(od);

    if (cible == nullptr) {
        logger.Error("Échec de création de la cible hors écran");
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Renderer de la scène 3D.
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

    NkRenderer *renderer = NkRenderer::Create(device, cfgScene);

    if (renderer == nullptr || !renderer->Initialize()) {
        logger.Error("Échec de création du renderer 3D");
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Matériau du mur.
    const NkTexHandle murTexture =
        renderer->GetTextures()->Load("murs/photo.png");

    NkMaterial *murMaterial = NkMaterial::Create(
        renderer->GetMaterials(),
        NkMaterialType::NK_PBR_METALLIC
    );

    if (murMaterial == nullptr) {
        logger.Error("Échec de création du matériau du mur");
        NkRenderer::Destroy(renderer);
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    murMaterial->SetAlbedoMap(murTexture);

    renderer->SetFinalColorTarget(
        rEcran->GetTextures()->GetRHIHandle(
            cible->GetColorResult()
        )
    );

    auto &eventSystem = NkEvents();
    bool running = true;

    NkClock horloge;

    while (running) {
        // Traitement des événements.
        NkEvent *event;

        while (eventSystem.PollEvent(event)) {
            if (event->Is<NkWindowCloseEvent>()) {
                running = false;
            }
        }

        if (!running) {
            break;
        }

        const NkClock::NkTime &temps = horloge.Tick();

        if (!rEcran->BeginFrame()) {
            logger.Error("BeginFrame a refusé l'image");
            continue;
        }

        // Mesure du début réel de l'image.
        const double maintenantNs =
            static_cast<double>(NkChrono::Now().nanoseconds);

        ++imageNumber;

        if (!chronometreDemarre) {
            instantDepartNs = maintenantNs;
            chronometreDemarre = true;
        }

        // Calcul de l'écart entre l'instant réel actuel
        // et la prédiction de l'image précédente.
        if (predictionPrecedenteValide &&
            imageNumber > kImagesIgnorees &&
            nombreEcarts < kNombreEcarts) {

            const double ecartMs =
                (maintenantNs - preditPrecedentNs) /
                1'000'000.0;

            sommeEcartsMs += ecartMs;
            ++nombreEcarts;
        }

        // Instant prédit pour l'image suivante.
        const double preditNs = maintenantNs + periodeNs;

        // Écrire les 20 premières lignes à 72 Hz.
        if (hz == 72.0 &&
            imageNumber <= kNombreInstantsAffiches) {

            const double maintenantMs =
                (maintenantNs - instantDepartNs) / 1'000'000.0;

            const double preditMs =
                (preditNs - instantDepartNs) / 1'000'000.0;

            const double ecartMs =
                (preditNs - maintenantNs) / 1'000'000.0;

            std::printf(
                "maintenant = %.3f ms, predit = %.3f ms, ecart = %.3f ms\n",
                maintenantMs,
                preditMs,
                ecartMs
            );

            fichier
                << "maintenant = " << maintenantMs
                << " ms, predit = " << preditMs
                << " ms, ecart = " << ecartMs
                << " ms\n";

            fichier.flush();
        }

        preditPrecedentNs = preditNs;
        predictionPrecedenteValide = true;

        // Préparation du rendu.
        NkICommandBuffer *cmd = rEcran->GetCmd();
        NkRender3D *r3d = renderer->GetRender3D();

        renderer->FlushGraphRebuilds();
        r3d->ResetFrame();

        if (auto *mc = renderer->GetMaterialCollection()) {
            mc->Upload();
        }

        // Position et orientation de la caméra.
        NkCamera3DData cd;
        cd.position = {0.30f, 1.70f, 1.20f};
        cd.target = {-0.70f, 1.10f, -kDemi};
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
        sctx.time = temps.total;

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

        // Exécution du graphe de rendu.
        if (auto *graph = renderer->GetRenderGraph()) {
            graph->Execute(cmd);
        }

        // Affichage de la cible hors écran.
        if (auto *r2d = rEcran->GetRender2D()) {
            r2d->Begin(cmd, devInit.width, devInit.height);

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

        // Arrêt automatique après le nombre d'images demandé.
        if (imageNumber >= limiteImages) {
            running = false;
        }
    }

    // Écriture du résultat moyen.
    if (nombreEcarts > 0) {
        const double moyenneEcartsMs =
            sommeEcartsMs / static_cast<double>(nombreEcarts);

        std::printf(
            "hz = %.0f, periode = %.3f ms, reel - predit = %.3f ms\n",
            hz,
            periodeMs,
            moyenneEcartsMs
        );

        fichier
            << "hz = " << hz
            << ", periode = " << periodeMs
            << " ms, reel - predit = " << moyenneEcartsMs
            << " ms\n";

        fichier.flush();
    } else {
        std::printf(
            "Mesures insuffisantes : %llu écarts collectés.\n",
            static_cast<unsigned long long>(nombreEcarts)
        );
    }

    // Libération des ressources.
    NkMaterial::Destroy(murMaterial);
    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);

    fichier.close();

    return 0;
}