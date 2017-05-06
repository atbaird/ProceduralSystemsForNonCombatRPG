#include "Game/Debugging/DebuggingFileHandler.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/GameSettings.hpp"
#include "Game/TheGame.hpp"
#include <ctime>

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Constructors
DebuggingFileHandler::DebuggingFileHandler()
{

}

DebuggingFileHandler::~DebuggingFileHandler()
{
	Close();
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Operations
bool DebuggingFileHandler::Open(const std::string& folderToPlaceIn, const char* fileNameWithoutExtention,
	const char* fileExtention)
{
	return Open(folderToPlaceIn.c_str(), fileNameWithoutExtention, fileExtention);
}

bool DebuggingFileHandler::Open(const std::string& folderToPlaceIn, const std::string& fileNameWithoutExtention,
	const char* fileExtention)
{
	return Open(folderToPlaceIn.c_str(), fileNameWithoutExtention.c_str(), fileExtention);
}

bool DebuggingFileHandler::Open(const char* folderToPlaceIn, const std::string& fileNameWithoutExtention,
	const char* fileExtention)
{
	return Open(folderToPlaceIn, fileNameWithoutExtention.c_str(), fileExtention);
}

bool DebuggingFileHandler::Open(const char* folderToPlaceIn, const char* fileNameWithoutExtention,
	const char* fileExtention)
{
	if (folderToPlaceIn == nullptr
		|| fileNameWithoutExtention == nullptr
		|| fileExtention == nullptr)
	{
		return false;
	}

	if (m_file != nullptr
		|| m_fileWithTimeStamp != nullptr)
	{
		return false;
	}


	std::string firstHalfOfFileLocation = std::string(folderToPlaceIn) + std::string(fileNameWithoutExtention);
	std::string timeStamp = GetCurrentTimeAndDateAsString();
	std::string fileExt = std::string(fileExtention);
	std::string fileOpen = firstHalfOfFileLocation + fileExt;
	std::string timeStampOpen = firstHalfOfFileLocation + timeStamp + fileExt;
	m_file = fopen(fileOpen.c_str(), "w");
	if (m_file == nullptr)
	{
		return false;
	}
	m_fileWithTimeStamp = fopen(timeStampOpen.c_str(), "w");
	if (m_fileWithTimeStamp == nullptr)
	{
		return false;
	}
	return true;
}

bool DebuggingFileHandler::Open()
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
	{
		return false;
	}
	return Open(settings->m_DebugReportFolderLocation.c_str(),
		settings->m_DebugReportFileNameWithoutExtention.c_str());
}


void DebuggingFileHandler::Write(const std::string& str)
{
	Write(str.c_str());
}

void DebuggingFileHandler::Write(const char* str)
{
	if (str == nullptr)
	{
		return;
	}

	if (m_file != nullptr)
	{
		fwrite(str, sizeof(const char), strlen(str), m_file);
		//m_file->write(str, strlen(str));
	}
	if (m_fileWithTimeStamp != nullptr)
	{
		fwrite(str, sizeof(const char), strlen(str), m_fileWithTimeStamp);
		//m_fileWithTimeStamp->write(str, strlen(str));
	}
}

void DebuggingFileHandler::Close()
{
	if (m_file != nullptr)
	{
		fclose(m_file);
		m_file = nullptr;
	}
	if (m_fileWithTimeStamp != nullptr)
	{
		fclose(m_fileWithTimeStamp);
		m_fileWithTimeStamp = nullptr;
	}
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Getters
bool DebuggingFileHandler::GetIsFileOpen() const
{
	return m_file != nullptr;
}

bool DebuggingFileHandler::GetIsFileWithTimeStampOpen() const
{
	return m_fileWithTimeStamp != nullptr;
}

const FILE* DebuggingFileHandler::GetFile() const
{
	return m_file;
}

const FILE* DebuggingFileHandler::GetFileWithTimeStamp() const
{
	return m_fileWithTimeStamp;
}

FILE* DebuggingFileHandler::GetEditableFile()
{
	return m_file;
}

FILE* DebuggingFileHandler::GetEditableFileWithTimeStamp()
{
	return m_fileWithTimeStamp;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Static functions
STATIC std::string DebuggingFileHandler::GetCurrentTimeAndDateAsString()
{
	std::string time_out;
	time_t time_val = time(0);
	struct tm* now = localtime(&time_val);
	
	//Date
	time_out += "_" + std::to_string(now->tm_mon + 1) + "_";
	time_out += std::to_string(now->tm_mday) + "_";
	time_out += std::to_string(now->tm_year + +1900) + "__";

	//Time
	time_out += std::to_string(now->tm_hour) + "_";
	time_out += std::to_string(now->tm_min) + "_";
	time_out += std::to_string(now->tm_sec) + "_";

	return time_out;
}