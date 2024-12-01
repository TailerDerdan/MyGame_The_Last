#include "ShadowLight.h"

ShadowLight::ShadowLight() {      }
ShadowLight::~ShadowLight() {     }

std::vector<uint32_t> ShadowLight::MakeCircle(const std::vector<bool>& mapOfLightInBool)
{
    std::vector<uint32_t> matrix(200, 0);

    for (int y = 0; y < 200; ++y)
    {
        for (int x = 0; x < 154; ++x)
        {
            if (mapOfLightInBool[x * 200 + y])
            {
                matrix[y] |= (1 << x);
            }
        }
    }

    return matrix;
}

std::vector<GreedyQuad> ShadowLight::GreedyMeshBinaryPlane(std::vector<uint32_t>& data, int size)
{
    std::vector<GreedyQuad> res;

    for (int row = 0; row < data.size(); row++)
    {
        int y = 0;
        uint32_t height = 0;
        while (y < size)
        {
            y += CountTrailingZeros(data[row] >> (int)y);

            if (y >= size)
            {
                break;
            }

            int h = (int)CountTrailingOnes(data[row] >> (int)y);
            uint32_t hAsMask;
            hAsMask = (1u << (int)h) - 1;

            uint32_t mask = hAsMask << (int)y;
            int w = 1;
            while (row + w < size)
            {
                uint32_t nextRowH = (data[row + w] >> y) & hAsMask;
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
        topLeft.color = sf::Color(255, 255, 255);

        botRight.position = sf::Vector2f(i.x + i.w, i.y + i.h) * textureSize;
        topLeft.color = sf::Color(255, 255, 255);
        botRight.texCoords = { 1, 1 };

        botLeft.position = sf::Vector2f(i.x, i.y + i.h) * textureSize;
        botLeft.texCoords = { 1, 0 };
        topLeft.color = sf::Color(255, 255, 255);

        res.push_back(topLeft);
        res.push_back(topRight);
        res.push_back(botRight);
        res.push_back(botLeft);
    }

    return res;
}