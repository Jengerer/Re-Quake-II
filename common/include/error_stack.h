#pragma once

#include "common_define.h"

// Error stack node.
class ErrorStackNode
{

public:

	ErrorStackNode(char *message, ErrorStackNode *next);
	~ErrorStackNode();
	inline const char *GetMessage() const;
	inline ErrorStackNode *GetNext() const;

private:

	char *message;

	// Next node in the list.
	ErrorStackNode *next;

};

// Static class for reporting errors.
class CommonLibrary ErrorStack
{

public:

	static void Initialize();
	static void Shutdown();
	static void Log(const char *format, ...);

	// Print all errors to standard error.
	static void Dump();

	// Clear errors.
	static void Clear();

private:

	static ErrorStackNode *head;

};

const char *ErrorStackNode::GetMessage() const
{
	return message;
}

ErrorStackNode *ErrorStackNode::GetNext() const
{
	return next;
}