/* ----------------------------------------------------------------- */
/*           The Japanese TTS System "Open JTalk"                    */
/*           developed by HTS Working Group                          */
/*           http://open-jtalk.sourceforge.net/                      */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2008-2013  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the HTS working group nor the names of its  */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */


#ifndef OPEN_JTALK_C

#define OPEN_JTALK_C
#define OPEN_JTALK_C_START
#define OPEN_JTALK_C_END

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

typedef struct _Open_JTalk {
	Mecab mecab;
	NJD njd;
	JPCommon jpcommon;
	HTS_Engine engine;
} Open_JTalk;

class openjtalk
{
public:
	openjtalk();
	~openjtalk();

	/* Open JTalk */
	Open_JTalk data;

	/* output file pointers */
	FILE *wavfp = NULL;
	FILE *outfp = NULL;

	/*Binay Buffer*/
	char *buffer;

	/* dictionary directory */
	char *dn_dict = NULL;

	/* HTS voice */
	char *fn_voice = NULL;

	int sampling = 0;

	static void Open_JTalk_initialize(Open_JTalk * open_jtalk);
	static void Open_JTalk_clear(Open_JTalk * open_jtalk);
	static int Open_JTalk_load(Open_JTalk * open_jtalk, char *dn_mecab, char *fn_voice);
	static void Open_JTalk_set_sampling_frequency(Open_JTalk * open_jtalk, size_t i);
	static void Open_JTalk_set_fperiod(Open_JTalk * open_jtalk, size_t i);
	static void Open_JTalk_set_alpha(Open_JTalk * open_jtalk, double f);
	static void Open_JTalk_set_beta(Open_JTalk * open_jtalk, double f);
	static void Open_JTalk_set_speed(Open_JTalk * open_jtalk, double f);
	static void Open_JTalk_add_half_tone(Open_JTalk * open_jtalk, double f);
	static void Open_JTalk_set_msd_threshold(Open_JTalk * open_jtalk, size_t i, double f);
	static void Open_JTalk_set_gv_weight(Open_JTalk * open_jtalk, size_t i, double f);
	static void Open_JTalk_set_audio_buff_size(Open_JTalk * open_jtalk, size_t i);
	static int Open_JTalk_synthesis(Open_JTalk * open_jtalk, const char *txt, FILE * wavfp, FILE * logfp);
	static void usage();
private:

};

openjtalk::openjtalk()
{
	
}

openjtalk::~openjtalk()
{
	/* free memory */
	Open_JTalk_clear(&data);
	free(buffer);
}

void openjtalk:: Open_JTalk_initialize(Open_JTalk * open_jtalk)
{
	Mecab_initialize(&open_jtalk->mecab);
	NJD_initialize(&open_jtalk->njd);
	JPCommon_initialize(&open_jtalk->jpcommon);
	HTS_Engine_initialize(&open_jtalk->engine);
}

void openjtalk::Open_JTalk_clear(Open_JTalk * open_jtalk)
{
	Mecab_clear(&open_jtalk->mecab);
	NJD_clear(&open_jtalk->njd);
	JPCommon_clear(&open_jtalk->jpcommon);
	HTS_Engine_clear(&open_jtalk->engine);
}

int openjtalk::Open_JTalk_load(Open_JTalk * open_jtalk, char *dn_mecab, char *fn_voice)
{
	if (Mecab_load(&open_jtalk->mecab, dn_mecab) != TRUE) {
		Open_JTalk_clear(open_jtalk);
		return 0;
	}
	if (HTS_Engine_load(&open_jtalk->engine, &fn_voice, 1) != TRUE) {
		Open_JTalk_clear(open_jtalk);
		return 0;
	}
	return 1;
}

void openjtalk::Open_JTalk_set_sampling_frequency(Open_JTalk * open_jtalk, size_t i)
{
	HTS_Engine_set_sampling_frequency(&open_jtalk->engine, i);
}

void openjtalk::Open_JTalk_set_fperiod(Open_JTalk * open_jtalk, size_t i)
{
	HTS_Engine_set_fperiod(&open_jtalk->engine, i);
}

void openjtalk::Open_JTalk_set_alpha(Open_JTalk * open_jtalk, double f)
{
	HTS_Engine_set_alpha(&open_jtalk->engine, f);
}

void openjtalk::Open_JTalk_set_beta(Open_JTalk * open_jtalk, double f)
{
	HTS_Engine_set_beta(&open_jtalk->engine, f);
}

void openjtalk::Open_JTalk_set_speed(Open_JTalk * open_jtalk, double f)
{
	HTS_Engine_set_speed(&open_jtalk->engine, f);
}

void openjtalk::Open_JTalk_add_half_tone(Open_JTalk * open_jtalk, double f)
{
	HTS_Engine_add_half_tone(&open_jtalk->engine, f);
}

void openjtalk::Open_JTalk_set_msd_threshold(Open_JTalk * open_jtalk, size_t i, double f)
{
	HTS_Engine_set_msd_threshold(&open_jtalk->engine, i, f);
}

void openjtalk::Open_JTalk_set_gv_weight(Open_JTalk * open_jtalk, size_t i, double f)
{
	HTS_Engine_set_gv_weight(&open_jtalk->engine, i, f);
}

void openjtalk::Open_JTalk_set_audio_buff_size(Open_JTalk * open_jtalk, size_t i)
{
	HTS_Engine_set_audio_buff_size(&open_jtalk->engine, i);
}

int openjtalk::Open_JTalk_synthesis(Open_JTalk * open_jtalk, const char *txt, FILE * wavfp, FILE * logfp)
{
	//cout << "STR:" << txt << endl;

	int result = 0;
	char buff[MAXBUFLEN];

	text2mecab(buff, txt);
	Mecab_analysis(&open_jtalk->mecab, buff);
	mecab2njd(&open_jtalk->njd, Mecab_get_feature(&open_jtalk->mecab),
		Mecab_get_size(&open_jtalk->mecab));
	njd_set_pronunciation(&open_jtalk->njd);
	njd_set_digit(&open_jtalk->njd);
	njd_set_accent_phrase(&open_jtalk->njd);
	njd_set_accent_type(&open_jtalk->njd);
	njd_set_unvoiced_vowel(&open_jtalk->njd);
	njd_set_long_vowel(&open_jtalk->njd);
	njd2jpcommon(&open_jtalk->jpcommon, &open_jtalk->njd);
	JPCommon_make_label(&open_jtalk->jpcommon);
	if (JPCommon_get_label_size(&open_jtalk->jpcommon) > 2) {
		if (HTS_Engine_synthesize_from_strings
			(&open_jtalk->engine, JPCommon_get_label_feature(&open_jtalk->jpcommon),
			JPCommon_get_label_size(&open_jtalk->jpcommon)) == TRUE)
			result = 1;
		if (wavfp != NULL)
			HTS_Engine_save_riff(&open_jtalk->engine, wavfp);
		if (logfp != NULL) {
			fprintf(logfp, "[Text analysis result]\n");
			NJD_fprint(&open_jtalk->njd, logfp);
			fprintf(logfp, "\n[Output label]\n");
			HTS_Engine_save_label(&open_jtalk->engine, logfp);
			fprintf(logfp, "\n");
			HTS_Engine_save_information(&open_jtalk->engine, logfp);
		}
		HTS_Engine_refresh(&open_jtalk->engine);
	}
	JPCommon_refresh(&open_jtalk->jpcommon);
	NJD_refresh(&open_jtalk->njd);
	Mecab_refresh(&open_jtalk->mecab);

	return result;
}

void openjtalk::usage()
{
	fprintf(stderr, "The Japanese TTS System \"Open JTalk\"\n");
	fprintf(stderr, "Version 1.07 (http://open-jtalk.sourceforge.net/)\n");
	fprintf(stderr, "Copyright (C) 2008-2013 Nagoya Institute of Technology\n");
	fprintf(stderr, "All rights reserved.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "%s", HTS_COPYRIGHT);
	fprintf(stderr, "\n");
	fprintf(stderr, "Yet Another Part-of-Speech and Morphological Analyzer \"Mecab\"\n");
	fprintf(stderr, "Version 0.994 (http://mecab.sourceforge.net/)\n");
	fprintf(stderr, "Copyright (C) 2001-2008 Taku Kudo\n");
	fprintf(stderr, "              2004-2008 Nippon Telegraph and Telephone Corporation\n");
	fprintf(stderr, "All rights reserved.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "NAIST Japanese Dictionary\n");
	fprintf(stderr, "Version 0.6.1-20090630 (http://naist-jdic.sourceforge.jp/)\n");
	fprintf(stderr, "Copyright (C) 2009 Nara Institute of Science and Technology\n");
	fprintf(stderr, "All rights reserved.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "open_jtalk - The Japanese TTS system \"Open JTalk\"\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       open_jtalk [ options ] [ infile ] \n");
	fprintf(stderr,
		"  options:                                                                   [  def][ min-- max]\n");
	fprintf(stderr,
		"    -x  dir        : dictionary directory                                    [  N/A]\n");
	fprintf(stderr,
		"    -m  htsvoice   : HTS voice files                                         [  N/A]\n");
	fprintf(stderr,
		"    -ow s          : filename of output wav audio (generated speech)         [  N/A]\n");
	fprintf(stderr,
		"    -ot s          : filename of output trace information                    [  N/A]\n");
	fprintf(stderr,
		"    -s  i          : sampling frequency                                      [ auto][   1--    ]\n");
	fprintf(stderr,
		"    -p  i          : frame period (point)                                    [ auto][   1--    ]\n");
	fprintf(stderr,
		"    -a  f          : all-pass constant                                       [ auto][ 0.0-- 1.0]\n");
	fprintf(stderr,
		"    -b  f          : postfiltering coefficient                               [  0.0][ 0.0-- 1.0]\n");
	fprintf(stderr,
		"    -r  f          : speech speed rate                                       [  1.0][ 0.0--    ]\n");
	fprintf(stderr,
		"    -fm f          : additional half-tone                                    [  0.0][    --    ]\n");
	fprintf(stderr,
		"    -u  f          : voiced/unvoiced threshold                               [  0.5][ 0.0-- 1.0]\n");
	fprintf(stderr,
		"    -jm f          : weight of GV for spectrum                               [  1.0][ 0.0--    ]\n");
	fprintf(stderr,
		"    -jf f          : weight of GV for log F0                                 [  1.0][ 0.0--    ]\n");
#ifdef HTS_MELP
	fprintf(stderr,
		"    -jl f          : weight of GV for low-pass filter                        [  1.0][ 0.0--    ]\n");
#endif                          /* HTS_MELP */
	fprintf(stderr,
		"    -z  i          : audio buffer size (if i==0, turn off)                   [    0][   0--    ]\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr,
		"    text file                                                                [stdin]\n");
	fprintf(stderr, "\n");

	exit(0);
}



#endif