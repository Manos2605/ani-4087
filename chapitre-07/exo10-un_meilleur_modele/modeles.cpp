// =============================================================================
//  Étape 5 — la salle, en cubes, aux dimensions du chapitre 2.
//
//  Pas encore de casque, pas encore de tête : une caméra fixe, posée à hauteur
//  d'yeux, qui regarde la porte. C'est le décor sur lequel tout le reste du
//  livre se jugera, et c'est pour cela qu'il est bâti maintenant.
//
//  Toutes les dimensions sont en MÈTRES, et viennent du tableau du chapitre 7 :
//      sol 4 x 4, murs 2,5 de haut, porte 0,9 x 2,0,
//      deux fenêtres 1,2 x 1,0, table 1,2 x 0,8 avec le plateau à 0,75.
//
//  Le cube et la sphère de NkMeshSystem sont UNITAIRES, de demi-étendue 0,5 :
//  une échelle S donne donc une taille S, et poser un objet au sol demande de
//  le translater de S/2. Le plan de GetPlane() est unitaire LUI AUSSI, de
//  demi-étendue 0,5 : une échelle S y donne un côté S, et non un demi-côté.
//  Se tromper là-dessus donne un sol deux fois trop petit, qui flotte au
//  milieu de la pièce, et rien ne le signale.
// =============================================================================

#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"

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
#include "NKMath/NkMat.h"
#include "NKMath/NkQuat.h"
#include "NKMath/NkVec.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;
using namespace nkentseu::renderer;

namespace {

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

    const double kPi = 3.14159265358979323846;

    // ---------------------------------------------------------------------
    // Les quatre enregistrements.
    // ---------------------------------------------------------------------
    struct Enregistrement {
        const char *nom;
        const char *description;
        double amplitude;     // degres
        double duree;         // secondes
        double vitesseLigne;  // deg/s, arret brusque uniquement
        bool arretBrusque;
    };

    double Lacet(const Enregistrement &e, double t) {
        if (e.arretBrusque) {
            // Vitesse rigoureusement constante, puis arret net.
            if (t <= 0.0) return 0.0;
            if (t >= e.duree) return e.vitesseLigne * e.duree;
            return e.vitesseLigne * t;
        }

        // Depart et arrivee doux : cosinus sureleve.
        if (t <= 0.0) return 0.0;
        if (t >= e.duree) return e.amplitude;
        return e.amplitude * 0.5 *
               (1.0 - cos(kPi * t / e.duree));
    }

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

        NkDrawCall3D sol;
        sol.mesh = meshes->GetPlane();
        sol.transform = NkMat4f::Scale({kPiece, 1.0f, kPiece});
        sol.aabb = {{-kDemi, 0.0f, -kDemi}, {kDemi, 0.0f, kDemi}};
        sol.castShadow = false;
        sol.tint = grisSol;
        sol.metallic = 0.0f;
        sol.roughness = 1.0f;
        r3d->Submit(sol);

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
        Pave(r3d, meshes, {0.0f, kPlafond + kMur * 0.5f, 0.0f},
             {kPiece, kMur, kPiece}, grisMur);

        Pave(r3d, meshes, {-0.70f, kPorteH * 0.5f, -kDemi + 0.02f},
             {kPorteL, kPorteH, 0.04f}, bois, 0.6f);

        Pave(r3d, meshes, {-kDemi + 0.02f, kFenY, -0.90f},
             {0.04f, kFenH, kFenL}, vitre, 0.15f);
        Pave(r3d, meshes, {-kDemi + 0.02f, kFenY, 0.90f},
             {0.04f, kFenH, kFenL}, vitre, 0.15f);

        const float32 tx = 0.60f;
        const float32 tz = 0.70f;
        const float32 ep = 0.05f;

        Pave(r3d, meshes, {tx, kTableH - ep * 0.5f, tz},
             {kTableL, ep, kTableP}, bois, 0.5f);

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

        Pave(r3d, meshes, {-1.10f, 0.50f, 1.20f},
             {1.0f, 1.0f, 1.0f}, {0.85f, 0.80f, 0.30f}, 0.8f);
    }

    // ---------------------------------------------------------------------
    // Les deux modeles, sur le lacet seul, en degres.
    // ---------------------------------------------------------------------
    struct Resultat {
        double moyenneVitesse;
        double maxVitesse;
        double moyenneAcceleration;
        double maxAcceleration;
        int images;
    };

    Resultat Mesurer(const Enregistrement &e, double periode) {

        // Une pose par periode, de 0 jusqu'a la fin du geste + 200 ms.
        const double fin = e.duree + 0.200;
        const int images = static_cast<int>(fin / periode) + 2;

        double lacet[4096];
        for (int k = 0; k < images; ++k) {
            lacet[k] = Lacet(e, static_cast<double>(k) * periode);
        }

        double sommeVitesse = 0.0;
        double maxVitesse = 0.0;
        double sommeAcceleration = 0.0;
        double maxAcceleration = 0.0;
        int compte = 0;

        // Il faut trois poses pour l'acceleration, et une de plus pour la
        // verite : on demarre a k = 2.
        for (int k = 2; k < images - 1; ++k) {

            // Vitesse tiree des deux dernieres poses.
            const double vitesse =
                (lacet[k] - lacet[k - 1]) / periode;

            // Acceleration tiree des trois dernieres poses.
            const double acceleration =
                (lacet[k] - 2.0 * lacet[k - 1] + lacet[k - 2]) /
                (periode * periode);

            const double vrai = lacet[k + 1];

            const double preditVitesse = lacet[k] + vitesse * periode;

            const double preditAcceleration =
                lacet[k] + vitesse * periode +
                acceleration * periode * periode / 2.0;

            const double erreurVitesse =
                std::fabs(preditVitesse - vrai);
            const double erreurAcceleration =
                std::fabs(preditAcceleration - vrai);

            sommeVitesse += erreurVitesse;
            sommeAcceleration += erreurAcceleration;

            maxVitesse = erreurVitesse > maxVitesse
                             ? erreurVitesse : maxVitesse;
            maxAcceleration = erreurAcceleration > maxAcceleration
                                  ? erreurAcceleration : maxAcceleration;

            ++compte;
        }

        Resultat r;
        r.moyenneVitesse = compte > 0 ? sommeVitesse / compte : 0.0;
        r.maxVitesse = maxVitesse;
        r.moyenneAcceleration = compte > 0 ? sommeAcceleration / compte : 0.0;
        r.maxAcceleration = maxAcceleration;
        r.images = compte;
        return r;
    }

    void Afficher(const Enregistrement &e, const Resultat &r) {
        std::printf(
            "%s (%s) : vitesse %.3f deg (max %.3f), "
            "acceleration %.3f deg (max %.3f)\n",
            e.nom, e.description,
            r.moyenneVitesse, r.maxVitesse,
            r.moyenneAcceleration, r.maxAcceleration);
        std::fflush(stdout);
    }

} // namespace

int nkmain(const NkEntryState &state) {
    (void)state;

    const double periode = 1.0 / 72.0;

    const Enregistrement enregistrements[] = {
        {"lent", "30 deg en 800 ms", 30.0, 0.800, 0.0, false},
        {"rapide", "90 deg en 400 ms", 90.0, 0.400, 0.0, false},
        {"demi-tour", "180 deg en 200 ms", 180.0, 0.200, 0.0, false},
        {"arret", "200 deg/s puis arret net", 0.0, 0.300, 200.0, true}
    };

    std::printf("modeles a 72 Hz, periode = %.3f ms\n", periode * 1000.0);

    for (const Enregistrement &e : enregistrements) {
        Afficher(e, Mesurer(e, periode));
    }

    if (std::getenv("MASALLE_MESURES") != nullptr) {
        return 0;
    }

    NkWindowConfig cfg;
    cfg.title = "Chapitre 7 - Exo 10 : un meilleur modele";
    cfg.width = 1280;
    cfg.height = 720;

    NkWindow window;
    if (!window.Create(cfg)) {
        logger.Error("La fenetre n'a pas ete creee");
        return -1;
    }

    NkDeviceInitInfo devInit{};
    devInit.surface = window.GetSurfaceDesc();
    devInit.width = window.GetSize().x;
    devInit.height = window.GetSize().y;

    NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);
    if (device == nullptr) {
        logger.Error("Echec de creation du device");
        return -2;
    }

    NkRendererConfig cfgEcran =
        NkRendererConfig::For2D(devInit.api, devInit.width, devInit.height);

    NkRenderer *rEcran = NkRenderer::Create(device, cfgEcran);
    if (rEcran == nullptr || !rEcran->Initialize()) {
        logger.Error("Echec de creation du renderer");
        NkDeviceFactory::Destroy(device);
        return 1;
    }

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

    const double debut =
        static_cast<double>(NkChrono::Now().nanoseconds) * 1e-9;

    while (running) {
        NkEvent *event;
        while (eventSystem.PollEvent(event)) {
            if (event->Is<NkWindowCloseEvent>()) {
                running = false;
            }
        }

        if (!running) {
            break;
        }

        if (!rEcran->BeginFrame()) {
            continue;
        }

        NkICommandBuffer *cmd = rEcran->GetCmd();
        NkRender3D *r3d = renderer->GetRender3D();

        renderer->FlushGraphRebuilds();
        r3d->ResetFrame();

        if (auto *mc = renderer->GetMaterialCollection()) {
            mc->Upload();
        }

        const double ecoule =
            static_cast<double>(NkChrono::Now().nanoseconds) * 1e-9 - debut;

        const NkQuatf orientation =
            NkQuatf::RotateY(NkAngle(Lacet(enregistrements[1], ecoule)));

        NkCamera3DData cd;
        cd.position = {0.30f, 1.70f, 1.20f};
        cd.target = cd.position + orientation * NkVec3f(0.0f, 0.0f, -1.0f);
        cd.up = {0.0f, 1.0f, 0.0f};
        cd.fovY = 70.0f;
        cd.aspect = (float32)devInit.width / (float32)devInit.height;
        cd.nearPlane = 0.05f;
        cd.farPlane = 100.0f;

        NkSceneContext sctx;
        sctx.camera = NkCamera3D(cd);
        sctx.time = static_cast<float32>(ecoule);

        NkLightDesc soleil;
        soleil.type = NkLightType::NK_DIRECTIONAL;
        soleil.direction = {-0.5f, -1.0f, -0.35f};
        soleil.color = {1.0f, 0.96f, 0.88f};
        soleil.intensity = 3.0f;
        soleil.castShadow = false;

        sctx.lights.PushBack(soleil);

        r3d->SetIBLStrength(0.35f);
        r3d->BeginScene(sctx);

        SoumettreLaSalle(r3d, renderer->GetMeshSystem(), murMaterial);

        if (auto *graph = renderer->GetRenderGraph()) {
            graph->Execute(cmd);
        }

        if (auto *r2d = rEcran->GetRender2D()) {
            r2d->Begin(cmd, devInit.width, devInit.height);
            r2d->DrawImage(
                cible->GetColorResult(),
                {0.0f, 0.0f, (float32)devInit.width, (float32)devInit.height});
            r2d->End();
        }

        rEcran->Present();
        rEcran->EndFrame();

        if (ecoule > 2.0) {
            running = false;
        }
    }

    NkMaterial::Destroy(murMaterial);
    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);

    return 0;
}
