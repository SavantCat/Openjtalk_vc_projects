#include<node.h>
#include<v8.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <fstream>
#include <math.h>

#include <unicode/unistr.h>

using namespace std;
using namespace v8;

#include "openjtalk.h"

openjtalk *ojt;

#define MAXCHAR 100

void node_str_converter(char *str,char *buff){
	icu::UnicodeString src(str,"utf8");
	int32_t len = src.extract(0, src.length(), NULL, "shift_jis");
	buff = new char[len+1];
	src.extract(0, src.length(), buff, "shift_jis");
}

int read_wav(FILE *fp){
	int  size;
	int file_size;


	file_size = ftell(fp);
	ojt->buffer = new char[file_size];

	rewind(fp);

	size = fread(ojt->buffer, sizeof(char), file_size, fp);

	/*
	for (int i = 0; i < size;i++){
	if (i % 15 == 0 && i > 0){
	printf("\n");
	}
	printf("%#x,",buf[i]);
	}
	printf("\n");

	FILE *fpw;
	fpw = fopen("test.wav", "wb");
	fwrite(buf, sizeof(char), size, fpw);
	fclose(fpw);

	rewind(fp);
	*/

	rewind(fp);
	return size;
}

Handle<Value> Openjtalk_setup(const Arguments& args) { //dic voi 4800 tmp.wav
	HandleScope scope;

	cout << "Setup nodejtalk." << endl;

	if (0 < args.Length() && args.Length() <= 4){

		ojt = new openjtalk;
		ojt->Open_JTalk_initialize(&(ojt->data));

		if (args[0]->IsString() == false){
			cout << "Error:no.0 not string" << endl;
			return scope.Close(Boolean::New(false));
		}

		if (args[1]->IsString() == false){
			cout << "Error:no.1 not string" << endl;
			return scope.Close(Boolean::New(false));
		}

		if (args[2]->IsNumber() == false){
			cout << "Error:no,2 not number" << endl;
			return scope.Close(Boolean::New(false));
		}

		if (args[3]->IsString() == false){
			cout << "Error:no.3 not string" << endl;
			return scope.Close(Boolean::New(false));
		}

		String::Utf8Value dic(args[0]->ToString());
		ojt->dn_dict = *dic;
		String::Utf8Value voice(args[1]->ToString());
		ojt->fn_voice = *voice;
		ojt->sampling = args[2]->Int32Value();
		String::Utf8Value tmp(args[3]->ToString());
		if ((ojt->wavfp = fopen(*tmp, "wb+")) == NULL) {
			cout << "Error: open" << endl;
			return scope.Close(Boolean::New(false));
		}

		/* load dictionary and HTS voice */
		if (ojt->Open_JTalk_load(&(ojt->data), (ojt->dn_dict), (ojt->fn_voice)) != TRUE) {
			cout << "Dictionay:" << ojt->dn_dict << endl;
			cout << "Voice:" << ojt->fn_voice << endl;
			cout << "Error: Dictionary or HTS voice cannot be loaded." << endl;
			//ojt->Open_JTalk_clear(&(ojt->data));
			return scope.Close(Boolean::New(false));
		}

		
		ojt->Open_JTalk_set_sampling_frequency(&(ojt->data), ojt->sampling);

		return scope.Close(Boolean::New(true));
	}else{
		cout << "Error: Not values." << endl;
		return scope.Close(Boolean::New(false));
	}	

	return scope.Close(Undefined());
}

Handle<Value> Openjtalk_run(const Arguments& args) {
	HandleScope scope;

	cout << "Make voice." << endl;

	char *buff,*wav;
	int size;

	if (args.Length() <= 1 && *args[0] != NULL){
		String::Utf8Value str(args[0]);

		/*Change UTF-8 to Shift-JIS by ICU*/
		icu::UnicodeString src(*str, "utf8");
		int32_t len = src.extract(0, src.length(), NULL, "shift_jis");
		buff = new char[len + 1];
		src.extract(0, src.length(), buff, "shift_jis");

		/* Make voice data */
		if (ojt->Open_JTalk_synthesis(&(ojt->data), buff, ojt->wavfp, ojt->outfp) != TRUE) {
			fprintf(stderr, "Error: waveform cannot be synthesized.\n");
			return scope.Close(Boolean::New(false));
		}

		/*Read stream*/
		size = read_wav(ojt->wavfp);

		return scope.Close(Boolean::New(true));
	}else{
		return scope.Close(Boolean::New(false));
	}

}

Handle<Value> Openjtalk_clear(const Arguments& args) {
	HandleScope scope;
	if (ojt != NULL){
		fclose(ojt->wavfp);
		delete ojt;
		cout << "Exit nodejtalk system." << endl;
	}
	else{
		cout << "Error: no clear" << endl;
	}

	return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("setup"), FunctionTemplate::New(Openjtalk_setup)->GetFunction());
	exports->Set(String::NewSymbol("run"), FunctionTemplate::New(Openjtalk_run)->GetFunction());
	exports->Set(String::NewSymbol("end"), FunctionTemplate::New(Openjtalk_clear)->GetFunction());
}

NODE_MODULE(nodejtalk, init)