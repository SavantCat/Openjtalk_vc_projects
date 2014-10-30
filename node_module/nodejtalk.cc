#ifndef OPEN_JTALK_C
#define OPEN_JTALK_C

#ifdef __cplusplus
#define OPEN_JTALK_C_START extern "C" {
#define OPEN_JTALK_C_END   }
#else
#define OPEN_JTALK_C_START
#define OPEN_JTALK_C_END
#endif                          /* __CPLUSPLUS */

OPEN_JTALK_C_START;

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

/* Main headers */
#include "mecab.h"
#include "njd.h"
#include "jpcommon.h"
#include "HTS_engine.h"

/* Sub headers */
#include "text2mecab.h"
#include "mecab2njd.h"
#include "njd_set_pronunciation.h"
#include "njd_set_digit.h"
#include "njd_set_accent_phrase.h"
#include "njd_set_accent_type.h"
#include "njd_set_unvoiced_vowel.h"
#include "njd_set_long_vowel.h"
#include "njd2jpcommon.h"

#define MAXBUFLEN 1024

OPEN_JTALK_C_END;
#endif                          /* !OPEN_JTALK_C */