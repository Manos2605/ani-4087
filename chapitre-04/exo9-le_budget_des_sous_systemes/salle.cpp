#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"

#include "NKRHI/Core/NkDeviceFactory.h"
#include "NKRenderer/NkRenderer.h"
#include "NKRenderer/Core/NkCamera.h"
#include "NKRenderer/Mesh/NkMeshSystem.h"
#include "NKRenderer/Tools/Render3D/NkRender3D.h"

#include <chrono>
#include <cstdio>
#include <cstring>

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData& data) {
    data.appName = "MaSalleBudget";
}

NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

struct Piece {
    NkVec3f centre;
    NkVec3f taille;
};

static const Piece pieces[] = {
    // Salle
    {{0.f, -0.05f, 0.f}, {6.f, 0.1f, 4.f}},
    {{0.f, 1.4f, -2.05f}, {6.2f, 2.8f, 0.1f}},
    {{0.f, 1.4f, 2.05f}, {6.2f, 2.8f, 0.1f}},
    {{-3.05f, 1.4f, 0.f}, {0.1f, 2.8f, 4.f}},
    {{3.05f, 1.4f, 0.f}, {0.1f, 2.8f, 4.f}},

    // Porte
    {{-1.5f, 1.f, 1.98f}, {0.9f, 2.f, 0.04f}},

    // Fenetres
    {{-1.5f, 1.5f, -1.98f}, {1.2f, 1.f, 0.04f}},

    // Bureau
    {{0.5f, 0.78f, 0.f}, {1.6f, 0.04f, 0.8f}},
    {{-0.04f, 0.38f, -0.34f}, {0.06f, 0.76f, 0.06f}},
    {{1.04f, 0.38f, -0.34f}, {0.06f, 0.76f, 0.06f}},
    {{-0.04f, 0.38f, 0.34f}, {0.06f, 0.76f, 0.06f}},
    {{1.04f, 0.38f, 0.34f}, {0.06f, 0.76f, 0.06f}},

    // Ecran
    {{0.4f, 0.81f, 0.f}, {0.34f, 0.02f, 0.24f}},

    // Unite centrale
    {{0.9f, 1.025f, -0.2f}, {0.15f, 0.45f, 0.15f}}
};

static bool EstSalle(const NkEntryState& state) {
    for (const NkString& argument : state.args) {
        if (std::strcmp(argument.CStr(), "SALLE") == 0) {
            return true;
        }
    }

    return false;
}

static void EnregistrerMesure(const char* nom, long long ms) {
    std::FILE* fichier = std::fopen("mesures_brutes.txt", "a");

    if (fichier == nullptr) {
        return;
    }

    std::fprintf(fichier, "%s %lld\n", nom, ms);
    std::fclose(fichier);
}

int nkmain(const NkEntryState& state) {
    const auto debut = std::chrono::steady_clock::now();

    bool salle = EstSalle(state);

    const char* nomConfiguration = "ALL";

    if (salle) {
        nomConfiguration = "SALLE";
    }

    NkWindowConfig configFenetre;
    configFenetre.title = "Ma Salle";
    configFenetre.width = 1280;
    configFenetre.height = 720;
    configFenetre.centered = true;

    NkWindow window(configFenetre);

    if (!window.IsValid()) {
        return 1;
    }

    uint32 largeur = (uint32)window.GetSize().width;
    uint32 hauteur = (uint32)window.GetSize().height;

    NkDeviceInitInfo deviceInfo{};
    deviceInfo.surface = window.GetSurfaceDesc();
    deviceInfo.width = largeur;
    deviceInfo.height = hauteur;
    deviceInfo.api = NkGraphicsApi::NK_GFX_API_OPENGL;

    NkIDevice* device = NkDeviceFactory::CreateWithFallback(
        deviceInfo,
        {NkGraphicsApi::NK_GFX_API_OPENGL}
    );

    if (device == nullptr || !device->IsValid()) {
        return 2;
    }

    NkRendererConfig configRenderer;
    configRenderer.api = device->GetApi();
    configRenderer.width = largeur;
    configRenderer.height = hauteur;

    if (salle) {
        configRenderer.subsystems = NK_SS_RENDER3D | NK_SS_SHADOW;
    }
    else {
        configRenderer.subsystems = NK_SS_ALL;
    }

    NkRenderer* renderer = NkRenderer::Create(device, configRenderer);

    if (renderer == nullptr || !renderer->Initialize()) {
        return 3;
    }

    NkRender3D* render3D = renderer->GetRender3D();
    NkMeshHandle cube = renderer->GetMeshSystem()->GetCube();

    bool imagePresentee = false;

    for (int i = 0; i < 120 && !imagePresentee; i++) {
        NkEvents().PollEvents();

        if (!renderer->BeginFrame()) {
            continue;
        }

        NkCamera3DData cameraData;
        cameraData.up = {0.f, 1.f, 0.f};
        cameraData.fovY = 70.f;
        cameraData.aspect = (float32)largeur / (float32)hauteur;
        cameraData.nearPlane = 0.05f;
        cameraData.farPlane = 50.f;

        NkCamera3D camera(cameraData);

        camera.SetPosition({-1.8f, 1.6f, 1.6f});
        camera.SetTarget({0.5f, 0.8f, 0.f});

        NkSceneContext scene;
        scene.camera = camera;

        NkLightDesc lumiere;
        lumiere.type = NkLightType::NK_DIRECTIONAL;
        lumiere.direction = {-0.3f, -1.f, -0.4f};
        lumiere.color = {1.f, 1.f, 1.f};
        lumiere.intensity = 3.f;
        lumiere.castShadow = true;

        scene.lights.PushBack(lumiere);

        render3D->BeginScene(scene);

        for (const Piece& piece : pieces) {
            NkDrawCall3D dessin;

            dessin.mesh = cube;
            dessin.transform =
                NkMat4f::Translate(piece.centre) *
                NkMat4f::Scale(piece.taille);

            dessin.aabb = {
                {
                    piece.centre.x - piece.taille.x * 0.5f,
                    piece.centre.y - piece.taille.y * 0.5f,
                    piece.centre.z - piece.taille.z * 0.5f
                },
                {
                    piece.centre.x + piece.taille.x * 0.5f,
                    piece.centre.y + piece.taille.y * 0.5f,
                    piece.centre.z + piece.taille.z * 0.5f
                }
            };

            render3D->Submit(dessin);
        }

        renderer->Present();
        renderer->EndFrame();

        imagePresentee = true;
    }

    device->WaitIdle();

    const auto fin = std::chrono::steady_clock::now();

    long long temps = (long long)
        std::chrono::duration_cast<std::chrono::milliseconds>(
            fin - debut
        ).count();

    if (imagePresentee) {
        EnregistrerMesure(nomConfiguration, temps);
    }

    NkRenderer::Destroy(renderer);
    NkDeviceFactory::Destroy(device);

    window.Close();

    return imagePresentee ? 0 : 4;
}