#include "Engine/2D/SpriteDatabase.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"

const bool SpriteResourceSorter::operator==(const SpriteResourceSorter& other) const
{
	if (other.tex == tex && other.spriteID == spriteID)
	{
		return true;
	}
	return false;
}
const bool SpriteResourceSorter::operator<(const SpriteResourceSorter& other) const
{
	if (tex < other.tex && spriteID < other.spriteID)
	{
		return true;
	}
	return false;
}


//operators
const bool SpriteAnimationSorter::operator==(const SpriteAnimationSorter& other) const
{
	if (other.m_spriteAnimationID == other.m_spriteAnimationID && SimpleStrCmp(m_spriteAnimationTag, other.m_spriteAnimationTag))
	{
		return true;
	}
	return false;
}

const bool SpriteAnimationSorter::operator<(const SpriteAnimationSorter& other) const
{
	if (other.m_spriteAnimationID < other.m_spriteAnimationID)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Constructors
size_t SpriteDatabase::availableSpriteResourceID = 0;
size_t SpriteDatabase::availableSpriteAnimationID = 0;
SpriteDatabase* SpriteDatabase::g_SpriteDatabase = nullptr;

const std::string SpriteDatabase::s_CommonSpriteDatabaseSaveLocation = "Data/SpriteResourcesSaves/SpriteResources.Save.xml";

SpriteDatabase::SpriteDatabase()
{
}
SpriteDatabase::~SpriteDatabase()
{
	ClearDatabase();
}



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Register
const bool SpriteDatabase::RegisterNewSprite(Texture* spriteSheet, const size_t& spriteID, SpriteResource* resource)
{
	//NOTE: This method does NOT allow for editting already existing sprite resources.
	if (resource == nullptr)
	{
		return false;
	}
	bool found1 = false;
	bool found = false;
	
	//Performs two checks, one to be sure wanted id doesn't already exist, another to ensure the resource does not already exist.
	//size_t spriteResourceID1 = GetSpriteResourceIDForSpriteSheetAndSpriteID(resource->GetSpriteSheet(), resource->GetSpriteResourceID(), found);
	//size_t spriteResourceID = GetSpriteResourceIDForSpriteSheetAndSpriteID(spriteSheet, spriteID, found);

	if (found == true || found1 == true)
	{
		//SpriteResource already exists!
		return false;
	}
	else
	{
		SpriteResourceSorter sorter;
		sorter.tex = spriteSheet;
		sorter.spriteID = spriteID;
		sorter.spriteResourceID = availableSpriteResourceID;
		availableSpriteResourceID++;
		resource->SetSpriteResourceID(spriteID);
		resource->SetSpriteSheet(spriteSheet);

		m_resourceIDs.push_back(sorter);
		m_spriteResources.push_back(resource);
		return true;
	}

}

const bool SpriteDatabase::RegisterNewAnimation(SpriteAnimationSequence* sequence, std::string& spriteTag)
{
	if (sequence == nullptr)
	{
		return false;
	}

	bool foundIt = false;
	GetSpriteAnimationIdxForTag(spriteTag, foundIt);
	if (foundIt)
	{
		return false;
	}
	else
	{
		SpriteAnimationSorter sorter;
		sorter.m_spriteAnimationID = availableSpriteAnimationID;
		availableSpriteAnimationID++;
		sorter.m_spriteAnimationTag = spriteTag;

		m_animationIDs.push_back(sorter);
		m_spriteAnimations.push_back(sequence);
		return true;
	}

}

void SpriteDatabase::ClearDatabase()
{
	for (size_t i = 0; i < m_spriteResources.size(); i++)
	{
		if (m_spriteResources.at(i) != nullptr)
		{
			delete m_spriteResources[i];
			m_spriteResources[i] = nullptr;
		}
	}
	m_spriteResources.clear();
	m_resourceIDs.clear();
	availableSpriteResourceID = 0;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//Getters
SpriteResource* SpriteDatabase::EditSprite(const size_t& spriteResourceID)
{
	if (spriteResourceID >= m_spriteResources.size())
	{
		return nullptr;
	}
	return m_spriteResources.at(spriteResourceID);
}
const SpriteResource* SpriteDatabase::GetSprite(const size_t& spriteResourceID)
{
	if (spriteResourceID >= m_spriteResources.size())
	{
		return nullptr;
	}
	return m_spriteResources.at(spriteResourceID);
}
const size_t SpriteDatabase::GetSpriteResourceIdxForSpriteSheetAndSpriteID(Texture* spriteSheet, const size_t& spriteID, bool& foundIt)
{
	for (size_t i = 0; i < m_resourceIDs.size(); i++)
	{
		if (m_resourceIDs.at(i).tex == spriteSheet && m_resourceIDs.at(i).spriteID == spriteID)
		{
			foundIt = true;
			return i;
		}
	}
	foundIt = false;
	return 0;
}
const size_t SpriteDatabase::GetSpriteResourceIDForSpriteSheetAndSpriteID(Texture* spriteSheet, const size_t& spriteID, bool& foundIt)
{
	if (spriteSheet == nullptr)
	{
		foundIt = false;
		return 0;
	}

	for (size_t i = 0; i < m_resourceIDs.size(); i++)
	{
		if (m_resourceIDs.at(i).tex == spriteSheet && m_resourceIDs.at(i).spriteID == spriteID)
		{
			foundIt = true;
			return m_resourceIDs.at(i).spriteResourceID;
		}
	}
	foundIt = false;
	return 0;
}

const size_t SpriteDatabase::GetNextAvailableSpriteID(Texture* spriteSheet)
{
	if (spriteSheet == nullptr)
	{
		return 0;
	}
	std::vector<size_t> spriteIDs;
	size_t availableID = 0;
	for (size_t resourceIDIdx = 0; resourceIDIdx < m_resourceIDs.size(); resourceIDIdx++)
	{
		if (m_resourceIDs[resourceIDIdx].tex == spriteSheet)
		{
			if (availableID == m_resourceIDs[resourceIDIdx].spriteID)
			{
				availableID++;
				for (size_t i = 0; i < spriteIDs.size(); i++)
				{
					size_t id = spriteIDs[i];
					if (id == availableID)
					{
						availableID++;
						i = 0;
					}
					else if (id < availableID)
					{
						spriteIDs.erase(spriteIDs.begin() + i);
						i--;
					}
				}
			}
			else if (availableID < m_resourceIDs[resourceIDIdx].spriteID)
			{
				spriteIDs.push_back(m_resourceIDs[resourceIDIdx].spriteID);
			}
		}
	}
	return availableID;
}


const size_t SpriteDatabase::GetSpriteAnimationIdxForTag(std::string& tag, bool& foundIt)
{
	for (size_t i = 0; i < m_animationIDs.size(); i++)
	{
		if (SimpleStrCmp(m_animationIDs.at(i).m_spriteAnimationTag, tag))
		{
			foundIt = true;
			return m_animationIDs.at(i).m_spriteAnimationID;
		}
	}
	foundIt = false;
	return 0;
}


const bool SpriteDatabase::FindIfSpriteSheetAndIDAlreadyExists(Texture* spriteSheet, const size_t& spriteID)
{
	for (size_t i = 0; i < m_resourceIDs.size(); i++)
	{
		if (m_resourceIDs.at(i).spriteID == spriteID && m_resourceIDs.at(i).tex == spriteSheet)
		{
			return true;
		}
	}
	return false;
}


void SpriteDatabase::SaveOutAllSpriteResourcesToXML(const std::string& xmlFileLocation)
{

	XMLNode SpriteResources = XMLNode::createXMLTopNode("SpriteResources");
	s_textureRegistry;
	XMLNode textureNode = SpriteResources.addChild("textureLocations");
	for (size_t i = 0; i < m_resourceIDs.size(); i++)
	{
		if (SimpleStrCmp(m_resourceIDs.at(i).tex->m_imageFilePath, "") == false && m_resourceIDs.at(i).tex != nullptr)
		{
			XMLNode location = textureNode.addChild("textureLocation");
			location.addAttribute("texLocation", m_resourceIDs.at(i).tex->m_imageFilePath.c_str());
			location.addAttribute("spriteID", std::to_string(m_resourceIDs.at(i).spriteID).c_str());
			location.addAttribute("spriteResourceID", std::to_string(m_resourceIDs.at(i).spriteResourceID).c_str());
		}
	}
	for (size_t i = 0; i <  m_spriteResources.size(); i++)
	{
		SpriteResource* resource = m_spriteResources.at(i);
		if (resource != nullptr)
		{
			Texture* tex = resource->GetSpriteSheet();
			if (tex == nullptr)
			{
				continue;
			}
			AABB2 spriteCoords = resource->GetTextureCoords();
			std::string coordsAsString = std::to_string(spriteCoords.mins.x) + "," + std::to_string(spriteCoords.mins.y)
				+ "," + std::to_string(spriteCoords.maxs.x) + "," + std::to_string(spriteCoords.maxs.y);
			size_t resourceId = resource->GetSpriteResourceID();
			Vector2 virtualCoordSize = resource->GetVirtualSpaceSize();
			std::string coordSizeAsString = std::to_string(virtualCoordSize.x) + "," + std::to_string(virtualCoordSize.y);
			const Material* mat = resource->GetMaterial();
			ShaderProgram* prog =nullptr;

			XMLNode resourceNode = SpriteResources.addChild("SpriteResource");

			resourceNode.addAttribute("SpriteCoords", coordsAsString.c_str());
			resourceNode.addAttribute("imageFilePath", tex->m_imageFilePath.c_str());
			resourceNode.addAttribute("ResourceID", std::to_string(resourceId).c_str());
			resourceNode.addAttribute("VirtualSize", coordSizeAsString.c_str());
			if (mat != nullptr)
			{
				prog = mat->GetProgram();

			}
			if (prog != nullptr)
			{
				ShaderFileLocations fileLocations = prog->GetShaderFileLocations();
				resourceNode.addAttribute("VertexShader", fileLocations.vertShaderLocation.c_str());
				resourceNode.addAttribute("FragmentShader", fileLocations.fragShaderLocation.c_str());
			}

		}
	}



	SpriteResources.writeToFile(xmlFileLocation.c_str());
}
const bool SpriteDatabase::TryLoadAllSpriteResourcesFromXML(const std::string xmlFileLocation)
{
	bool couldNotLoad = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(xmlFileLocation, "SpriteResources", couldNotLoad);
	if (couldNotLoad == true)
	{
		return false;
	}

	for (int rootChildIdx = 0; rootChildIdx < root.nChildNode(); rootChildIdx++)
	{
		XMLNode childNode = root.getChildNode(rootChildIdx);
		std::string lowerCaseName = childNode.getName();
		lowerCaseName = MakeLower(lowerCaseName);
		if (SimpleStrCmp(lowerCaseName, "texturelocations") == true)
		{
			for (int texIdx = 0; texIdx < childNode.nChildNode(); texIdx++)
			{
				XMLNode texNode = childNode.getChildNode(texIdx);
				SpriteResourceSorter sorter;
				for (int attrIdx = 0; attrIdx < texNode.nAttribute(); attrIdx++)
				{
					XMLAttribute attr = texNode.getAttribute(attrIdx);
					std::string lowerCase = attr.lpszName;
					lowerCase = MakeLower(lowerCase);
					if (SimpleStrCmp(lowerCase.c_str(), "texlocation") == true)
					{
						Texture* tex = Texture::CreateOrGetTexture(attr.lpszValue);
						sorter.tex = nullptr;
						sorter.tex = tex;
					}
					else if (SimpleStrCmp(lowerCase.c_str(), "spriteid") == true)
					{
						sorter.spriteID = atoi(attr.lpszValue);
					}
					else if (SimpleStrCmp(lowerCase.c_str(), "spriteresourceid") == true)
					{
						sorter.spriteResourceID = atoi(attr.lpszValue);
					}
				}
				m_resourceIDs.push_back(sorter);
			}
		}
		else if (SimpleStrCmp(lowerCaseName, "spriteresource") == true)
		{
			std::string vert = "";
			std::string frag = "";
			Texture* tex = nullptr;
			Vector2 virtualSize = Vector2::vec2_ones;
			AABB2 spriteCoords;
			size_t resourceID = 0;
			SpriteResource* resource = nullptr;
			for (int childIdx = 0; childIdx < childNode.nAttribute(); childIdx++)
			{
				XMLAttribute attr = childNode.getAttribute(childIdx);
				std::string lowerCase = attr.lpszName;
				lowerCase = MakeLower(lowerCase);

				if (SimpleStrCmp(lowerCase.c_str(), "spritecoords") == true)
				{
					std::string val = ReplaceCharInString(std::string(attr.lpszValue), ',', ' ');
					std::vector<std::string> parsed = ParseString(val);
					if (parsed.size() > 0)
					{
						spriteCoords.mins.x = (float)atof(parsed.at(0).c_str());
					}
					if (parsed.size() > 1)
					{
						spriteCoords.mins.y = (float)atof(parsed.at(1).c_str());
					}
					if (parsed.size() > 2)
					{
						spriteCoords.maxs.x = (float)atof(parsed.at(2).c_str());
					}
					if (parsed.size() > 3)
					{
						spriteCoords.maxs.y = (float)atof(parsed.at(3).c_str());
					}
				}
				else if (SimpleStrCmp(lowerCase.c_str(), "spriteresourceid") == true)
				{
					resourceID = atoi(attr.lpszValue);
				}
				else if (SimpleStrCmp(lowerCase.c_str(), "virtualsize") == true)
				{
					std::string val = ReplaceCharInString(std::string(attr.lpszValue), ',', ' ');
					std::vector<std::string> parsed = ParseString(val);
					if (parsed.size() > 0)
					{
						virtualSize.x = (float)atof(parsed.at(0).c_str());
					}
					if (parsed.size() > 1)
					{
						virtualSize.y = (float)atof(parsed.at(1).c_str());
					}
				}
				else if (SimpleStrCmp(lowerCase.c_str(), "imagefilepath") == true)
				{
					tex = Texture::CreateOrGetTexture(attr.lpszValue);
				}
				else if (SimpleStrCmp(lowerCase.c_str(), "vertexshader"))
				{
					vert = std::string(attr.lpszValue);
				}
				else if (SimpleStrCmp(lowerCase.c_str(), "fragmentshader"))
				{
					frag = std::string(attr.lpszValue);
				}
			}
			ShaderProgram* prog = nullptr;
			if (SimpleStrCmp(vert, "") == false && SimpleStrCmp(frag, "") == false)
			{
				prog = ShaderProgram::CreateOrGetShaderProgram(vert, frag);
			}
			resource = new SpriteResource();
			resource->SetSpriteCoords(spriteCoords);
			resource->SetSpriteResourceID(m_spriteResources.size());
			resource->SetSpriteSheet(tex);
			resource->SetVirtualSpaceSize(virtualSize);
			Material* mat = resource->GetEditableMaterial();
			for (size_t resourceIdx = 0; resourceIdx < m_resourceIDs.size(); resourceIdx++)
			{
				if (tex == m_resourceIDs.at(resourceIdx).tex)
				{
					m_resourceIDs.at(resourceIdx).spriteResourceID = m_spriteResources.size();
				}
			}

			if (prog != nullptr)
			{
				mat->SetProgram(prog);
			}
			m_spriteResources.push_back(resource);
		}
	}
	return true;
}