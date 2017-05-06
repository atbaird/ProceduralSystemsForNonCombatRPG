#pragma once

#ifndef QUESTTRIGGERCOMPLETEQUEST_HPP
#define QUESTTRIGGERCOMPLETEQUEST_HPP
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerCompleteQuestsFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultCompleteQuestsFunc();

class QuestTriggerCompleteQuest : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_CompleteQuestsRegistration;
public:
	//Constructors
	QuestTriggerCompleteQuest();
	QuestTriggerCompleteQuest(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerCompleteQuest(const QuestTriggerCompleteQuest& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerCompleteQuest();

	//Setters

	//Getters

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};
#endif