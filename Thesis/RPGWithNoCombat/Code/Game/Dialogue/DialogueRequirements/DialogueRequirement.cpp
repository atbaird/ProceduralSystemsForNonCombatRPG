#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Constructors
DialogueRequirement::DialogueRequirement()
{

}

DialogueRequirement::DialogueRequirement(const XMLNode& node)
{
	node;
}

DialogueRequirement::DialogueRequirement(const DialogueRequirement& other)
{
	other;
}

DialogueRequirement::~DialogueRequirement()
{

}