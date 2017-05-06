#include "Engine/Font/Font.hpp"
#include <fstream>
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/StringUtils.hpp"

//---------------------------------------------------------------------------
STATIC std::map< std::string, Font* >* s_fontRegistry = nullptr;
const std::string Font::allLetters = 
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()/\\?><.,|][{}\";:";

Glyph::Glyph()
	: id(0),
	pixelStartPosition(0.f,0.f),
	size(0.f,0.f),
	offset(0.f,0.f),
	xAdvancement(0.f),
	pageNum(0),
	chnl(0)
{
}

const AABB2 Glyph::GetPixelCoords() const
{
	AABB2 spriteCoords;
	spriteCoords.mins = pixelStartPosition;
	spriteCoords.maxs = pixelStartPosition + size;
	return spriteCoords;
}

Kerning::Kerning()
	: firstId(0),
	secondId(0),
	cursorAdjustment(0)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Constructors
Font::Font(const std::string& fileDirectoryAllAreIn, const std::string& fontFnt, bool isFNTFileLoc)
{
	/*
	info face="Arial" size=64 bold=0 italic=0 charset="ANSI" unicode=0 stretchH=100 smooth=1 aa=1 padding=1,1,1,1 spacing=1,1 outline=0
	common lineHeight=63 base=51 scaleW=1024 scaleH=1024 pages=1 packed=0 alphaChnl=1 redChnl=0 greenChnl=0 blueChnl=0
	page id=0 file="DebugFont_0.png"
	chars count=216
	char id=32   x=1018  y=145   width=5     height=3     xoffset=-2    yoffset=61    xadvance=15    page=0  chnl=15
	....
	kernings count=119
	kerning first=32  second=65  amount=-3
	...
	*/

	if (isFNTFileLoc)
	{
		std::string ftnPage;
		bool opennedSuccessfully = 
			FileUtils::LoadTextFileToBuffer((fileDirectoryAllAreIn + fontFnt), ftnPage);

		if (opennedSuccessfully == false)
		{
			return;
		}

		std::string singleWord;
		for (size_t i = 0; i < ftnPage.length(); i++)
		{
			unsigned char at = ftnPage.at(i);
			if (at != ' ' && at != '\n')
			{
				singleWord += at;
			}
			else
			{
				if (singleWord.substr(0, 5) == "face=")
				{
					m_fontName = singleWord.substr(6, singleWord.length() - 7);
				}
				else if (singleWord.substr(0,11) == "lineHeight=")
				{
					m_lineHeight = (unsigned int)atoi(singleWord.substr(11, singleWord.length() - 11).c_str());
				}
				else if (singleWord.substr(0, 5) != "pages" && singleWord.substr(0, 4) == "page")
				{
					int pageId = -1;
					m_fontPages.push_back(LoadPageData(ftnPage, i, pageId, fileDirectoryAllAreIn));
				}
				else if (singleWord.substr(0, 5) != "chars" && singleWord.substr(0, 4) == "char")
				{
					m_glyphs.push_back(LoadGlyphData(ftnPage, i));
				}
				else if (singleWord.substr(0, 7) == "kerning")
				{
					m_kernings.push_back(LoadKerningData(ftnPage, i));
				}
				singleWord = "";
			}
		}
	}
	else
	{
		/*
				std::string dir = fileDirectory;
				std::string imageLocation = dir + singleWord;
				newTex = Texture::CreateOrGetTexture(imageLocation);
		*/
		std::string assumedImage = fileDirectoryAllAreIn + fontFnt;
		Texture* text = Texture::CreateOrGetTexture(assumedImage);
		if (text != nullptr)
		{
			m_fontPages.push_back(text);

		}
	}
	m_spaceGlyphIdx = FindGlyphIdx(' ');
	m_spacePageNum = m_glyphs[m_spaceGlyphIdx]->pageNum;
}
Font::~Font()
{
	for (size_t i = 0; i < m_glyphs.size(); i++)
	{
		if (m_glyphs[i] != nullptr)
		{
			delete m_glyphs[i];
			m_glyphs[i] = nullptr;
		}
	}
	m_glyphs.empty();
	for (size_t i = 0; i < m_kernings.size(); i++)
	{
		if (m_kernings[i] != nullptr)
		{
			delete m_kernings[i];
			m_kernings[i] = nullptr;
		}
	}
	m_kernings.empty();
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//static methods
STATIC Font* Font::GetFontByNameWithDirectoryLoc(const std::string& fontFntWithDirectory)
{
	//write code here; // #FIXME: Implement this method yourself, based on the description above
	if (s_fontRegistry == nullptr)
	{
		return nullptr;
	}

	std::map<std::string, Font*>::iterator it;
	it = s_fontRegistry->find(fontFntWithDirectory);
	if (it != s_fontRegistry->end())
	{
		return s_fontRegistry->at(fontFntWithDirectory);
	}
	return nullptr;
}

STATIC Font* Font::CreateOrGetFont(const std::string& fileDirectory, const std::string& fontFnt)
{
	std::string combinedLocation = fileDirectory + fontFnt;

	if (s_fontRegistry == nullptr)
	{
		s_fontRegistry = new std::map<std::string, Font*>();
	}

	//write code here; // #FIXME: Implement this method yourself, based on the description above
	std::map<std::string, Font*>::iterator it;
	it = s_fontRegistry->find(combinedLocation);
	if (it != s_fontRegistry->end())
	{
		return s_fontRegistry->at(combinedLocation);
	}
	else
	{
		//if (FileUtils::VerifyFileExists(fontFnt))
		{
			s_fontRegistry->insert(std::pair<std::string, Font*>(combinedLocation, new Font(fileDirectory, fontFnt)));
			return s_fontRegistry->at(combinedLocation);
		}
		//else
		//{
		//	return nullptr;
		//}
	}
}

STATIC Font* Font::CreateOrGetFont(const std::string& fontFntWithDirectory)
{
	int idxOfFirstBackSlash = GetNthEncounterIdxOfCharInStrReverse(fontFntWithDirectory, '/', 1);
	std::string fileDirectory = fontFntWithDirectory.substr(0, idxOfFirstBackSlash + 1);
	std::string fontFntLoc = fontFntWithDirectory.substr(idxOfFirstBackSlash + 1, fontFntWithDirectory.length() - idxOfFirstBackSlash - 1);
	return CreateOrGetFont(fileDirectory, fontFntLoc);
}

STATIC void Font::ClearFonts()
{
	if (s_fontRegistry == nullptr)
	{
		return;
	}

	for (std::map<std::string, Font*>::iterator it = s_fontRegistry->begin(); it != s_fontRegistry->end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	delete s_fontRegistry;
	s_fontRegistry = nullptr;
}

STATIC void Font::DeleteFont(Font* font)
{
	if (font == nullptr || s_fontRegistry == nullptr)
	{
		return;
	}
	for (std::map<std::string, Font*>::iterator it = s_fontRegistry->begin(); it != s_fontRegistry->end(); it++)
	{
		if (it->second == font)
		{
			delete it->second;
			it->second = nullptr;
			s_fontRegistry->erase(it);
			return;
		}
	}

	if (s_fontRegistry->size() == 0)
	{
		delete s_fontRegistry;
		s_fontRegistry = nullptr;
	}
}


//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Font calls
Texture* Font::LoadPageData(const std::string& ftnPage, size_t& curIndexPosition, int& returnedId, const std::string& fileDirectory)
{
	std::string singleWord;
	Texture* newTex = nullptr;
	for (size_t i = curIndexPosition; i < ftnPage.length(); i++)
	{
		curIndexPosition++;
		unsigned char curChar = ftnPage.at(i);
		if (curChar != ' ' && curChar != '\n')
		{
			singleWord += curChar;
		}
		else
		{
			if (singleWord.substr(0, 3) == "id=")
			{
				std::string idStr = singleWord.substr(3, singleWord.length());
				returnedId = atoi(idStr.c_str());
			}
			else if (singleWord.substr(0, 5) == "file=")
			{
				singleWord.replace(0, 6, "\\");
				singleWord.replace(singleWord.length() - 1, 1, "\\");
				singleWord.erase(0, 1);
				singleWord.erase(singleWord.length() - 1, 1);
				std::string dir = fileDirectory;
				std::string imageLocation = dir + singleWord;
				newTex = Texture::CreateOrGetTexture(imageLocation);
				curIndexPosition = i;
				break;
			}
			singleWord = "";
		}
	}
	return newTex;
}

Glyph* Font::LoadGlyphData(const std::string& ftnPage, size_t& curIndexPosition)
{
	std::string singleWord;
	Glyph* newGlyph = new Glyph();
	for (size_t i = curIndexPosition; i < ftnPage.length(); i++)
	{
		curIndexPosition++;
		unsigned char curChar = ftnPage.at(i);
		if (curChar != ' ' && curChar != '\n')
		{
			singleWord += curChar;
		}
		else
		{
			if (singleWord.substr(0, 3) == "id=")
			{
				std::string idStr = singleWord.substr(3, singleWord.length());
				newGlyph->id = unsigned char(atoi(idStr.c_str()));
			}
			else if (singleWord.substr(0, 2) == "x=")
			{
				std::string xstr = singleWord.substr(2, singleWord.length());
				newGlyph->pixelStartPosition.x = (float)atof(xstr.c_str());
			}
			else if (singleWord.substr(0, 2) == "y=")
			{
				std::string ystr = singleWord.substr(2, singleWord.length());
				newGlyph->pixelStartPosition.y = (float)atof(ystr.c_str());
			}
			else if (singleWord.substr(0, 6) == "width=")
			{
				std::string widthstr = singleWord.substr(6, singleWord.length());
				newGlyph->size.x = (float)atof(widthstr.c_str());
			}
			else if (singleWord.substr(0, 7) == "height=")
			{
				std::string heightstr = singleWord.substr(7, singleWord.length());
				newGlyph->size.y = (float)atof(heightstr.c_str());
			}
			else if (singleWord.substr(0, 8) == "xoffset=")
			{
				std::string offsetxstr = singleWord.substr(8, singleWord.length());
				newGlyph->offset.x = (float)atof(offsetxstr.c_str());
			}
			else if (singleWord.substr(0, 8) == "yoffset=")
			{
				std::string offsetystr = singleWord.substr(8, singleWord.length());
				newGlyph->offset.y = (float)atof(offsetystr.c_str());
			}
			else if (singleWord.substr(0, 9) == "xadvance=")
			{
				std::string advancex = singleWord.substr(9, singleWord.length());
				newGlyph->xAdvancement = (float)atof(advancex.c_str());
			}
			else if (singleWord.substr(0, 5) == "page=")
			{
				std::string pageNum = singleWord.substr(5, singleWord.length());
				newGlyph->pageNum = atoi(pageNum.c_str());
			}
			else if (singleWord.substr(0, 5) == "chnl=")
			{
				std::string chnl = singleWord.substr(5, singleWord.length());
				newGlyph->chnl = atoi(chnl.c_str());
				curIndexPosition = i;
				break;
			}
			singleWord = "";
		}
	}
	return newGlyph;
}

Kerning* Font::LoadKerningData(const std::string& ftnPage, size_t& curIndexPosition)
{
	std::string singleWord;
	Kerning* newKerning = new Kerning();
	for (size_t i = curIndexPosition; i < ftnPage.length(); i++)
	{
		curIndexPosition++;
		unsigned char curChar = ftnPage.at(i);
		if (curChar != ' ' && curChar != '\n')
		{
			singleWord += curChar;
		}
		else
		{
			if (singleWord.substr(0, 6) == "first=")
			{
				std::string id1Str = singleWord.substr(6, singleWord.length());
				newKerning->firstId = unsigned char(atoi(id1Str.c_str()));
			}
			else if (singleWord.substr(0, 7) == "second=")
			{
				std::string id2Str = singleWord.substr(7, singleWord.length());
				newKerning->secondId = unsigned char(atoi(id2Str.c_str()));
			}
			else if (singleWord.substr(0, 7) == "amount=")
			{
				std::string amount = singleWord.substr(7, singleWord.length());
				newKerning->cursorAdjustment = (float)atof(amount.c_str());
				curIndexPosition = i;
				break;
			}
			singleWord = "";
		}
	}
	return newKerning;
}


int Font::FindGlyphIdx(char curChar) const
{
	for (size_t glyphIndex = 0; glyphIndex < m_glyphs.size(); glyphIndex++)
	{
		if (curChar == m_glyphs[glyphIndex]->id)
		{
			return glyphIndex;
		}
	}
	return -1;
}


int Font::FindKerningIdx(char curChar, char prevChar) const
{
	for (size_t i = 0; i < m_kernings.size(); i++)
	{
		if (m_kernings[i]->firstId == prevChar && m_kernings[i]->secondId == curChar)
		{
			return i;
		}
	}
	return -1;
}

const Texture* Font::RequestPage(size_t pageNumber) const
{
	if (pageNumber >= m_fontPages.size())
	{
		return nullptr;
	}
	return m_fontPages[pageNumber];
}

const Glyph* Font::GetGlyph(const unsigned char& character) const
{
	for (size_t index = 0; index < m_glyphs.size(); index++)
	{
		if (m_glyphs[index]->id == character)
		{
			return m_glyphs[index];
		}
	}
	return nullptr;
}

const float Font::GetKerningAdjustment(const unsigned char& char1, const unsigned char& char2) const
{
	for (size_t index = 0; index < m_kernings.size(); index++)
	{
		if (((m_kernings[index]->firstId) == char1) && ((m_kernings[index]->secondId) == char2))
		{
			return m_kernings[index]->cursorAdjustment;
		}
	}
	return 0.f;
}

void Font::AdjustForAlignment(const Font_HorizontalAlignment& alignment, const std::string& currentTrueString, const float& fontSize, const Vector3& direction,
	std::vector<unsigned int>& startIndex, std::vector<unsigned int>& endIndex, std::vector<std::vector<Vertex_PCT>>* vertices) const
{
	if (alignment == Font_LeftHand_Aligned)
	{
		return;
	}
	float maxLength = CalcTextWidth(currentTrueString, fontSize);
	Vector3 adjustment = direction * maxLength;
	if (alignment == Font_Center_Aligned)
	{
		adjustment = adjustment / 2.f;
	}
	for (size_t page_index = 0; page_index < vertices->size(); page_index++)
	{
		for (size_t index = startIndex.at(page_index); index < endIndex.at(page_index); index++)
		{
			vertices->at(page_index)[index].m_pos -= adjustment;
		}
	}

}

void Font::GetTextToRender(std::vector<std::vector<Vertex_PCT>>& vertices, 
	std::vector<std::vector<unsigned int>>& indices,
	const Vector3& position, const std::string& toDraw,
	const float& fontSize, const Vector3& direction , const Vector3& up,
	const Rgba& color, const Font_HorizontalAlignment& alignment, const bool& enableNewLines,
	float additionalSpaceBetweenNewLines, const bool& clearVerts)
{
	if (clearVerts)
	{
		vertices.clear();
		indices.clear();
	}
	if (m_fontPages.size() == 0)
	{
		return;
	}

	Vector3 xDirection = direction;
	xDirection.normalize();
	std::vector<unsigned int> startPositions;
	std::vector<unsigned int> endPositions;
	vertices.resize(m_fontPages.size());
	indices.resize(m_fontPages.size());
	std::vector<unsigned int> startIndices;
	startIndices.resize(m_fontPages.size(), 0);
	startPositions.resize(m_fontPages.size());
	endPositions.resize(m_fontPages.size());
	for (size_t i = 0; i < m_fontPages.size(); i++)
	{
		if (clearVerts)
		{
			vertices[i].clear();
			indices[i].clear();
		}
		startIndices[i] = vertices[i].size();
		startPositions[i] = 0;
		endPositions[i] = 0;
	}
	unsigned char prevChar = 0;
	float generalAdjust = xDirection.x * fontSize;
	Vector3 currentPosition = position;
	float line = 0.f;
	float maxHeight = CalcTextHeight(allLetters, fontSize);
	std::string currentString = "";

	for (size_t index = 0; index < toDraw.length(); index++)
	{
		unsigned char curChar = toDraw.at(index);
		//does the glyph exist?
		if (enableNewLines == true && curChar == '\n')
		{
			line += 1.0f;
			prevChar = 0;

			AdjustForAlignment(alignment, currentString, fontSize, direction,
				startPositions, endPositions, &vertices);
			currentString = "";
			for (size_t i = 0; i < startPositions.size(); i++)
			{
				startPositions[i] = endPositions[i];
			}

			currentPosition = position - ((up * line * maxHeight) 
				+ (up * maxHeight * additionalSpaceBetweenNewLines));
			continue;
		}
		else if (enableNewLines == false && curChar == '\n')
		{
			continue;
		}
		int indexInGlyph = FindGlyphIdx(curChar);
		if (indexInGlyph == -1)
		{
			//no glyph? then put white space.
			indexInGlyph = m_spaceGlyphIdx;
		}

		{
			//is there a kerning?
			int kerningIndex = FindKerningIdx(curChar, prevChar);
			if (kerningIndex != -1)
			{
				float adjustment = 
					m_kernings[kerningIndex]->cursorAdjustment * generalAdjust;
				currentPosition.x += xDirection.x * adjustment;
				currentPosition.y += xDirection.y * adjustment;
			}
			size_t pageNum = m_glyphs[indexInGlyph]->pageNum;

			Vector3 offset = (Vector3((m_glyphs[indexInGlyph]->offset.x),
				-(m_glyphs[indexInGlyph]->offset.y), 0.f) * fontSize);
			currentPosition += offset;

			IntVector2 texelSize = m_fontPages[pageNum]->m_texelSize;
			AABB2 spriteCoords = m_glyphs[indexInGlyph]->GetPixelCoords();
			spriteCoords.mins = Vector2((spriteCoords.mins.x / (float)texelSize.x), 
				(spriteCoords.mins.y / (float)texelSize.y));
			spriteCoords.maxs = Vector2((spriteCoords.maxs.x / (float)texelSize.x),
				(spriteCoords.maxs.y / (float)texelSize.y));

			Vector2 adjustGlyph = m_glyphs[indexInGlyph]->size * generalAdjust;

			currentPosition.y += maxHeight;

			size_t previousNumOfVerts = vertices[pageNum].size();
			vertices[pageNum].push_back(Vertex_PCT(Vector3(currentPosition.x,
				currentPosition.y - (m_glyphs[indexInGlyph]->size.y * generalAdjust),
				currentPosition.z),
				color,
				Vector2(spriteCoords.mins.x, spriteCoords.maxs.y)));
			vertices[pageNum].push_back(Vertex_PCT(Vector3(currentPosition.x + (adjustGlyph.x),
				currentPosition.y - (m_glyphs[indexInGlyph]->size.y * generalAdjust),
				currentPosition.z),
				color,
				spriteCoords.maxs));
			vertices[pageNum].push_back(Vertex_PCT(Vector3(currentPosition.x + (adjustGlyph.x),
				currentPosition.y,
				currentPosition.z),
				color,
				Vector2(spriteCoords.maxs.x, spriteCoords.mins.y)));
			vertices[pageNum].push_back(Vertex_PCT(Vector3(currentPosition.x,
				currentPosition.y,
				currentPosition.z),
				color,
				spriteCoords.mins));

			unsigned int startIdx = startIndices[pageNum];
			indices[pageNum].push_back(startIdx + previousNumOfVerts + 0);
			indices[pageNum].push_back(startIdx + previousNumOfVerts + 1);
			indices[pageNum].push_back(startIdx + previousNumOfVerts + 2);
			indices[pageNum].push_back(startIdx + previousNumOfVerts + 0);
			indices[pageNum].push_back(startIdx + previousNumOfVerts + 2);
			indices[pageNum].push_back(startIdx + previousNumOfVerts + 3);

			currentString += curChar;
			endPositions[pageNum] += 4;

			currentPosition.y -= maxHeight;
			currentPosition -= offset;

			currentPosition.x += (m_glyphs[indexInGlyph]->xAdvancement * generalAdjust);
			currentPosition.y += (m_glyphs[indexInGlyph]->xAdvancement * fontSize * xDirection.y);
		}
		prevChar = curChar;
	}
	AdjustForAlignment(alignment, currentString, fontSize, direction, 
		startPositions, endPositions, &vertices);
}

void Font::RenderText(const Vector3& position, const std::string& toDraw, const float& fontSize,
	const Vector3& direction, const Vector3& up, const Rgba& color, 
	const Font_HorizontalAlignment& alignment, const bool& enableNewLines) const
{
	Vector3 xDirection = direction;
	xDirection.normalize();
	std::vector<std::vector<Vertex_PCT>> vertices;
	std::vector<unsigned int> startPositions;
	std::vector<unsigned int> endPositions;
	vertices.resize(m_fontPages.size());
	startPositions.resize(m_fontPages.size());
	endPositions.resize(m_fontPages.size());
	for (size_t i = 0; i < m_fontPages.size(); i++)
	{
		startPositions[i] = 0;
		endPositions[i] = 0;
	}
	unsigned char prevChar = 0;
	float generalAdjust = xDirection.x * fontSize;
	Vector3 currentPosition = position;
	float line = 0.f;
	float maxHeight = CalcTextHeight(allLetters, fontSize);
	std::string currentString = "";
	for (size_t index = 0; index < toDraw.length(); index++)
	{
		unsigned char curChar = toDraw.at(index);
		//does the glyph exist?
		if (enableNewLines == true && curChar == '\n')
		{
			line += 1.0f;
			prevChar = 0;

			AdjustForAlignment(alignment, currentString, fontSize, direction, startPositions,
				endPositions, &vertices);
			currentString = "";
			for (size_t i = 0; i < startPositions.size(); i++)
			{
				startPositions[i] = endPositions[i];
			}

			currentPosition = position - (up * line * maxHeight);
			continue;
		}
		else if (enableNewLines == false && curChar == '\n')
		{
			continue;
		}
		int indexInGlyph = FindGlyphIdx(curChar);
		if (indexInGlyph == -1)
		{
			//no glyph? put in white space.
			vertices[0].push_back(Vertex_PCT(currentPosition, color, Vector2()));
			vertices[0].push_back(Vertex_PCT(Vector3(currentPosition.x + (xDirection.x * fontSize),
				currentPosition.y, currentPosition.z), color, Vector2()));
			vertices[0].push_back(Vertex_PCT(Vector3(currentPosition.x + (xDirection.x * fontSize),
				currentPosition.y + (xDirection.y * fontSize), currentPosition.z), color, Vector2()));
			vertices[0].push_back(Vertex_PCT(Vector3(currentPosition.x, currentPosition.y 
				+ (xDirection.y * fontSize), currentPosition.z), color, Vector2()));
			currentString += " ";
			endPositions[0] += 4;
			continue;
		}
		else
		{
			//is there a kerning?
			int kerningIndex = FindKerningIdx(curChar, prevChar);
			if (kerningIndex != -1)
			{
				float adjustment = m_kernings[kerningIndex]->cursorAdjustment * generalAdjust;
				currentPosition.x += xDirection.x * adjustment;
				currentPosition.y += xDirection.y * adjustment;
			}
			size_t pageNum = m_glyphs[indexInGlyph]->pageNum;

			Vector3 offset = (Vector3((m_glyphs[indexInGlyph]->offset.x), 
				-(m_glyphs[indexInGlyph]->offset.y), 0.f) * fontSize);;
			currentPosition += offset;

			IntVector2 texelSize = m_fontPages[pageNum]->m_texelSize;
			AABB2 spriteCoords = m_glyphs[indexInGlyph]->GetPixelCoords();
			spriteCoords.mins = Vector2((spriteCoords.mins.x / (float)texelSize.x), 
				(spriteCoords.mins.y / (float)texelSize.y));
			spriteCoords.maxs = Vector2((spriteCoords.maxs.x / (float)texelSize.x), 
				(spriteCoords.maxs.y / (float)texelSize.y));

			Vector2 adjustGlyph = m_glyphs[indexInGlyph]->size * generalAdjust;

			currentPosition.y += maxHeight;

			vertices[pageNum].push_back(Vertex_PCT(Vector3(currentPosition.x,
				currentPosition.y - (m_glyphs[indexInGlyph]->size.y * generalAdjust),
				currentPosition.z),
				color,
				Vector2(spriteCoords.mins.x, spriteCoords.maxs.y)));
			vertices[pageNum].push_back(Vertex_PCT(Vector3(currentPosition.x + (adjustGlyph.x),
				currentPosition.y - (m_glyphs[indexInGlyph]->size.y * generalAdjust),
				currentPosition.z),
				color,
				spriteCoords.maxs));
			vertices[pageNum].push_back(Vertex_PCT(Vector3(currentPosition.x + (adjustGlyph.x),
				currentPosition.y,
				currentPosition.z),
				color,
				Vector2(spriteCoords.maxs.x, spriteCoords.mins.y)));
			vertices[pageNum].push_back(Vertex_PCT(Vector3(currentPosition.x,
				currentPosition.y,
				currentPosition.z),
				color,
				spriteCoords.mins));

			currentString += curChar;
			endPositions[pageNum] += 4;

			currentPosition.y -= maxHeight;
			currentPosition -= offset;

			// 			if (kerningIndex != -1)
			// 			{
			// 				float adjustment = m_kernings[kerningIndex]->cursorAdjustment * generalAdjust;//* fontSize * xDirection.x;
			// 				currentPosition.x -= xDirection.x * adjustment;
			// 				currentPosition.y -= xDirection.y * adjustment;
			// 			}

			currentPosition.x += (m_glyphs[indexInGlyph]->xAdvancement * generalAdjust);
			currentPosition.y += (m_glyphs[indexInGlyph]->xAdvancement * fontSize * xDirection.y);
		}
		prevChar = curChar;
	}
	AdjustForAlignment(alignment, currentString, fontSize, direction, startPositions,
		endPositions, &vertices);
	for (size_t pageNum = 0; pageNum < m_fontPages.size(); pageNum++)
	{
		g_Renderer->DrawVertexArray(vertices[pageNum], PRIMITIVE_QUADS, m_fontPages[pageNum]);
	}
}
void Font::RenderText(const std::vector<std::vector<Vertex_PCT>>& vertices, 
	const std::vector<std::vector<unsigned int>>& indices, unsigned int pageIdx) const
{
	indices;
	if (pageIdx >= m_fontPages.size())
	{
		return;
	}
	g_Renderer->DrawVertexArray(vertices[pageIdx], PRIMITIVE_QUADS, m_fontPages[pageIdx]);

	//for (size_t pageIdx = 0; pageIdx < fontPages.size() && pageIdx < vertices.size(); pageIdx++)
	//{
	//	g_Renderer->DrawVertexArray(vertices[pageIdx], PRIMITIVE_QUADS, fontPages[pageIdx]);
	//}
}

const float Font::CalcTextWidth(const std::string& text, const float& fontSize) const
{
	float width = 0.f;
	unsigned char prevChar = 0;
	for (size_t charIndex = 0; charIndex < text.length(); charIndex++)
	{
		unsigned char curChar = text.at(charIndex);
		int indexInGlyph = -1;
		//does the glyph exist?
		for (size_t glyphIndex = 0; glyphIndex < m_glyphs.size(); glyphIndex++)
		{
			if (curChar == m_glyphs[glyphIndex]->id)
			{
				indexInGlyph = glyphIndex;
				break;
			}
		}
		if (indexInGlyph == -1)
		{
			width += fontSize;
		}
		else
		{
			//is there a kerning?
			int kerningIndex = -1;
			for (size_t i = 0; i < m_kernings.size(); i++)
			{
				if (m_kernings[i]->firstId == prevChar && m_kernings[i]->secondId == curChar)
				{
					kerningIndex = i;
					break;
				}
			}
			if (kerningIndex != -1)
			{
				width += m_kernings[kerningIndex]->cursorAdjustment * fontSize;
			}
			width += (m_glyphs[indexInGlyph]->xAdvancement * fontSize);
		}
		prevChar = curChar;
	}

	return width;
}


const float Font::CalcTextHeight(const std::string& text, const float& fontSize, 
	const bool& enableNewLines) const
{
	if (enableNewLines == false)
	{
		return fontSize * float(m_lineHeight);
	}
	float numLines = 0; //m_lineHeight


	for (size_t charIndex = 0; charIndex < text.length(); charIndex++)
	{
		unsigned char curChar = text.at(charIndex);
		//does the glyph exist?
		if (enableNewLines == true 
			&& (SimpleStrCmp("" + (curChar), "\n") == true || SimpleStrCmp("" + curChar, "\0") == true))
		{
			numLines += 1.f;
			continue;
		}
	}
	numLines += 1.f; //might want to consider removing this line; not sure yet.
	return fontSize * numLines * (float(m_lineHeight));
}

const int Font::GetPageCount() const
{
	return m_fontPages.size();
}

const Texture* Font::GetPageForIdx(int idx) const
{
	if (idx < 0 || (size_t)idx >= m_fontPages.size())
	{
		return nullptr;
	}
	return m_fontPages[idx];
}