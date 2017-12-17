#pragma once

#include <memory>
#include <type_traits>
#include <vector>
#include <string>

namespace nlang {

using u64 = uint64_t;
using i64 = int64_t;
using u32 = uint32_t;
using i32 = int32_t;
using u16 = uint16_t;
using i16 = int16_t;
using u8 = uint8_t;
using i8 = int8_t;


template <typename To, typename From, typename Deleter>
std::unique_ptr<To, Deleter> dynamic_unique_cast(std::unique_ptr<From, Deleter>&& p) {
	if(To* cast = dynamic_cast<To*>(p.get())) {
		std::unique_ptr<To, Deleter> result(cast, std::move(p.get_deleter()));
		p.release();
		return result;
	}
	return std::unique_ptr<To, Deleter>(nullptr);
}


}