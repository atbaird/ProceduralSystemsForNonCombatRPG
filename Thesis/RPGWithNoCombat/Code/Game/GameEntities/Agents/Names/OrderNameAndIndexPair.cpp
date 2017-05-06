#include "Game/GameEntities/Agents/Names/OrderNameAndIndexPair.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"


//Constructors
OrderNameAndIndexPair::OrderNameAndIndexPair(const std::string& name)
	: m_orderName(MakeLower(name))
{

}


void OrderNameAndIndexPair::WriteOrderNameAndIndexPairToString(std::string& str, int indentationAmt) const
{
	indentationAmt;
	//GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += m_orderName;
}