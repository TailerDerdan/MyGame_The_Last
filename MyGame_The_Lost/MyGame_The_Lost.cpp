﻿#include "Camera.h"
#include "ShadowLight.h"
#include "Disaster.h"
#include "StartLobby.h"
#include "Erosion.h"

void redrawFrame(sf::RenderWindow& window, Map* map, Camera* camera, const sf::Shader& shadowShader, const sf::VertexArray& blocks, const sf::Shader& corosionShader)
{
    window.clear(sf::Color(0, 0, 0));

    camera->DrawRenderTexture(window, shadowShader, corosionShader);

    window.display();
    camera->castTexture.display();
    camera->renderTextureForLight.display();
    camera->renderTextureForPlayerState.display();
}

struct Light
{
    ShadowLight* light = new ShadowLight();
    std::vector<uint64_t> vec;
    std::vector<GreedyQuad> res;
    std::vector<sf::Vertex> vertecies;

    sf::VertexArray blocks;
};

void MakeLight(Light& light, Map* map, sf::Vector2f playerCoord, bool& isFirstTimeOfSpreadLight)
{
    light.vec.clear();
    light.res.clear();
    light.vertecies.clear();
    light.blocks.clear();
    
    sf::Vector2f tilePlayerCoord = { floor(playerCoord.x / 25), floor(playerCoord.y / 25) };

    map->SpreadTheLight(tilePlayerCoord, isFirstTimeOfSpreadLight);
    isFirstTimeOfSpreadLight = false;

    light.vec = light.light->MakeCircle(map->GetMapOfLightInBool());
    light.res = light.light->GreedyMeshBinaryPlane(light.vec, 64);
    light.vertecies = light.light->CreateFromGreed(light.res);

    light.blocks.setPrimitiveType(sf::Quads);
    light.blocks.resize(light.vertecies.size());

    for (int i = 0; i < light.vertecies.size(); i++)
    {
        light.blocks[i] = light.vertecies[i];
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    Light light;
    sf::Shader shadowShader;

    shadowShader.loadFromFile("light.vert", "light.frag");
    shadowShader.setUniform("resolution", sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

    Erosion erosion;
    sf::Shader corosionShader;
    corosionShader.loadFromFile("Corosion.frag", sf::Shader::Fragment);

    Camera* camera = new Camera();

    sf::Texture textureOfCave;
    textureOfCave.loadFromFile("../assets/textureForCave.png");
    sf::Texture textureOfTeam;
    textureOfTeam.loadFromFile("../assets/hole.png");
    sf::Texture textureOfFlower;
    textureOfFlower.loadFromFile("../assets/flower.png");
    sf::Texture textureOfFlowerAngry;
    textureOfFlowerAngry.loadFromFile("../assets/flower2.png");

    StartLobby start;
    EndGame* end = new EndGame();

    Map* map = new Map(camera->GetView(), camera->castTexture, textureOfCave, textureOfTeam);
    Player* player = new Player(map, end, camera->GetViewPosition(), camera->castTexture);

    sf::Vector2f mouseCoords = { 0, 0 };
    bool isMouseMove = false;
    
    sf::Clock clock;
    sf::Clock clockForFallingStone;
    
    camera->SetPlayer(player);

    bool isFirstTimeOfSpreadLight = true;

    Disaster* disasters = new Disaster(map, player, camera->GetView(), light.light);
    Flower* flower = new Flower(map, textureOfFlower, textureOfFlowerAngry);

    while (camera->m_window.isOpen())
    {
        if (player->GetElapsedTimeAfterStartLevel() <= 3.0f)
        {
            player->SetIsNextLevel(false);
            continue;
        }
        /*if (!start.GetStateDialogue())
        {
            start.DrawDialouge(camera->m_window);
            continue;
        }*/

        if (end->GetStateDialogue())
        {
            end->DrawDialouge(camera->m_window);
            continue;
        }

        //disasters->MakeRandomDisaster(player->GetPosition(), player->GetDirectionOfMovement());
        float deltaTimeForMovement = clock.restart().asSeconds();

        camera->Update(mouseCoords, isMouseMove, disasters);
        camera->SetPlayerCoordsBeforeMove(player->GetPosition());
        map->MoveWater();

        map->UpdateMap(camera->GetView(), camera->castTexture);
        flower->DrawFlowers(camera->castTexture);

        player->Update(camera->castTexture, camera->renderTextureForPlayerState, camera->GetView(), deltaTimeForMovement, camera->m_window, flower);

        if (player->GetIsNextLevel())
        {
            continue;
        }
        camera->SetPlayerCoordsAfterMove(player->GetPosition());
        camera->UpdatePostionCamera();

        MakeLight(light, map, player->GetPosition(), isFirstTimeOfSpreadLight);

        camera->renderTextureForLight.draw(light.blocks);

        disasters->FallingStone(deltaTimeForMovement, camera->m_window);
        disasters->Shake(deltaTimeForMovement, camera->m_window);
        disasters->DoTurningOnTheLight();

        sf::Vector2f mousePos = sf::Vector2f{ (float)sf::Mouse::getPosition().x, (float)(sf::Mouse::getPosition().y) };
        mousePos.x /= 1920;
        mousePos.y /= 1080;

        corosionShader.setUniform("erosion", erosion.noiseTexture);
        corosionShader.setUniform("secondPoint", sf::Vector2f{ 0.5f, 0.5f });
        corosionShader.setUniform("firstPoint", player->GetFirstCoordForCorosion());
        corosionShader.setUniform("erosion_max_offset", 1.0f);

        shadowShader.setUniform("mousePosition", player->GetPosition() - camera->GetViewPosition());
        redrawFrame(camera->m_window, map, camera, shadowShader, light.blocks, corosionShader);

        camera->m_window.setTitle(std::to_string(1 / deltaTimeForMovement));
    }

    return 0;
}