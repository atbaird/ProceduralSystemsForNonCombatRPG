#pragma once

#ifndef NAMEGENERATOR_HPP
#define NAMEGENERATOR_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/GameEntities/Agents/Names/NameSet.hpp"
#include "Game/GameEntities/Agents/Names/OrderNameAndIndexPair.hpp"
#include <map>

struct NameGeneratorLoadingData
{
	std::vector<std::string> m_files;
	std::string m_fileDirectory = "Data/XML/NameGenerators/";
	std::string m_rootNodeName = "NameGenerators";
	std::string m_NodeName = "";
	int m_index = 0;
	double m_amountOfTimeToTakePerFrameLoadingNameGenerators = 0.25;
	bool m_readData = false;
};

class NameGenerator
{
private:
protected:
	//Static Variables
	static std::map<std::string, NameGenerator>* s_NameGenerators;
	static NameGeneratorLoadingData* s_loadingData;
	static std::string s_DefaultName;
	static const char* s_CommonNameGeneratorFileDirectory;
	static const char* s_NameGeneratorsRootNodeName;
	static constexpr char* s_CommonFemaleSibling = "Sister";
	static constexpr char* s_CommonMaleSibling = "Brother";
	static constexpr char* s_CommonFemaleIsPronoun = "She";
	static constexpr char* s_CommonMaleIsPronoun = "He";
	static constexpr char* s_CommonFemaleOwningPronoun = "Her";
	static constexpr char* s_CommonMaleOwningPronoun = "His";
	static constexpr char* s_CommonFemaleReferAs = "Female";
	static constexpr char* s_CommonMaleReferAs = "Male";

	//Variables
	std::vector<OrderNameAndIndexPair> m_setOrder;
	std::vector<OrderNameAndIndexPair> m_maleOrder;
	std::vector<OrderNameAndIndexPair> m_femaleOrder;
	std::vector<NameSet> m_nameSets;
	std::string m_generatorName = "";
	std::string m_whatToReferMaleAs = "Male";
	std::string m_whatToReferFemaleAs = "Female";
	std::string m_whatToReferMaleSiblingAs = "Brother";
	std::string m_whatToReferFemaleSiblingAs = "Sister";
	std::string m_maleOwningPronoun = "His";
	std::string m_femaleOwningPronoun = "Her";
	std::string m_maleIsPronoun = "He";
	std::string m_femaleIsPronoun = "She";
	bool m_specializedOrderForMale = false;
	bool m_specializedOrderForFemale = false;
	bool m_orderSet = false;
	bool m_generatorNameSet = false;

public:
	//Constructors
	NameGenerator();
	NameGenerator(const XMLNode& node);
	virtual ~NameGenerator();

	//Operations
	const std::string GenerateRandomName(bool careAboutGenderIfcan, bool genderIsMale = false) const;
	const std::string GenerateRandomNameFromSetOrder() const;
	const std::string GenerateRandomNameFromMaleOrder() const;
	const std::string GenerateRandomNameFromFemaleOrder() const;

	//Setters
	void SetGeneratorName(const std::string name);
	void SetOrder(const std::vector<std::string>& order);
	void SetFemaleOrder(const std::vector<std::string>& order);
	void SetMaleOrder(const std::vector<std::string>& order);
	void SetNameSets(const std::vector<NameSet>& sets);
	void AddNameSet(const NameSet& nameSet);
	void RecalcSetOrderIndexes();
	void RecalcFemaleOrderIndexes();
	void RecalcMaleOrderIndexes();

	//Getters
	const std::vector<OrderNameAndIndexPair>& GetSetOrder() const;
	const std::vector<OrderNameAndIndexPair>& GetFemaleSetOrder() const;
	const std::vector<OrderNameAndIndexPair>& GetMaleSetOrder() const;
	const std::vector<NameSet>& GetNameSet() const;
	const std::string& GetGeneratorName() const;
	const std::string& GetWhatToReferMalesAs() const;
	const std::string& GetWhatToReferFemaleAs() const;
	const std::string& GetWhatToReferMaleSiblingAs() const;
	const std::string& GetWhatToReferFemaleSiblingAs() const;
	const std::string& GetMaleOwningPronoun() const;
	const std::string& GetFemaleOwningPronoun() const;
	const std::string& GetMaleIsPronoun() const;
	const std::string& GetFemaleIsPronoun() const;
	bool GetSpecializedOrderForMale() const;
	bool GetSpecializedOrderForFemale() const;
	bool GetOrderSet() const;
	bool GetGeneratorNameSet() const;
	void WriteNameGeneratorToString(std::string& str, int indentationAmt = 0) const;

	//Static Methods
	static void WriteAllNameGeneratorsToString(std::string& str, int indentationAmt = 0);
	static void ClearAllGenerators();
	static void ReadInGeneratorsFromCommonNameGeneratorsFolderAllAtOnce();
	static bool OnUpdateReadInGeneratorsFromCommonNameGeneratorsFolder();
protected:
	static void ReadSingleNameGeneratorFromXMLFileLocation(const std::string& fileStr);
public:
	static const std::string GlobalGenerateARandomName();
	static const std::string GlobalGenerateARandomNameForGeneratorName(const std::string& genName);
	static const std::string GlobalGenerateARandomNameForGeneratorNameByGender(const std::string& genName, bool isMale);
	static const std::string GlobalGenerateARandomNameFromDefault();
	static const std::string GlobalGenerateARandomNameFromDefaultByGender(bool isMale);
	static const std::string GlobalGetWhatReferMaleSiblingAs(const std::string& genName);
	static const std::string GlobalGetWhatReferFemaleSiblingAs(const std::string& genName);
	static const std::string GlobalGetWhatReferMaleAs(const std::string& genName);
	static const std::string GlobalGetWhatReferFemaleAs(const std::string& genName);
	static const std::string GlobalGetMaleOwningPronoun(const std::string& genName);
	static const std::string GlobalGetFemaleOwningPronoun(const std::string& genName);
	static const std::string GlobalGetMaleIsPronoun(const std::string& genName);
	static const std::string GlobalGetFemaleIsPronoun(const std::string& genName);
};

#endif