#include "game_module.h"

namespace GameManager
{

	Module::Module()
		: utilities(nullptr)
	{
	}

	// Set utilities interface.
	void Module::SetUtilities(GameManager::Utilities *utilities)
	{
		this->utilities = utilities;
	}

}