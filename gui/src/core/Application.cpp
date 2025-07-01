/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** Application
*/

#include "../../include/Application.hpp"
#include <getopt.h>
#include <cmath>
#include <raymath.h>

bool g_levelUpTriggered = false;
bool g_victoryTriggered = false;
bool g_showVictoryPopup = false;
std::string g_winnerTeamName = "";

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

InitWindow(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT, "Zappy C++ GUI");    SetTargetFPS(60);

    InitAudioDevice();
    loadMusic();
    loadSounds();
    initializeButtons();

    try {
        gameState = std::make_unique<GameState>();
        networkClient = std::make_unique<NetworkClient>();
        protocolHandler = std::make_unique<ProtocolHandler>(gameState.get(), this);
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
        
        if (fullscreen) {
            int monitorWidth = GetMonitorWidth(0);
            int monitorHeight = GetMonitorHeight(0);
            
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowSize(monitorWidth, monitorHeight);
            SetWindowPosition(0, 0);
            
            std::cout << "Fullscreen fenêtré: " << monitorWidth << "x" << monitorHeight << std::endl;
        } else {
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowSize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
            
            int monitorWidth = GetMonitorWidth(0);
            int monitorHeight = GetMonitorHeight(0);
            SetWindowPosition(
                (monitorWidth - BASE_WINDOW_WIDTH) / 2,
                (monitorHeight - BASE_WINDOW_HEIGHT) / 2
            );
            
            std::cout << "Mode fenêtré: " << BASE_WINDOW_WIDTH << "x" << BASE_WINDOW_HEIGHT << std::endl;
        }
        
        updateFullscreenScale();
    }
    break;
        default: break;
    }
}

void Application::handleMenuInput()
{
    if (IsKeyPressed(KEY_ENTER)) {
        startTransition(AppStateTarget::GAME);
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
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        renderer->handleMouseClick(mousePos, *camera, *gameState);
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
    if (IsKeyPressed(KEY_I)) {
        showFullMapInfo = !showFullMapInfo;
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
    DrawModel(torus, {3.0f, 0.0f, 1.0f}, 1.0f, LIGHTGRAY);
    EndMode3D();

    float scale = getScreenScale();
    int titleSize = (int)(50 * scale);
    if (titleSize < 30) titleSize = 30;
    
    DrawText("ZAPPY", screenWidth / 2 - MeasureText("ZAPPY", titleSize) / 2, 
             screenHeight / 4, titleSize, RAYWHITE);

    int subtitleSize = (int)(20 * scale);
    if (subtitleSize < 12) subtitleSize = 12;
    
    const char* subtitle = "A Tribute to Zaphod Beeblebrox";
    DrawText(subtitle, screenWidth / 2 - MeasureText(subtitle, subtitleSize) / 2, 
             screenHeight / 4 + titleSize + 10, subtitleSize, LIGHTGRAY);

    updateButtons(menuButtons);
    renderButtons(menuButtons);

    int instructSize = (int)(14 * scale);
    if (instructSize < 8) instructSize = 8;
    
    DrawText("Use mouse to interact with buttons", 
             screenWidth / 2 - MeasureText("Use mouse to interact with buttons", instructSize) / 2, 
             screenHeight - 50, instructSize, Fade(LIGHTGRAY, 0.7f));

    EndDrawing();
}

void Application::renderLegend()
{
    if (!showLegend) return;
    
    float scale = getScreenScale();
    
    int legendWidth = (int)(300 * scale);
    int legendHeight = (int)(300 * scale);
    int legendX = GetScreenWidth() - legendWidth - (int)(12 * scale);
    int legendY = (int)(110 * scale);
    
    DrawRectangle(legendX, legendY, legendWidth, legendHeight, Fade(BLACK, 0.8f));
    DrawRectangleLines(legendX, legendY, legendWidth, legendHeight, WHITE);
    
    int titleFont = (int)(14 * scale);
    int normalFont = (int)(12 * scale);
    int smallFont = (int)(10 * scale);
    int tinyFont = (int)(9 * scale);
    
    if (titleFont < 8) titleFont = 8;
    if (normalFont < 6) normalFont = 6;
    if (smallFont < 6) smallFont = 6;
    if (tinyFont < 6) tinyFont = 6;
    
    DrawText("LEGEND (Press L to hide)", legendX + 10, legendY + 10, titleFont, YELLOW);
    
    int yPos = legendY + (int)(35 * scale);
    int lineHeight = (int)(20 * scale);
    
    DrawText("RESOURCES:", legendX + 10, yPos, normalFont, WHITE);
    yPos += lineHeight;
    
    const char* resourceNames[] = {
        "Food", "Linemate", "Deraumere", "Sibur", 
        "Mendiane", "Phiras", "Thystame"
    };
    
    for (int i = 0; i < 7; ++i) {
        Color color = getResourceColorForLegend(static_cast<ResourceType>(i));
        
        int iconSize = (int)(4 * scale);
        if (iconSize < 2) iconSize = 2;
        
        if (i == 0) {
            DrawCircle(legendX + 20, yPos + 8, iconSize, color);
        } else {
            Vector2 center = {static_cast<float>(legendX + 20), static_cast<float>(yPos + 8)};
            Vector2 points[4] = {
                {center.x, center.y - iconSize}, {center.x + iconSize/2, center.y},
                {center.x, center.y + iconSize}, {center.x - iconSize/2, center.y}
            };
            for (int p = 0; p < 4; ++p) {
                Vector2 next = points[(p + 1) % 4];
                DrawLineEx(points[p], next, 2, color);
            }
        }
        
        DrawText(resourceNames[i], legendX + 40, yPos, smallFont, LIGHTGRAY);
        yPos += (int)(15 * scale);
    }
    
    yPos += (int)(10 * scale);
    
    DrawText("OTHERS:", legendX + 10, yPos, normalFont, WHITE);
    yPos += lineHeight;
    
    int playerIconSize = (int)(8 * scale);
    if (playerIconSize < 4) playerIconSize = 4;
    
    DrawCircle(legendX + 20, yPos + 8, playerIconSize, BLUE);
    DrawText("Players (size = level)", legendX + 40, yPos, smallFont, LIGHTGRAY);
    yPos += (int)(15 * scale);
    
    int eggIconSize = (int)(6 * scale);
    if (eggIconSize < 3) eggIconSize = 3;
    
    DrawCircle(legendX + 20, yPos + 8, eggIconSize, WHITE);
    DrawCircleLines(legendX + 20, yPos + 8, eggIconSize, GOLD);
    DrawText("Eggs", legendX + 40, yPos, smallFont, LIGHTGRAY);
    yPos += (int)(15 * scale);
    
    yPos += (int)(10 * scale);
    DrawText("CONTROLS:", legendX + 10, yPos, normalFont, WHITE);
    yPos += lineHeight;
    DrawText("WASD - Move camera", legendX + 10, yPos, tinyFont, GRAY);
    yPos += (int)(12 * scale);
    DrawText("Mouse wheel - Zoom", legendX + 10, yPos, tinyFont, GRAY);
    yPos += (int)(12 * scale);
    DrawText("L - Toggle legend", legendX + 10, yPos, tinyFont, GRAY);
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
        ClearBackground(BLACK);

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
    if (showFullMapInfo) {
        renderFullMapInfoPanel();
    }

    updateButtons(gameButtons);
    renderButtons(gameButtons);
    renderVictoryPopup();
    EndDrawing();
}

void Application::renderHUDFrame(int x, int y, int width, int height, const std::string& title, Color bgColor)
{
    if (!hudStyleMode) return;
    
    float scale = getScreenScale();
    
    DrawRectangle(x, y, width, height, bgColor);
    DrawRectangleLines(x, y, width, height, HUD_BORDER);
    DrawRectangleLines(x + 1, y + 1, width - 2, height - 2, HUD_ACCENT);
    
    if (!title.empty()) {
        int titleHeight = (int)(25 * scale);
        DrawRectangle(x, y, width, titleHeight, Fade(HUD_BORDER, 0.8f));
        DrawRectangleLines(x, y, width, titleHeight, HUD_ACCENT);
        
        int titleFont = (int)(14 * scale);
        if (titleFont < 8) titleFont = 8;
        int titleWidth = MeasureText(title.c_str(), titleFont);
        DrawText(title.c_str(), x + (width - titleWidth) / 2, y + (int)(6 * scale), titleFont, WHITE);
    }
}

void Application::renderGameInfoHUD()
{
    if (!showHUD) return;
    
    float scale = getScreenScale();
    
    Position mapSize = gameState->getMapSize();
    const auto& teams = gameState->getTeams();
    const auto& players = gameState->getPlayers();
    
    int alivePlayerCount = 0;
    for (const auto& pair : players) {
        if (pair.second.alive) alivePlayerCount++;
    }
    int mainPanelWidth = (int)(350 * scale);
    int mainPanelHeight = (int)(100 * scale);
    int posX = (int)(10 * scale);
    int posY = (int)(10 * scale);
    
    renderHUDFrame(posX, posY, mainPanelWidth, mainPanelHeight, "GAME INFO", PASTEL_BLUE);
    
    int fontSize = (int)(14 * scale);
    if (fontSize < 8) fontSize = 8;
    
    int textY = hudStyleMode ? (int)(35 * scale) : posY + 10;
    
    std::string info = "Map: " + std::to_string(mapSize.x) + "x" + std::to_string(mapSize.y) +
                      " | Players: " + std::to_string(alivePlayerCount) +
                      " | Teams: " + std::to_string(teams.size());
    
    DrawText(info.c_str(), posX + 5, textY, fontSize, hudStyleMode ? BLACK : WHITE);
    
    std::string timeInfo = "Time Unit: " + std::to_string(gameState->getTimeUnit());
    DrawText(timeInfo.c_str(), posX + 5, textY + (int)(20 * scale), fontSize, hudStyleMode ? BLACK : WHITE);
}

void Application::renderTeamsHUD()
{
    if (!showHUD) return;
    
    float scale = getScreenScale();
    
    const auto& teams = gameState->getTeams();
    const auto& players = gameState->getPlayers();
    
    if (teams.empty()) return;
    
    int teamsPanelWidth = (int)(280 * scale);
    int teamsPanelHeight = (int)(teams.size() * 30 * scale + 60 * scale);
    int panelX = (int)(10 * scale);
    int panelY = (int)(120 * scale);
    
    renderHUDFrame(panelX, panelY, teamsPanelWidth, teamsPanelHeight, "TEAMS", PASTEL_GREEN);
    
    int yOffset = hudStyleMode ? panelY + (int)(25 * scale) : panelY + 10;
    int fontSize = (int)(12 * scale);
    if (fontSize < 6) fontSize = 6;
    
    for (const auto& teamPair : teams) {
        const TeamData& team = teamPair.second;
        
        int rectSize = (int)(20 * scale);
        DrawRectangle(panelX + 10, yOffset, rectSize, (int)(15 * scale), team.color);
        DrawRectangleLines(panelX + 10, yOffset, rectSize, (int)(15 * scale), hudStyleMode ? BLACK : WHITE);
        
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
        DrawText(teamInfo.c_str(), panelX + rectSize + 15, yOffset, fontSize, textColor);
        
        yOffset += (int)(25 * scale);
    }
}

void Application::renderConnectionHUD()
{
    if (!showHUD) return;
    
    float scale = getScreenScale();
    
    int connPanelWidth = (int)(200 * scale);
    int connPanelHeight = (int)(80 * scale);
    int panelX = GetScreenWidth() - connPanelWidth - (int)(10 * scale);
    int panelY = (int)(10 * scale);
    
    Color panelColor = connected ? PASTEL_MINT : PASTEL_PINK;
    renderHUDFrame(panelX, panelY, connPanelWidth, connPanelHeight, "CONNECTION", panelColor);
    
    int textY = hudStyleMode ? panelY + (int)(25 * scale) : panelY + 10;
    int fontSize = (int)(16 * scale);
    if (fontSize < 8) fontSize = 8;
    
    std::string status = connected ? "Connected" : "Offline";
    Color statusColor = connected ? (hudStyleMode ? DARKGREEN : GREEN) : (hudStyleMode ? MAROON : RED);
    DrawText(status.c_str(), panelX + 10, textY, fontSize, statusColor);
    
    if (connected) {
        std::string serverInfo = serverHost + ":" + std::to_string(serverPort);
        Color infoColor = hudStyleMode ? GRAY : LIGHTGRAY;
        int smallFont = (int)(12 * scale);
        if (smallFont < 6) smallFont = 6;
        DrawText(serverInfo.c_str(), panelX + 10, textY + (int)(20 * scale), smallFont, infoColor);
    }
}

void Application::renderSelectedPlayerHUD()
{
    if (!showHUD) return;
    
    float scale = getScreenScale();
    
    const auto& selected = renderer->getSelectedObject();
    
    int playerPanelWidth = (int)(320 * scale);
    int playerPanelHeight = (int)(200 * scale);
    int panelX = GetScreenWidth() - playerPanelWidth - (int)(10 * scale);
    int panelY = GetScreenHeight() - playerPanelHeight - (int)(10 * scale);
    
    int titleFont = (int)(14 * scale);
    int normalFont = (int)(12 * scale);
    int smallFont = (int)(10 * scale);
    if (titleFont < 8) titleFont = 8;
    if (normalFont < 6) normalFont = 6;
    if (smallFont < 6) smallFont = 6;
    
    if (selected.type == Renderer::SelectedObject::PLAYER) {
        const PlayerData* player = gameState->getPlayer(selected.playerId);
        if (player) {
            renderHUDFrame(panelX, panelY, playerPanelWidth, playerPanelHeight, 
                          "SELECTED PLAYER", PASTEL_PURPLE);
            
            int yPos = hudStyleMode ? panelY + (int)(25 * scale) : panelY + 10;
            Color textColor = hudStyleMode ? BLACK : WHITE;
            
            std::string playerInfo = "ID: " + std::to_string(player->id) + 
                                   " | Team: " + player->teamName;
            DrawText(playerInfo.c_str(), panelX + 10, yPos, titleFont, textColor);
            yPos += (int)(20 * scale);
            
            std::string levelInfo = "Level: " + std::to_string(player->level);
            DrawText(levelInfo.c_str(), panelX + 10, yPos, titleFont, textColor);
            yPos += (int)(20 * scale);
            
            std::string posInfo = "Position: (" + std::to_string(player->pos.x) + 
                                 "," + std::to_string(player->pos.y) + ")";
            DrawText(posInfo.c_str(), panelX + 10, yPos, normalFont, textColor);
            yPos += (int)(25 * scale);
            
            DrawText("Inventory:", panelX + 10, yPos, normalFont, DARKGRAY);
            yPos += (int)(15 * scale);
            const char* resNames[] = {"Food", "Linemate", "Deraumere", "Sibur", 
                                    "Mendiane", "Phiras", "Thystame"};
            for (int i = 0; i < 7; ++i) {
                if (player->inventory[i] > 0) {
                    std::string inv = std::string(resNames[i]) + ": " + 
                                     std::to_string(player->inventory[i]);
                    DrawText(inv.c_str(), panelX + 15, yPos, smallFont, textColor);
                    yPos += (int)(12 * scale);
                }
            }
        }
    } else if (selected.type == Renderer::SelectedObject::TILE) {
        const TileData* tile = gameState->getTile(selected.position.x, selected.position.y);
        if (tile) {
            renderHUDFrame(panelX, panelY, playerPanelWidth, playerPanelHeight, 
                          "SELECTED TILE", PASTEL_YELLOW);
            
            int yPos = hudStyleMode ? panelY + (int)(25 * scale) : panelY + 10;
            Color textColor = hudStyleMode ? BLACK : WHITE;
            
            std::string tileInfo = "Position: (" + std::to_string(selected.position.x) + 
                                 "," + std::to_string(selected.position.y) + ")";
            DrawText(tileInfo.c_str(), panelX + 10, yPos, titleFont, textColor);
            yPos += (int)(25 * scale);
            
            DrawText("Resources:", panelX + 10, yPos, normalFont, DARKGRAY);
            yPos += (int)(15 * scale);
            const char* resNames[] = {"Food", "Linemate", "Deraumere", "Sibur", 
                                    "Mendiane", "Phiras", "Thystame"};
            for (int i = 0; i < 7; ++i) {
                if (tile->resources[i] > 0) {
                    std::string res = std::string(resNames[i]) + ": " + 
                                     std::to_string(tile->resources[i]);
                    DrawText(res.c_str(), panelX + 15, yPos, smallFont, textColor);
                    yPos += (int)(12 * scale);
                }
            }
            if (!tile->players.empty()) {
                yPos += (int)(5 * scale);
                DrawText("Players on tile:", panelX + 10, yPos, normalFont, DARKGRAY);
                yPos += (int)(15 * scale);
                for (int playerId : tile->players) {
                    std::string playerText = "Player " + std::to_string(playerId);
                    DrawText(playerText.c_str(), panelX + 15, yPos, smallFont, textColor);
                    yPos += (int)(12 * scale);
                }
            }
        }
    } else {
        renderHUDFrame(panelX, panelY, playerPanelWidth, playerPanelHeight, 
                      "GAME STATS", PASTEL_PURPLE);
        
        int yPos = hudStyleMode ? panelY + (int)(25 * scale) : panelY + 10;
        Color textColor = hudStyleMode ? BLACK : WHITE;
        
        const auto& players = gameState->getPlayers();
        int totalPlayers = static_cast<int>(players.size());
        int maxLevel = 1;
        
        for (const auto& pair : players) {
            if (pair.second.alive && pair.second.level > maxLevel) {
                maxLevel = pair.second.level;
            }
        }
        
        std::string statsInfo = "Total Players: " + std::to_string(totalPlayers);
        DrawText(statsInfo.c_str(), panelX + 10, yPos, titleFont, textColor);
        yPos += (int)(20 * scale);
        
        std::string levelInfo = "Highest Level: " + std::to_string(maxLevel);
        DrawText(levelInfo.c_str(), panelX + 10, yPos, titleFont, textColor);
        yPos += (int)(20 * scale);
        
        DrawText("Click on players/tiles to select", panelX + 10, yPos + (int)(20 * scale), normalFont, GRAY);
    }
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

         updateButtons(helpButtons);
    renderButtons(helpButtons);

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

         updateButtons(creditsButtons);
    renderButtons(creditsButtons);

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
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
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
    DrawRectangleGradientV(0, 0, screenWidth, screenHeight, color1, color2);
    
    float scale = getScreenScale();
    int numParticles = (int)(15 * scale);
    if (numParticles < 5) numParticles = 5;
    
    for (int i = 0; i < numParticles; ++i) {
        float x = (sinf(videoTime * 0.4f + i) * 0.5f + 0.5f) * screenWidth;
        float y = (cosf(videoTime * 0.3f + i * 0.5f) * 0.5f + 0.5f) * screenHeight;
        float size = (3 + sinf(videoTime + i) * 2) * scale;
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
    float scale = getScreenScale();

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.3f));

    int titleFont = (int)(40 * scale);
    if (titleFont < 20) titleFont = 20;
    DrawText("Settings", screenWidth / 2 - MeasureText("Settings", titleFont) / 2, 
             (int)(50 * scale), titleFont, YELLOW);

    int labelFont = (int)(25 * scale);
    int valueFont = (int)(20 * scale);
    if (labelFont < 15) labelFont = 15;
    if (valueFont < 12) valueFont = 12;
    
    int volumeY = (int)(150 * scale);
    DrawText("Music Volume:", (int)(50 * scale), volumeY, labelFont, WHITE);
    
    int barX = (int)(250 * scale);
    int barY = volumeY + (int)(5 * scale);
    int barWidth = (int)(200 * scale);
    int barHeight = (int)(20 * scale);
    
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, (int)(barWidth * musicVolume), barHeight, GREEN);
    DrawRectangleLines(barX, barY, barWidth, barHeight, WHITE);
    
    std::string volumeText = std::to_string((int)(musicVolume * 100)) + "%";
    DrawText(volumeText.c_str(), barX + barWidth + (int)(40 * scale), volumeY, valueFont, WHITE);

    int fullscreenY = (int)(200 * scale);
    DrawText("Fullscreen:", (int)(50 * scale), fullscreenY, labelFont, WHITE);
    

    int instructFont = (int)(16 * scale);
    if (instructFont < 10) instructFont = 10;
    
    DrawText("Use arrow buttons to adjust settings", 
             screenWidth / 2 - MeasureText("Use arrow buttons to adjust settings", instructFont) / 2,
             screenHeight - (int)(100 * scale), instructFont, LIGHTGRAY);
    
    DrawText("Click fullscreen button to toggle", 
             screenWidth / 2 - MeasureText("Click fullscreen button to toggle", instructFont) / 2,
             screenHeight - (int)(80 * scale), instructFont, LIGHTGRAY);

    updateButtons(settingsButtons);
    renderButtons(settingsButtons);

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
    const float fadeSpeed = 1.6f * GetFrameTime();
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
   if (state == AppState::MENU || state == AppState::HELP || 
            state == AppState::CREDITS || state == AppState::SETTINGS) {
            updateMusic();
        } else {
            if (musicLoaded && IsMusicStreamPlaying(backgroundMusic)) {
                StopMusicStream(backgroundMusic);
            }
        }        processAudioEvents();
        updateFullscreenScale();
        static int lastWidth = 0, lastHeight = 0;
if (GetScreenWidth() != lastWidth || GetScreenHeight() != lastHeight) {
    initializeButtons();
    lastWidth = GetScreenWidth();
    lastHeight = GetScreenHeight();
}
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

float Application::getScreenScale() const
{
    if (fullscreen) {
float scaleX = (float)GetScreenWidth() / BASE_WINDOW_WIDTH;
float scaleY = (float)GetScreenHeight() / BASE_WINDOW_HEIGHT;
        return std::min(scaleX, scaleY);
    }
    return 1.0f;
}

void Application::updateFullscreenScale()
{
    Camera2D& cam = const_cast<Camera2D&>(camera->getRaylibCamera());
    cam.offset.x = GetScreenWidth() / 2.0f;
    cam.offset.y = GetScreenHeight() / 2.0f;
}

void Application::renderFullMapInfoPanel()
{
    float scale = getScreenScale();
    
    int panelWidth = (int)(400 * scale);
    int panelHeight = GetScreenHeight() - (int)(20 * scale);
    int panelX = (int)(10 * scale);
    int panelY = (int)(10 * scale);
    
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(BLACK, 0.9f));
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, WHITE);
    
    int titleFont = (int)(16 * scale);
    if (titleFont < 10) titleFont = 10;
    DrawText("MAP OVERVIEW (Press I to hide)", panelX + 10, panelY + 10, titleFont, YELLOW);
    
    int yPos = panelY + (int)(40 * scale);
    int normalFont = (int)(12 * scale);
    int smallFont = (int)(10 * scale);
    if (normalFont < 8) normalFont = 8;
    if (smallFont < 6) smallFont = 6;
    
    Position mapSize = gameState->getMapSize();
    const auto& teams = gameState->getTeams();
    const auto& players = gameState->getPlayers();
    
    DrawText("=== MAP STATS ===", panelX + 10, yPos, normalFont, LIGHTGRAY);
    yPos += (int)(20 * scale);
    
    std::string mapInfo = "Size: " + std::to_string(mapSize.x) + "x" + std::to_string(mapSize.y);
    DrawText(mapInfo.c_str(), panelX + 15, yPos, smallFont, WHITE);
    yPos += (int)(15 * scale);
    
    std::string teamInfo = "Teams: " + std::to_string(teams.size());
    DrawText(teamInfo.c_str(), panelX + 15, yPos, smallFont, WHITE);
    yPos += (int)(15 * scale);
    
    std::string playerInfo = "Total Players: " + std::to_string(players.size());
    DrawText(playerInfo.c_str(), panelX + 15, yPos, smallFont, WHITE);
    yPos += (int)(25 * scale);
    
    DrawText("=== TEAMS & PLAYERS ===", panelX + 10, yPos, normalFont, LIGHTGRAY);
    yPos += (int)(20 * scale);
    
    for (const auto& teamPair : teams) {
        const TeamData& team = teamPair.second;
        
        DrawRectangle(panelX + 15, yPos, (int)(15 * scale), (int)(10 * scale), team.color);
        std::string teamName = team.name + ":";
        DrawText(teamName.c_str(), panelX + 35, yPos, smallFont, WHITE);
        yPos += (int)(15 * scale);
        
        int teamPlayerCount = 0;
        for (const auto& playerPair : players) {
            const PlayerData& player = playerPair.second;
            if (player.alive && player.teamName == team.name) {
                teamPlayerCount++;
                
                std::string playerDetails = "  P" + std::to_string(player.id) + 
                                          " Lvl:" + std::to_string(player.level) +
                                          " (" + std::to_string(player.pos.x) + "," + 
                                          std::to_string(player.pos.y) + ")";
                
                DrawText(playerDetails.c_str(), panelX + 25, yPos, smallFont, LIGHTGRAY);
                yPos += (int)(12 * scale);
                
                if (yPos > panelY + panelHeight - (int)(50 * scale)) {
                    DrawText("... (too many to display)", panelX + 25, yPos, smallFont, GRAY);
                    goto end_display;
                }
            }
        }
        
        if (teamPlayerCount == 0) {
            DrawText("  (no players)", panelX + 25, yPos, smallFont, GRAY);
            yPos += (int)(12 * scale);
        }
        
        yPos += (int)(10 * scale);
    }
    
    end_display:
    
    if (yPos < panelY + panelHeight - (int)(100 * scale)) {
        yPos += (int)(20 * scale);
        DrawText("=== TOTAL RESOURCES ===", panelX + 10, yPos, normalFont, LIGHTGRAY);
        yPos += (int)(20 * scale);
        
        std::array<int, 7> totalResources{};
        for (int y = 0; y < mapSize.y; ++y) {
            for (int x = 0; x < mapSize.x; ++x) {
                const TileData* tile = gameState->getTile(x, y);
                if (tile) {
                    for (int i = 0; i < 7; ++i) {
                        totalResources[i] += tile->resources[i];
                    }
                }
            }
        }
        
        const char* resNames[] = {"Food", "Linemate", "Deraumere", "Sibur", 
                                "Mendiane", "Phiras", "Thystame"};
        
        for (int i = 0; i < 7; ++i) {
            if (totalResources[i] > 0) {
                std::string resText = std::string(resNames[i]) + ": " + 
                                     std::to_string(totalResources[i]);
                DrawText(resText.c_str(), panelX + 15, yPos, smallFont, WHITE);
                yPos += (int)(12 * scale);
            }
        }
    }
}

void Application::loadSounds()
{
    levelUpSound = LoadSound("gui/assets/audio/levelup.wav");
    victorySound = LoadSound("gui/assets/audio/victory.wav");
    
    if (levelUpSound.frameCount > 0) {
        SetSoundVolume(levelUpSound, 0.3f);
        soundsLoaded = true;
        std::cout << "Level up sound loaded" << std::endl;
    } else {
        std::cout << "Warning: Could not load levelup sound" << std::endl;
    }
    
    if (victorySound.frameCount > 0) {
        SetSoundVolume(victorySound, 0.7f);
        std::cout << "Victory sound loaded" << std::endl;
    } else {
        std::cout << "Warning: Could not load victory sound" << std::endl;
    }
}

void Application::processAudioEvents()
{
    if (!soundsLoaded) return;
    
    if (g_levelUpTriggered) {
        PlaySound(levelUpSound);
        std::cout << "Level up sound played!" << std::endl;
        g_levelUpTriggered = false;
    }
    
    if (g_victoryTriggered) {
        PlaySound(victorySound);
        std::cout << "Victory sound played!" << std::endl;
        g_victoryTriggered = false;
    }

    if (g_showVictoryPopup) {
        showVictoryPopup = true;
        winnerTeamName = g_winnerTeamName;
        g_showVictoryPopup = false;
    }
}

void Application::initializeButtons()
{
    float scale = getScreenScale();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    menuButtons.clear();
    
    int buttonWidth = (int)(200 * scale);
    int buttonHeight = (int)(40 * scale);
    int buttonX = screenWidth/2 - buttonWidth/2;
    int startY = screenHeight/2 - (int)(40 * scale);
    int spacing = (int)(50 * scale);
    
    menuButtons.emplace_back(
        Rectangle{(float)buttonX, (float)startY, (float)buttonWidth, (float)buttonHeight},
        "START GAME", DARKGREEN, GREEN,
        [this]() { startTransition(AppStateTarget::GAME); }
    );
    
    menuButtons.emplace_back(
        Rectangle{(float)buttonX, (float)(startY + spacing), (float)buttonWidth, (float)buttonHeight},
        "HOW TO PLAY", DARKBLUE, BLUE,
        [this]() { state = AppState::HELP; }
    );
    
    menuButtons.emplace_back(
        Rectangle{(float)buttonX, (float)(startY + spacing*2), (float)buttonWidth, (float)buttonHeight},
        "SETTINGS", DARKGRAY, GRAY,
        [this]() { state = AppState::SETTINGS; }
    );
    
    menuButtons.emplace_back(
        Rectangle{(float)buttonX, (float)(startY + spacing*3), (float)buttonWidth, (float)buttonHeight},
        "CREDITS", PURPLE, VIOLET,
        [this]() { state = AppState::CREDITS; }
    );
    
    menuButtons.emplace_back(
        Rectangle{(float)buttonX, (float)(startY + spacing*4), (float)buttonWidth, (float)buttonHeight},
        "LEAVE GAME", DARK_RED, RED,
        [this]() { running = false; }
    );
    
    gameButtons.clear();
    
    int gameButtonWidth = (int)(80 * scale);
    int gameButtonHeight = (int)(30 * scale);
    
    gameButtons.emplace_back(
        Rectangle{10*scale, screenHeight - 50*scale, (float)gameButtonWidth, (float)gameButtonHeight},
        "HUD", DARKGREEN, GREEN,
        [this]() { showHUD = !showHUD; }
    );
    
    gameButtons.emplace_back(
        Rectangle{100*scale, screenHeight - 50*scale, (float)gameButtonWidth, (float)gameButtonHeight},
        "LEGEND", DARKBLUE, BLUE,
        [this]() { showLegend = !showLegend; }
    );
    
    gameButtons.emplace_back(
        Rectangle{190*scale, screenHeight - 50*scale, (float)gameButtonWidth, (float)gameButtonHeight},
        "INFO", PURPLE, VIOLET,
        [this]() { showFullMapInfo = !showFullMapInfo; }
    );
    
    gameButtons.emplace_back(
        Rectangle{screenWidth - 90*scale, 100*scale, (float)gameButtonWidth, (float)gameButtonHeight},
        "MENU", DARK_RED, RED,
        [this]() { state = AppState::MENU; }
    );

    helpButtons.clear();
    int backButtonWidth = (int)(60 * scale);
    int backButtonHeight = (int)(40 * scale);

    helpButtons.emplace_back(
        Rectangle{20*scale, 20*scale, (float)backButtonWidth, (float)backButtonHeight},
        "BACK", DARKGRAY, GRAY,
        [this]() { state = AppState::MENU; }
    );

    creditsButtons.clear();
    creditsButtons.emplace_back(
        Rectangle{20*scale, 20*scale, (float)backButtonWidth, (float)backButtonHeight},
        "BACK", DARKGRAY, GRAY,
        [this]() { state = AppState::MENU; }
    );

    settingsButtons.clear();

    settingsButtons.emplace_back(
        Rectangle{20*scale, 20*scale, (float)backButtonWidth, (float)backButtonHeight},
        "BACK", DARKGRAY, GRAY,
        [this]() { state = AppState::MENU; }
    );

    int barX = (int)(250 * scale);
    int barWidth = (int)(200 * scale);
    int volumeY = (int)(150 * scale);
    int arrowSize = (int)(25 * scale);

    settingsButtons.emplace_back(
    Rectangle{barX - arrowSize - 5*scale, (float)volumeY, (float)arrowSize, (float)arrowSize},
    "<", DARKBLUE, BLUE,
    [this]() { 
        musicVolume = std::max(0.0f, musicVolume - 0.1f);
        if (musicLoaded) SetMusicVolume(backgroundMusic, musicVolume);
        }
    );

    settingsButtons.emplace_back(
    Rectangle{barX + barWidth + 5*scale, (float)volumeY, (float)arrowSize, (float)arrowSize},
    ">", DARKBLUE, BLUE,
    [this]() { 
        musicVolume = std::min(1.0f, musicVolume + 0.1f);
        if (musicLoaded) SetMusicVolume(backgroundMusic, musicVolume);
        }
    );

    int fullscreenY = (int)(200 * scale);

    settingsButtons.emplace_back(
    Rectangle{200*scale, (float)fullscreenY, 150*scale, (float)arrowSize},
    fullscreen ? "ON" : "OFF", 
    fullscreen ? DARKGREEN : DARK_RED,
    fullscreen ? GREEN : RED,
    [this]() { 
        fullscreen = !fullscreen;
        if (fullscreen) {
            int monitorWidth = GetMonitorWidth(0);
            int monitorHeight = GetMonitorHeight(0);
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowSize(monitorWidth, monitorHeight);
            SetWindowPosition(0, 0);
        } else {
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowSize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
            int monitorWidth = GetMonitorWidth(0);
            int monitorHeight = GetMonitorHeight(0);
            SetWindowPosition(
                (monitorWidth - BASE_WINDOW_WIDTH) / 2,
                (monitorHeight - BASE_WINDOW_HEIGHT) / 2
            );
        }
        updateFullscreenScale();
        initializeButtons();
    }
);
}

void Application::updateButtons(std::vector<Button>& buttons)
{
    Vector2 mousePos = GetMousePosition();
    
    for (auto& button : buttons) {
        button.isHovered = CheckCollisionPointRec(mousePos, button.rect);
        
        if (button.isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            button.onClick();
        }
    }
}

void Application::renderButtons(const std::vector<Button>& buttons)
{
    for (const auto& button : buttons) {
        Color currentColor = button.isHovered ? button.hoverColor : button.color;
        
        DrawRectangleRec(button.rect, currentColor);
        DrawRectangleLinesEx(button.rect, 2, WHITE);
        
        float scale = getScreenScale();
        int fontSize = (int)(16 * scale);
        if (fontSize < 10) fontSize = 10;
        
        int textWidth = MeasureText(button.text.c_str(), fontSize);
        Vector2 textPos = {
            button.rect.x + (button.rect.width - textWidth) / 2,
            button.rect.y + (button.rect.height - fontSize) / 2
        };
        
        DrawText(button.text.c_str(), (int)textPos.x, (int)textPos.y, fontSize, WHITE);
    }
}

void Application::renderVictoryPopup()
{
    if (!showVictoryPopup) return;
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float scale = getScreenScale();
    
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
    
    int popupWidth = (int)(500 * scale);
    int popupHeight = (int)(300 * scale);
    int popupX = (screenWidth - popupWidth) / 2;
    int popupY = (screenHeight - popupHeight) / 2;
    
    DrawRectangle(popupX, popupY, popupWidth, popupHeight, Fade(BLACK, 0.9f));
    DrawRectangleLines(popupX, popupY, popupWidth, popupHeight, GOLD);
    DrawRectangleLines(popupX + 2, popupY + 2, popupWidth - 4, popupHeight - 4, YELLOW);
    
    int titleFont = (int)(40 * scale);
    if (titleFont < 25) titleFont = 25;
    const char* title = "GAME OVER";
    int titleWidth = MeasureText(title, titleFont);
    DrawText(title, popupX + (popupWidth - titleWidth) / 2, popupY + (int)(30 * scale), titleFont, RED);
    
    int winnerFont = (int)(30 * scale);
    if (winnerFont < 18) winnerFont = 18;
    std::string winnerMsg = "Team " + winnerTeamName + " Wins!";
    int winnerWidth = MeasureText(winnerMsg.c_str(), winnerFont);
    DrawText(winnerMsg.c_str(), popupX + (popupWidth - winnerWidth) / 2, 
             popupY + (int)(100 * scale), winnerFont, GOLD);
    
    int congratsFont = (int)(20 * scale);
    if (congratsFont < 12) congratsFont = 12;
    const char* congrats = "Congratulations!";

    int congratsWidth = MeasureText(congrats, congratsFont);
    DrawText(congrats, popupX + (popupWidth - congratsWidth) / 2, 
             popupY + (int)(150 * scale), congratsFont, WHITE);
    int buttonWidth = (int)(200 * scale);
    int buttonHeight = (int)(50 * scale);
    int buttonX = popupX + (popupWidth - buttonWidth) / 2;
    int buttonY = popupY + (int)(200 * scale);
    
    Rectangle menuButton = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, menuButton);
    Color buttonColor = isHovered ? DARKGREEN : GREEN;
    
    DrawRectangleRec(menuButton, buttonColor);
    DrawRectangleLinesEx(menuButton, 3, WHITE);
    
    int buttonFont = (int)(20 * scale);
    if (buttonFont < 12) buttonFont = 12;
    const char* buttonText = "Return to Menu";
    int buttonTextWidth = MeasureText(buttonText, buttonFont);
    DrawText(buttonText, buttonX + (buttonWidth - buttonTextWidth) / 2, 
             buttonY + (buttonHeight - buttonFont) / 2, buttonFont, WHITE);
    
    if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        showVictoryPopup = false;
        state = AppState::MENU;
        std::cout << "Returning to menu..." << std::endl;
    }
    static float animTime = 0;
    animTime += GetFrameTime();
    for (int i = 0; i < 10; ++i) {
        float x = popupX + sinf(animTime * 2 + i) * 100 + popupWidth/2;
        float y = popupY + cosf(animTime * 1.5f + i) * 50 + popupHeight/2;
        DrawCircle((int)x, (int)y, 3, Fade(GOLD, 0.5f));
    }
}

}