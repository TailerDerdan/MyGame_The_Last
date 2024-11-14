#include "Map.h"
#include "Camera.h"

void redrawFrame(sf::RenderWindow& window, Map* map)
{
    window.clear(sf::Color(0, 0, 0));
    map->DrawMap(window);
    window.display();
}

int main()
{
    Camera camera;

    Map* map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetViewPosition());

    while (camera.m_window.isOpen())
    {
        camera.Update();
        //map->UpdateMap(WINDOW_WIDTH, WINDOW_HEIGHT, camera.GetViewPosition());
        redrawFrame(camera.m_window, map);
    }

    return 0;
}