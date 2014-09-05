#ifndef _ERROR_STACK_H
#define _ERROR_STACK_H_

// Node for an error with intrusive list nodes.
typedef struct error_stack_node
{
	char *error;
	
	// Intrusive list node.
	struct error_stack_node *next;
} error_stack_node_t;

// Structure for keeping track of a stack of errors.
typedef struct error_stack
{
	error_stack_node_t *head;
} error_stack_t;

// Error stack node initialization and destruction.
void error_stack_node_null(error_stack_node_t *node);
void error_stack_node_destroy(error_stack_node_t *node);

// Error stack initialization and destruction.
void error_stack_null(error_stack_t *stack);
void error_stack_initialize(void);
void error_stack_destroy(error_stack_t *stack);

// Error stack logging and dumping.
void error_stack_log(const char *format, ...);
void error_stack_dump(void);

#endif // _ERROR_STACK_H_