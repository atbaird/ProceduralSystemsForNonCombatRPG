#include "Game/GameEntities/Agents/Names/NameGenerator.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/TheGame.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Static Variables
STATIC std::map<std::string, NameGenerator>* NameGenerator::s_NameGenerators = nullptr;
STATIC NameGeneratorLoadingData* NameGenerator::s_loadingData = nullptr;
STATIC std::string NameGenerator::s_DefaultName = "Billy Bob";
STATIC const char* NameGenerator::s_CommonNameGeneratorFileDirectory = "Data/XML/NameGenerators/";
STATIC const char* NameGenerator::s_NameGeneratorsRootNodeName = "NameGenerators";

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Constructors
NameGenerator::NameGenerator()
{

}

NameGenerator::NameGenerator(const XMLNode& node)
{
	//Read in Attributes
	SetGeneratorName(node.getName());
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		std::string attrVal = std::string(attr.lpszValue);
		if (attrName == "order")
		{
			attrVal = ReplaceCharInString(attrVal, ',', ' ');
			attrVal = ReplaceCharInString(attrVal, '-', ' ');
			attrVal = MakeLower(attrVal);
			std::vector<std::string> vals = ParseString(attrVal, ' ');
			m_setOrder.insert(m_setOrder.end(), vals.begin(), vals.end());
			m_orderSet = true;
		}
		else if (attrName == "femaleorder"
			|| attrName == "female")
		{
			attrVal = ReplaceCharInString(attrVal, ',', ' ');
			attrVal = ReplaceCharInString(attrVal, '-', ' ');
			attrVal = MakeLower(attrVal);
			std::vector<std::string> vals = ParseString(attrVal, ' ');
			m_femaleOrder.insert(m_femaleOrder.end(), vals.begin(), vals.end());
			m_specializedOrderForFemale = true;
			m_orderSet = true;
		}
		else if (attrName == "maleorder"
			|| attrName == "male")
		{
			attrVal = ReplaceCharInString(attrVal, ',', ' ');
			attrVal = ReplaceCharInString(attrVal, '-', ' ');
			attrVal = MakeLower(attrVal);
			std::vector<std::string> vals = ParseString(attrVal, ' ');
			m_maleOrder.insert(m_maleOrder.end(), vals.begin(), vals.end());
			m_specializedOrderForMale = true;
			m_orderSet = true;
		}
		else if (attrName == "refermaleas")
		{
			m_whatToReferMaleAs = attrVal;
		}
		else if (attrName == "referfemaleas")
		{
			m_whatToReferFemaleAs = attrVal;
		}
		else if (attrName == "refermalesiblingas")
		{
			m_whatToReferMaleSiblingAs = attrVal;
		}
		else if (attrName == "referfemalesiblingas")
		{
			m_whatToReferFemaleSiblingAs = attrVal;
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		NameSet set(child);
		m_nameSets.push_back(set);
		
	}

	RecalcSetOrderIndexes();
	RecalcFemaleOrderIndexes();
	RecalcMaleOrderIndexes();
}

NameGenerator::~NameGenerator()
{
}


//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Operations
const std::string NameGenerator::GenerateRandomName(bool careAboutGenderIfcan, bool genderIsMale) const
{
	std::string nameOut = "";
	if (careAboutGenderIfcan)
	{
		if (genderIsMale)
		{
			if (m_maleOrder.size() == 0)
			{
				nameOut = GenerateRandomNameFromSetOrder();
			}
			else
			{
				nameOut = GenerateRandomNameFromMaleOrder();
			}
		}
		else
		{
			if (m_femaleOrder.size() == 0)
			{
				nameOut = GenerateRandomNameFromSetOrder();
			}
			else
			{
				nameOut = GenerateRandomNameFromFemaleOrder();
			}
		}
	}
	else
	{
		nameOut = GenerateRandomNameFromSetOrder();
	}

	return nameOut;
}

const std::string NameGenerator::GenerateRandomNameFromSetOrder() const
{
	std::string nameOut = "";
	bool insertedOneAlready = false;
	for (size_t i = 0; i < m_setOrder.size(); i++)
	{
		if (m_setOrder[i].m_index == -1)
		{
			continue;
		}
		const NameSet& set = m_nameSets[m_setOrder[i].m_index];
		bool optional = set.GetOptional();
		if (optional)
		{
			float percentageTarget = set.GetPercentageNeedToBeHigherThanToReceiveOptionalName();
			float percentageChance = GetRandomFloatZeroToOne();
			if (percentageChance >= percentageTarget)
			{
				if (insertedOneAlready == true)
				{
					nameOut += " ";
				}
				else
				{
					insertedOneAlready = true;
				}
				nameOut += set.GenerateRandomName();
			}
		}
		else
		{
			if (insertedOneAlready == true)
			{
				nameOut += " ";
			}
			else
			{
				insertedOneAlready = true;
			}
			nameOut += set.GenerateRandomName();
		}
	}

	if (nameOut == "")
	{
		nameOut = s_DefaultName;
	}
	return nameOut;
}

const std::string NameGenerator::GenerateRandomNameFromMaleOrder() const
{
	std::string nameOut = "";
	bool insertedOneAlready = false;
	for (size_t i = 0; i < m_maleOrder.size(); i++)
	{
		const NameSet& set = m_nameSets[m_maleOrder[i].m_index];
		bool optional = set.GetOptional();
		if (optional)
		{
			float percentageTarget = set.GetPercentageNeedToBeHigherThanToReceiveOptionalName();
			float percentageChance = GetRandomFloatZeroToOne();
			if (percentageChance >= percentageTarget)
			{
				if (insertedOneAlready == true)
				{
					nameOut += " ";
				}
				else
				{
					insertedOneAlready = true;
				}
				nameOut += set.GenerateRandomName();
			}
		}
		else
		{
			if (insertedOneAlready == true)
			{
				nameOut += " ";
			}
			else
			{
				insertedOneAlready = true;
			}
			nameOut += set.GenerateRandomName();
		}
	}

	if (nameOut == "")
	{
		nameOut = s_DefaultName;
	}
	return nameOut;
}

const std::string NameGenerator::GenerateRandomNameFromFemaleOrder() const
{
	std::string nameOut = "";
	bool insertedOneAlready = false;
	for (size_t i = 0; i < m_femaleOrder.size(); i++)
	{
		const NameSet& set = m_nameSets[m_femaleOrder[i].m_index];
		bool optional = set.GetOptional();
		if (optional)
		{
			float percentageTarget = set.GetPercentageNeedToBeHigherThanToReceiveOptionalName();
			float percentageChance = GetRandomFloatZeroToOne();
			if (percentageChance >= percentageTarget)
			{
				if (insertedOneAlready == true)
				{
					nameOut += " ";
				}
				else
				{
					insertedOneAlready = true;
				}
				nameOut += set.GenerateRandomName();
			}
		}
		else
		{
			if (insertedOneAlready == true)
			{
				nameOut += " ";
			}
			else
			{
				insertedOneAlready = true;
			}
			nameOut += set.GenerateRandomName();
		}
	}

	if (nameOut == "")
	{
		nameOut = s_DefaultName;
	}
	return nameOut;
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Setters
void NameGenerator::SetGeneratorName(const std::string name)
{
	m_generatorName = MakeLower(name);
	if (m_generatorName != "")
	{
		m_generatorNameSet = true;
	}
	else
	{
		m_generatorNameSet = false;
	}
}

void NameGenerator::SetOrder(const std::vector<std::string>& order)
{
	m_setOrder.clear();
	for (size_t i = 0; i < order.size(); i++)
	{
		std::string name = MakeLower(order[i]);
		m_setOrder.push_back(name);
	}
}

void NameGenerator::SetFemaleOrder(const std::vector<std::string>& order)
{
	m_femaleOrder.clear();
	for (size_t i = 0; i < order.size(); i++)
	{
		std::string name = MakeLower(order[i]);
		m_femaleOrder.push_back(name);
	}
}

void NameGenerator::SetMaleOrder(const std::vector<std::string>& order)
{
	m_maleOrder.clear();
	for (size_t i = 0; i < order.size(); i++)
	{
		std::string name = MakeLower(order[i]);
		m_maleOrder.push_back(name);
	}
}

void NameGenerator::SetNameSets(const std::vector<NameSet>& sets)
{
	m_nameSets = sets;
}

void NameGenerator::AddNameSet(const NameSet& nameSet)
{
	m_nameSets.push_back(nameSet);
}

void NameGenerator::RecalcSetOrderIndexes()
{
	for (size_t setOrderPairIdx = 0; setOrderPairIdx < m_setOrder.size(); setOrderPairIdx++)
	{
		bool found = false;
		OrderNameAndIndexPair& pair = m_setOrder[setOrderPairIdx];
		std::string pairSetName = pair.m_orderName;
		for (size_t setIdx = 0; setIdx < m_nameSets.size(); setIdx++)
		{
			NameSet& set = m_nameSets[setIdx];
			std::string setName = set.GetKeyName();
			if (setName == pairSetName)
			{
				found = true;
				pair.m_index = setIdx;
				break;
			}
		}
		if (found == false)
		{
			pair.m_index = -1;
		}
	}
}

void NameGenerator::RecalcFemaleOrderIndexes()
{
	for (size_t setOrderPairIdx = 0; setOrderPairIdx < m_femaleOrder.size(); setOrderPairIdx++)
	{
		bool found = false;
		OrderNameAndIndexPair& pair = m_femaleOrder[setOrderPairIdx];
		for (size_t setIdx = 0; setIdx < m_nameSets.size(); setIdx++)
		{
			NameSet& set = m_nameSets[setIdx];
			if (set.GetKeyName() == pair.m_orderName)
			{
				found = true;
				pair.m_index = setIdx;
				break;
			}
		}
		if (found == false)
		{
			pair.m_index = -1;
		}
	}
}

void NameGenerator::RecalcMaleOrderIndexes()
{
	for (size_t setOrderPairIdx = 0; setOrderPairIdx < m_maleOrder.size(); setOrderPairIdx++)
	{
		bool found = false;
		OrderNameAndIndexPair& pair = m_maleOrder[setOrderPairIdx];
		for (size_t setIdx = 0; setIdx < m_nameSets.size(); setIdx++)
		{
			NameSet& set = m_nameSets[setIdx];
			if (set.GetKeyName() == pair.m_orderName)
			{
				found = true;
				pair.m_index = setIdx;
				break;
			}
		}
		if (found == false)
		{
			pair.m_index = -1;
		}
	}
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Getters
const std::vector<OrderNameAndIndexPair>& NameGenerator::GetSetOrder() const
{
	return m_setOrder;
}

const std::vector<OrderNameAndIndexPair>& NameGenerator::GetFemaleSetOrder() const
{
	return m_femaleOrder;
}

const std::vector<OrderNameAndIndexPair>& NameGenerator::GetMaleSetOrder() const
{
	return m_maleOrder;
}

const std::vector<NameSet>& NameGenerator::GetNameSet() const
{
	return m_nameSets;
}

const std::string& NameGenerator::GetGeneratorName() const
{
	return m_generatorName;
}

const std::string& NameGenerator::GetWhatToReferMalesAs() const
{
	return m_whatToReferMaleAs;
}

const std::string& NameGenerator::GetWhatToReferFemaleAs() const
{
	return m_whatToReferFemaleAs;
}

const std::string& NameGenerator::GetWhatToReferMaleSiblingAs() const
{
	return m_whatToReferMaleSiblingAs;
}

const std::string& NameGenerator::GetWhatToReferFemaleSiblingAs() const
{
	return m_whatToReferFemaleSiblingAs;
}

const std::string& NameGenerator::GetMaleOwningPronoun() const
{
	return m_maleOwningPronoun;
}

const std::string& NameGenerator::GetFemaleOwningPronoun() const
{
	return m_femaleOwningPronoun;
}

const std::string& NameGenerator::GetMaleIsPronoun() const
{
	return m_maleIsPronoun;
}

const std::string& NameGenerator::GetFemaleIsPronoun() const
{
	return m_femaleIsPronoun;
}

bool NameGenerator::GetSpecializedOrderForMale() const
{
	return m_specializedOrderForMale;
}

bool NameGenerator::GetSpecializedOrderForFemale() const
{
	return m_specializedOrderForFemale;
}

bool NameGenerator::GetOrderSet() const
{
	return m_orderSet;
}

bool NameGenerator::GetGeneratorNameSet() const
{
	return m_generatorNameSet;
}

void NameGenerator::WriteNameGeneratorToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Name Generator Name: " + m_generatorName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "What to refer Male as: " + m_whatToReferMaleAs + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "What to refer Female as: " + m_whatToReferFemaleAs + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "What to call a Male sibling: " + m_whatToReferMaleSiblingAs + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "What to call a Female sibling: " + m_whatToReferFemaleSiblingAs + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Male Owning Pronounce: " + m_maleOwningPronoun + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Female Owning Pronounce: " + m_femaleOwningPronoun + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Male Is Pronoun: " + m_maleIsPronoun + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Female Is Pronoun: " + m_femaleIsPronoun + "\n";

	if (m_setOrder.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Default Order Set: ";
		for (size_t i = 0; i < m_setOrder.size(); i++)
		{
			str += m_setOrder[i].m_orderName;
			if (i == m_setOrder.size() - 1)
			{
				str += "\n";
			}
			else
			{
				str += ", ";
			}
		}
	}

	if (m_maleOrder.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Male Order Set: ";
		for (size_t i = 0; i < m_maleOrder.size(); i++)
		{
			str += m_maleOrder[i].m_orderName;
			if (i == m_maleOrder.size() - 1)
			{
				str += "\n";
			}
			else
			{
				str += ", ";
			}
		}
	}

	if (m_femaleOrder.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Female Order Set: ";
		for (size_t i = 0; i < m_femaleOrder.size(); i++)
		{
			str += m_femaleOrder[i].m_orderName;
			if (i == m_femaleOrder.size() - 1)
			{
				str += "\n";
			}
			else
			{
				str += ", ";
			}
		}
	}

	int nextLevel = indentationAmt + 3;
	if (m_nameSets.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Name Sets: \n";
		for (size_t i = 0; i < m_nameSets.size(); i++)
		{
			const NameSet& set = m_nameSets[i];
			set.WriteNameSetToString(str, nextLevel);
		}
	}
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Static Methods
STATIC void NameGenerator::WriteAllNameGeneratorsToString(std::string& str, int indentationAmt)
{
	if (s_NameGenerators == nullptr)
	{
		return;
	}

	str += "Number of Name Generators: " + std::to_string(s_NameGenerators->size()) + "\n"; 

	for (std::map<std::string, NameGenerator>::const_iterator it = s_NameGenerators->begin();
		it != s_NameGenerators->end(); it++)
	{
		const NameGenerator& gen = it->second;
		gen.WriteNameGeneratorToString(str, indentationAmt + 3);
	}
}

STATIC void NameGenerator::ClearAllGenerators()
{
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}
	if (s_NameGenerators != nullptr)
	{
		delete s_NameGenerators;
		s_NameGenerators = nullptr;
	}
}

STATIC void NameGenerator::ReadInGeneratorsFromCommonNameGeneratorsFolderAllAtOnce()
{
	std::string commonNameGeneratorsFileDirection = s_CommonNameGeneratorFileDirectory;
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonNameGeneratorsFileDirection = settings->m_commonNameGeneratorsFileDirection;
	}
	
	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(
		commonNameGeneratorsFileDirection, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadSingleNameGeneratorFromXMLFileLocation(fileStr);
	}
}

STATIC bool NameGenerator::OnUpdateReadInGeneratorsFromCommonNameGeneratorsFolder()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new NameGeneratorLoadingData();
	}

	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingData->m_readData)
	{
		s_loadingData->m_readData = true;
		if (settings != nullptr)
		{
			s_loadingData->m_fileDirectory = settings->m_commonNameGeneratorsFileDirection;
			s_loadingData->m_amountOfTimeToTakePerFrameLoadingNameGenerators 
				= settings->m_amountOfTimeLoadingNameGneratorsPerFrame;
		}
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(
			s_loadingData->m_fileDirectory, "*", true);
		return s_loadingData->m_files.size() == 0;
	}

	if (s_loadingData->m_amountOfTimeToTakePerFrameLoadingNameGenerators <= 0)
	{
		return true;
	}
	int numberReadThisFrame = 0;

	while (numberReadThisFrame < s_loadingData->m_amountOfTimeToTakePerFrameLoadingNameGenerators
		&& (size_t)s_loadingData->m_index < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
		ReadSingleNameGeneratorFromXMLFileLocation(fileStr);
		s_loadingData->m_index++;
		numberReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakePerFrameLoadingNameGenerators)
		{
			return false;
		}
	}
	return (size_t)s_loadingData->m_index >= s_loadingData->m_files.size();
}

STATIC void NameGenerator::ReadSingleNameGeneratorFromXMLFileLocation(const std::string& fileStr)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, s_NameGeneratorsRootNodeName, errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		{
			NameGenerator newGenerator = NameGenerator(child);
			if (newGenerator.m_generatorNameSet == false)
			{
				continue;
			}

			if (s_NameGenerators == nullptr)
			{
				s_NameGenerators = new std::map<std::string, NameGenerator>();
			}
			s_NameGenerators->insert(std::pair<std::string, NameGenerator>(
				MakeLower(newGenerator.m_generatorName), newGenerator));
		}
	}
}

STATIC const std::string NameGenerator::GlobalGenerateARandomName()
{
	if (s_NameGenerators == nullptr)
	{
		return s_DefaultName;
	}
	size_t numOfGenerators = s_NameGenerators->size();
	
	int ran = GetRandomIntLessThan(numOfGenerators);
	int start = 0;
	for (std::map<std::string, NameGenerator>::iterator it = s_NameGenerators->begin(); 
		it != s_NameGenerators->end(); it++)
	{
		if (start != ran)
		{
			start++;
			continue;
		}
		else
		{
			return it->second.GenerateRandomName(false);
		}
	}

	return s_DefaultName;

}

STATIC const std::string NameGenerator::GlobalGenerateARandomNameForGeneratorName(const std::string& genName)
{
	if (s_NameGenerators == nullptr)
	{
		return s_DefaultName;
	}
	std::string lowerCase = MakeLower(genName);

	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCase);
	if (it == s_NameGenerators->end())
	{
		return s_DefaultName;
	}
	else
	{
		return it->second.GenerateRandomName(false);
	}
}

STATIC const std::string NameGenerator::GlobalGenerateARandomNameForGeneratorNameByGender(
	const std::string& genName, bool isMale)
{
	if (s_NameGenerators == nullptr)
	{
		return s_DefaultName;
	}
	std::string lowerCase = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCase);
	if (it == s_NameGenerators->end())
	{
		return s_DefaultName;
	}
	else
	{
		return it->second.GenerateRandomName(true, isMale);
	}
}


STATIC const std::string NameGenerator::GlobalGenerateARandomNameFromDefault()
{
	return GlobalGenerateARandomNameForGeneratorName("Default");
}

STATIC const std::string NameGenerator::GlobalGenerateARandomNameFromDefaultByGender(bool isMale)
{
	return GlobalGenerateARandomNameForGeneratorNameByGender("Default", isMale);
}

STATIC const std::string NameGenerator::GlobalGetWhatReferMaleSiblingAs(const std::string& genName)
{
	if (s_NameGenerators == nullptr
		|| genName == "")
	{
		return s_CommonMaleSibling;
	}
	std::string lowerCaseName = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCaseName);
	if (it == s_NameGenerators->end())
	{
		return s_CommonMaleSibling;
	}
	return it->second.GetWhatToReferMaleSiblingAs();
}

STATIC const std::string NameGenerator::GlobalGetWhatReferFemaleSiblingAs(const std::string& genName)
{
	if (s_NameGenerators == nullptr
		|| genName == "")
	{
		return s_CommonFemaleSibling;
	}
	std::string lowerCaseName = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCaseName);
	if (it == s_NameGenerators->end())
	{
		return s_CommonFemaleSibling;
	}
	return it->second.GetWhatToReferFemaleSiblingAs();
}

STATIC const std::string NameGenerator::GlobalGetWhatReferMaleAs(const std::string& genName)
{
	if (s_NameGenerators == nullptr
		|| genName == "")
	{
		return s_CommonMaleReferAs;
	}
	std::string lowerCaseName = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCaseName);
	if (it == s_NameGenerators->end())
	{
		return s_CommonMaleReferAs;
	}
	return it->second.GetWhatToReferMalesAs();
}

STATIC const std::string NameGenerator::GlobalGetWhatReferFemaleAs(const std::string& genName)
{
	if (s_NameGenerators == nullptr
		|| genName == "")
	{
		return s_CommonFemaleReferAs;
	}
	std::string lowerCaseName = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCaseName);
	if (it == s_NameGenerators->end())
	{
		return s_CommonFemaleReferAs;
	}
	return it->second.GetWhatToReferFemaleAs();
}

STATIC const std::string NameGenerator::GlobalGetMaleOwningPronoun(const std::string& genName)
{
	if (s_NameGenerators == nullptr
		|| genName == "")
	{
		return s_CommonMaleOwningPronoun;
	}
	std::string lowerCaseName = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCaseName);
	if (it == s_NameGenerators->end())
	{
		return s_CommonMaleOwningPronoun;
	}
	return it->second.GetMaleOwningPronoun();
}

STATIC const std::string NameGenerator::GlobalGetFemaleOwningPronoun(const std::string& genName)
{
	if (s_NameGenerators == nullptr
		|| genName == "")
	{
		return s_CommonFemaleOwningPronoun;
	}
	std::string lowerCaseName = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCaseName);
	if (it == s_NameGenerators->end())
	{
		return s_CommonFemaleOwningPronoun;
	}
	return it->second.GetFemaleOwningPronoun();
}

STATIC const std::string NameGenerator::GlobalGetMaleIsPronoun(const std::string& genName)
{
	if (s_NameGenerators == nullptr
		|| genName == "")
	{
		return s_CommonMaleIsPronoun;
	}
	std::string lowerCaseName = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCaseName);
	if (it == s_NameGenerators->end())
	{
		return s_CommonMaleIsPronoun;
	}
	return it->second.GetMaleIsPronoun();
}

STATIC const std::string NameGenerator::GlobalGetFemaleIsPronoun(const std::string& genName)
{
	if (s_NameGenerators == nullptr
		|| genName == "")
	{
		return s_CommonFemaleIsPronoun;
	}
	std::string lowerCaseName = MakeLower(genName);
	std::map < std::string, NameGenerator>::iterator it = s_NameGenerators->find(lowerCaseName);
	if (it == s_NameGenerators->end())
	{
		return s_CommonFemaleIsPronoun;
	}
	return it->second.GetFemaleOwningPronoun();
}