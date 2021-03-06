//
// Created by Semyon Tikhonenko on 10/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MAPS_H
#define VOCALTRAINER_MAPS_H

#include <vector>
#include <algorithm>
#include <optional>
#include <map>

namespace CppUtils {
    namespace Maps {
        template<typename Map, typename Value>
        void Values(const Map& map, std::vector<Value>* result) {
            result->resize(map.size());
            std::transform(map.begin(), map.end(), result->begin(), [] (const auto& pair) {
                return pair.second;
            });
        }

        template<typename Map>
        auto Values(const Map& map) {
            std::vector<decltype(map.begin()->second)> result;
            Values(map, &result);
            return result;
        }

        template <typename K, typename V>
        const std::map<K, V>& emptyMap() {
            static std::map<K, V> map;
            return map;
        }

        template <typename Map>
        std::optional<typename Map::mapped_type> GetAndRemove(Map& map, const typename Map::key_type& key) {
            auto iter = map.find(key);
            if (iter != map.end()) {
                auto result = iter->second;
                map.erase(iter);
                return result;
            } else {
                return std::nullopt;
            }
        }

        template <typename Map>
        auto GetOrDefault(const Map& map,
                decltype(map.begin()->first) key,
                decltype(map.begin()->second) defaultValue)
        {
            auto iter = map.find(key);
            if (iter != map.end()) {
                return iter->second;
            } else {
                return defaultValue;
            }
        }

        template <typename Map, typename Key>
        std::optional<typename Map::mapped_type> GetIfExists(const Map& map, const Key& key) {
            auto iter = map.find(key);
            if (iter == map.end()) {
                return std::nullopt;
            }

            return iter->second;
        }

        template <typename Map>
        typename Map::mapped_type* InsertAndGetInsertedValue(Map& map, const typename Map::key_type& key,
                const typename Map::mapped_type& value) {
            return &map.insert(std::make_pair(key, value)).first->second;
        }
    };
}

#endif //VOCALTRAINER_MAPS_H
