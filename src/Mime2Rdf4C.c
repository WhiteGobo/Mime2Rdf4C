#include "Mime2Rdf4C.h"
#include <stdlib.h>
#include <string.h>
#include <CInterfaceOxJsonld.h>
#include <NQuadsRDF.h>
#include <CInterfaceOxTTL.h>

typedef enum {
	PARSERTYPE_JSONLD,
	PARSERTYPE_NQUADS,
	PARSERTYPE_TURTLE,
	PARSERTYPE_TRIG,
} PARSERTYPE;

typedef struct mime2Rdf4C_ParserConfig {
	PARSERTYPE parserid;
	union {
		void* config;
		JSONLDConfig* jsonld_config;
		TTLConfig* turtle_config;
		TrigConfig* trig_config;
	};
} Mime2Rdf4C_ParserConfig;


static Mime2Rdf4C_ParserConfig* Mime2Rdf4C_get_parser(PARSERTYPE t){
	Mime2Rdf4C_ParserConfig* ret;
	ret = malloc(sizeof(Mime2Rdf4C_ParserConfig));
	ret->config = NULL;
	ret->parserid = t;
	switch (t){
		case PARSERTYPE_JSONLD:
		case PARSERTYPE_NQUADS:
		case PARSERTYPE_TURTLE:
		case PARSERTYPE_TRIG:
			return ret;
		default:
			free(ret);
			return NULL;
	}
}

Mime2Rdf4C_ParserConfig* Mime2Rdf4C_get_parser_from_ext(const char* ext){
	if(0==strcmp(ext, "json") || 0==strcmp(ext, "jsonld")){
		return Mime2Rdf4C_get_parser(PARSERTYPE_JSONLD);
	} else if (0==strcmp(ext, "ttl")) {
		return Mime2Rdf4C_get_parser(PARSERTYPE_TURTLE);
	} else if (0==strcmp(ext, "trig")) {
		return Mime2Rdf4C_get_parser(PARSERTYPE_TRIG);
	} else if (0==strcmp(ext, "nq")) {
		return Mime2Rdf4C_get_parser(PARSERTYPE_NQUADS);
	}
	return NULL;
}

Mime2Rdf4C_ParserConfig* Mime2Rdf4C_get_parser_from_mediatype(const char* type){
	if( 0 == strcmp(type, "application/ld+json")|| 0==strcmp(type, "application/json")){
		return Mime2Rdf4C_get_parser(PARSERTYPE_JSONLD);
	} else if (0==strcmp(type, "text/turtle")) {
		return Mime2Rdf4C_get_parser(PARSERTYPE_TURTLE);
	} else if (0==strcmp(type, "application/trig")) {
		return Mime2Rdf4C_get_parser(PARSERTYPE_TRIG);
	} else if (0==strcmp(type, "text/nquads")) {
		return Mime2Rdf4C_get_parser(PARSERTYPE_NQUADS);
	}
	return NULL;
}


void free_Mime2Rdf4CParserConfig(Mime2Rdf4C_ParserConfig* config){
	if(config == NULL) return;
	switch(config->parserid){
		case PARSERTYPE_JSONLD:
			if(config->jsonld_config != NULL){
				free_JSONLDConfig(config->jsonld_config);
			}
			break;
		case PARSERTYPE_NQUADS:
		case PARSERTYPE_TURTLE:
		case PARSERTYPE_TRIG:
			break;
	}
	free(config);
}


int Mime2Rdf4C_set_baseiri(Mime2Rdf4C_ParserConfig* config, const char* baseiri){
	if(config == NULL) return -1;
	switch(config->parserid){
		case PARSERTYPE_JSONLD:
			config->jsonld_config = JSONLDConfig_set_baseiri(
					config->jsonld_config, baseiri);
			if (config->jsonld_config == NULL){
				return -1;
			}
			break;
		case PARSERTYPE_NQUADS:
		case PARSERTYPE_TURTLE:
		case PARSERTYPE_TRIG:
			break;
		default:
			return -1;
	}
	return 0;
}


int Mime2Rdf4C_enable_LoadDocumentCallback_over_http(
					Mime2Rdf4C_ParserConfig* config)
{
	if(config == NULL) return -1;
	switch(config->parserid){
		case PARSERTYPE_JSONLD:
			config->jsonld_config = JSONLDConfig_enable_LoadDocumentCallback_over_http(config->jsonld_config);
			if (config->jsonld_config == NULL){
				return -1;
			}
			break;
		case PARSERTYPE_NQUADS:
		case PARSERTYPE_TURTLE:
		case PARSERTYPE_TRIG:
			break;
		default:
			return -1;
	}
	return 0;
}


int Mime2Rdf4C_enable_LoadDocumentCallback_for_localfiles(
					Mime2Rdf4C_ParserConfig* config)
{
	if(config == NULL) return -1;
	switch(config->parserid){
		case PARSERTYPE_JSONLD:
			config->jsonld_config = JSONLDConfig_enable_LoadDocumentCallback_over_http(config->jsonld_config);
			if (config->jsonld_config == NULL){
				return -1;
			}
			break;
		case PARSERTYPE_NQUADS:
		case PARSERTYPE_TURTLE:
		case PARSERTYPE_TRIG:
			break;
		default:
			return -1;
	}
	return 0;
}


int Mime2Rdf4C_enable_LoadDocumentCallback_for_relativefiles(
					Mime2Rdf4C_ParserConfig* config,
					const char *baseuri_c,
					const char *basepath_c)
{
	if(config == NULL) return -1;
	switch(config->parserid){
		case PARSERTYPE_JSONLD:
			config->jsonld_config = JSONLDConfig_enable_LoadDocumentCallback_for_relativefiles(config->jsonld_config, baseuri_c, basepath_c);
			if (config->jsonld_config == NULL){
				return -1;
			}
			break;
		case PARSERTYPE_NQUADS:
		case PARSERTYPE_TURTLE:
		case PARSERTYPE_TRIG:
			break;
		default:
			return -1;
	}
	return 0;
}


int64_t Mime2Rdf4C_parse(
		const char *input, TripleHandler hook,
		void* hook_data, Mime2Rdf4C_ParserConfig* config)
{
	if(config == NULL) return -1;
	switch(config->parserid){
		case PARSERTYPE_JSONLD:
			fprintf(stderr, "qq\n");
			return parse_jsonld(input, hook, hook_data, config->jsonld_config);
		case PARSERTYPE_NQUADS:
			return nquads_parse(input, hook, hook_data);
		case PARSERTYPE_TURTLE:
			return parse_ttl(input, hook, hook_data, config->turtle_config);
		case PARSERTYPE_TRIG:
			return parse_trig(input, hook, hook_data, config->trig_config);
		default:
			return -2;
	}
	return 0;
}
