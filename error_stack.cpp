#include "error_stack.h"
#include "memory_manager.h"
#include <stdarg.h>
#include <stdio.h>

// Error stack head.
ErrorStackNode *ErrorStack::head;

// Set up error stack node.
ErrorStackNode::ErrorStackNode(char *message, ErrorStackNode *next)
	: message(message), next(next)
{
}

// Destroy error stack node.
ErrorStackNode::~ErrorStackNode()
{
	MemoryManager::Free(message);
}

// Set up empty error stack.
void ErrorStack::Initialize()
{
	head = nullptr;
}

// Free all error stack elements.
void ErrorStack::Shutdown()
{
	Clear();
}

// Log an error message to the stack.
void ErrorStack::Log(const char *format, ...)
{
	// Get size of string.
	va_list args;
	va_list args_copy;
	va_start(args, format);
	va_copy(args_copy, args);
	int length = vsnprintf(nullptr, 0, format, args_copy);
	va_end(args_copy);
	if (length < 0) {
		return;
	}
	
	// Allocate space and copy the string.
	char *buffer = reinterpret_cast<char*>(MemoryManager::Allocate(length + 1));
	if (buffer == nullptr) {
		return;
	}
	int written = vsnprintf(buffer, length, format, args);
	va_end(args);
	if (written < 0) {
		MemoryManager::Free(buffer);
		return;
	}
	buffer[length] = '\0';

	// Allocate a stack node.
	ErrorStackNode *node;
	if (!MemoryManager::Allocate(&node)) {}
	node = reinterpret_cast<ErrorStackNode*>(MemoryManager::Allocate(sizeof(ErrorStackNode)));
	if (node == NULL) {
		MemoryManager::Free(buffer);
		return;
	}

	// Set this as new head.
	new (node) ErrorStackNode(buffer, head);
	head = node;
}

// Dump all errors.
void ErrorStack::Dump()
{
	int index = 1;
	for (ErrorStackNode *node = head; node != nullptr; node = node->GetNext(), ++index) {
		fprintf(stderr, "#%d: %s\n", index, node->GetMessage());
	}
}

// Clear all errors in the stack.
void ErrorStack::Clear()
{
	ErrorStackNode *node = head;
	while (node != nullptr) {
		ErrorStackNode *next = node->GetNext();
		MemoryManager::Destroy(node);
		node = next;
	}
}
