#pragma once

#include "raylib.h"

#include <vector>
#include <iostream>
#include <unordered_map>

std::vector<Vector2*> generateMeshGrid(Vector2 topLeft, Vector2 bottomRight, float spacing) {
    std::vector<Vector2*> meshGrid;
    
    // Ensure that topLeft coordinates are indeed on the top-left and bottomRight on the bottom-right
    if (topLeft.x >= bottomRight.x || topLeft.y >= bottomRight.y) {
        std::cerr << "Invalid input: topLeft must be top-left and bottomRight must be bottom-right." << std::endl;
        return meshGrid; // Empty grid
    }
    
    // Calculate the number of points in x and y directions
    int numXPoints = static_cast<int>((bottomRight.x - topLeft.x) / spacing) + 1;
    int numYPoints = static_cast<int>((bottomRight.y - topLeft.y) / spacing) + 1;
    
    std::cout << "numXPoints: " << numXPoints << ", numYPoints: " << numYPoints << std::endl;
    
    // Generate the mesh grid points
    for (int i = 0; i < numYPoints; ++i) {
        for (int j = 0; j < numXPoints; ++j) {
            Vector2* point = new Vector2;
            point->x = topLeft.x + j * spacing;
            point->y = topLeft.y + i * spacing;
            meshGrid.push_back(point);
        }
    }
    
    return meshGrid;
}

// TODO: use this for deleteEntity???
template <typename T1, typename T2>
void eraseFromMap(T1 _map, T2 _key)
{
    auto elem = _map->find(_key);
    delete elem->second;
    _map->erase(elem);
}
