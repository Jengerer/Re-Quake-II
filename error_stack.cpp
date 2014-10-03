#include "error_stack.h"
#include "memory_manager.h"
#include <stdarg.h>
#include <stdio.h>

// Static instance of error stack.
static error_stack_t stack;

/* Null error stack node for clean destruction. */
void error_stack_node_null(error_stack_node_t *node)
{
	node->error = NULL;
	node->next = NULL;
}

/* Destroy error stack node. */
void error_stack_node_destroy(error_stack_node_t *node)
{
	char *buffer;

	// Clean up string if any.
	buffer = node->error;
	if (buffer != NULL) {
		memory_free(buffer);
	}
	error_stack_node_null(node);
}

/* Null error stack for clean destruction. */
void error_stack_null(error_stack_t *stack)
{
	stack->head = NULL;
}

/* Initialize the global error stack. */
void error_stack_initialize(void)
{
	error_stack_null(&stack);
}

/* Clean up an error stack. */
void error_stack_destroy(error_stack_t *stack)
{
	error_stack_node_t *node;

	// Clean up nodes.
	node = stack->head;
	while (node != NULL) {
		error_stack_node_t *next;

		// Get next and clean up.
		next = node->next;
		error_stack_node_destroy(node);
		memory_free(node);
		node = next;
	}
	error_stack_null(stack);
}

/*
 * Log a string to the error stack.
 * No return value since we can't do anything more if it fails.
 */
void error_stack_log(const char *format, ...)
{
	va_list args;
	va_list args_copy;
	char *buffer;
	int length, written;
	error_stack_node_t *node;

	// Get size of string.
	va_start(args, format);
	va_copy(args_copy, args);
	length = vsnprintf(NULL, 0, format, args_copy);
	va_end(args_copy);
	if (length < 0) {
		return;
	}
	
	// Allocate space and copy the string.
	buffer = memory_array_allocate(sizeof(char), length + 1);
	if (buffer == NULL) {
		return;
	}
	written = vsnprintf(buffer, length, format, args);
	va_end(args);
	if (written < 0) {
		memory_free(buffer);
		return;
	}
	buffer[length] = '\0';

	// Allocate a stack node.
	node = (error_stack_node_t*)memory_allocate(sizeof(error_stack_node_t));
	if (node == NULL) {
		memory_free(buffer);
		return;
	}
	node->error = buffer;
	node->next = stack.head;
	stack.head = node;
}

/* Dump the errors to standard output. */
void error_stack_dump(void)
{

}