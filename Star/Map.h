#pragma once

#include <map>
#include <optional>

namespace Star
{
	struct Map
	{
		template<typename K, typename V>
		static std::map<K, V> Combine(
			const std::map<K, V>& map1,
			const std::map<K, V>& map2
		)
		{
			auto result = std::map<K, V>(map1);
			result.insert(map2.begin(), map2.end());
			return result;
		};

		template<typename K, typename V>
		static std::optional<std::pair<K, V>> TryFind(
			const std::map<K, V>& map,
			K key
		)
		{
			auto iterator = map.find(key);
			return (
				iterator == map.end()
					? std::nullopt
					: std::optional(iterator)
			);
		}

		template<typename K, typename V>
		static std::optional<V> TryFindValue(
			const std::map<K, V>& map,
			K key
		)
		{
			auto iterator = map.find(key);
			return (
				iterator == map.end()
					? std::nullopt
					: std::optional(iterator->second)
			);
		}
	};
}