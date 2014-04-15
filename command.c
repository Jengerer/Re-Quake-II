#include "command.h"

/*
 * Fill out a command struct with a command definition.
 */
void define_command(const char *name,
	const char *description,
	const command_parameter_t *parameters,
	int num_parameters,
	command_t *out)
{
	out->name = name;
	out->description = description;
	out->parameters = parameters;
	out->num_parameters = num_parameters;
}

/*
 * Fill out the command parameter struct with definition.
 */
void define_command_parameter(const char *name,
	command_parameter_type_t type,
	command_t *out)
{
	out->name = name;
	out->type = type;
}

