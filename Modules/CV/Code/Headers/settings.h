#ifndef SETTINGS_MOBSEL
#define SETTINGS_MOBSEL

#include <stddef.h>		//For NULL and another standart defines

#include "overall_settings.h"
#define DEBUG
#define SHOW_CPU_TIME

/* Overall cv settings */
//ONLY ODD SIZE!
#define SIZE_SQUARE 7
#define ALLOWABLE_DIFF 100
#define MIN_SIZE 1000
//SS5,AD100,MS1000 FOR MY


#define IS_OBJECT 1
#define IS_NOT_OBJECT -2
#define IS_UNDEFINED -1

#define HAVE_WAY 1
#define HAVE_NOT_WAY 0

#define REAL_OBJ 1
#define PSEUDO_OBJ 0

#define VARS_INITIALIZED 1
#define VARS_NOT_INITIALIZED 2

/* Settings for fiil by ways */

	/*
	 *
	 *		* | *   |  *   | *   | *
	 *		* | 128 |  1   | 2   | *
	 *		* | 64  |  o   | 4   | *
	 *		* | 32  |  16  | 8   | *
	 *		* | *   |  *   | *   | *
	 *
	 */

#define UP_WAY 1
#define UR_WAY 2
#define RIGHT_WAY 4
#define DR_WAY 8
#define DOWN_WAY 16
#define DL_WAY 32
#define LEFT_WAY 64
#define UL_WAY 128

#endif
