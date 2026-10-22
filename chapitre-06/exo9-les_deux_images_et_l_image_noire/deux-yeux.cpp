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

#include <algorithm>
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

    // La tête et l'écart entre les pupilles du simulateur.
    const NkVec3f kTete = {0.30f, 1.70f, 1.20f};
    const float32 kIpd = 0.063f;

    // Chaque œil a la moitié de la fenêtre.
    const uint32 kLargeurOeil = 640;
    const uint32 kHauteurOeil = 720;

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

    NkCamera3DData CameraOeil(const NkVec3f &position,
                              const NkQuatf &orientation) {

        NkCamera3DData cd;

        cd.position = position;
        cd.target = position + orientation * NkVec3f{0.0f, 0.0f, -1.0f};
        cd.up = orientation * NkVec3f{0.0f, 1.0f, 0.0f};
        cd.fovY = 70.0f;
        cd.aspect = (float32)kLargeurOeil / (float32)kHauteurOeil;
        cd.nearPlane = 0.05f;
        cd.farPlane = 100.0f;

        return cd;
    }

    void SoumettreOeil(NkRenderer *renderer, NkMaterial *materiau,
                       const NkVec3f &position, const NkQuatf &orientation,
                       NkICommandBuffer *cmd, float64 temps) {

        renderer->FlushGraphRebuilds();

        NkRender3D *r3d = renderer->GetRender3D();
        r3d->ResetFrame();

        if (auto *mc = renderer->GetMaterialCollection()) {
            mc->Upload();
        }

        NkSceneContext sctx;
        sctx.camera = NkCamera3D(
            CameraOeil(position, orientation));
        sctx.time = temps;

        NkLightDesc soleil;
        soleil.type = NkLightType::NK_DIRECTIONAL;
        soleil.direction = {-0.5f, -1.0f, -0.35f};
        soleil.color = {1.0f, 0.96f, 0.88f};
        soleil.intensity = 3.0f;
        soleil.castShadow = false;

        sctx.lights.PushBack(soleil);

        r3d->SetIBLStrength(0.35f);
        r3d->BeginScene(sctx);

        SoumettreLaSalle(r3d, renderer->GetMeshSystem(), materiau);

        if (auto *graph = renderer->GetRenderGraph()) {
            graph->Execute(cmd);
        }
    }

    struct Ecart {
        int pixels;
        int maxEcart;
    };

    // Compare deux images, écrit leur différence et rend les deux mesures.
    Ecart Comparer(const NkImage &a, const NkImage &b, const char *chemin) {

        Ecart resultat = {0, 0};

        const int32 largeur = a.Width();
        const int32 hauteur = a.Height();

        NkImage difference = NkImage::Create(
            (uint32)largeur, (uint32)hauteur, 4);

        for (int32 y = 0; y < hauteur; ++y) {
            for (int32 x = 0; x < largeur; ++x) {

                const NkColor ca = a.GetPixel(x, y);
                const NkColor cb = b.GetPixel(x, y);

                const int dr = std::abs((int)ca.r - (int)cb.r);
                const int dg = std::abs((int)ca.g - (int)cb.g);
                const int db = std::abs((int)ca.b - (int)cb.b);

                const int ecart = std::max(dr, std::max(dg, db));

                if (ecart > 1) {
                    ++resultat.pixels;
                }
                if (ecart > resultat.maxEcart) {
                    resultat.maxEcart = ecart;
                }

                // Quatre fois l'écart, borné à 255. Le noir veut dire pareil.
                int valeur = ecart * 4;
                if (valeur > 255) {
                    valeur = 255;
                }

                difference.SetPixel(
                    x, y,
                    NkColor((uint8)valeur, (uint8)valeur, (uint8)valeur, 255));
            }
        }

        if (!difference.SavePNG(chemin)) {
            logger.Error("Echec d'ecriture de {0}", chemin);
        }

        return resultat;
    }

} // namespace

int nkmain(const NkEntryState &state) {

    (void)state;

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

    // Deux renderers de salle, un par œil.
    NkRendererConfig cfgScene =
        NkRendererConfig::ForGame(devInit.api, kLargeurOeil, kHauteurOeil);

    cfgScene.Disable(NK_SS_RENDER2D);
    cfgScene.Disable(NK_SS_TEXT);
    cfgScene.Disable(NK_SS_OVERLAY);
    cfgScene.Enable(NK_SS_OFFSCREEN);

    NkOffscreenDesc od;
    od.width = kLargeurOeil;
    od.height = kHauteurOeil;
    od.hasDepth = true;
    od.hdr = false;
    od.colorFmt = NkGPUFormat::NK_RGBA8_UNORM;
    od.readable = true;
    od.readback = true;
    od.name = "Render_Oeil";

    NkRenderer *rendererGauche = NkRenderer::Create(device, cfgScene);
    NkRenderer *rendererDroit = NkRenderer::Create(device, cfgScene);

    if (rendererGauche == nullptr || !rendererGauche->Initialize() ||
        rendererDroit == nullptr || !rendererDroit->Initialize()) {
        logger.Error("Echec de creation des renderers de salle");
        NkRenderer::Destroy(rendererGauche);
        NkRenderer::Destroy(rendererDroit);
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // Une cible par œil, chacune de la moitié de la fenêtre.
    NkOffscreenTarget *cibleGauche = rEcran->CreateOffscreen(od);
    NkOffscreenTarget *cibleDroite = rEcran->CreateOffscreen(od);

    if (cibleGauche == nullptr || cibleDroite == nullptr) {
        logger.Error("Echec de creation des cibles");
        NkRenderer::Destroy(rendererGauche);
        NkRenderer::Destroy(rendererDroit);
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    // La sortie de chaque renderer va vers sa cible, à sa taille.
    rendererGauche->SetFinalColorTarget(
        rEcran->GetTextures()->GetRHIHandle(cibleGauche->GetColorResult()));
    rendererGauche->SetRenderSizeOverride(kLargeurOeil, kHauteurOeil);

    rendererDroit->SetFinalColorTarget(
        rEcran->GetTextures()->GetRHIHandle(cibleDroite->GetColorResult()));
    rendererDroit->SetRenderSizeOverride(kLargeurOeil, kHauteurOeil);

    // Un matériau par renderer, chacun avec sa bibliothèque de textures.
    NkMaterial *materiauGauche = NkMaterial::Create(
        rendererGauche->GetMaterials(), NkMaterialType::NK_PBR_METALLIC);
    NkMaterial *materiauDroit = NkMaterial::Create(
        rendererDroit->GetMaterials(), NkMaterialType::NK_PBR_METALLIC);

    if (materiauGauche == nullptr || materiauDroit == nullptr) {
        logger.Error("Echec de creation des materiaux");
        NkMaterial::Destroy(materiauGauche);
        NkMaterial::Destroy(materiauDroit);
        NkRenderer::Destroy(rendererGauche);
        NkRenderer::Destroy(rendererDroit);
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    materiauGauche->SetAlbedoMap(
        rendererGauche->GetTextures()->Load("murs/photo.png"));
    materiauDroit->SetAlbedoMap(
        rendererDroit->GetTextures()->Load("murs/photo.png"));

    auto &eventSystem = NkEvents();
    bool running = true;

    NkClock horloge;

    uint32 imageNumber = 0;
    bool yeuxCaptures = false;
    bool oublieeCapturee = false;

    // La tête reste droite, le regard vers -Z.
    const NkQuatf orientation = NkQuatf::Identity();
    const NkVec3f axeDroit = orientation * NkVec3f{1.0f, 0.0f, 0.0f};
    const float32 demiEcart = kIpd * 0.5f;

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

        const NkClock::NkTime &temps = horloge.Tick();

        if (!rEcran->BeginFrame()) {
            logger.Error("BeginFrame a refuse l'image");
            continue;
        }

        ++imageNumber;

        NkICommandBuffer *cmd = rEcran->GetCmd();

        // À l'image 31, on recommence en oubliant de décaler les yeux.
        const float32 decalage = (imageNumber == 31) ? 0.0f : demiEcart;

        const NkVec3f positionGauche = kTete - axeDroit * decalage;
        const NkVec3f positionDroite = kTete + axeDroit * decalage;

        // La même salle est soumise deux fois ; seuls la caméra et le haut
        // de chaque œil changent.
        SoumettreOeil(rendererGauche, materiauGauche,
                      positionGauche, orientation, cmd, temps.total);

        SoumettreOeil(rendererDroit, materiauDroit,
                      positionDroite, orientation, cmd, temps.total);

        // Les deux cibles côte à côte dans la fenêtre.
        if (auto *r2d = rEcran->GetRender2D()) {

            r2d->Begin(cmd, devInit.width, devInit.height);

            r2d->DrawImage(
                cibleGauche->GetColorResult(),
                {0.0f, 0.0f, (float32)kLargeurOeil,
                 (float32)kHauteurOeil});

            r2d->DrawImage(
                cibleDroite->GetColorResult(),
                {(float32)kLargeurOeil, 0.0f, (float32)kLargeurOeil,
                 (float32)kHauteurOeil});

            r2d->End();
        }

        rEcran->Present();
        rEcran->EndFrame();

        // Les deux yeux à la même image.
        if (!yeuxCaptures && imageNumber == 30) {

            if (!cibleGauche->Capture("oeil-gauche.png")) {
                logger.Error("Echec de capture de l'oeil gauche");
            }
            if (!cibleDroite->Capture("oeil-droit.png")) {
                logger.Error("Echec de capture de l'oeil droit");
            }

            yeuxCaptures = true;
        }

        // La vue oubliée, pour la comparaison négative.
        if (!oublieeCapturee && imageNumber == 31) {

            if (!cibleGauche->Capture("vue-oubliee-gauche.png")) {
                logger.Error("Echec de capture de la vue oubliee gauche");
            }
            if (!cibleDroite->Capture("vue-oubliee-droit.png")) {
                logger.Error("Echec de capture de la vue oubliee droite");
            }

            oublieeCapturee = true;
            running = false;
        }
    }

    // Comparaison des deux yeux, puis de la vue où le décalage est oublié.
    NkImage imageGauche;
    NkImage imageDroite;
    NkImage oublieeGauche;
    NkImage oublieeDroite;

    Ecart ecartYeux = {0, 0};
    Ecart ecartOubliee = {0, 0};

    if (!imageGauche.Load("oeil-gauche.png") ||
        !imageDroite.Load("oeil-droit.png") ||
        !oublieeGauche.Load("vue-oubliee-gauche.png") ||
        !oublieeDroite.Load("vue-oubliee-droit.png")) {

        logger.Error("Echec de rechargement des captures");
    } else {

        ecartYeux = Comparer(imageGauche, imageDroite, "difference.png");
        ecartOubliee = Comparer(oublieeGauche, oublieeDroite,
                                "difference-vue-oubliee.png");

        const int total = imageGauche.Width() * imageGauche.Height();

        std::printf("deux yeux : %d pixels sur %d, ecart max %d\n",
                    ecartYeux.pixels, total, ecartYeux.maxEcart);
        std::printf("vue oubliee : %d pixels sur %d, ecart max %d\n",
                    ecartOubliee.pixels, total, ecartOubliee.maxEcart);

        if (std::FILE *f = std::fopen("difference.txt", "w")) {
            std::fprintf(f,
                         "deux yeux : %d pixels sur %d, ecart max %d\n",
                         ecartYeux.pixels, total, ecartYeux.maxEcart);
            std::fprintf(f,
                         "vue oubliee : %d pixels sur %d, ecart max %d\n",
                         ecartOubliee.pixels, total, ecartOubliee.maxEcart);
            std::fclose(f);
        }
    }

    // Les deux captures de la vue oubliée ne font pas partie du rendu.
    std::remove("vue-oubliee-gauche.png");
    std::remove("vue-oubliee-droit.png");

    // Libération des ressources.
    NkMaterial::Destroy(materiauGauche);
    NkMaterial::Destroy(materiauDroit);
    NkRenderer::Destroy(rendererGauche);
    NkRenderer::Destroy(rendererDroit);
    rEcran->DestroyOffscreen(cibleGauche);
    rEcran->DestroyOffscreen(cibleDroite);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);

    return 0;
}
