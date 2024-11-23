#include "Camera.h"
#include "ShadowLight.h"

void redrawFrame(sf::RenderWindow& window, Map* map, Player* player, ShadowLight* shadow)
{
    window.clear(sf::Color(0, 0, 0));
    map->DrawMap(window);
    player->Draw(window);
    shadow->DrawLines(window);
    shadow->DrawTriangles(window);
    window.display();
}

int main()
{
    Camera camera;

    Map* map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetViewPosition());
    Player* player = new Player(map);

    sf::Vector2f mouseCoords = { 0, 0 };
    bool isMouseMove = false;

    ShadowLight* shadow = new ShadowLight(map, mouseCoords);

    
    camera.SetPlayer(player);

    while (camera.m_window.isOpen())
    {
        camera.Update(mouseCoords, isMouseMove);
        //std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
        player->Update();
        shadow->Update(mouseCoords, isMouseMove);
        map->UpdateMap(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetViewPosition());
        redrawFrame(camera.m_window, map, player, shadow);
    }

    return 0;
}