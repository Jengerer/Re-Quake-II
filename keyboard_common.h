#ifndef _KEYBOARD_COMMON_H_
#define _KEYBOARD_COMMON_H_

// Keyboard state types and flags.
typedef unsigned char key_code_t;
typedef union key_state
{
	struct {
		int pressed : 1;
		int changed : 1;
		int padding : 6;
	} flags;
	unsigned char as_char;
} key_state_t;
#define MASK_KEY_PRESSED (KEY_DOWN | KEY_CHANGED)
#define MASK_KEY_RELEASED (KEY_CHANGED)

// Keyboard key values.
// Alphabetical characters correspond to lower case ASCII values.
#define ENGINE_KEY_INVALID 0
#define ENGINE_KEY_A 65
#define ENGINE_KEY_B 66
#define ENGINE_KEY_C 67
#define ENGINE_KEY_D 68
#define ENGINE_KEY_E 69
#define ENGINE_KEY_F 70
#define ENGINE_KEY_G 71
#define ENGINE_KEY_H 72
#define ENGINE_KEY_I 73
#define ENGINE_KEY_J 74
#define ENGINE_KEY_K 75
#define ENGINE_KEY_L 76
#define ENGINE_KEY_M 77
#define ENGINE_KEY_N 78
#define ENGINE_KEY_O 79
#define ENGINE_KEY_P 80
#define ENGINE_KEY_Q 81
#define ENGINE_KEY_R 82
#define ENGINE_KEY_S 83
#define ENGINE_KEY_T 84
#define ENGINE_KEY_U 85
#define ENGINE_KEY_V 86
#define ENGINE_KEY_W 87
#define ENGINE_KEY_X 88
#define ENGINE_KEY_Y 89
#define ENGINE_KEY_Z 90
#define ENGINE_KEY_MAX 256

#endif // _KEYBOARD_COMMON_H_