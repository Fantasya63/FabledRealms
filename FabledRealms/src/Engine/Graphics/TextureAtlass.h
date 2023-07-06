#pragma once
#include <unordered_map>

class TextureAtlass
{
public:
	static void Init();

	struct ElementProps {
		int index;
		std::string path;
	};

	static int GetElementIndex(const std::string& name);

	static int TextureAtlasSizeX;
	static int TextureAtlasSizeY;

private:
	static void CreateAtlasFile(const std::string atlasFilename, const std::string prefix, const std::string extentsion);
	const static std::unordered_map<std::string, ElementProps> CreateElementsData();

private:
	const static std::unordered_map<std::string, ElementProps> m_ElementsData;
};

