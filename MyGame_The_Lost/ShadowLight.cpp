#include "ShadowLight.h"

ShadowLight::ShadowLight() {      }
ShadowLight::~ShadowLight() {     }

std::vector<uint64_t> ShadowLight::MakeCircle(const std::vector<bool>& mapOfLightInBool)
{
    std::vector<uint64_t> matrix(HEIGHT_MAP, 0);

    for (int y = 0; y < HEIGHT_MAP; ++y)
    {
        for (int x = 0; x < WIDTH_MAP; ++x)
        {
            if (mapOfLightInBool[x * HEIGHT_MAP + y])
            {
                matrix[y] |= (uint64_t(1) << x);
            }
        }
    }

    return matrix;
}

std::vector<GreedyQuad> ShadowLight::GreedyMeshBinaryPlane(std::vector<uint64_t>& data, int size)
{
    std::vector<GreedyQuad> res;

    for (int row = 0; row < data.size(); row++)
    {
        uint64_t y = 0;
        uint64_t height = 0;
        while (y < size)
        {
            y += CountTrailingZeros(data[row] >> (uint64_t)y);

            if (y >= size)
            {
                break;
            }

            uint64_t h = (uint64_t)CountTrailingOnes(data[row] >> (uint64_t)y);
            uint64_t hAsMask;
            hAsMask = (uint64_t(1ll) << (uint64_t)h) - 1;

            uint64_t mask = hAsMask << (uint64_t)y;
            uint64_t w = 1;
            while (row + w < size)
            {
                uint64_t nextRowH = (data[row + w] >> y) & hAsMask;
                if (nextRowH != hAsMask)
                {
                    height = nextRowH;
                    break;
                }

                data[row + w] &= ~mask;
                w++;
            }
            GreedyQuad quad;

            quad.x = row - 1;
            quad.y = y - 1;
            quad.w = w + 2;
            quad.h = h + 2;

            res.push_back(quad);
            y += h;
        }
    }

    return res;
}

std::vector<sf::Vertex> ShadowLight::CreateFromGreed(const std::vector<GreedyQuad>& quads)
{
    std::vector<sf::Vertex> res;
    const float textureSize = 25;

    int colorGray = 5;

    for (auto i : quads)
    {
        sf::Vertex topLeft;
        sf::Vertex topRight;
        sf::Vertex botRight;
        sf::Vertex botLeft;

        topLeft.position = sf::Vector2f(i.x, i.y) * textureSize;
        topLeft.texCoords = { 0, 0 };
        topLeft.color = sf::Color(255, 255, 255);

        topRight.position = sf::Vector2f(i.x + i.w, i.y) * textureSize;
        topRight.texCoords = { 0, 1 };
        topRight.color = sf::Color(255, 255, 255);

        botRight.position = sf::Vector2f(i.x + i.w, i.y + i.h) * textureSize;
        botRight.texCoords = { 1, 1 };
        botRight.color = sf::Color(255, 255, 255);

        botLeft.position = sf::Vector2f(i.x, i.y + i.h) * textureSize;
        botLeft.texCoords = { 1, 0 };
        botLeft.color = sf::Color(255, 255, 255);

        res.push_back(topLeft);
        res.push_back(topRight);
        res.push_back(botRight);
        res.push_back(botLeft);
    }

    return res;
}