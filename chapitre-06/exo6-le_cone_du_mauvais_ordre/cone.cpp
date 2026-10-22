#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKEvent/NkMouseEvent.h"
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
#include <cstring>

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

    // Le tangage est imposé par l'exercice.
    const float32 kTangageDeg = 30.0f;

    // Sensibilité de la souris, en radians par compte de capteur.
    const float32 kSensibilite = 0.002f;

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

    NkQuatf OrientationTete(float32 lacetRad, bool ordreTete) {

        const NkQuatf qLacet =
            NkQuatf::RotateY(NkAngle::FromRad(lacetRad));
        const NkQuatf qTangage =
            NkQuatf::RotateX(NkAngle(kTangageDeg));

        const NkQuatf q = ordreTete
                              ? qLacet * qTangage
                              : qTangage * qLacet;

        // Tout quaternion issu d'un calcul se normalise.
        return q.Normalized();
    }

    float32 HauteurDeg(const NkQuatf &orientation) {

        const NkVec3f regard = orientation * NkVec3f{0.0f, 0.0f, -1.0f};

        return NkAngle::FromRad(
                   std::asin(NkClamp(regard.y, -1.0f, 1.0f)))
            .Deg();
    }

    void AfficherHauteurs() {

        const float32 lacets[] = {0.0f, 45.0f, 90.0f, 135.0f, 180.0f};

        for (int ordre = 0; ordre < 2; ++ordre) {

            const bool ordreTete = (ordre == 0);
            const char *nom = ordreTete ? "lacet*tangage" : "tangage*lacet";

            for (float32 lacet : lacets) {

                const NkQuatf q =
                    OrientationTete(NkAngle(lacet).Rad(), ordreTete);

                std::printf("%s, lacet %.0f : hauteur %.4f deg\n",
                            nom, lacet, HauteurDeg(q));
            }
        }
    }

} // namespace

int nkmain(const NkEntryState &state) {

    (void)state;

    // Les dix hauteurs sont calculées avant d'ouvrir la fenêtre.
    AfficherHauteurs();

    // Crochet de test : le calcul seul, sans fenêtre ni carte graphique.
    if (std::getenv("MASALLE_MESURES") != nullptr) {
        return 0;
    }

    // Pose figée : MASALLE_LACET donne le lacet en degrés et coupe la souris.
    const char *lacetEnv = std::getenv("MASALLE_LACET");
    const bool lacetFige = (lacetEnv != nullptr);

    float32 lacetRad = lacetFige
                           ? NkAngle(std::strtof(lacetEnv, nullptr)).Rad()
                           : 0.0f;

    // Pour les captures : l'ordre peut être choisi avant le lancement.
    const char *ordreEnv = std::getenv("MASALLE_ORDRE");
    bool ordreTete = !(ordreEnv != nullptr &&
                       std::strcmp(ordreEnv, "mauvais") == 0);

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

    // Cacher le pointeur et recevoir son déplacement brut.
    window.ShowMouse(false);
    window.CaptureMouse(true);
    window.ClipMouseToClient(true);

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

    // Déplacements bruts accumulés entre deux images.
    int accumRawX = 0;
    int accumRawY = 0;

    NkClock horloge;

    while (running) {

        // Les événements arrivent quand ils veulent : on accumule.
        NkEvent *event;

        while (eventSystem.PollEvent(event)) {

            if (auto *souris = event->As<NkMouseRawEvent>()) {
                accumRawX += souris->GetDeltaX();
                accumRawY += souris->GetDeltaY();
            }

            if (auto *touche = event->As<NkKeyPressEvent>()) {
                if (touche->GetKey() == NkKey::NK_SPACE) {
                    ordreTete = !ordreTete;
                    logger.Info("Ordre : {0}",
                                ordreTete ? "lacet*tangage"
                                          : "tangage*lacet");
                }
            }

            if (event->Is<NkWindowCloseEvent>()) {
                running = false;
            }
        }

        if (!running) {
            break;
        }

        // Une fois par image : on lit ET on vide.
        if (!lacetFige) {
            lacetRad -= accumRawX * kSensibilite;
        }
        accumRawX = 0;
        accumRawY = 0;

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

            const NkQuatf orientation = OrientationTete(lacetRad, ordreTete);

            // La caméra suit le quaternion, regard et haut compris.
            // Point de vue reculé vers le fond : la porte est à 45 degrés
            // du regard aux lacets 0 et 90, et le plafond reste dans le haut
            // du cadre.
            NkCamera3DData cd;
            cd.position = {0.20f, 1.70f, -1.10f};
            cd.target = cd.position +
                        orientation * NkVec3f{0.0f, 0.0f, -1.0f};
            cd.up = orientation * NkVec3f{0.0f, 1.0f, 0.0f};
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

    // Rendre le pointeur à la fin.
    window.ShowMouse(true);
    window.CaptureMouse(false);
    window.ClipMouseToClient(false);

    // Libération des ressources.
    NkMaterial::Destroy(murMaterial);
    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);

    return 0;
}
