#pragma once
#include <iostream>

namespace GigaGra {
#define T(s) Translations::get(s)
#define T_(s) T(s).c_str()
#define TF(s, ...) std::vformat(T(s), std::make_wformat_args(__VA_ARGS__))
	namespace Translations {
		void load(const std::string& langFile);
		const std::wstring& get(const std::wstring& key);
	}
}