#include "Mime2Rdf4C.h"
#include <stdlib.h>
#include <string.h>
#include <CInterfaceOxJsonld.h>
#include <NQuadsRDF.h>
#include <CInterfaceOxTTL.h>


typedef enum {
	SERIALIZERTYPE_JSONLD,
	SERIALIZERTYPE_NQUADS,
	SERIALIZERTYPE_TURTLE,
	SERIALIZERTYPE_TRIG,
} SERIALIZERTYPE;


typedef struct mime2Rdf4C_SerializerConfig {
	SERIALIZERTYPE serializerid;
	union {
		void* config;
		JSONLDSerializer* jsonld_config;
		NQuadsSerializer* nquads_config;
		TTLSerializer* turtle_config;
		TrigSerializer* trig_config;
	};
} Mime2Rdf4C_SerializerConfig;

static Mime2Rdf4C_SerializerConfig* Mime2Rdf4C_get_serializer(SERIALIZERTYPE t);



Mime2Rdf4C_SerializerConfig* Mime2Rdf4C_get_serializer_from_ext(const char* ext)
{
	if(strcmp(ext, "json") || strcmp(ext, "jsonld")){
		return Mime2Rdf4C_get_serializer(SERIALIZERTYPE_JSONLD);
	} else if (0==strcmp(ext, "ttl")) {
		return Mime2Rdf4C_get_serializer(SERIALIZERTYPE_TURTLE);
	} else if (0==strcmp(ext, "trig")) {
		return Mime2Rdf4C_get_serializer(SERIALIZERTYPE_TRIG);
	} else if (strcmp(ext, "nq")) {
		return Mime2Rdf4C_get_serializer(SERIALIZERTYPE_NQUADS);
	}
	return NULL;
}


Mime2Rdf4C_SerializerConfig* Mime2Rdf4C_get_serializer_from_mediatype(const char* type)
{
	if(0==strcmp(type, "application/ld+json") || 0==strcmp(type, "application/json")){
		return Mime2Rdf4C_get_serializer(SERIALIZERTYPE_JSONLD);
	} else if (0==strcmp(type, "text/turtle")) {
		return Mime2Rdf4C_get_serializer(SERIALIZERTYPE_TURTLE);
	} else if (0==strcmp(type, "application/trig")) {
		return Mime2Rdf4C_get_serializer(SERIALIZERTYPE_TRIG);
	} else if (0==strcmp(type, "text/nquads")) {
		return Mime2Rdf4C_get_serializer(SERIALIZERTYPE_NQUADS);
	}
	return NULL;
}


int64_t Mime2Rdf4C_add(const char* subject, uint8_t subject_type,
                const char* predicate,
                const char* object, const char* object_suffix,
                uint8_t object_type,
                const char* graph_id, uint8_t graph_type,
                Mime2Rdf4C_SerializerConfig* config)
{
	switch(config->serializerid){
		case SERIALIZERTYPE_JSONLD:
			return JSONLD_SER_add(subject, subject_type, predicate,
					object, object_suffix, object_type,
					graph_id, graph_type,
					config->jsonld_config);
		case SERIALIZERTYPE_NQUADS:
			return NQuads_SER_add(subject, subject_type, predicate,
					object, object_suffix, object_type,
					graph_id, graph_type,
					config->nquads_config);
		case SERIALIZERTYPE_TURTLE:
			return TTL_SER_add(subject, subject_type, predicate,
					object, object_suffix, object_type,
					graph_id, graph_type,
					config->turtle_config);
		case SERIALIZERTYPE_TRIG:
			return Trig_SER_add(subject, subject_type, predicate,
					object, object_suffix, object_type,
					graph_id, graph_type,
					config->trig_config);
	}
}


char* Mime2Rdf4C_finish(Mime2Rdf4C_SerializerConfig* config){
	char* ret = NULL;
	switch(config->serializerid){
		case SERIALIZERTYPE_JSONLD:
			ret = JSONLD_SER_finish(config->jsonld_config);
			break;
		case SERIALIZERTYPE_NQUADS:
			ret = NQuadsRDF_SER_finish(config->nquads_config);
			break;
		case SERIALIZERTYPE_TURTLE:
			ret = TTL_SER_finish(config->turtle_config);
			break;
		case SERIALIZERTYPE_TRIG:
			ret = Trig_SER_finish(config->trig_config);
			break;
	}
	free(config);
	return ret;
}


static Mime2Rdf4C_SerializerConfig* Mime2Rdf4C_get_serializer(SERIALIZERTYPE t){
	Mime2Rdf4C_SerializerConfig* ret;
	ret = malloc(sizeof(Mime2Rdf4C_SerializerConfig));
	switch(t){
		case SERIALIZERTYPE_JSONLD:
			ret->serializerid = SERIALIZERTYPE_JSONLD;
			ret->jsonld_config = JSONLD_SER_start();
			return ret;
		case SERIALIZERTYPE_NQUADS:
			ret->serializerid = SERIALIZERTYPE_NQUADS;
			ret->nquads_config = NQuadsRDF_SER_start();
			return ret;
		case SERIALIZERTYPE_TURTLE:
			ret->serializerid = SERIALIZERTYPE_TURTLE;
			ret->turtle_config = TTL_SER_start();
			return ret;
		case SERIALIZERTYPE_TRIG:
			ret->serializerid = SERIALIZERTYPE_TRIG;
			ret->trig_config = Trig_SER_start();
			return ret;
	}
	return NULL;
}
