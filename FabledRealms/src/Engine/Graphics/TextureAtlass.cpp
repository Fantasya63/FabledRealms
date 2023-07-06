#include "frpch.h"
#include "TextureAtlass.h"


#include <stb_image.h>
#include <stb_image_write.h>


static const std::string elementsPath = "Assets/Textures/block/";

#define AddElements(name, path) map[name] = { (int)map.size(), elementsPath + name}


const std::unordered_map<std::string, TextureAtlass::ElementProps> TextureAtlass::CreateElementsData()
{
	std::unordered_map<std::string, ElementProps> map;

	//Add Textures
	AddElements("bedrock");
	AddElements("stone");
	AddElements("dirt");
	AddElements("sand");
	AddElements("oak_log");
	AddElements("oak_log_top");
	AddElements("grass_block_side");
	AddElements("grass_block_top");
	AddElements("oak_planks");
	AddElements("oak_leaves");
	AddElements("black_wool");
	AddElements("orange_wool");
	AddElements("white_wool");
	AddElements("red_wool");
	AddElements("yellow_wool");
	AddElements("bricks");
    AddElements("gold_block");
    AddElements("gold_ore");
    AddElements("white_concrete");

	return map;
}

const std::unordered_map<std::string, TextureAtlass::ElementProps> TextureAtlass::m_ElementsData = TextureAtlass::CreateElementsData();

int TextureAtlass::TextureAtlasSizeX = glm::sqrt(m_ElementsData.size());
int TextureAtlass::TextureAtlasSizeY = glm::ceil(static_cast<float>(m_ElementsData.size()) / TextureAtlasSizeX);






void TextureAtlass::CreateAtlasFile(const std::string atlasFilename, const std::string prefix, const std::string extentsion)
{
    stbi_set_flip_vertically_on_load(true);

    int numImages = m_ElementsData.size();
    
    // Calculate the number of rows and columns in the texture atlas
    int numColumns = glm::sqrt(numImages);
    int numRows = std::ceil(static_cast<float>(numImages) / numColumns);


    const int ElementSize = 16;
    int atlasWidth = ElementSize * numColumns;
    int atlasHeight = ElementSize * numRows;

    int channelCount = 4;  // RGBA channels





    // Create a buffer for the texture atlas
    std::vector<stbi_uc> atlasData(atlasWidth * atlasHeight * channelCount);

    for (auto& [name, element] : m_ElementsData)
    {
        int i = element.index;

        int width, height;
        std::string imagePath = element.path + prefix + extentsion;

        // Load the image using stb_image library
        stbi_uc* imageData = stbi_load(imagePath.c_str(), &width, &height, nullptr, channelCount);
        if (imageData == nullptr) {
            std::cerr << "Failed to load image: " << imagePath << std::endl;
            continue;
        }

        // Calculate the position of the image in the texture atlas
        int column = i % numColumns;
        int row = i / numColumns;

        int xOffset = column * ElementSize;
        int yOffset = row * ElementSize;

        // Copy the image data into the texture atlas
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int atlasX = xOffset + x;
                int atlasY = yOffset + y;

                for (int c = 0; c < channelCount; ++c) {
                    atlasData[(atlasY * atlasWidth + atlasX) * channelCount + c] = imageData[(y * width + x) * channelCount + c];
                }
            }
        }

        // Free the image data
        stbi_image_free(imageData);
    }

    // Save the texture atlas to a file using stb_image_write library
    if (!stbi_write_png((atlasFilename + prefix + extentsion).c_str(), atlasWidth, atlasHeight, channelCount, atlasData.data(), atlasWidth * channelCount)) {
        std::cerr << "Failed to save texture atlas to file: " << atlasFilename << prefix << std::endl;
        return;
    }

    // Printing some information
    DLOG_CORE_INFO("Texture atlas created with " << numImages << " images.");
    DLOG_CORE_INFO("Atlas size: " << atlasWidth << "x" << atlasHeight);
    DLOG_CORE_INFO("Number of rows: " << numRows << ", Number of columns: " << numColumns);
    DLOG_CORE_INFO("Texture atlas saved to file: " << atlasFilename);
}

void TextureAtlass::Init()
{
   std::string atlasFilename = "Assets/Textures/atlas/terrain";
   CreateAtlasFile(atlasFilename, "", ".png");
   CreateAtlasFile(atlasFilename, "_n", ".png");
   CreateAtlasFile(atlasFilename, "_s", ".png");
}

int TextureAtlass::GetElementIndex(const std::string& name)
{
	FR_CORE_ASSERT(m_ElementsData.find(name) != m_ElementsData.end(), name << " does not exist in the texture atlass");
	return m_ElementsData.at(name).index;
} 
