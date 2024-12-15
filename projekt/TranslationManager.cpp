#include "TranslationManager.h"
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <locale>
#include <codecvt>

namespace GigaGra {

	std::unordered_map<std::wstring, std::wstring> translations{};

	void Translations::load(const std::string& langFile)
	{
		translations.clear();
		std::wifstream file(langFile);
		file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
		std::wstring line;
		while (std::getline(file, line)) {
			size_t pos = line.find(L"=");
			if (pos != std::string::npos) {
				std::wstring key = line.substr(0, pos);
				std::wstring value = line.substr(pos + 1);
				size_t pos = value.find(L"\\n");
				while (pos != std::string::npos) {
					value.replace(pos, 2, L"\n");
					pos = value.find(L"\\n");
				}
				//std::wcout << L"key: " << key << L" value: " << value << std::endl;
				translations[key] = value;
			}
		}
	}
	const std::wstring& Translations::get(const std::wstring& key)
	{
		//std::wcout << L"key: " << key << L" value: " << translations[key] << std::endl;
		if (!translations.contains(key))
			return key;
		else 
			return translations[key];
	}

}