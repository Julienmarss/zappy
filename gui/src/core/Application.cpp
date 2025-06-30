#include "../../include/Application.hpp"
#include <getopt.h>
#include <cmath>
#include <raymath.h>

namespace Zappy {

bool Application::parseArguments(int argc, char* argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "p:h:")) != -1) {
        switch (opt) {
            case 'p':
                serverPort = std::atoi(optarg);
                if (serverPort <= 0 || serverPort > 65535) {
                    std::cerr << "Error: Invalid port number" << std::endl;
                    return false;
                }
                break;
            case 'h':
                serverHost = optarg;
                break;
            default:
                printUsage(argv[0]);
                return false;
        }
    }

    return true;
}

void Application::printUsage(const std::string& programName) const
{
    std::cout << "USAGE: " << programName << " -p port -h machine" << std::endl;
    std::cout << "\t-p port\t\tport number" << std::endl;
    std::cout << "\t-h machine\thostname of the server" << std::endl;
}

bool Application::initialize(int argc, char* argv[])
{
    std::cout << "Initializing Zappy C++ GUI with Raylib..." << std::endl;

    if (!parseArguments(argc, argv)) {
        return false;
    }

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Zappy C++ GUI");
    SetTargetFPS(60);

    InitAudioDevice();
    loadMusic();

    try {
        gameState = std::make_unique<GameState>();
        networkClient = std::make_unique<NetworkClient>();
        protocolHandler = std::make_unique<ProtocolHandler>(gameState.get());
        camera = std::make_unique<Camera>();
        renderer = std::make_unique<Renderer>();

        simpleCamera.position = { 0.0f, 2.0f, 6.0f };
        simpleCamera.target = { 0.0f, 0.0f, 0.0f };
        simpleCamera.up = { 0.0f, 1.0f, 0.0f };
        simpleCamera.fovy = 45.0f;
        simpleCamera.projection = CAMERA_PERSPECTIVE;

        torus = LoadModel("gui/assets/model/torus.glb");
        if (torus.meshCount == 0)
            std::cerr << "Warning: Failed to load torus model" << std::endl;

        backImage = LoadTexture("gui/assets/background/sky.jpg");
        if (backImage.id == 0)
            std::cerr << "Warning: Failed to load background image" << std::endl;
        
        if (!renderer->loadResources())
            std::cerr << "Warning: Failed to load some renderer resources" << std::endl;
        
        connectToServer();
        
        running = true;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize application: " << e.what() << std::endl;
        return false;
    }
}

void Application::connectToServer()
{
    std::cout << "Connecting to server " << serverHost << ":" << serverPort << std::endl;
    
    if (networkClient->connect(serverHost, serverPort)) {
        std::cout << "Connected to server!" << std::endl;
        networkClient->send("GRAPHIC");
        connected = true;
    } else {
        std::cout << "Failed to connect to server. Running in offline mode." << std::endl;
        connected = false;
    }
}

void Application::processNetwork()
{
    if (!connected) return;

    networkClient->receive();

    while (networkClient->hasMessage()) {
        std::string message = networkClient->getMessage();

        #ifdef DEBUG
        std::cout << "Received: " << message << std::endl;
        #endif

        protocolHandler->processMessage(message);
    }
}

void Application::update()
{
    camera->update();
}

void Application::handleSettingsInput()
{
    if (IsKeyPressed(KEY_DOWN)) selected = (selected + 1) % (int)SettingsOption::COUNT;
    if (IsKeyPressed(KEY_UP)) selected = (selected - 1 + (int)SettingsOption::COUNT) % (int)SettingsOption::COUNT;

    if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
        running = false;
    }

    if (IsKeyPressed(KEY_B)) {
        state = AppState::MENU;
    }

    switch ((SettingsOption)selected) {
        case SettingsOption::MUSIC_VOLUME:
            if (IsKeyPressed(KEY_LEFT)) {
                musicVolume = std::max(0.0f, musicVolume - 0.1f);
            if (musicLoaded) SetMusicVolume(backgroundMusic, musicVolume);
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            musicVolume = std::min(1.0f, musicVolume + 0.1f);
            if (musicLoaded) SetMusicVolume(backgroundMusic, musicVolume);
        }
        break;

        case SettingsOption::FULLSCREEN:
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                fullscreen = !fullscreen;
                ToggleFullscreen();
            }
            break;
        default: break;
    }
}

void Application::handleMenuInput()
{
    if (IsKeyPressed(KEY_ENTER)) {
        startTransition(AppStateTarget::GAME);
        //state = AppState::GAME;
    } else if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
        running = false;
    }
    if (IsKeyPressed(KEY_H)) {
        state = AppState::HELP;
    }
    if (IsKeyPressed(KEY_C)) {
        state = AppState::CREDITS;
    }
    if (IsKeyPressed(KEY_S)) {
        state = AppState::SETTINGS;
    }
}

void Application::handleHelpInput()
{
    if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
        running = false;
    }
    if (IsKeyPressed(KEY_B)) {
        state = AppState::MENU;
    }
}

void Application::handleInput()
{
    if (showQuitConfirmation) {
        if (IsKeyPressed(KEY_Y))
            running = false;
        else if (IsKeyPressed(KEY_N))
            showQuitConfirmation = false;
        return;
    }

    Vector2 movement{0, 0};
    const float speed = 200.0f * GetFrameTime();
    
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) movement.y -= speed;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) movement.y += speed;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.x -= speed;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.x += speed;
    
    if (movement.x != 0 || movement.y != 0) {
        camera->move(movement);
    }
    
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        camera->zoom(wheel * 0.1f);
    }
    if (IsKeyPressed(KEY_L)) {
        showLegend = !showLegend;
    }
    if (IsKeyPressed(KEY_H)) {
        showHUD = !showHUD;
    }
    if (IsKeyPressed(KEY_U)) {
        hudStyleMode = !hudStyleMode;
    }
    if (IsKeyPressed(KEY_B)) {
        state = AppState::MENU;
    }
    if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
        showQuitConfirmation = true;
    }
}

void Application::handleCreditsInput()
{
    if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
        running = false;
    }
    if (IsKeyPressed(KEY_B)) {
        state = AppState::MENU;
    }
}

void Application::quitGame()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int boxWidth = 400;
    int boxHeight = 150;
    int boxX = (screenWidth - boxWidth) / 2;
    int boxY = (screenHeight - boxHeight) / 2;

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, RAYWHITE);
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, DARKGRAY);

    const char* msg = "Are you sure you want to quit?";
    const char* options = "[Y]es     [N]o";

    DrawText(msg, boxX + (boxWidth - MeasureText(msg, 20)) / 2, boxY + 30, 20, BLACK);

    DrawText(options, boxX + (boxWidth - MeasureText(options, 20)) / 2, boxY + 80, 20, DARKGRAY);
}

void Application::renderMenu()
{
    BeginDrawing();
    renderVideoBackground();

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    BeginMode3D(simpleCamera);

    // float t = GetTime();
    // float floatY = sinf(t * 0.7f) * 0.5f;

    // Matrix rotation = MatrixRotateY(t);
    // Matrix translation = MatrixTranslate(0.0f, floatY, 0.0f);
    // torus.transform = MatrixMultiply(rotation, translation);

    DrawModel(torus, {3.0f, 0.0f, 1.0f}, 1.0f, LIGHTGRAY);
    EndMode3D();

    DrawText("ZAPPY", screenWidth / 2 - MeasureText("ZAPPY", 40) / 2, screenHeight / 3, 40, RAYWHITE);
    DrawText("Press ENTER to Start", screenWidth / 2 - MeasureText("Press ENTER to Start", 20) / 2, screenHeight / 2, 20, LIGHTGRAY);
    DrawText("Press ESC to Exit", screenWidth / 2 - MeasureText("Press ESC to Exit", 20) / 2, screenHeight / 2 + 30, 20, LIGHTGRAY);
    DrawText("How to play? (Press H)", screenWidth / 2 - MeasureText("How to play? (Press H)", 20) / 2, screenHeight / 2 + 220, 20, LIGHTGRAY);
    DrawText("Settings (Press S)", screenWidth / 2 - MeasureText("Settings (Press S)", 20) / 2, screenHeight / 2 + 280, 20, LIGHTGRAY);
    DrawText("Credits (Press C)", screenWidth / 2 - MeasureText("Credits (Press C)", 20) / 2, screenHeight / 2 + 310, 20, LIGHTGRAY);

    EndDrawing();
}

void Application::renderLegend()
{
    if (!showLegend) return;
    
    int legendWidth = 300;
    int legendHeight = 300;
    int legendX = WINDOW_WIDTH - legendWidth - 12;
    int legendY = 110;
    
    DrawRectangle(legendX, legendY, legendWidth, legendHeight, Fade(BLACK, 0.8f));
    DrawRectangleLines(legendX, legendY, legendWidth, legendHeight, WHITE);
    
    DrawText("LEGEND (Press L to hide)", legendX + 10, legendY + 10, 14, YELLOW);
    
    int yPos = legendY + 35;
    int lineHeight = 20;
    
    DrawText("RESOURCES:", legendX + 10, yPos, 12, WHITE);
    yPos += lineHeight;
    
    const char* resourceNames[] = {
        "Food", "Linemate", "Deraumere", "Sibur", 
        "Mendiane", "Phiras", "Thystame"
    };
    
    for (int i = 0; i < 7; ++i) {
        Color color = getResourceColorForLegend(static_cast<ResourceType>(i));
        
        if (i == 0) {
            DrawCircle(legendX + 20, yPos + 8, 4, color);
        } else {
            Vector2 center = {static_cast<float>(legendX + 20), static_cast<float>(yPos + 8)};
            Vector2 points[4] = {
                {center.x, center.y - 3.0f}, {center.x + 2.0f, center.y},
                {center.x, center.y + 3.0f}, {center.x - 2.0f, center.y}
            };
            for (int p = 0; p < 4; ++p) {
                Vector2 next = points[(p + 1) % 4];
                DrawLineEx(points[p], next, 2, color);
            }
        }
        
        DrawText(resourceNames[i], legendX + 40, yPos, 10, LIGHTGRAY);
        yPos += 15;
    }
    
    yPos += 10;
    
    DrawText("OTHERS:", legendX + 10, yPos, 12, WHITE);
    yPos += lineHeight;
    
    DrawCircle(legendX + 20, yPos + 8, 8, BLUE);
    DrawText("Players (size = level)", legendX + 40, yPos, 10, LIGHTGRAY);
    yPos += 15;
    
    DrawCircle(legendX + 20, yPos + 8, 6, WHITE);
    DrawCircleLines(legendX + 20, yPos + 8, 6, GOLD);
    DrawText("Eggs", legendX + 40, yPos, 10, LIGHTGRAY);
    yPos += 15;
    
    yPos += 10;
    DrawText("CONTROLS:", legendX + 10, yPos, 12, WHITE);
    yPos += lineHeight;
    DrawText("WASD - Move camera", legendX + 10, yPos, 9, GRAY);
    yPos += 12;
    DrawText("Mouse wheel - Zoom", legendX + 10, yPos, 9, GRAY);
    yPos += 12;
    DrawText("L - Toggle legend", legendX + 10, yPos, 9, GRAY);
}

Color Application::getResourceColorForLegend(ResourceType type) const
{
    switch (type) {
        case ResourceType::FOOD:      return GREEN;
        case ResourceType::LINEMATE:  return WHITE;
        case ResourceType::DERAUMERE: return YELLOW;
        case ResourceType::SIBUR:     return BLUE;
        case ResourceType::MENDIANE:  return RED;
        case ResourceType::PHIRAS:    return PURPLE;
        case ResourceType::THYSTAME:  return ORANGE;
        default:                      return GRAY;
    }
}

void Application::renderGame() 
{
    BeginDrawing();

    DrawTexturePro(
    backImage,
    { 0, 0, (float)backImage.width, (float)backImage.height },
    { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
    { 0, 0 },
    0.0f,
    WHITE
    );

    renderer->renderFrame(*gameState, *camera);
    renderGameInfoHUD();
    renderTeamsHUD();
    renderConnectionHUD();
    renderSelectedPlayerHUD();
    
    renderLegend();

    if (showQuitConfirmation) {
        quitGame();
    }
    
    EndDrawing();
}

void Application::renderHUDFrame(int x, int y, int width, int height, const std::string& title, Color bgColor)
{
    if (!hudStyleMode) return;
    
    DrawRectangle(x, y, width, height, bgColor);
    
    DrawRectangleLines(x, y, width, height, HUD_BORDER);
    
    DrawRectangleLines(x + 1, y + 1, width - 2, height - 2, HUD_ACCENT);
    
    if (!title.empty()) {
        DrawRectangle(x, y, width, 25, Fade(HUD_BORDER, 0.8f));
        DrawRectangleLines(x, y, width, 25, HUD_ACCENT);
        
        // Titre centré
        int titleWidth = MeasureText(title.c_str(), 14);
        DrawText(title.c_str(), x + (width - titleWidth) / 2, y + 6, 14, WHITE);
    }
}

void Application::renderGameInfoHUD()
{
    if (!showHUD) return;
    
    Position mapSize = gameState->getMapSize();
    const auto& teams = gameState->getTeams();
    
    const auto& players = gameState->getPlayers();
    int alivePlayerCount = 0;
    for (const auto& pair : players) {
        if (pair.second.alive) alivePlayerCount++;
    }
    
    int mainPanelWidth = 350;
    int mainPanelHeight = 100;
    renderHUDFrame(10, 10, mainPanelWidth, mainPanelHeight, "GAME INFO", PASTEL_BLUE);
    
    int textY = hudStyleMode ? 35 : 10;
    std::string info = "Map: " + std::to_string(mapSize.x) + "x" + std::to_string(mapSize.y) +
                      " | Players: " + std::to_string(alivePlayerCount) +
                      " | Teams: " + std::to_string(teams.size());
    
    DrawText(info.c_str(), 15, textY, 14, hudStyleMode ? BLACK : WHITE);
    
    std::string timeInfo = "Time Unit: " + std::to_string(gameState->getTimeUnit());
    DrawText(timeInfo.c_str(), 15, textY + 20, 14, hudStyleMode ? BLACK : WHITE);
    
    std::string controls = "Controls: H-Toggle HUD | U-Style | L-Legend | B-Menu";
    DrawText(controls.c_str(), 15, textY + 40, 12, hudStyleMode ? DARKGRAY : LIGHTGRAY);
    std::string controls2 = "D - 3D Mode";
    DrawText(controls2.c_str(), 15, textY + 57, 12, hudStyleMode ? DARKGRAY : LIGHTGRAY);
}

void Application::renderTeamsHUD()
{
    if (!showHUD) return;
    
    const auto& teams = gameState->getTeams();
    const auto& players = gameState->getPlayers();
    
    if (teams.empty()) return;
    
    // PANNEAU ÉQUIPES (à gauche)
    int teamsPanelWidth = 280;
    int teamsPanelHeight = static_cast<int>(teams.size() * 30 + 60);
    int panelX = 10;
    int panelY = 120;
    
    renderHUDFrame(panelX, panelY, teamsPanelWidth, teamsPanelHeight, "TEAMS", PASTEL_GREEN);
    
    int yOffset = hudStyleMode ? panelY + 35 : panelY + 10;
    
    for (const auto& teamPair : teams) {
        const TeamData& team = teamPair.second;
        
        DrawRectangle(panelX + 10, yOffset, 20, 15, team.color);
        DrawRectangleLines(panelX + 10, yOffset, 20, 15, hudStyleMode ? BLACK : WHITE);
        
        int teamPlayers = 0;
        float avgLevel = 0;
        int count = 0;
        for (const auto& playerPair : players) {
            if (playerPair.second.alive && playerPair.second.teamName == team.name) {
                teamPlayers++;
                avgLevel += playerPair.second.level;
                count++;
            }
        }
        if (count > 0) avgLevel /= count;
        
        std::string teamInfo = team.name + " (" + std::to_string(teamPlayers) + 
                              " players, avg lvl " + std::to_string(static_cast<int>(avgLevel)) + ")";
        
        Color textColor = hudStyleMode ? DARKGRAY : WHITE;
        DrawText(teamInfo.c_str(), panelX + 35, yOffset, 12, textColor);
        
        yOffset += 25;
    }
}

void Application::renderConnectionHUD()
{
    if (!showHUD) return;
    
    // PANNEAU CONNEXION (en haut à droite)
    int connPanelWidth = 200;
    int connPanelHeight = 80;
    int panelX = WINDOW_WIDTH - connPanelWidth - 10;
    int panelY = 10;
    
    Color panelColor = connected ? PASTEL_MINT : PASTEL_PINK;
    renderHUDFrame(panelX, panelY, connPanelWidth, connPanelHeight, "CONNECTION", panelColor);
    
    int textY = hudStyleMode ? panelY + 35 : panelY + 10;
    
    std::string status = connected ? "Connected" : "Offline";
    Color statusColor = connected ? (hudStyleMode ? DARKGREEN : GREEN) : (hudStyleMode ? MAROON : RED);
    DrawText(status.c_str(), panelX + 10, textY, 16, statusColor);
    
    if (connected) {
        std::string serverInfo = serverHost + ":" + std::to_string(serverPort);
        Color infoColor = hudStyleMode ? GRAY : LIGHTGRAY;
        DrawText(serverInfo.c_str(), panelX + 10, textY + 20, 12, infoColor);
    }
}

void Application::renderSelectedPlayerHUD()
{
    if (!showHUD) return;
    
    // Pour l'instant, on affiche juste des stats générales
    // car on n'a pas encore implémenté la sélection de joueur
    
    const auto& players = gameState->getPlayers();
    if (players.empty()) return;
    
    // PANNEAU STATS GÉNÉRALES (en bas à droite)
    int playerPanelWidth = 320;
    int playerPanelHeight = 120;
    int panelX = WINDOW_WIDTH - playerPanelWidth - 10;
    int panelY = WINDOW_HEIGHT - playerPanelHeight - 10;
    
    renderHUDFrame(panelX, panelY, playerPanelWidth, playerPanelHeight, 
                   "GAME STATS", PASTEL_PURPLE);
    
    int yPos = hudStyleMode ? panelY + 35 : panelY + 10;
    Color textColor = hudStyleMode ? BLACK : WHITE;
    Color labelColor = hudStyleMode ? DARKGRAY : LIGHTGRAY;
    
    // Statistiques générales
    int totalPlayers = static_cast<int>(players.size());
    int maxLevel = 1;
    
    for (const auto& pair : players) {
        if (pair.second.alive && pair.second.level > maxLevel) {
            maxLevel = pair.second.level;
        }
    }
    
    std::string statsInfo = "Total Players: " + std::to_string(totalPlayers);
    DrawText(statsInfo.c_str(), panelX + 10, yPos, 14, textColor);
    yPos += 20;
    
    std::string levelInfo = "Highest Level: " + std::to_string(maxLevel);
    DrawText(levelInfo.c_str(), panelX + 10, yPos, 14, textColor);
    yPos += 20;
    
    DrawText("Click on a player to select", panelX + 10, yPos, 12, labelColor);
}

void Application::renderHelp()
{
    BeginDrawing();
    renderVideoBackground();

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.3f));

    DrawText("How to Play", screenWidth / 2 - MeasureText("How to Play", 40) / 2, 40, 40, GOLD);

    int boxX = 60;
    int lineHeight = 40;
    int startY = 120;

    DrawText("CONTROLS:", boxX, startY, 25, SKYBLUE);
    DrawText("- Move with arrow keys", boxX + 20, startY + lineHeight, 20, RAYWHITE);
    DrawText("- Zoom with mouse wheel", boxX + 20, startY + 2 * lineHeight, 20, RAYWHITE);

    int goalY = startY + 4 * lineHeight;
    DrawText("GOAL:", boxX, goalY, 25, YELLOW);
    DrawText("Teams confront each other on Trantor, a tiled map,", boxX + 20, goalY + lineHeight, 20, RAYWHITE);
    DrawText("collecting food resources to level up.", boxX + 20, goalY + 2 * lineHeight, 20, RAYWHITE);
    DrawText("The first team with at least 6 players at max level wins.", boxX + 20, goalY + 3 * lineHeight, 20, RED);

    int tipsY = goalY + 5 * lineHeight;
    DrawText("TIPS:", boxX, tipsY, 25, GREEN);
    DrawText("- Stick together and communicate with your team!", boxX + 20, tipsY + lineHeight, 20, RAYWHITE);
    DrawText("- Don't forget to collect food regularly.", boxX + 20, tipsY + 2 * lineHeight, 20, RAYWHITE);

    float pulse = (sinf(GetTime() * 3.0f) + 1.0f) * 0.5f; // 0 → 1
    Color faded = Fade(LIGHTGRAY, 0.5f + 0.5f * pulse);
    DrawText("Press B to return to menu",
        screenWidth / 2 - MeasureText("Press B to return to menu", 20) / 2,
        screenHeight - 60, 20, faded);

    EndDrawing();
}

void Application::renderCredits()
{
    BeginDrawing();
    renderVideoBackground();

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText("Credits", screenWidth / 2 - MeasureText("Credits", 30) / 2, screenHeight / 4, 30, LIGHTGRAY);
    DrawText("Yanis Asselman, Soukaina Hassani - Graphical User Interface (GUI)", 50, screenHeight / 2 - 40, 20, LIGHTGRAY);
    DrawText("Mathis Jusy - Artificial Intelligence", 50, screenHeight / 2 - 10, 20, LIGHTGRAY);
    DrawText("Killyan Touat - Player Management and Commands", 50, screenHeight / 2 + 20, 20, LIGHTGRAY);
    DrawText("Julien Mars - Server Core and Networking", 50, screenHeight / 2 + 50, 20, LIGHTGRAY);

    DrawText("Tek2, June 2025", 50, screenHeight / 2 + 90, 20, LIGHTGRAY);

    float pulse = (sinf(GetTime() * 3.0f) + 1.0f) * 0.5f; // 0 → 1
    Color faded = Fade(LIGHTGRAY, 0.5f + 0.5f * pulse);
    DrawText("Press B to return to menu",
        screenWidth / 2 - MeasureText("Press B to return to menu", 20) / 2,
        screenHeight - 60, 20, faded);

    EndDrawing();
}

void Application::loadMusic()
{
    backgroundMusic = LoadMusicStream("gui/assets/audio/theme_test.mp3");
    if (backgroundMusic.stream.buffer != nullptr) {
        musicLoaded = true;
        SetMusicVolume(backgroundMusic, musicVolume);
        PlayMusicStream(backgroundMusic);
        std::cout << "Background music loaded successfully" << std::endl;
    } else {
        std::cout << "Warning: Could not load background music" << std::endl;
        musicLoaded = false;
    }
}

void Application::updateMusic()
{
    if (musicLoaded) {
        UpdateMusicStream(backgroundMusic);
        SetMusicVolume(backgroundMusic, musicVolume);
    }
}

void Application::updateVideo()
{
    if (videoEnabled) {
        videoTime += GetFrameTime();
    }
}

void Application::renderVideoBackground()
{
    if (!videoEnabled) return;
    float wave1 = sinf(videoTime * 0.3f) * 0.2f + 0.5f;
    float wave2 = cosf(videoTime * 0.2f) * 0.3f + 0.7f;
    
    Color color1 = {
        (unsigned char)(30 + wave1 * 50),
        (unsigned char)(50 + wave2 * 30), 
        (unsigned char)(80 + sinf(videoTime * 0.15f) * 40),
        255
    };
    
    Color color2 = {
        (unsigned char)(10 + wave2 * 20),
        (unsigned char)(20 + wave1 * 40),
        (unsigned char)(60 + cosf(videoTime * 0.1f) * 30),
        255
    };
    
    DrawRectangleGradientV(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, color1, color2);
    
    for (int i = 0; i < 15; ++i) {
        float x = (sinf(videoTime * 0.4f + i) * 0.5f + 0.5f) * WINDOW_WIDTH;
        float y = (cosf(videoTime * 0.3f + i * 0.5f) * 0.5f + 0.5f) * WINDOW_HEIGHT;
        float size = 3 + sinf(videoTime + i) * 2;
        float alpha = 0.1f + 0.2f * sinf(videoTime * 0.6f + i);
        
        DrawCircle((int)x, (int)y, size, Fade(WHITE, alpha));
    }
}

void Application::renderSettings()
{
    BeginDrawing();
    renderVideoBackground();

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int y = 100;
    int lineHeight = 40;

    DrawText("Settings", screenWidth / 2 - MeasureText("Settings", 30) / 2, 30, 30, YELLOW);

    Color normalTextColor = LIGHTGRAY;
    Color selectedTextColor = GOLD;

    auto drawOption = [&](int idx, const std::string& label, const std::string& value) {
        Color color = (selected == idx) ? selectedTextColor : normalTextColor;
        DrawText((label + ": " + value).c_str(), 100, y + idx * lineHeight, 20, color);
    };

    drawOption((int)SettingsOption::MUSIC_VOLUME, "Music Volume", std::to_string((int)(musicVolume * 100)) + "%");
    drawOption((int)SettingsOption::FULLSCREEN, "Fullscreen", fullscreen ? "ON" : "OFF");
    float pulse = (sinf(GetTime() * 3.0f) + 1.0f) * 0.5f; // 0 → 1

    Color faded = Fade(LIGHTGRAY, 0.5f + 0.5f * pulse);
    DrawText("Press B to return to menu",
        screenWidth / 2 - MeasureText("Press B to return to menu", 20) / 2,
        screenHeight - 60, 20, faded);


    EndDrawing();

}

void Application::renderTransition()
{
    BeginDrawing();
    ClearBackground(BLANK);

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(WHITE, transitionAlpha));

    EndDrawing();
}

void Application::startTransition(AppStateTarget target)
{
    transitioning = true,
    transitionAlpha = 0.0f;
    transitionTarget = target;
    state = AppState::TRANSITION;
}

void Application::updateTransition()
{
    const float fadeSpeed = 1.6f * GetFrameTime(); // you can tweak this
    transitionAlpha += fadeSpeed;

    if (transitionAlpha >= 1.0f) {
        transitioning = false;
        state = static_cast<AppState>(transitionTarget);
        transitionTarget = AppStateTarget::NONE;
        transitionAlpha = 0.0f;
    }
}

void Application::run()
{
    std::cout << "Starting C++ main loop..." << std::endl;
    
    while (running) {
        updateMusic();
        updateVideo();
        switch (state) {
            case AppState::MENU:
                handleMenuInput();
                renderMenu();
                break;
            case AppState::GAME:
                handleInput();
                processNetwork();
                update();
                renderGame();
                break;
            case AppState::HELP:
                handleHelpInput();
                renderHelp();
                break;
            case AppState::CREDITS:
                handleCreditsInput();
                renderCredits();
                break;
            case AppState::SETTINGS:
                handleSettingsInput();
                renderSettings();
                break;
            case AppState::TRANSITION:
                updateTransition();
                renderTransition();
        }
    }
    
    std::cout << "Application exiting..." << std::endl;
    UnloadTexture(backImage);
    UnloadModel(torus);
    CloseWindow();
}

}