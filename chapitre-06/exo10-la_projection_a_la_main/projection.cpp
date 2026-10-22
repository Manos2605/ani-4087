#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkClock.h"
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

    // Dimensions de la fenêtre.
    const float32 kFenL = 1.20f;
    const float32 kFenH = 1.00f;
    const float32 kFenY = 1.40f;

    // Dimensions de la table.
    const float32 kTableL = 1.20f;
    const float32 kTableP = 0.80f;
    const float32 kTableH = 0.75f;

    const float32 kDemi = kPiece * 0.5f;

    // Dessine un objet avec ses dimensions et sa position.
    void Pave(NkRender3D *r3d, NkMeshSystem *meshes,
              const NkVec3f &centre, const NkVec3f &taille,
              const NkVec3f &couleur, float32 rugosite = 1.0f,
              NkMaterial *material = nullptr) {

        NkDrawCall3D dc;

        dc.mesh = meshes->GetCube();
        dc.transform = NkMat4f::Translate(centre) * NkMat4f::Scale(taille);

        dc.aabb = {
            {centre.x - taille.x * 0.5f,
             centre.y - taille.y * 0.5f,
             centre.z - taille.z * 0.5f},

            {centre.x + taille.x * 0.5f,
             centre.y + taille.y * 0.5f,
             centre.z + taille.z * 0.5f}
        };

        dc.tint = couleur;
        dc.metallic = 0.0f;
        dc.roughness = rugosite;
        if (material != nullptr) {
            dc.material = material->GetInstHandle();
        }

        r3d->Submit(dc);
    }

    void SoumettreLaSalle(NkRender3D *r3d, NkMeshSystem *meshes,
                          NkMaterial *murMaterial) {

        const NkVec3f grisMur = {0.72f, 0.70f, 0.66f};
        const NkVec3f grisSol = {0.38f, 0.36f, 0.34f};
        const NkVec3f bois = {0.55f, 0.38f, 0.22f};
        const NkVec3f vitre = {0.75f, 0.85f, 0.92f};

        // Le sol.
        NkDrawCall3D sol;
        sol.mesh = meshes->GetPlane();
        sol.transform = NkMat4f::Scale({kPiece, 1.0f, kPiece});
        sol.aabb = {{-kDemi, 0.0f, -kDemi}, {kDemi, 0.0f, kDemi}};
        sol.castShadow = false;
        sol.tint = grisSol;
        sol.metallic = 0.0f;
        sol.roughness = 1.0f;
        r3d->Submit(sol);

        // Les quatre murs.
        const float32 yMur = kPlafond * 0.5f;
        const float32 bord = kDemi + kMur * 0.5f;

        Pave(r3d, meshes, {0.0f, yMur, -bord},
             {kPiece, kPlafond, kMur}, grisMur, 1.0f, murMaterial);

        Pave(r3d, meshes, {0.0f, yMur, bord},
             {kPiece, kPlafond, kMur}, grisMur);

        Pave(r3d, meshes, {-bord, yMur, 0.0f},
             {kMur, kPlafond, kPiece}, grisMur);

        Pave(r3d, meshes, {bord, yMur, 0.0f},
             {kMur, kPlafond, kPiece}, grisMur);

        // Le plafond.
        Pave(r3d, meshes, {0.0f, kPlafond + kMur * 0.5f, 0.0f},
             {kPiece, kMur, kPiece}, grisMur);

        // La porte sur le mur du fond.
        Pave(r3d, meshes, {-0.70f, kPorteH * 0.5f, -kDemi + 0.02f},
             {kPorteL, kPorteH, 0.04f}, bois, 0.6f);

        // Les deux fenêtres sur le mur de gauche.
        Pave(r3d, meshes, {-kDemi + 0.02f, kFenY, -0.90f},
             {0.04f, kFenH, kFenL}, vitre, 0.15f);

        Pave(r3d, meshes, {-kDemi + 0.02f, kFenY, 0.90f},
             {0.04f, kFenH, kFenL}, vitre, 0.15f);

        // Le plateau de la table.
        const float32 tx = 0.60f;
        const float32 tz = 0.70f;
        const float32 ep = 0.05f;

        Pave(r3d, meshes, {tx, kTableH - ep * 0.5f, tz},
             {kTableL, ep, kTableP}, bois, 0.5f);

        // Les quatre pieds de la table.
        const float32 pied = 0.06f;
        const float32 hp = kTableH - ep;

        for (int i = 0; i < 4; ++i) {

            const float32 dx =
                ((i & 1) ? 1.0f : -1.0f) * (kTableL * 0.5f - pied);

            const float32 dz =
                ((i & 2) ? 1.0f : -1.0f) * (kTableP * 0.5f - pied);

            Pave(r3d, meshes, {tx + dx, hp * 0.5f, tz + dz},
                 {pied, hp, pied}, bois, 0.6f);
        }

        // Un cube d'un mètre pour comparer les dimensions.
        Pave(r3d, meshes, {-1.10f, 0.50f, 1.20f},
             {1.0f, 1.0f, 1.0f}, {0.85f, 0.80f, 0.30f}, 0.8f);
    }

    // La matrice de projection décentrée, écrite à la main depuis la
    // matrice nulle. Les quatre angles sont en radians et signés.
    NkMat4f Projection(float32 gauche, float32 droite,
                       float32 haut, float32 bas,
                       float32 nearPlane, float32 farPlane) {

        const float32 tanLeft = NkTan(gauche);
        const float32 tanRight = NkTan(droite);
        const float32 tanUp = NkTan(haut);
        const float32 tanDown = NkTan(bas);

        const float32 tanWidth = tanRight - tanLeft;
        const float32 tanHeight = tanUp - tanDown;

        NkMat4f result; // la matrice NULLE

        result.mat[0][0] = 2.f / tanWidth;
        result.mat[1][1] = 2.f / tanHeight;

        result.mat[2][0] = (tanRight + tanLeft) / tanWidth;
        result.mat[2][1] = (tanUp + tanDown) / tanHeight;

        result.mat[2][3] = -1.f;

        result.mat[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
        result.mat[3][2] =
            2.f * farPlane * nearPlane / (nearPlane - farPlane);

        return result;
    }

    // Compare deux matrices et imprime une ligne par ligne de matrice.
    void AfficherEcarts(const char *nom,
                        const NkMat4f &a, const NkMat4f &b) {

        float64 plusGrand = 0.0;

        for (int ligne = 0; ligne < 4; ++ligne) {

            std::printf("%s, ligne %d :", nom, ligne);

            for (int colonne = 0; colonne < 4; ++colonne) {

                const float64 ecart = std::fabs(
                    (float64)a.mat[colonne][ligne] -
                    (float64)b.mat[colonne][ligne]);

                std::printf(" %.9f", ecart);

                if (ecart > plusGrand) {
                    plusGrand = ecart;
                }
            }

            std::printf("\n");
        }

        std::printf("%s : plus grand ecart %.9f\n", nom, plusGrand);
    }

} // namespace

int nkmain(const NkEntryState &state) {

    (void)state;

    const float32 nearPlane = 0.05f;
    const float32 farPlane = 100.0f;

    // Épreuve symétrique : 70 degrés verticalement, rapport 1280 / 720.
    // Les quatre angles se déduisent du champ vertical et du rapport.
    const float32 fovYDeg = 70.0f;
    const float32 aspect = 1280.0f / 720.0f;

    const float32 angleUp = NkAngle(fovYDeg * 0.5f).Rad();
    const float32 angleDown = -angleUp;
    const float32 angleRight = std::atan(NkTan(angleUp) * aspect);
    const float32 angleLeft = -angleRight;

    const NkMat4f symetriqueManuel = Projection(
        angleLeft, angleRight, angleUp, angleDown,
        nearPlane, farPlane);

    const NkMat4f symetriqueMoteur = NkMat4f::Perspective(
        NkAngle(fovYDeg), aspect, nearPlane, farPlane);

    AfficherEcarts("symetrique", symetriqueManuel, symetriqueMoteur);

    // Épreuve quest : l'œil gauche d'un casque, relevé sur un vrai casque.
    const float32 questGauche = NkAngle(-52.0f).Rad();
    const float32 questDroite = NkAngle(45.0f).Rad();
    const float32 questHaut = NkAngle(48.0f).Rad();
    const float32 questBas = NkAngle(-50.0f).Rad();

    const NkMat4f questManuel = Projection(
        questGauche, questDroite, questHaut, questBas,
        nearPlane, farPlane);

    NkCamera3D camera;
    camera.SetNearFar(nearPlane, farPlane);
    camera.SetFovAsym(questGauche, questDroite, questHaut, questBas);

    const NkMat4f questMoteur = camera.GetProj();

    AfficherEcarts("quest", questManuel, questMoteur);

    // Le centre de l'image ne regarde pas l'axe de la caméra : les deux
    // termes de décentrage valent directement la position du centre.
    std::printf("symetrique, centre optique : clip x %.6f, clip y %.6f\n",
                symetriqueManuel.mat[2][0], symetriqueManuel.mat[2][1]);

    std::printf("quest, centre optique : clip x %.6f, clip y %.6f\n",
                questManuel.mat[2][0], questManuel.mat[2][1]);

    // Crochet de test : le calcul seul, sans fenêtre ni carte graphique.
    if (std::getenv("MASALLE_MESURES") != nullptr) {
        return 0;
    }

    const char *shotFrameEnv = std::getenv("MASALLE_SHOT");
    const uint32 shotFrame = shotFrameEnv != nullptr
                                 ? static_cast<uint32>(std::strtoul(shotFrameEnv, nullptr, 10))
                                 : 0;
    const char *shotPath = std::getenv("MASALLE_SHOT_PATH");
    uint32 imageNumber = 0;
    bool shotCaptured = false;

    // Création de la fenêtre.
    NkWindowConfig cfg;
    cfg.title = "Ma salle";
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

    NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);

    if (device == nullptr) {
        logger.Error("Echec de creation du device");
        return -2;
    }

    // Création du renderer de la fenêtre.
    NkRendererConfig cfgEcran =
        NkRendererConfig::For2D(devInit.api, devInit.width, devInit.height);

    NkRenderer *rEcran = NkRenderer::Create(device, cfgEcran);

    if (rEcran == nullptr || !rEcran->Initialize()) {
        logger.Error("Echec de creation du renderer");
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Création de la cible où la scène sera dessinée.
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
        logger.Error("Echec de creation de la cible");
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Création du renderer 3D.
    NkRendererConfig cfgScene =
        NkRendererConfig::ForGame(devInit.api, devInit.width, devInit.height);

    cfgScene.Disable(NK_SS_RENDER2D);
    cfgScene.Disable(NK_SS_TEXT);
    cfgScene.Disable(NK_SS_OVERLAY);
    cfgScene.Enable(NK_SS_OFFSCREEN);

    NkRenderer *renderer = NkRenderer::Create(device, cfgScene);

    if (renderer == nullptr || !renderer->Initialize()) {
        logger.Error("Echec de creation du renderer 3D");
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    const NkTexHandle murTexture =
        renderer->GetTextures()->Load("murs/photo.png");
    NkMaterial *murMaterial = NkMaterial::Create(
        renderer->GetMaterials(), NkMaterialType::NK_PBR_METALLIC);
    if (murMaterial == nullptr) {
        logger.Error("Echec de creation du materiau du mur");
        NkRenderer::Destroy(renderer);
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }
    murMaterial->SetAlbedoMap(murTexture);

    renderer->SetFinalColorTarget(
        rEcran->GetTextures()->GetRHIHandle(cible->GetColorResult()));

    auto &eventSystem = NkEvents();
    bool running = true;

    NkClock horloge;

    while (running) {

        // Vérification des événements de la fenêtre.
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

        if (rEcran->BeginFrame()) {
            ++imageNumber;

            NkICommandBuffer *cmd = rEcran->GetCmd();
            NkRender3D *r3d = renderer->GetRender3D();

            renderer->FlushGraphRebuilds();
            r3d->ResetFrame();

            if (auto *mc = renderer->GetMaterialCollection()) {
                mc->Upload();
            }

            // Position de la caméra.
            NkCamera3DData cd;
            cd.position = {0.30f, 1.70f, 1.20f};
            cd.target = {-0.70f, 1.10f, -kDemi};
            cd.up = {0.0f, 1.0f, 0.0f};
            cd.fovY = 70.0f;
            cd.aspect = (float32)devInit.width / (float32)devInit.height;
            cd.nearPlane = 0.05f;
            cd.farPlane = 100.0f;

            // Lumière de la salle.
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

            // Dessin de la salle.
            SoumettreLaSalle(r3d, renderer->GetMeshSystem(), murMaterial);

            // Affichage de la scène.
            if (auto *graph = renderer->GetRenderGraph()) {
                graph->Execute(cmd);
            }

            if (auto *r2d = rEcran->GetRender2D()) {
                r2d->Begin(cmd, devInit.width, devInit.height);

                r2d->DrawImage(
                    cible->GetColorResult(),
                    {0.0f, 0.0f, (float32)devInit.width,
                     (float32)devInit.height});

                r2d->End();
            }

            rEcran->Present();
            rEcran->EndFrame();

            if (!shotCaptured && shotFrame > 0 && imageNumber == shotFrame) {
                if (shotPath == nullptr || shotPath[0] == '\0') {
                    logger.Error("MASALLE_SHOT_PATH n'est pas defini");
                } else if (!cible->Capture(shotPath)) {
                    logger.Error("Echec de capture de la salle");
                }
                shotCaptured = true;
            }

        } else {
            logger.Error("BeginFrame a refuse l'image");
        }
    }

    // Libération des ressources.
    NkMaterial::Destroy(murMaterial);
    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);

    return 0;
}
