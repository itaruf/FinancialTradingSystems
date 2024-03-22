#pragma once

#include <iostream>
#include <vector>

// Helper class for vector-centric helper functions
class VectorHelper
{
public:
    template <typename T>
    inline static void print(std::vector<T>& vec)
    {
        int size = vec.size();
        for (int i = 0; i < size; ++i)
            std::cout << vec[i] << std::endl;
    }
};