#pragma once
#include "Common.hpp"
#include "NetworkClient.hpp"
#include "ProtocolHandler.hpp"
#include "GameState.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"

namespace Zappy {

    enum class AppState {
        MENU,
        GAME,
        HELP,
        SETTINGS,
        CREDITS,
        TRANSITION
    };

    // Pages suivantes (pour les transitions!!)
    enum class AppStateTarget {
        MENU,
        GAME,
        HELP,
        SETTINGS,
        CREDITS,
        NONE
    };


    enum class SettingsOption {
        MUSIC_VOLUME,
        FULLSCREEN,
        BACK,
        COUNT
    };

    class Application {
    private:
        std::unique_ptr<NetworkClient> networkClient;
        std::unique_ptr<ProtocolHandler> protocolHandler;
        std::unique_ptr<GameState> gameState;
        std::unique_ptr<Camera> camera;
        std::unique_ptr<Renderer> renderer;

        bool running{false};
        bool connected{false};
        bool showQuitConfirmation{false};

        AppState state{AppState::MENU};

        std::string serverHost{"localhost"};
        int serverPort{4242};

        float musicVolume = 0.5f;
        bool fullscreen = false;
        int selected = 0;

        // Transition blanche floutée
        AppStateTarget transitionTarget = AppStateTarget::NONE;
        bool transitioning = false;
        float transitionAlpha = 0.0f;

        Model torus;
        Camera3D simpleCamera{};

        Texture2D backImage;

        bool parseArguments(int argc, char* argv[]);
        void printUsage(const std::string& programName) const;
        void handleInput();
        void update();
        void renderGame();
        void connectToServer();
        void processNetwork();

        void renderMenu(); 
        void handleMenuInput();

        void renderHelp();
        void handleHelpInput();

        void renderCredits();
        void handleCreditsInput();

        void renderSettings();
        void handleSettingsInput();

        void quitGame();

        Music backgroundMusic{};
        bool musicLoaded{false};

        bool videoEnabled{true};
        float videoTime{0.0f};
        void updateVideo();
        void renderVideoBackground();
        
        bool showLegend{false};
        void renderLegend();
        Color getResourceColorForLegend(ResourceType type) const;

        // Méthodes transitions
        void startTransition(AppStateTarget target);
        void renderTransition();
        void updateTransition();

        bool showHUD{true};
        bool hudStyleMode{true};

        void renderHUDFrame(int x, int y, int width, int height, const std::string& title, Color bgColor);
        void renderGameInfoHUD();
        void renderTeamsHUD();
        void renderConnectionHUD();
        void renderSelectedPlayerHUD();

    public:
        Application() = default;
        ~Application() = default;

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void loadMusic();
        void updateMusic();

        bool initialize(int argc, char* argv[]);
        void run();
    };
}