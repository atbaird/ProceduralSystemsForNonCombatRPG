#pragma once

#ifndef DEBUGGINGFILEHANDLER_HPP
#define DEBUGGINGFILEHANDLER_HPP
#include <stdio.h>
#include <string>
//#include <fstream>

#define STATIC

class DebuggingFileHandler
{
private:
protected:
	//Static Variables
	static constexpr char* s_DefaultFileExtention = ".txt";

	//Variables
	FILE* m_file = nullptr;
	FILE* m_fileWithTimeStamp = nullptr;
public:
	//Constructors
	DebuggingFileHandler();
	virtual ~DebuggingFileHandler();

	//Operations
	bool Open(const std::string& folderToPlaceIn, const char* fileNameWithoutExtention,
		const char* fileExtention);
	bool Open(const std::string& folderToPlaceIn, const std::string& fileNameWithoutExtention,
		const char* fileExtention);
	bool Open(const char* folderToPlaceIn, const std::string& fileNameWithoutExtention,
		const char* fileExtention);
	bool Open(const char* folderToPlaceIn, const char* fileNameWithoutExtention,
		const char* fileExtention = s_DefaultFileExtention);
	bool Open();
	void Write(const std::string& str);
	void Write(const char* str);
	void Close();

	//Getters
	bool GetIsFileOpen() const;
	bool GetIsFileWithTimeStampOpen() const;
	const FILE* GetFile() const;
	const FILE* GetFileWithTimeStamp() const;
	FILE* GetEditableFile();
	FILE* GetEditableFileWithTimeStamp();

	//Static functions
	static std::string GetCurrentTimeAndDateAsString();
};

#endif