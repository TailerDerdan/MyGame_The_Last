#include "Map.h"
#include "Camera.h"

void redrawFrame(sf::RenderWindow& window, Map* map, Player* player)
{
    window.clear(sf::Color(0, 0, 0));
    map->DrawMap(window);
    player->Draw(window);
    window.display();
}

int main()
{
    Camera camera;

    Map* map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetViewPosition());
    Player* player = new Player(map);

    camera.SetPlayer(player);

    while (camera.m_window.isOpen())
    {
        camera.Update();
        player->Update();
        //player->DisplayMovement();
        map->UpdateMap(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetViewPosition());
        redrawFrame(camera.m_window, map, player);
    }

    return 0;
}