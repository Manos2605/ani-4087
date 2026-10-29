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

    // Un quart de tour en 400 ms, depart et arrivee doux.
    double Lacet(double t) {   // t en secondes depuis le debut du geste
        const double amplitude = 90.0, duree = 0.400;
        if (t <= 0.0) return 0.0;
        if (t >= duree) return amplitude;
        return amplitude * 0.5 * (1.0 - cos(kPi * t / duree));
    }

    // Lacet lu sur un quaternion : 2 * atan2(q.y, q.w), en degres.
    double LacetDuQuaternion(const NkQuatf &q) {
        return 2.0 * atan2(static_cast<double>(q.y),
                           static_cast<double>(q.w)) *
               180.0 / kPi;
    }

    NkVec3f VitesseAngulaire(const NkQuatf &from, const NkQuatf &to,
                             float32 dtSeconds) {
        if (dtSeconds <= 0.f) {
            return NkVec3f(0.f, 0.f, 0.f);
        }

        NkQuatf delta = (to * from.Conjugate()).Normalized();

        if (delta.w < 0.f) {
            delta = NkQuatf(-delta.x, -delta.y, -delta.z, -delta.w);
        }

        const float32 angleRad = delta.Angle().Rad();

        if (angleRad <= 1e-6f) {
            return NkVec3f(0.f, 0.f, 0.f);
        }

        return delta.Axis() * (angleRad / dtSeconds);
    }

    NkQuatf Extrapoler(const NkQuatf &orientation, const NkVec3f &vitesse,
                       float32 dtSeconds) {
        const float32 speedRad = vitesse.Len();

        if (speedRad > 1e-6f) {
            const NkVec3f axis = vitesse * (1.f / speedRad);

            // Delta en espace de BASE : il se compose a GAUCHE de
            // l'orientation courante (meme convention que la mesure).
            const NkQuatf delta(NkAngle::FromRad(speedRad * dtSeconds), axis);

            return (delta * orientation).Normalized();
        }

        return orientation;
    }

    double EcartDeg(const NkQuatf &a, const NkQuatf &b) {
        return static_cast<double>(
            (b * a.Conjugate()).Normalized().Angle().Deg());
    }

    // ---------------------------------------------------------------------
    // Le simulateur : 64 poses horodatees, echantillon rendu en retard.
    // ---------------------------------------------------------------------
    const uint32 kMaxSamples = 64u;

    struct NkXrTimedPose {
        double time;
        NkQuatf pose;
        NkVec3f velocity;
    };

    struct Simulateur {
        NkXrTimedPose mSamples[kMaxSamples];
        uint32 mSampleHead = 0u;
        uint32 mSampleCount = 0u;

        void Push(double time, const NkQuatf &pose,
                  const NkVec3f &velocity) {
            if (mSampleCount < kMaxSamples) {
                mSamples[mSampleCount].time = time;
                mSamples[mSampleCount].pose = pose;
                mSamples[mSampleCount].velocity = velocity;
                mSampleHead = mSampleCount;
                ++mSampleCount;
                return;
            }

            mSampleHead = (mSampleHead + 1u) % kMaxSamples;
            mSamples[mSampleHead].time = time;
            mSamples[mSampleHead].pose = pose;
            mSamples[mSampleHead].velocity = velocity;
        }

        const NkXrTimedPose &SampleAt(double now, double latency) const {
            // Latence nulle : l'echantillon le plus recent, point.
            if (latency <= 0.0 || mSampleCount <= 1u) {
                return mSamples[mSampleHead];
            }

            // Latence simulee : on rend l'echantillon d'il y a « latence »
            // secondes — la prediction doit alors combler l'ecart.
            const double target = now - latency;

            uint32 index = mSampleHead;
            for (uint32 i = 0u; i < mSampleCount; ++i) {
                if (mSamples[index].time <= target) {
                    return mSamples[index];
                }
                index = (index + kMaxSamples - 1u) % kMaxSamples;
            }

            // Plus vieux disponible : l'historique ne remonte pas si loin.
            return mSamples[(mSampleHead + kMaxSamples -
                             (mSampleCount - 1u)) % kMaxSamples];
        }

        // Pose predite a l'instant demande. La fenetre d'extrapolation est
        // bornee : au-dela de 100 ms le modele « vitesses constantes » ment
        // plus qu'il n'aide (et OpenXR borne pareil cote runtimes).
        NkQuatf PredictAt(const NkXrTimedPose &sample, double when,
                          bool bornee) const {
            double dt = when - sample.time;

            if (bornee) {
                if (dt < -0.1) dt = -0.1;
                if (dt > 0.1) dt = 0.1;
            }

            return Extrapoler(sample.pose, sample.velocity,
                              static_cast<float32>(dt));
        }
    };

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
    // Mesure : moyenne et maximum de l'erreur, pour chaque latence.
    // ---------------------------------------------------------------------
    struct Resultat {
        double moyenneAvec;
        double maxAvec;
        double moyenneSans;
        double maxSans;
        int images;
    };

    const int kNombreLatences = 5;

    // Ligne de la borne : l'image dont l'echantillon est le plus proche de
    // t = 1,2 s, le milieu du geste.
    struct LigneBorne {
        double tempsEchantillon;
        double horizonMs;
        double preditDeg;
        double vraiDeg;
        double instantAffichage;
        bool trouvee;
    };

} // namespace

int nkmain(const NkEntryState &state) {
    (void)state;

    const double periode = 1.0 / 72.0;

    // Trois secondes de poses, une par periode de 72 hertz.
    const int nombrePoses = static_cast<int>(3.0 / periode) + 1;

    NkQuatf poses[512];
    double temps[512];

    for (int k = 0; k < nombrePoses; ++k) {
        temps[k] = static_cast<double>(k) * periode;
        poses[k] = NkQuatf::RotateY(NkAngle(Lacet(temps[k] - 1.0)));
    }

    const double latences[kNombreLatences] = {
        0.0, 0.010, 0.020, 0.040, 0.080
    };

    Resultat resultats[kNombreLatences];
    for (int i = 0; i < kNombreLatences; ++i) {
        resultats[i] = Resultat{0.0, 0.0, 0.0, 0.0, 0};
    }

    LigneBorne sansBorne;
    sansBorne.tempsEchantillon = 0.0;
    sansBorne.horizonMs = 0.0;
    sansBorne.preditDeg = 0.0;
    sansBorne.vraiDeg = 0.0;
    sansBorne.instantAffichage = 0.0;
    sansBorne.trouvee = false;

    LigneBorne avecBorne = sansBorne;

    double meilleurEcart = 1e9;

    // Le simulateur, alimente dans l'ordre, et mesure a chaque image :
    // a l'instant t, le tampon ne contient que les poses deja passees.
    Simulateur sim;

    for (int k = 0; k < nombrePoses; ++k) {
        NkVec3f vitesse(0.f, 0.f, 0.f);

        if (k > 0) {
            vitesse = VitesseAngulaire(poses[k - 1], poses[k],
                                       static_cast<float32>(periode));
        }

        sim.Push(temps[k], poses[k], vitesse);

        const double t = temps[k];

        if (k + 1 >= nombrePoses) {
            continue;
        }

        // Cinq latences, de t = 1,0 s a t = 1,9 s.
        if (t >= 1.0 && t <= 1.9) {
            for (int i = 0; i < kNombreLatences; ++i) {
                const NkXrTimedPose &sample =
                    sim.SampleAt(t, latences[i]);
                const NkQuatf &vraie = poses[k + 1];

                const NkQuatf predite =
                    sim.PredictAt(sample, t + periode, true);

                const double erreurAvec = EcartDeg(predite, vraie);
                const double erreurSans = EcartDeg(sample.pose, vraie);

                Resultat &r = resultats[i];

                r.moyenneAvec += erreurAvec;
                r.moyenneSans += erreurSans;
                r.maxAvec = erreurAvec > r.maxAvec ? erreurAvec : r.maxAvec;
                r.maxSans = erreurSans > r.maxSans ? erreurSans : r.maxSans;
                ++r.images;
            }
        }

        // L'image dont l'echantillon est le plus proche de t = 1,2 s,
        // avec une latence d'une seconde.
        if (t >= 1.0) {
            const NkXrTimedPose &sample = sim.SampleAt(t, 1.0);
            const double ecart = std::fabs(sample.time - 1.2);

            if (ecart < meilleurEcart) {
                meilleurEcart = ecart;

                const double quand = t + periode;

                sansBorne.tempsEchantillon = sample.time;
                sansBorne.instantAffichage = t;
                sansBorne.horizonMs = (quand - sample.time) * 1000.0;
                sansBorne.preditDeg = LacetDuQuaternion(
                    sim.PredictAt(sample, quand, false));
                sansBorne.vraiDeg = LacetDuQuaternion(poses[k + 1]);
                sansBorne.trouvee = true;

                avecBorne = sansBorne;
                avecBorne.horizonMs =
                    (quand - sample.time > 0.1 ? 0.1 : quand - sample.time)
                    * 1000.0;
                avecBorne.preditDeg = LacetDuQuaternion(
                    sim.PredictAt(sample, quand, true));
            }
        }
    }

    std::printf("latences mesurees de t = 1,0 s a t = 1,9 s, "
                "periode = %.3f ms\n", periode * 1000.0);

    for (int i = 0; i < kNombreLatences; ++i) {
        Resultat &r = resultats[i];

        if (r.images > 0) {
            r.moyenneAvec /= r.images;
            r.moyenneSans /= r.images;
        }

        std::printf(
            "latence %.0f ms : avec %.3f deg (max %.3f), "
            "sans %.3f deg (max %.3f), sur %d images\n",
            latences[i] * 1000.0,
            r.moyenneAvec, r.maxAvec,
            r.moyenneSans, r.maxSans,
            r.images);
    }

    std::printf(
        "sans borne : echantillon a %.3f s (affiche a %.3f s), "
        "horizon %.1f ms, predit %.1f deg, vrai %.1f deg\n",
        sansBorne.tempsEchantillon,
        sansBorne.instantAffichage,
        sansBorne.horizonMs,
        sansBorne.preditDeg,
        sansBorne.vraiDeg);

    // La borne remise, meme latence, meme image.
    std::printf(
        "avec borne : echantillon a %.3f s (affiche a %.3f s), "
        "horizon %.1f ms, predit %.1f deg, vrai %.1f deg\n",
        avecBorne.tempsEchantillon,
        avecBorne.instantAffichage,
        avecBorne.horizonMs,
        avecBorne.preditDeg,
        avecBorne.vraiDeg);

    std::fflush(stdout);

    if (std::getenv("MASALLE_MESURES") != nullptr) {
        return 0;
    }

    NkWindowConfig cfg;
    cfg.title = "Chapitre 7 - Exo 7 : la latence et la borne";
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
            NkQuatf::RotateY(NkAngle(Lacet(ecoule - 1.0)));

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

        if (ecoule > 2.5) {
            running = false;
        }
    }

    NkMaterial::Destroy(murMaterial);
    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);

    return 0;
}
