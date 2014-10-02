#pragma once

// Error stack node.
class ErrorStackNode
{

public:

	ErrorStackNode(char *message);
	~ErrorStackNode();

	// Get error stack message.
	const char *GetMessage() const;

private:

	char *message;

	// Next node in the list.
	ErrorStackNode *next;

};

// Static class for reporting errors.
class ErrorStack
{

public:

// Error stack initialization and destruction.

	static void Initialize();
	static void Shutdown();

// Error logging and reporting.

	static void Log(const char *format, ...);
	static void Dump();
	static void Clear();

};