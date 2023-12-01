#pragma once

#include <bitset>
#include <cstdint>
#include <string>

namespace stardust
{
	using id_t = std::string;
	using stat_value_t = int32_t;
	using target_rule_t = std::bitset<4>;

	struct modifier_info
	{
		stat_value_t value{};
		id_t id{};
		const class ability *originator{};
	};
}
