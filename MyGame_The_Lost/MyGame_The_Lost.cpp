#include "Camera.h"
#include "ShadowLight.h"

void redrawFrame(sf::RenderWindow& window, Map* map, Player* player, ShadowLight* shadow, const sf::View& view, sf::RenderTexture& castTexture)
{
    window.clear(sf::Color(0, 0, 0));
    map->DrawMap(window, castTexture);
    shadow->DrawTriangles(window, view);
    player->Draw(window);
    window.display();
}

int main()
{
    Camera camera;

    sf::Texture textureOfCave;
    textureOfCave.loadFromFile("../assets/textureForCave.jpg");

    Map* map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetView(), camera.castTexture, textureOfCave);
    Player* player = new Player(map);

    sf::Vector2f mouseCoords = { 0, 0 };
    bool isMouseMove = false;

    ShadowLight* shadow = new ShadowLight(map, mouseCoords, WINDOW_WIDTH, WINDOW_HEIGHT);
    shadow->SetViewForRenderTexture(camera.GetView());
    
    sf::Clock timer;
    
    camera.SetPlayer(player);

    while (camera.m_window.isOpen())
    {
        camera.Update(mouseCoords, isMouseMove);
        //std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
        player->Update();
        shadow->Update(player->GetPosition(), isMouseMove, camera.m_window);
        map->UpdateMap(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetView(), camera.castTexture);
        redrawFrame(camera.m_window, map, player, shadow, camera.GetView(), camera.castTexture);
        camera.m_window.setTitle(std::to_string(1 / timer.restart().asSeconds()));
    }

    return 0;
}