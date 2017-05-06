#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/IntVector2.hpp"





EngineXMLParser::EngineXMLParser()
{

}

EngineXMLParser::~EngineXMLParser()
{

}

STATIC const XMLNode EngineXMLParser::ParseXMLFile(const std::string& xmlFileLocation, const char* tag, bool& errorOccurred)
{
	if (FileUtils::VerifyFileExists(xmlFileLocation) == false)
	{
		errorOccurred = true;
		return XMLNode();
	}
	XMLResults result;
	XMLNode node = XMLNode::parseFile(xmlFileLocation.c_str(), tag, &result);
	if (result.error != eXMLErrorNone)
	{
		//DebuggerPrintf("\nFailed to parse XML File");
		errorOccurred = (true);
		return node;
	}
	errorOccurred = (false);
	return node;
}

STATIC const XMLNode EngineXMLParser::TraverseDownNodeChildren(const XMLNode& node, std::vector<std::string> nodeNames, bool& successfullyFound)
{
	XMLNode current = node;
	int numberOfChildren = current.nChildNode();

	for (int i = 0; i < (int)nodeNames.size() && numberOfChildren > 0; i++)
	{
		XMLNode child = current.getChildNodeByPath(nodeNames.at(i).c_str());
		numberOfChildren = child.nChildNode();
		if (numberOfChildren <= 0)
		{
			successfullyFound = false;
			return child;
		}
		current = child;
	}

	successfullyFound = true;
	return current;

}

STATIC XMLNodeWrapper* EngineXMLParser::WrapUpNodeAndRegister(const XMLNode& node, int idToListenForWhenUnregistering)
{
	return XMLNodeWrapper::FullyRegisterXMLNodeIntoTree(node, idToListenForWhenUnregistering);
}

STATIC void EngineXMLParser::CleanUpAndRemoveAllRegisteredWrapperNodes()
{
	XMLNodeWrapper::EmptyAllRegisteredXMLNodes();
}

STATIC void EngineXMLParser::CleanUpAndRemoveAllRegisteredWrapperNodesBasedOnID(int id)
{
	XMLNodeWrapper::EmptyAllRegisteredXMLNodesThatHaveID(id);
}

STATIC std::vector<StrXMLAttr> EngineXMLParser::GetAttributesOffOfNode(const XMLNode& node)
{
	std::vector<StrXMLAttr> attributes;

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute xmlAttr = node.getAttribute(attrIdx);
		StrXMLAttr attr = StrXMLAttr("" + std::string(xmlAttr.lpszName), "" + std::string(xmlAttr.lpszValue));
		attributes.push_back(attr);
	}

	return attributes;
}

STATIC std::string EngineXMLParser::GetAttributeOffNodeAsString(const XMLNode& node, const char* attributeName,
	const std::string& defaultVal)
{
	if (attributeName == nullptr)
	{
		return defaultVal;
	}
	if (node.isAttributeSet(attributeName) == false)
	{
		return defaultVal;
	}

	const char* attrVal = node.getAttribute(attributeName);
	if (attrVal == nullptr)
	{
		return defaultVal;
	}
	return std::string(attrVal);
}

STATIC Vector2 EngineXMLParser::ParseXMLAttributeForVector2(const XMLAttribute& attr, 
	bool& sizeOf1Found, bool& sizeOf2Found)
{
	sizeOf1Found = false;
	sizeOf2Found = false;
	Vector2 vec2 = Vector2::vec2_zeros;
	std::string val = ReplaceCharInString(attr.lpszValue, '-', ',');
	std::vector<std::string> parsed = ParseString(val, ',');
	if (parsed.size() > 0)
	{
		vec2.x = (float)atof(parsed[0].c_str());
		sizeOf1Found = true;
	}
	if (parsed.size() > 1)
	{
		vec2.y = (float)atof(parsed[1].c_str());
		sizeOf2Found = true;
	}
	return vec2;
}


STATIC Vector2 EngineXMLParser::ParseXMLAttributeForVector2(const XMLAttribute& attr, bool& sizeOf1Found)
{
	bool sizeOf2Found = false;
	return ParseXMLAttributeForVector2(attr, sizeOf1Found, sizeOf2Found);
}

STATIC IntVector2 EngineXMLParser::ParseXMLAttributeForIntVector2(const XMLAttribute& attr,
	bool& sizeOf1Found, bool& sizeOf2Found)
{
	sizeOf1Found = false;
	sizeOf2Found = false;
	IntVector2 vec2 = IntVector2::ivec2_zeros;
	std::string val = ReplaceCharInString(attr.lpszValue, '-', ',');
	std::vector<std::string> parsed = ParseString(val, ',');
	if (parsed.size() > 0)
	{
		vec2.x = atoi(parsed[0].c_str());
		sizeOf1Found = true;
	}
	if (parsed.size() > 1)
	{
		vec2.y = atoi(parsed[1].c_str());
		sizeOf2Found = true;
	}
	return vec2;
}

STATIC IntVector2 EngineXMLParser::ParseXMLAttributeForIntVector2(const XMLAttribute& attr, bool& sizeOf1Found)
{
	bool sizeOf2Found = false;
	return ParseXMLAttributeForIntVector2(attr, sizeOf1Found, sizeOf2Found);
}