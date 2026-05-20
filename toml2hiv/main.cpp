#include <filesystem>
#include <string>
#include <vector>

#include <windows.h>
#include <offreg.h>

#include "toml.hpp"

DWORD CreateKeyRecursive(ORHKEY root, const std::wstring& path, ORHKEY* out)
{
	ORHKEY current = root;
	ORHKEY next = nullptr;

	size_t start = 0;

	while (start < path.size())
	{
		size_t pos = path.find(L'\\', start);
		std::wstring part = (pos == std::wstring::npos) ? path.substr(start) : path.substr(start, pos - start);

		DWORD err = ORCreateKey(current, part.c_str(), nullptr, REG_OPTION_NON_VOLATILE, nullptr, &next, nullptr);
		if (err != ERROR_SUCCESS)
			return err;

		if (current != root)
			ORCloseKey(current);

		if (pos == std::wstring::npos)
			break;

		current = next;
		start = pos + 1;
	}

	*out = current;
	return ERROR_SUCCESS;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("toml2hiv <toml> <hive>\n");
		return ERROR_INVALID_PARAMETER;
	}

	ORHKEY root = nullptr;
	auto err = ORCreateHive(&root);
	if (err != ERROR_SUCCESS)
	{
		printf("ORCreateHive failed: Win32 err %d\n", err);
		return err;
	}

	auto t = toml::parse_file(argv[1]);
	for (const auto& kv : t)
	{
		if (kv.second.is_table())
		{
			auto& key = kv.first;
			auto& values = kv.second;

			auto wkey = std::wstring(key.begin(), key.end());
			printf("[%ls]\n", wkey.c_str());

			ORHKEY keyHandle = nullptr;
			err = CreateKeyRecursive(root, wkey, &keyHandle);
			if (err != ERROR_SUCCESS)
			{
				printf("ORCreateKey for %ls failed: Win32 error %d\n\n", wkey.c_str(), err);
				continue;
			}

			for (const auto& val : *values.as_table())
			{
			
			}

			printf("\n");

			ORCloseKey(keyHandle);
		}
	}

	auto out = std::wstring(argv[2], argv[2] + strlen(argv[2]));
	err = ORSaveHive(root, out.c_str(), 6, 1);
	if (err != ERROR_SUCCESS)
	{
		printf("ORSaveHive to %ls failed: Win32 error %d\n", out.c_str(), err);
		return err;
	}

	return ERROR_SUCCESS;
}
