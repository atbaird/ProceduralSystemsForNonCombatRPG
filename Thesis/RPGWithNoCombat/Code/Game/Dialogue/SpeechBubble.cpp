#include "Game/Dialogue/SpeechBubble.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Game/TheGame.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"


STATIC std::vector<SpeechBubble*>* SpeechBubble::s_SpeechBubbles = nullptr;
STATIC SpeechBubbleLoadingData* SpeechBubble::s_loadingData = nullptr;
STATIC const char* SpeechBubble::s_CommonSpeechBubblesXMLFolder = "Data/XML/SpeechBubbles/";
STATIC const char* SpeechBubble::s_SpeechBubbleRootXML = "SpeechBubbles";
STATIC const Vector2 SpeechBubble::s_CentralBubbleAdjustmentPercentagesFromCenterOfScreen = Vector2(0.f, -1.f / 3.f);
STATIC const Vector2 SpeechBubble::s_CentralChoiceBubbleAdjustmentPercentagesFromCenterOfScreen = Vector2(1.f / 3.f, 0.f);

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//Constructors
SpeechBubble::SpeechBubble()
{
	m_mesh.CreateVAOID();
	m_mesh.PregenerateVBOAndIBO();
	m_choiceMesh.CreateVAOID();
	m_choiceMesh.PregenerateVBOAndIBO();
	m_material.SetProgram(ShaderProgram::GetDefaultShaderProgram());
	m_choiceModelMatrix.MatrixMakeIdentity();
	m_modelMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_modelMatrix, 0.f, 0.f, 0.f, Vector3::vec3_zeros);
	Matrix44::MakeEulerRotation(m_choiceModelMatrix, 0.f, 0.f, 0.f, Vector3::vec3_zeros);
}

SpeechBubble::SpeechBubble(const XMLNode& node)
{
	m_mesh.CreateVAOID();
	m_mesh.PregenerateVBOAndIBO();
	m_choiceMesh.CreateVAOID();
	m_choiceMesh.PregenerateVBOAndIBO();
	m_material.SetProgram(ShaderProgram::GetDefaultShaderProgram());
	m_choiceModelMatrix.MatrixMakeIdentity();
	m_modelMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_modelMatrix, 0.f, 0.f, 0.f, Vector3::vec3_zeros);
	Matrix44::MakeEulerRotation(m_choiceModelMatrix, 0.f, 0.f, 0.f, Vector3::vec3_zeros);

	m_bubbleName = MakeLower(node.getName());

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "resource")
		{
			m_resource = SpriteResourceHandler::GetSpriteResourceByName(attr.lpszValue);
		}
	}
	BuildMesh();
	BuildChoiceMesh();
}

SpeechBubble::SpeechBubble(const SpeechBubble& other)
	: m_vertices(other.m_vertices),
	m_choiceVertices(other.m_choiceVertices),
	m_indices(other.m_indices),
	m_choiceIndices(other.m_choiceIndices),
	m_modelMatrix(other.m_modelMatrix),
	m_choiceModelMatrix(other.m_choiceModelMatrix),
	m_mesh(other.m_mesh),
	m_choiceMesh(other.m_choiceMesh),
	m_material(other.m_material),
	m_resource(other.m_resource),
	m_bubbleName(other.m_bubbleName)
{

}

SpeechBubble::~SpeechBubble()
{

}

void SpeechBubble::BuildMesh()
{
	m_vertices.clear();
	//m_indices.clear();
	if (m_resource == nullptr)
	{
		return;
	}
	Texture* tex = m_resource->GetSpriteSheet();
	if (tex == nullptr)
	{
		return;
	}
	m_material.SetOrAddTexture(tex, 0, "gDiffuseTex", 0);
	AABB2 coords = m_resource->GetTextureCoords();
	Vector2 difference = coords.maxs - coords.mins;
	Vector2 thirdDiff = difference * (1.f / 3.f);

	float nYMax = coords.maxs.y;
	float nYMinCYMax = coords.maxs.y - thirdDiff.y;
	float cYMinSYMax = coords.maxs.y - (thirdDiff.y * 2.f);
	float sYMin = coords.mins.y;


	float eXMax = coords.maxs.x;
	float eXMinCXMax = coords.maxs.x - thirdDiff.x;
	float cXMinWXMax = coords.maxs.x - (thirdDiff.x * 2.f);
	float wXMin = coords.mins.x;

	//North Edge coords
	Vector2 nwCornerMins = Vector2(wXMin, nYMinCYMax);
	Vector2 nwCornerMaxs = Vector2(cXMinWXMax, nYMax);
	Vector2 nCornerMins = Vector2(cXMinWXMax, nYMinCYMax);
	Vector2 nCornerMaxs = Vector2(eXMinCXMax, nYMax);
	Vector2 neCornerMins = Vector2(eXMinCXMax, nYMinCYMax);
	Vector2 neCornerMaxs = Vector2(eXMax, nYMax);

	//Central Horizontal Coords
	Vector2 cwCornerMins = Vector2(wXMin, cYMinSYMax);
	Vector2 cwCornerMaxs = Vector2(cXMinWXMax, nYMinCYMax);
	Vector2 cCornerMins = Vector2(cXMinWXMax, cYMinSYMax);
	Vector2 cCornerMaxs = Vector2(eXMinCXMax, nYMinCYMax);
	Vector2 ceCornerMins = Vector2(eXMinCXMax, cYMinSYMax);
	Vector2 ceCornerMaxs = Vector2(eXMax, nYMinCYMax);

	//South Horizontal Coords
	Vector2 swCornerMins = Vector2(wXMin, sYMin);
	Vector2 swCornerMaxs = Vector2(cXMinWXMax, cYMinSYMax);
	Vector2 sCornerMins = Vector2(cXMinWXMax, sYMin);
	Vector2 sCornerMaxs = Vector2(eXMinCXMax, cYMinSYMax);
	Vector2 seCornerMins = Vector2(eXMinCXMax, sYMin);
	Vector2 seCornerMaxs = Vector2(eXMax, cYMinSYMax);


	if (m_indices.size() == 0)
	{
		MeshBuilder builder;
		std::vector<unsigned int> orgIndices;
		builder.BuildQuad(Vector3(0.f, 1.f, 0.f), Vector3(1.f, 0.f, 0.f), 1.f, 1.f, 1, 1, Rgba(1.f, 1.f, 1.f, 1.f));
		builder.GetIndices(orgIndices);

		for (int set = 0; set < 9; set++)
		{
			size_t baseToAdd = set * 4;
			for (size_t indiceIdx = 0; indiceIdx < orgIndices.size(); indiceIdx++)
			{
				unsigned int indice = baseToAdd + orgIndices[indiceIdx];
				m_indices.push_back(indice);
			}
		}
	}

	Vector2 screenSize = Vector2(g_TheGame->GetWindowWidth(), g_TheGame->GetWindowHeight());
	Vector2 thirdScreenSize = screenSize * (1.f / 3.f);

	Vector2 centralPosition = GetCentralBubblePosition();

	Vector2 MinSize = Vector2(1.f, 1.f);
	Vector2 MaxSize = Vector2(screenSize.x * 0.75f, 1.f);
	Vector2 halfMinSize = MinSize * 0.5f;
	Vector2 halfMaxSize = MaxSize * 0.5f;
	Vector2 halfMinAndMaxSize = halfMinSize + halfMaxSize;
	Vector2 MaxXMinY = Vector2(MaxSize.x, MinSize.y);
	Vector2 MinXMaxY = Vector2(MinSize.x, MaxSize.y);

	Vector2 nwTrans = centralPosition + Vector2(-(halfMinAndMaxSize.x), (halfMinAndMaxSize.y));
	Vector2 nTrans = centralPosition + Vector2(0.f, (halfMinAndMaxSize.y));
	Vector2 neTrans = centralPosition + Vector2((halfMinAndMaxSize.x), (halfMinAndMaxSize.y));

	Vector2 cwTrans = centralPosition + Vector2(-(halfMinAndMaxSize.x), 0.f);
	Vector2 cTrans = centralPosition + Vector2::vec2_zeros;
	Vector2 ceTrans = centralPosition + Vector2((halfMinAndMaxSize.x), 0.f);

	Vector2 swTrans = centralPosition + Vector2(-(halfMinAndMaxSize.x), -(halfMinAndMaxSize.y));
	Vector2 sTrans = centralPosition + Vector2(0.f, -(halfMinAndMaxSize.y));
	Vector2 seTrans = centralPosition + Vector2((halfMinAndMaxSize.x), -(halfMinAndMaxSize.y));
	//Parts needing vertical resizing: 3, 4, 5
	//Parts needing horizontal resizing: 1, 4, 7
	AddVerticesWith(m_vertices, nwCornerMins, nwCornerMaxs, nwTrans, MinSize);
	AddVerticesWith(m_vertices, nCornerMins, nCornerMaxs, nTrans, MaxXMinY);
	AddVerticesWith(m_vertices, neCornerMins, neCornerMaxs, neTrans, MinSize);
	AddVerticesWith(m_vertices, cwCornerMins, cwCornerMaxs, cwTrans, MinXMaxY);
	AddVerticesWith(m_vertices, cCornerMins, cCornerMaxs, cTrans, MaxSize);
	AddVerticesWith(m_vertices, ceCornerMins, ceCornerMaxs, ceTrans, MinXMaxY);
	AddVerticesWith(m_vertices, swCornerMins, swCornerMaxs, swTrans, MinSize);
	AddVerticesWith(m_vertices, sCornerMins, sCornerMaxs, sTrans, MaxXMinY);
	AddVerticesWith(m_vertices, seCornerMins, seCornerMaxs, seTrans, MinSize);

	m_mesh.setVBO(m_vertices);
	m_mesh.setIBO(m_indices);
}

void SpeechBubble::BuildChoiceMesh()
{
	m_choiceVertices.clear();
	if (m_resource == nullptr)
	{
		return;
	}
	Texture* tex = m_resource->GetSpriteSheet();
	if (tex == nullptr)
	{
		return;
	}
	m_material.SetOrAddTexture(tex, 0, "gDiffuseTex", 0);
	AABB2 coords = m_resource->GetTextureCoords();
	Vector2 difference = coords.maxs - coords.mins;
	Vector2 thirdDiff = difference * (1.f / 3.f);

	float nYMax = coords.maxs.y;
	float nYMinCYMax = coords.maxs.y - thirdDiff.y;
	float cYMinSYMax = coords.maxs.y - (thirdDiff.y * 2.f);
	float sYMin = coords.mins.y;


	float eXMax = coords.maxs.x;
	float eXMinCXMax = coords.maxs.x - thirdDiff.x;
	float cXMinWXMax = coords.maxs.x - (thirdDiff.x * 2.f);
	float wXMin = coords.mins.x;

	//North Edge coords
	Vector2 nwCornerMins = Vector2(wXMin, nYMinCYMax);
	Vector2 nwCornerMaxs = Vector2(cXMinWXMax, nYMax);
	Vector2 nCornerMins = Vector2(cXMinWXMax, nYMinCYMax);
	Vector2 nCornerMaxs = Vector2(eXMinCXMax, nYMax);
	Vector2 neCornerMins = Vector2(eXMinCXMax, nYMinCYMax);
	Vector2 neCornerMaxs = Vector2(eXMax, nYMax);

	//Central Horizontal Coords
	Vector2 cwCornerMins = Vector2(wXMin, cYMinSYMax);
	Vector2 cwCornerMaxs = Vector2(cXMinWXMax, nYMinCYMax);
	Vector2 cCornerMins = Vector2(cXMinWXMax, cYMinSYMax);
	Vector2 cCornerMaxs = Vector2(eXMinCXMax, nYMinCYMax);
	Vector2 ceCornerMins = Vector2(eXMinCXMax, cYMinSYMax);
	Vector2 ceCornerMaxs = Vector2(eXMax, nYMinCYMax);

	//South Horizontal Coords
	Vector2 swCornerMins = Vector2(wXMin, sYMin);
	Vector2 swCornerMaxs = Vector2(cXMinWXMax, cYMinSYMax);
	Vector2 sCornerMins = Vector2(cXMinWXMax, sYMin);
	Vector2 sCornerMaxs = Vector2(eXMinCXMax, cYMinSYMax);
	Vector2 seCornerMins = Vector2(eXMinCXMax, sYMin);
	Vector2 seCornerMaxs = Vector2(eXMax, cYMinSYMax);


	if (m_choiceIndices.size() == 0)
	{
		MeshBuilder builder;
		std::vector<unsigned int> orgIndices;
		builder.BuildQuad(Vector3(0.f, 1.f, 0.f), Vector3(1.f, 0.f, 0.f),
			1.f, 1.f, 1, 1, Rgba(1.f, 1.f, 1.f, 1.f));
		builder.GetIndices(orgIndices);

		for (int set = 0; set < 9; set++)
		{
			size_t baseToAdd = set * 4;
			for (size_t indiceIdx = 0; indiceIdx < orgIndices.size(); indiceIdx++)
			{
				unsigned int indice = baseToAdd + orgIndices[indiceIdx];
				m_choiceIndices.push_back(indice);
			}
		}
	}

	Vector2 screenSize = Vector2(g_TheGame->GetWindowWidth(), g_TheGame->GetWindowHeight());
	Vector2 thirdScreenSize = screenSize * (1.f / 3.f);

	Vector2 centralPosition = GetCentralChoiceBubblePosition();

	Vector2 MinSize = Vector2(1.8f, 1.f);
	Vector2 MaxSize = Vector2(1.8f, screenSize.y * 0.4f);
	Vector2 halfMinSize = MinSize * 0.5f;
	Vector2 halfMaxSize = MaxSize * 0.5f;
	Vector2 halfMinAndMaxSize = halfMinSize + halfMaxSize;
	Vector2 MaxXMinY = Vector2(MaxSize.x, MinSize.y);
	Vector2 MinXMaxY = Vector2(MinSize.x, MaxSize.y);

	Vector2 nwTrans = centralPosition + Vector2(-(halfMinAndMaxSize.x), (halfMinAndMaxSize.y));
	Vector2 nTrans = centralPosition + Vector2(0.f, (halfMinAndMaxSize.y));
	Vector2 neTrans = centralPosition + Vector2((halfMinAndMaxSize.x), (halfMinAndMaxSize.y));

	Vector2 cwTrans = centralPosition + Vector2(-(halfMinAndMaxSize.x), 0.f);
	Vector2 cTrans = centralPosition + Vector2::vec2_zeros;
	Vector2 ceTrans = centralPosition + Vector2((halfMinAndMaxSize.x), 0.f);

	Vector2 swTrans = centralPosition + Vector2(-(halfMinAndMaxSize.x), -(halfMinAndMaxSize.y));
	Vector2 sTrans = centralPosition + Vector2(0.f, -(halfMinAndMaxSize.y));
	Vector2 seTrans = centralPosition + Vector2((halfMinAndMaxSize.x), -(halfMinAndMaxSize.y));
	//Parts needing vertical resizing: 3, 4, 5
	//Parts needing horizontal resizing: 1, 4, 7
	AddVerticesWith(m_choiceVertices, nwCornerMins, nwCornerMaxs, nwTrans, MinSize);
	AddVerticesWith(m_choiceVertices, nCornerMins, nCornerMaxs, nTrans, MaxXMinY);
	AddVerticesWith(m_choiceVertices, neCornerMins, neCornerMaxs, neTrans, MinSize);
	AddVerticesWith(m_choiceVertices, cwCornerMins, cwCornerMaxs, cwTrans, MinXMaxY);
	AddVerticesWith(m_choiceVertices, cCornerMins, cCornerMaxs, cTrans, MaxSize);
	AddVerticesWith(m_choiceVertices, ceCornerMins, ceCornerMaxs, ceTrans, MinXMaxY);
	AddVerticesWith(m_choiceVertices, swCornerMins, swCornerMaxs, swTrans, MinSize);
	AddVerticesWith(m_choiceVertices, sCornerMins, sCornerMaxs, sTrans, MaxXMinY);
	AddVerticesWith(m_choiceVertices, seCornerMins, seCornerMaxs, seTrans, MinSize);

	m_choiceMesh.setVBO(m_choiceVertices);
	m_choiceMesh.setIBO(m_choiceIndices);
}


void SpeechBubble::AddVerticesWith(std::vector<SpriteVert>& verts,
	const Vector2& minsCoords, const Vector2& maxsCoords, const Vector2& translation, const Vector2& size)
{
	std::vector<SpriteVert> vertices;
	MeshBuilder builder;
	builder.BuildQuad(Vector3(0.f, 1.f, 0.f), Vector3(1.f, 0.f, 0.f), size.x, size.y, 1, 1, Rgba(1.f, 1.f, 1.f, 1.f), translation);
	builder.GetVertices(vertices);
	for (size_t i = 0; i < vertices.size(); i++)
	{
		SpriteVert vert = vertices[i];
		if (i % 2 == 0)
		{
			vert.m_texCoords.x = minsCoords.x;
		}
		else
		{
			vert.m_texCoords.x = maxsCoords.x;
		}
		if ((i / 2) == 0)
		{
			vert.m_texCoords.y = minsCoords.y;
		}
		else
		{
			vert.m_texCoords.y = maxsCoords.y;
		}
		verts.push_back(vert);
	}
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//Updates and Render
void SpeechBubble::Render() const
{
	m_material.SetModelMatrix(m_modelMatrix);
	ShaderProgram* prog = m_material.GetProgram();
	int colorSchemeChoice = 0;
	prog->SetProgramValueInt("gColorChoice", &colorSchemeChoice, 1);
	m_mesh.DrawWith(&m_material);
	
}

void SpeechBubble::RenderChoiceBubble() const
{
	m_material.SetModelMatrix(m_choiceModelMatrix);
	ShaderProgram* prog = m_material.GetProgram();
	int colorSchemeChoice = 0;
	prog->SetProgramValueInt("gColorChoice", &colorSchemeChoice, 1);
	m_choiceMesh.DrawWith(&m_material);
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//Setters
void SpeechBubble::SetBubbleName(const std::string& name)
{
	m_bubbleName = name;
}

void SpeechBubble::SetResource(const SpriteResource* res)
{
	m_resource = res;
	BuildMesh();
	BuildChoiceMesh();
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//Getters
const std::string& SpeechBubble::GetBubbleName() const
{
	return m_bubbleName;
}

const SpriteResource* SpeechBubble::GetSpriteResource() const
{
	return m_resource;
}

void SpeechBubble::WriteSpeechBubbleToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Speech Bubble Name: " + m_bubbleName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Sprite Resource: ";
	if (m_resource == nullptr)
	{
		str += "NULL\n";
	}
	else
	{
		str += m_resource->GetSpriteResourceName() + "\n";
	}
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//Static Methods
STATIC void SpeechBubble::WriteAllSpeechBubblesToString(std::string& str, int indentationAmt)
{
	if (s_SpeechBubbles == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < s_SpeechBubbles->size(); i++)
	{
		const SpeechBubble* bubble = s_SpeechBubbles->at(i);
		if (bubble == nullptr)
		{
			GameDebuggingCommons::AddIndentation(str, indentationAmt);
			str += "NULL SPEECH BUBBLE\n";
		}
		else
		{
			bubble->WriteSpeechBubbleToString(str, indentationAmt);
		}
	}
}

STATIC void SpeechBubble::ReadInAllSpeechBubblesFromDefaultFolderXMLAllAtOnce()
{
	std::string fileLocation = s_CommonSpeechBubblesXMLFolder;
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		fileLocation = settings->m_commonSpeechBubbleXMLFolder;
	}

	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(fileLocation,
		"*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadInSpeechBubbleXMLNodeFromFileLocation(fileStr);
	}
}

STATIC bool SpeechBubble::OnUpdateReadInAllSpeechBubblesFromDefaultFolderXML()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new SpeechBubbleLoadingData();
	}

	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingData->m_readInFiles)
	{
		s_loadingData->m_readInFiles = true;
		s_loadingData->m_index = 0;
		if (settings != nullptr)
		{
			s_loadingData->m_amountOfTimeToTakeForLoadingInSpeechBubbles = settings->m_amountOfTimeLoadingSpeechBubblesPerFrame;
			s_loadingData->m_fileLocation = settings->m_commonSpeechBubbleXMLFolder;
		}
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(s_loadingData->m_fileLocation,
			"*", true);
		return false;
	}

	if (s_loadingData->m_amountOfTimeToTakeForLoadingInSpeechBubbles <= 0)
	{
		return true;
	}
	int numberOfSpeechBubblesReadThisFrame = 0;
	while ( (size_t)s_loadingData->m_index < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
		ReadInSpeechBubbleXMLNodeFromFileLocation(fileStr);

		numberOfSpeechBubblesReadThisFrame++;
		s_loadingData->m_index++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakeForLoadingInSpeechBubbles)
		{
			return false;
		}
	}
	if ((size_t)s_loadingData->m_index >= s_loadingData->m_files.size())
	{
		return true;
	}
	return false;
}

STATIC void SpeechBubble::ReadInSpeechBubbleXMLNodeFromFileLocation(std::string fileStr)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, s_SpeechBubbleRootXML, errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		{
			SpeechBubble* speechBubble = new SpeechBubble(child);
			if (s_SpeechBubbles == nullptr)
			{
				s_SpeechBubbles = new std::vector<SpeechBubble*>();
			}
			s_SpeechBubbles->push_back(speechBubble);
		}
	}
}

STATIC void SpeechBubble::ClearAllSpeechBubbles()
{
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}
	if (s_SpeechBubbles == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < s_SpeechBubbles->size(); i++)
	{
		delete s_SpeechBubbles->at(i);
	}
	delete s_SpeechBubbles;
	s_SpeechBubbles = nullptr;
}

STATIC const SpeechBubble* SpeechBubble::GetSpeechBubbleByName(const std::string& name)
{
	if (s_SpeechBubbles == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);
	for (size_t i = 0; i < s_SpeechBubbles->size(); i++)
	{
		SpeechBubble* bubble = s_SpeechBubbles->at(i);
		if (bubble != nullptr && bubble->GetBubbleName() == lowerCase)
		{
			return bubble;
		}
	}
	return nullptr;
}

STATIC bool SpeechBubble::RegisterSpeechBubbleByName(const std::string& name, SpeechBubble* bubble)
{
	if (bubble == nullptr)
	{
		return false;
	}

	for (size_t i = 0; i < s_SpeechBubbles->size(); i++)
	{
		SpeechBubble* bub = s_SpeechBubbles->at(i);
		if (bub != nullptr && bub->GetBubbleName() == name)
		{
			return false;
		}
	}
	s_SpeechBubbles->push_back(bubble);
	return true;
}

STATIC bool SpeechBubble::RegisterSpeechBubbleByName(const std::string& name, const SpeechBubble& bubble)
{
	for (size_t i = 0; i < s_SpeechBubbles->size(); i++)
	{
		SpeechBubble* bub = s_SpeechBubbles->at(i);
		if (bub != nullptr && bub->GetBubbleName() == name)
		{
			return false;
		}
	}
	s_SpeechBubbles->push_back(new SpeechBubble(bubble));
	return true;
}

STATIC Vector2 SpeechBubble::GetCentralBubblePosition()
{
	Vector2 screenSize = Vector2(g_TheGame->GetWindowWidth(), g_TheGame->GetWindowHeight());
	Vector2 centralPosition = Vector2(s_CentralBubbleAdjustmentPercentagesFromCenterOfScreen.x * screenSize.x,
		s_CentralBubbleAdjustmentPercentagesFromCenterOfScreen.y * screenSize.y);
	return centralPosition;
}

STATIC Vector2 SpeechBubble::GetCentralChoiceBubblePosition()
{
	Vector2 screenSize = Vector2(g_TheGame->GetWindowWidth(), g_TheGame->GetWindowHeight());
	Vector2 centralPosition = Vector2(s_CentralChoiceBubbleAdjustmentPercentagesFromCenterOfScreen.x * screenSize.x,
		s_CentralChoiceBubbleAdjustmentPercentagesFromCenterOfScreen.y * screenSize.y)
		+ Vector2(0.f, screenSize.y * 0.15f);
	return centralPosition;
}