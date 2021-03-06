//
// Created by Semyon Tikhonenko on 6/14/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MEMORY_H
#define VOCALTRAINER_MEMORY_H

#include <cstring>

namespace CppUtils {
    namespace Memory {
        template <typename T>
        void FillZero(T* array, int size) {
            std::memset(array, 0, sizeof(T) * size);
        }

        template<typename PointerContainer>
        void DeleteAll(PointerContainer& container) {
            for (auto* pointer : container) {
                delete pointer;
            }
        }
    }
}


#endif //VOCALTRAINER_MEMORY_H
