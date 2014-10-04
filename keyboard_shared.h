#pragma once

// Keyboard state types and flags.
typedef unsigned char KeyCode;
union KeyState
{
	struct {
		char pressed : 1;
		char changed : 1;
		char padding : 6;
	} bits;
	char asCharacter;
};

// Keyboard key values.
enum EngineKey
{
	EngineKeyInvalid = 0,

	// Alphanumeric characters map to lower case ASCII values.
	EngineKeyA = 65,
	EngineKeyB = 66,
	EngineKeyC = 67,
	EngineKeyD = 68,
	EngineKeyE = 69,
	EngineKeyF = 70,
	EngineKeyG = 71,
	EngineKeyH = 72,
	EngineKeyI = 73,
	EngineKeyJ = 74,
	EngineKeyK = 75,
	EngineKeyL = 76,
	EngineKeyM = 77,
	EngineKeyN = 78,
	EngineKeyO = 79,
	EngineKeyP = 80,
	EngineKeyQ = 81,
	EngineKeyR = 82,
	EngineKeyS = 83,
	EngineKeyT = 84,
	EngineKeyU = 85,
	EngineKeyV = 86,
	EngineKeyW = 87,
	EngineKeyX = 88,
	EngineKeyY = 89,
	EngineKeyZ = 90,
	EngineKeyMax = 255
};