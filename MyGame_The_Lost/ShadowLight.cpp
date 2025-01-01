#include "ShadowLight.h"

ShadowLight::ShadowLight() 
{
    isLightWork = true;
}

ShadowLight::~ShadowLight() {     }

std::vector<uint64_t> ShadowLight::MakeLightInUint(const std::vector<bool>& mapOfLightInBool, sf::Vector2f borderForY)
{
    std::vector<uint64_t> matrix(HEIGHT_MAP, 0);

    if (!isLightWork) return matrix;

    int iter = 0;

    for (int y = 0; y < HEIGHT_MAP; y++)
    {
        for (int x = borderForY.x, iter = 0; x < borderForY.y; x++, iter++)
        {
            if (x >= WIDTH_MAP) continue;

            if (mapOfLightInBool[x * HEIGHT_MAP + y])
            {
                matrix[y] |= (uint64_t(1) << iter);
            }
        }
    }

    return matrix;
}

void ShadowLight::ChangeWorkingLight()
{
    isLightWork = !isLightWork;
}

std::vector<GreedyQuad> ShadowLight::GreedyMeshBinaryPlane(std::vector<uint64_t>& data, int size, sf::Vector2f borderForY)
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
            quad.y += borderForY.x;

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

    int alpha = 255;

    for (auto i : quads)
    {
        sf::Vertex topLeft;
        sf::Vertex topRight;
        sf::Vertex botRight;
        sf::Vertex botLeft;

        topLeft.position = sf::Vector2f(i.x, i.y) * textureSize;
        topLeft.texCoords = { 0, 0 };
        topLeft.color = sf::Color(255, 255, 255, alpha);

        topRight.position = sf::Vector2f(i.x + i.w, i.y) * textureSize;
        topRight.texCoords = { 0, 1 };
        topRight.color = sf::Color(255, 255, 255, alpha);

        botRight.position = sf::Vector2f(i.x + i.w, i.y + i.h) * textureSize;
        botRight.texCoords = { 1, 1 };
        botRight.color = sf::Color(255, 255, 255, alpha);

        botLeft.position = sf::Vector2f(i.x, i.y + i.h) * textureSize;
        botLeft.texCoords = { 1, 0 };
        botLeft.color = sf::Color(255, 255, 255, alpha);
        

        res.push_back(topLeft);
        res.push_back(topRight);
        res.push_back(botRight);
        res.push_back(botLeft);
    }

    return res;
}

sf::VertexArray ShadowLight::CreateAllVertexFromGreed(const std::vector<bool>& mapOfLightInBool)
{
    sf::Vector2f borderForX = { 0, 63 };

    sf::VertexArray blocks;
    blocks.setPrimitiveType(sf::Quads);

    while (borderForX.x < WIDTH_MAP)
    {
        std::vector<uint64_t> vec = MakeLightInUint(mapOfLightInBool, borderForX);
        std::vector<GreedyQuad> res = GreedyMeshBinaryPlane(vec, 64, borderForX);
        std::vector<sf::Vertex> vertecies = CreateFromGreed(res);

        size_t prevSizeBlocks = blocks.getVertexCount();

        blocks.resize(vertecies.size() + blocks.getVertexCount());

        int iter = 0;
        int iterEnd = 0;
        if (prevSizeBlocks == 0)
        {
            iter = 0;
            iterEnd = vertecies.size();
        }
        else
        {
            iter = prevSizeBlocks;
            iterEnd = vertecies.size() + prevSizeBlocks;
        }

        int iterForVertecies = 0;

        for (int i = iter, iterForVertecies = 0; i < iterEnd; i++, iterForVertecies++)
        {
            blocks[i] = vertecies[iterForVertecies];
        }

        borderForX.x += 64;
        borderForX.y += 64;
    }

    return blocks;
}