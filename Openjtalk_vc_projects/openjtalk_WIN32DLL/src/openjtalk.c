#include "openjtalk.h"

#define MAXBUFLEN 1024

#define DllExport  __declspec( dllexport ) 

typedef struct _Open_JTalk {
	Mecab mecab;
	NJD njd;
	JPCommon jpcommon;
	HTS_Engine engine;
} Open_JTalk;

static void Open_JTalk_initialize(Open_JTalk * open_jtalk)
{
	Mecab_initialize(&open_jtalk->mecab);
	NJD_initialize(&open_jtalk->njd);
	JPCommon_initialize(&open_jtalk->jpcommon);
	HTS_Engine_initialize(&open_jtalk->engine);
}

static void Open_JTalk_clear(Open_JTalk * open_jtalk)
{
	Mecab_clear(&open_jtalk->mecab);
	NJD_clear(&open_jtalk->njd);
	JPCommon_clear(&open_jtalk->jpcommon);
	HTS_Engine_clear(&open_jtalk->engine);
}

static int Open_JTalk_load(Open_JTalk * open_jtalk, char *dn_mecab, char *fn_voice)
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

static void Open_JTalk_set_sampling_frequency(Open_JTalk * open_jtalk, size_t i)
{
	HTS_Engine_set_sampling_frequency(&open_jtalk->engine, i);
}

static void Open_JTalk_set_fperiod(Open_JTalk * open_jtalk, size_t i)
{
	HTS_Engine_set_fperiod(&open_jtalk->engine, i);
}

static void Open_JTalk_set_alpha(Open_JTalk * open_jtalk, double f)
{
	HTS_Engine_set_alpha(&open_jtalk->engine, f);
}

static void Open_JTalk_set_beta(Open_JTalk * open_jtalk, double f)
{
	HTS_Engine_set_beta(&open_jtalk->engine, f);
}

static void Open_JTalk_set_speed(Open_JTalk * open_jtalk, double f)
{
	HTS_Engine_set_speed(&open_jtalk->engine, f);
}

static void Open_JTalk_add_half_tone(Open_JTalk * open_jtalk, double f)
{
	HTS_Engine_add_half_tone(&open_jtalk->engine, f);
}

static void Open_JTalk_set_msd_threshold(Open_JTalk * open_jtalk, size_t i, double f)
{
	HTS_Engine_set_msd_threshold(&open_jtalk->engine, i, f);
}

static void Open_JTalk_set_gv_weight(Open_JTalk * open_jtalk, size_t i, double f)
{
	HTS_Engine_set_gv_weight(&open_jtalk->engine, i, f);
}

static void Open_JTalk_set_audio_buff_size(Open_JTalk * open_jtalk, size_t i)
{
	HTS_Engine_set_audio_buff_size(&open_jtalk->engine, i);
}

