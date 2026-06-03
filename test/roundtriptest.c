#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <BasicRDFGraphComparator.h>
#include "Mime2Rdf4C.h"

#include "cwalk.h"

int main(int argc, char *argv[]);
static int parse_args(int argc, char *argv[]);
char* roundtrip(const char* initial, const char* type);
char* load_initial_string();

static int compare_data(const char* first, const char* first_ext, const char* second, const char* second_ext);
static char* transform_media(const char* data, const char* last, const char* next);


const char* testname = NULL;
const char* purpose = NULL;
const char* inputfile = NULL;
const char* base_uri = NULL;
const char* inputformat = NULL;
bool needs_context_awarness = false;



int main(int argc, char *argv[]){
	int err;
	int success;
	int fd;
	char *initial, *last;
	const char* starttype = "text/nquads";
	bool are_same_graphs;
	err = parse_args(argc, argv);
	if (0 != err){
		fprintf(stderr, "parsing args failed\n");
		exit(EXIT_FAILURE);
	}
	initial = load_initial_string();
	if (initial == NULL){
		fprintf(stderr, "failed loading initial string.");
		exit(EXIT_FAILURE);
	}
	last = roundtrip(initial, starttype);
	if (last == NULL){
		fprintf(stderr, "roundtrip failed.");
		exit(EXIT_FAILURE);
	}
	err = compare_data(initial, starttype, last, starttype);
	free(initial);
	free(last);
	switch(err){
		case 0 :
			exit(EXIT_SUCCESS);
		default:
			exit(EXIT_FAILURE);
	}
}


static struct option parse_options[] = {
	{"name", required_argument, NULL, 'n'},
	{"purpose", required_argument, NULL, 'p'},
	{"input", required_argument, NULL, 'i'},
	{"inputformat", required_argument, NULL, 'f'},
	{"base-uri", required_argument, NULL, 'b'},
	{"needs-context-awarness", no_argument, NULL, 'a'},
        {NULL, 0, NULL, 0}
};

static int parse_args(int argc, char *argv[]){
	int err = 0;
	int c = 0;
	int option_index;
	while(c != -1){
		c = getopt_long(argc, argv, "",
				parse_options, &option_index);
		switch(c){
			case -1: //end of arguments
				break;
			case 'n':
				testname = optarg;
				break;
			case 'p':
				purpose = optarg;
				break;
			case 'f':
				inputformat = optarg;
				break;
			case 'i':
				fprintf(stderr, "inputfile: %s\n", optarg);
				inputfile = optarg;
				break;
			case 'b':
				fprintf(stderr, "baseuri: %s\n", optarg);
				base_uri = optarg;
				break;
			case 'a':
				needs_context_awarness = true;
				break;
			default:
				fprintf(stderr, "unrecognized argument\n");
				err = 1;
				break;
		}
	}
	if (inputfile == NULL){
		return 2;
	}
	return err;
}



static int compare_data(const char* first, const char* first_ext, const char* second, const char* second_ext)
{
	int err;
	bool are_same_graphs;
	Mime2Rdf4C_ParserConfig* parser;
	TripleStream *initial_triples = new_TripleStream();
	TripleStream *last_triples = new_TripleStream();

	parser = Mime2Rdf4C_get_parser_from_mediatype(first_ext);
	fprintf(stderr, "brubru1 %s: %s\n", first_ext, first);
	fprintf(stderr, "brubru2 %s: %s\n", second_ext, second);
	err = Mime2Rdf4C_parse(first,
			(TripleHandler*) append_TripleStream, initial_triples,
			parser);
	free_Mime2Rdf4CParserConfig(parser);
	if (err != 0){
		fprintf(stderr, "Failed to reparse initial.\n");
		return 1;
	}
	parser = Mime2Rdf4C_get_parser_from_mediatype(second_ext);
	err = Mime2Rdf4C_parse(second,
			(TripleHandler*) append_TripleStream, last_triples,
			parser);
	free_Mime2Rdf4CParserConfig(parser);
	if (err != 0){
		fprintf(stderr, "Failed to parse last.\n");
		return 1;
	}

	are_same_graphs = compare_triples(initial_triples, last_triples);
	free_TripleStream(initial_triples);
	free_TripleStream(last_triples);
	if (are_same_graphs){
		return 0;
	} else {
		fprintf(stderr, "Not the same graphs.\n");
		return 1;
	}
}


char* load_initial_string(){
	char *ret;
	long fsize;
	FILE *f = fopen(inputfile, "rb");
	if (f == NULL) {
		fprintf(stderr, "Couldnt open ''\n", inputfile);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	rewind(f);

	ret = malloc(fsize + 1);
	fread(ret, fsize, 1, f);
	ret[fsize] = 0;
	fclose(f);
	return ret;
}


char* roundtrip(const char* initial, const char* starttype){
	int i=0;
	const char* last_type = starttype;
	Mime2Rdf4CParserData* next;
	char* new_data;
	char* data = malloc(strlen(initial));
	strcpy(data, initial);
	for (int i = 0; Mime2Rdf4C_Parser_List[i].type != NULL; i++){
		next = &Mime2Rdf4C_Parser_List[i];
		if (needs_context_awarness) {
			fprintf(stderr, "needs context awareness\n");
		}
		if (next->is_context_aware) {
			fprintf(stderr, "\"%s\" is context aware\n", next->type);
		} else {
			fprintf(stderr, "\"%s\" is not context aware\n", next->type);
		}
		if (needs_context_awarness && !next->is_context_aware) {
			fprintf(stderr, "skip\n");
			continue;
		}
		if (0 == strcmp(next->type, starttype)){
			continue;
		}
		new_data = transform_media(data, last_type, next->type);
		free(data);
		data = new_data;
		last_type = next->type;
		if (data==NULL) return NULL;
	}
	new_data = transform_media(data, last_type, starttype);
	free(data);
	return new_data;
}

static char* transform_media(const char* data, const char* last, const char* next){
	int64_t err;
	Mime2Rdf4C_ParserConfig* parser_cfg;
	Mime2Rdf4C_SerializerConfig* serializer_cfg;
	char* new_data;
	fprintf(stderr, "transform '%s' to '%s'\n", last, next);
	parser_cfg = Mime2Rdf4C_get_parser_from_mediatype(last);
	serializer_cfg = Mime2Rdf4C_get_serializer_from_mediatype(next);
	if (serializer_cfg == NULL){
		fprintf(stderr, "failed to load serializer for \"%s\"\n", next);
		return NULL;
	}

	err = Mime2Rdf4C_parse(data, (TripleHandler*) Mime2Rdf4C_add,
			serializer_cfg, parser_cfg);
	if (err != 0){
		fprintf(stderr, "Failed to parse.");
		return NULL;
	}
	new_data = Mime2Rdf4C_finish(serializer_cfg);
	if (new_data == NULL){
		fprintf(stderr, "Failed to finalize serialization.\n");
		return NULL;
	}
	fprintf(stderr, "to %s: %s\n", next, new_data);
	return new_data;
}
