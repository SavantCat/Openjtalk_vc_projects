#include<node.h>
#include<v8.h>

#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

using namespace std;
using namespace v8;

#include "openjtalk.h"

openjtalk *ojt;

#define MAXARRAY 1024



Handle<Value> Openjtalk_initialize(const Arguments& args) {
	HandleScope scope;

	ojt = new openjtalk;
	ojt->Open_JTalk_initialize(&(ojt->data));

	return scope.Close(Undefined());
}

Handle<Value> Openjtalk_setup(const Arguments& args) { //dic voi 4800 tmp.wav
	HandleScope scope;


	if (args.Length() < 4){
		if (args[0]->IsString() == false){
			cout << "Error: value" << endl;
			return scope.Close(Boolean::New(false));
		}
		else{
			String::Utf8Value str(args[0]->ToString());
			ojt->dn_dict = *str;
		}

		if (args[1]->IsString() == false){
			cout << "Error: value" << endl;
			return scope.Close(Boolean::New(false));
		}
		else{
			String::Utf8Value str(args[1]->ToString());
			ojt->fn_voice = *str;
		}

		if (args[2]->IsNumber() == false){
			cout << "Error: value" << endl;
			return scope.Close(Boolean::New(false));
		}
		else{
			ojt->sampling = args[2]->Int32Value();
		}

		if (args[3]->IsString() == false){
			cout << "Error: value" << endl;
			return scope.Close(Boolean::New(false));
		}
		else{
			String::Utf8Value str(args[3]->ToString());
			if ((ojt->wavfp = fopen(*str, "wb")) == NULL) {
				cout << "Error: open" << endl;
				return scope.Close(Boolean::New(false));
			}
		}

		/* load dictionary and HTS voice */
		if (ojt->Open_JTalk_load(&(ojt->data), ojt->dn_dict, ojt->fn_voice) != TRUE) {
			cout << "Error: Dictionary or HTS voice cannot be loaded." << endl;
			ojt->Open_JTalk_clear(&(ojt->data));
			return scope.Close(Boolean::New(false));
		}

		ojt->Open_JTalk_set_sampling_frequency(&(ojt->data), ojt->sampling);

		return scope.Close(Boolean::New(true));
	}else{
		return scope.Close(Boolean::New(false));
	}	
}

Handle<Value> Openjtalk_run(const Arguments& args) {
	HandleScope scope;

	if (args.Length() <= 0 && args[0]->IsString()){
		String::Utf8Value str(args[0]->ToString());

		if (ojt->Open_JTalk_synthesis(&(ojt->data), *str, ojt->wavfp, ojt->outfp) != TRUE) {
			fprintf(stderr, "Error: waveform cannot be synthesized.\n");
			ojt->Open_JTalk_clear(&(ojt->data));
			return scope.Close(Boolean::New(false));
		}

		return scope.Close(Undefined());
	}
	else{
		return scope.Close(Boolean::New(false));
	}

}

Handle<Value> Openjtalk_clear(const Arguments& args) {
	HandleScope scope;

	delete ojt;

	return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("nodejtalk_init"), FunctionTemplate::New(Openjtalk_initialize)->GetFunction());
}

NODE_MODULE(nodejtalk, init)