/***************************************************************************
 *                                                                         *
 *   (c) Art Tevs, MPI Informatik Saarbruecken                             *
 *       mailto: <tevs@mpi-sb.mpg.de>                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *    This definitions was get from SDL-Keysym. So you can directly map    *
 *    indices of nrEngine keys into SDL's own. I choosed this definition   *
 *    because of clearly defined key binding to ASCII-code.                *
 *    You can say: Why do this guy just not include sdl_keysym.h file. So my*
 *    answer is: Because we want to stay independet with our nrEngine.     *
 *    If you want to write new framework work class for our engine like    *
 *    GLFW-OpenGL-Framework, SDL, GLUT(OK not so great :-)) and so on, you *
 *    have to map keybindings from this engines to nrEngine owns. For some *
 *    not well written engines it can produce performance leck. Do it better*
 *    if you know how.                                                     *
 **************************************************************************/
 
#ifndef __NR_KEY_SYMS_H_
#define __NR_KEY_SYMS_H_

/**
 * What we really want is a mapping of every raw key on the keyboard.
 *  To support international keyboards, we use the range 0xA1 - 0xFF
 *  as international virtual keycodes.  We'll follow in the footsteps of X11...
 *  The names of the keys
 * \ingroup gp
 */
 
typedef enum _nrKeyIndex {
	/* The keyboard syms have been cleverly chosen to map to ASCII */
	NRKEY_UNKNOWN		= 0,
	NRKEY_FIRST			= 0,
	NRKEY_BACKSPACE		= 8,
	NRKEY_TAB			= 9,
	NRKEY_CLEAR			= 12,
	NRKEY_RETURN		= 13,
	NRKEY_PAUSE			= 19,
	NRKEY_ESCAPE		= 27,
	NRKEY_SPACE			= 32,
	NRKEY_EXCLAIM		= 33,
	NRKEY_QUOTEDBL		= 34,
	NRKEY_HASH			= 35,
	NRKEY_DOLLAR		= 36,
	NRKEY_AMPERSAND		= 38,
	NRKEY_QUOTE			= 39,
	NRKEY_LEFTPAREN		= 40,
	NRKEY_RIGHTPAREN	= 41,
	NRKEY_ASTERISK		= 42,
	NRKEY_PLUS			= 43,
	NRKEY_COMMA			= 44,
	NRKEY_MINUS			= 45,
	NRKEY_PERIOD		= 46,
	NRKEY_SLASH			= 47,
	NRKEY_0				= 48,
	NRKEY_1				= 49,
	NRKEY_2				= 50,
	NRKEY_3				= 51,
	NRKEY_4				= 52,
	NRKEY_5				= 53,
	NRKEY_6				= 54,
	NRKEY_7				= 55,
	NRKEY_8				= 56,
	NRKEY_9				= 57,
	NRKEY_COLON			= 58,
	NRKEY_SEMICOLON		= 59,
	NRKEY_LESS			= 60,
	NRKEY_EQUALS		= 61,
	NRKEY_GREATER		= 62,
	NRKEY_QUESTION		= 63,
	NRKEY_AT			= 64,
	
	/* 
	   Skip uppercase letters
	 */
	NRKEY_LEFTBRACKET	= 91,
	NRKEY_BACKSLASH		= 92,
	NRKEY_RIGHTBRACKET	= 93,
	NRKEY_CARET			= 94,
	NRKEY_UNDERSCORE	= 95,
	NRKEY_BACKQUOTE		= 96,
	NRKEY_a				= 97,
	NRKEY_b				= 98,
	NRKEY_c				= 99,
	NRKEY_d				= 100,
	NRKEY_e				= 101,
	NRKEY_f				= 102,
	NRKEY_g				= 103,
	NRKEY_h				= 104,
	NRKEY_i				= 105,
	NRKEY_j				= 106,
	NRKEY_k				= 107,
	NRKEY_l				= 108,
	NRKEY_m				= 109,
	NRKEY_n				= 110,
	NRKEY_o				= 111,
	NRKEY_p				= 112,
	NRKEY_q				= 113,
	NRKEY_r				= 114,
	NRKEY_s				= 115,
	NRKEY_t				= 116,
	NRKEY_u				= 117,
	NRKEY_v				= 118,
	NRKEY_w				= 119,
	NRKEY_x				= 120,
	NRKEY_y				= 121,
	NRKEY_z				= 122,
	NRKEY_DELETE		= 127,
	/* End of ASCII mapped keysyms */

	
	/* International keyboard syms */
	NRKEY_WORLD_0		= 160,		/* 0xA0 */
	NRKEY_WORLD_1		= 161,
	NRKEY_WORLD_2		= 162,
	NRKEY_WORLD_3		= 163,
	NRKEY_WORLD_4		= 164,
	NRKEY_WORLD_5		= 165,
	NRKEY_WORLD_6		= 166,
	NRKEY_WORLD_7		= 167,
	NRKEY_WORLD_8		= 168,
	NRKEY_WORLD_9		= 169,
	NRKEY_WORLD_10		= 170,
	NRKEY_WORLD_11		= 171,
	NRKEY_WORLD_12		= 172,
	NRKEY_WORLD_13		= 173,
	NRKEY_WORLD_14		= 174,
	NRKEY_WORLD_15		= 175,
	NRKEY_WORLD_16		= 176,
	NRKEY_WORLD_17		= 177,
	NRKEY_WORLD_18		= 178,
	NRKEY_WORLD_19		= 179,
	NRKEY_WORLD_20		= 180,
	NRKEY_WORLD_21		= 181,
	NRKEY_WORLD_22		= 182,
	NRKEY_WORLD_23		= 183,
	NRKEY_WORLD_24		= 184,
	NRKEY_WORLD_25		= 185,
	NRKEY_WORLD_26		= 186,
	NRKEY_WORLD_27		= 187,
	NRKEY_WORLD_28		= 188,
	NRKEY_WORLD_29		= 189,
	NRKEY_WORLD_30		= 190,
	NRKEY_WORLD_31		= 191,
	NRKEY_WORLD_32		= 192,
	NRKEY_WORLD_33		= 193,
	NRKEY_WORLD_34		= 194,
	NRKEY_WORLD_35		= 195,
	NRKEY_WORLD_36		= 196,
	NRKEY_WORLD_37		= 197,
	NRKEY_WORLD_38		= 198,
	NRKEY_WORLD_39		= 199,
	NRKEY_WORLD_40		= 200,
	NRKEY_WORLD_41		= 201,
	NRKEY_WORLD_42		= 202,
	NRKEY_WORLD_43		= 203,
	NRKEY_WORLD_44		= 204,
	NRKEY_WORLD_45		= 205,
	NRKEY_WORLD_46		= 206,
	NRKEY_WORLD_47		= 207,
	NRKEY_WORLD_48		= 208,
	NRKEY_WORLD_49		= 209,
	NRKEY_WORLD_50		= 210,
	NRKEY_WORLD_51		= 211,
	NRKEY_WORLD_52		= 212,
	NRKEY_WORLD_53		= 213,
	NRKEY_WORLD_54		= 214,
	NRKEY_WORLD_55		= 215,
	NRKEY_WORLD_56		= 216,
	NRKEY_WORLD_57		= 217,
	NRKEY_WORLD_58		= 218,
	NRKEY_WORLD_59		= 219,
	NRKEY_WORLD_60		= 220,
	NRKEY_WORLD_61		= 221,
	NRKEY_WORLD_62		= 222,
	NRKEY_WORLD_63		= 223,
	NRKEY_WORLD_64		= 224,
	NRKEY_WORLD_65		= 225,
	NRKEY_WORLD_66		= 226,
	NRKEY_WORLD_67		= 227,
	NRKEY_WORLD_68		= 228,
	NRKEY_WORLD_69		= 229,
	NRKEY_WORLD_70		= 230,
	NRKEY_WORLD_71		= 231,
	NRKEY_WORLD_72		= 232,
	NRKEY_WORLD_73		= 233,
	NRKEY_WORLD_74		= 234,
	NRKEY_WORLD_75		= 235,
	NRKEY_WORLD_76		= 236,
	NRKEY_WORLD_77		= 237,
	NRKEY_WORLD_78		= 238,
	NRKEY_WORLD_79		= 239,
	NRKEY_WORLD_80		= 240,
	NRKEY_WORLD_81		= 241,
	NRKEY_WORLD_82		= 242,
	NRKEY_WORLD_83		= 243,
	NRKEY_WORLD_84		= 244,
	NRKEY_WORLD_85		= 245,
	NRKEY_WORLD_86		= 246,
	NRKEY_WORLD_87		= 247,
	NRKEY_WORLD_88		= 248,
	NRKEY_WORLD_89		= 249,
	NRKEY_WORLD_90		= 250,
	NRKEY_WORLD_91		= 251,
	NRKEY_WORLD_92		= 252,
	NRKEY_WORLD_93		= 253,
	NRKEY_WORLD_94		= 254,
	NRKEY_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	NRKEY_KP0		= 256,
	NRKEY_KP1		= 257,
	NRKEY_KP2		= 258,
	NRKEY_KP3		= 259,
	NRKEY_KP4		= 260,
	NRKEY_KP5		= 261,
	NRKEY_KP6		= 262,
	NRKEY_KP7		= 263,
	NRKEY_KP8		= 264,
	NRKEY_KP9		= 265,
	NRKEY_KP_PERIOD		= 266,
	NRKEY_KP_DIVIDE		= 267,
	NRKEY_KP_MULTIPLY	= 268,
	NRKEY_KP_MINUS		= 269,
	NRKEY_KP_PLUS		= 270,
	NRKEY_KP_ENTER		= 271,
	NRKEY_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	NRKEY_UP			= 273,
	NRKEY_DOWN		= 274,
	NRKEY_RIGHT		= 275,
	NRKEY_LEFT		= 276,
	NRKEY_INSERT	= 277,
	NRKEY_HOME		= 278,
	NRKEY_END		= 279,
	NRKEY_PAGEUP	= 280,
	NRKEY_PAGEDOWN	= 281,

	/* Function keys */
	NRKEY_F1		= 282,
	NRKEY_F2		= 283,
	NRKEY_F3		= 284,
	NRKEY_F4		= 285,
	NRKEY_F5		= 286,
	NRKEY_F6		= 287,
	NRKEY_F7		= 288,
	NRKEY_F8		= 289,
	NRKEY_F9		= 290,
	NRKEY_F10		= 291,
	NRKEY_F11		= 292,
	NRKEY_F12		= 293,
	NRKEY_F13		= 294,
	NRKEY_F14		= 295,
	NRKEY_F15		= 296,

	/* Key state modifier keys */
	NRKEY_NUMLOCK	= 300,
	NRKEY_CAPSLOCK	= 301,
	NRKEY_SCROLLOCK	= 302,
	NRKEY_RSHIFT	= 303,
	NRKEY_LSHIFT	= 304,
	NRKEY_RCTRL		= 305,
	NRKEY_LCTRL		= 306,
	NRKEY_RALT		= 307,
	NRKEY_LALT		= 308,
	NRKEY_RMETA		= 309,
	NRKEY_LMETA		= 310,
	NRKEY_LSUPER	= 311,		/* Left "Windows" key */
	NRKEY_RSUPER	= 312,		/* Right "Windows" key */
	NRKEY_MODE		= 313,		/* "Alt Gr" key */
	NRKEY_COMPOSE	= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	NRKEY_HELP		= 315,
	NRKEY_PRINT		= 316,
	NRKEY_SYSREQ	= 317,
	NRKEY_BREAK		= 318,
	NRKEY_MENU		= 319,
	NRKEY_POWER		= 320,		/* Power Macintosh power key */
	NRKEY_EURO		= 321,		/* Some european keyboards */
	NRKEY_UNDO		= 322,		/* Atari keyboard has Undo */

	/* Add any other keys here */

	NRKEY_LAST
	
} nrKeyIndex;

/* Enumeration of valid key mods (possibly OR'd together) */
/*typedef enum {
	KMOD_NONE  = 0x0000,
	KMOD_LSHIFT= 0x0001,
	KMOD_RSHIFT= 0x0002,
	KMOD_LCTRL = 0x0040,
	KMOD_RCTRL = 0x0080,
	KMOD_LALT  = 0x0100,
	KMOD_RALT  = 0x0200,
	KMOD_LMETA = 0x0400,
	KMOD_RMETA = 0x0800,
	KMOD_NUM   = 0x1000,
	KMOD_CAPS  = 0x2000,
	KMOD_MODE  = 0x4000,
	KMOD_RESERVED = 0x8000
} SDLMod;

#define KMOD_CTRL	(KMOD_LCTRL|KMOD_RCTRL)
#define KMOD_SHIFT	(KMOD_LSHIFT|KMOD_RSHIFT)
#define KMOD_ALT	(KMOD_LALT|KMOD_RALT)
#define KMOD_META	(KMOD_LMETA|KMOD_RMETA)
*/

typedef enum _nrMouseButtonIndex{

	NRBUTTON_LEFT,
	NRBUTTON_RIGHT,
	NRBUTTON_WHEEL,
	NRBUTTON_WHEEL_UP,
	NRBUTTON_WHEEL_DOWN,

	NRBUTTON_COUNT

} nrMouseButtonIndex;

#endif
