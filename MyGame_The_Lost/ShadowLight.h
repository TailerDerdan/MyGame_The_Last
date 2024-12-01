#pragma once
#include "includes.h"
#include "Tile.h"
#include "Map.h"

struct GreedyQuad
{
	float x, y, w, h;
};

class ShadowLight
{
public:
	std::vector<uint32_t> MakeCircle(const std::vector<bool>& mapOfTileInBool);

	std::vector<GreedyQuad> GreedyMeshBinaryPlane(std::vector<uint32_t>& data, int size);

	std::vector<sf::Vertex> CreateFromGreed(const std::vector<GreedyQuad>& quads);

	ShadowLight();
	~ShadowLight();

private:
    template<typename T>
    int CountTrailingZeros(T n)
    {
        static_assert(std::is_integral<T>::value, "Тип должен быть целочисленным");

        if (n == 0)
        {
            return sizeof(T) * 8;
        }

        int count = 0;
        while ((n & 1) == 0)
        {
            n >>= 1;
            count++;
        }

        return count;
    }

    template<typename T>
    int CountTrailingOnes(T n)
    {
        return CountTrailingZeros(~n);
    }
};