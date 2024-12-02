#include "Camera.h"
#include "ShadowLight.h"

void redrawFrame(sf::RenderWindow& window, Map* map, Camera& camera, const sf::Shader& shadowShader, const sf::VertexArray& blocks)
{
    window.clear(sf::Color(0, 0, 0));

    camera.DrawRenderTexture(window, shadowShader);

    window.display();
    camera.castTexture.display();
    camera.renderTextureForLight.display();
}

struct Light
{
    ShadowLight light;
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

    int radius = 3;
    
    sf::Vector2f tilePlayerCoord = { floor(playerCoord.x / 25), floor(playerCoord.y / 25) };

    map->SpreadTheLight(tilePlayerCoord, radius, isFirstTimeOfSpreadLight);
    isFirstTimeOfSpreadLight = false;

    light.vec = light.light.MakeCircle(map->GetMapOfLightInBool());
    light.res = light.light.GreedyMeshBinaryPlane(light.vec, 64);
    light.vertecies = light.light.CreateFromGreed(light.res);

    light.blocks.setPrimitiveType(sf::Quads);
    light.blocks.resize(light.vertecies.size());

    for (int i = 0; i < light.vertecies.size(); i++)
    {
        light.blocks[i] = light.vertecies[i];
    }
}

int main()
{
    Light light;
    sf::Shader shadowShader;

    shadowShader.loadFromFile("light.vert", "light.frag");
    shadowShader.setUniform("resolution", sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

    Camera camera;

    sf::Texture textureOfCave;
    textureOfCave.loadFromFile("../assets/textureForCave.jpg");

    Map* map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetView(), camera.castTexture, textureOfCave);
    Player* player = new Player(map);

    sf::Vector2f mouseCoords = { 0, 0 };
    bool isMouseMove = false;
    
    sf::Clock clock;
    
    camera.SetPlayer(player);

    bool isFirstTimeOfSpreadLight = true;

    while (camera.m_window.isOpen())
    {
        float deltaTimeForMovement = clock.restart().asSeconds();

        camera.Update(mouseCoords, isMouseMove);

        map->UpdateMap(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetView(), camera.castTexture);

        player->Update(camera.castTexture, camera.GetView(), deltaTimeForMovement);

        MakeLight(light, map, player->GetPosition(), isFirstTimeOfSpreadLight);

        camera.renderTextureForLight.draw(light.blocks);

        shadowShader.setUniform("mousePosition", player->GetPosition() - camera.GetViewPosition());
        redrawFrame(camera.m_window, map, camera, shadowShader, light.blocks);

        camera.m_window.setTitle(std::to_string(1 / deltaTimeForMovement));
    }

    return 0;
}