#pragma once

// Engine window flag structure.
union WindowFlags
{
	struct {
		char fullscreen : 1;
		char borderless : 1;
		char verticalSync : 1;
	} bits;
	char raw;
};