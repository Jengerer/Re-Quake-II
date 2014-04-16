#ifndef _COMMAND_H_
#define _COMMAND_H_

// Enum for descriping parameter types.
typedef enum command_parameter_type
{
	INVALID_TYPE = -1,
	INTEGER_PARAMETER,
	FLOAT_PARAMETER,
	STRING_PARAMETER
} command_parameter_type_t;

// Struct for representing a command parameter.
typedef struct command_parameter
{
	const char *name;
	command_parameter_type_t type;
} command_parameter_t;

// Struct for representing a user command.
typedef struct command
{
	// Command schema.
	const char *name;
	const char *description;
	const command_parameter_t *parameters;
	int num_parameters;
} command_t;

// Command and argument declaration.
void define_command(const char *name,
	const char *description,
	const command_parameter_t *parameters,
	int num_parameters,
	command_t *out);
void define_command_parameter(const char *name,
	command_parameter_type_t type,
	command_t *out);

#endif // _COMMAND_H_
