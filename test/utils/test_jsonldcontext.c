#include <stdlib.h>
#include "Mime2Rdf4C.h"
#include <string.h>

const char* input;


static int parse_args(int argc, char* argv[]);
static int parse_input();
int8_t myhandler(
                const char* subject, uint8_t subject_type,
                const char* predicate,
                const char* object, const char* object_suffix,
                uint8_t object_type,
                const char* graphid, uint8_t graph_type,
                void* user);
static char* load_into_memory(const char* filepath);

int main(int argc, char *argv[]){
	int err;
	const char* ext;
	switch(parse_args(argc, argv)){
		case 0:
			break;
		default:
			exit(EXIT_FAILURE);
	}
	ext = strrchr(input, '.');
	if(ext == NULL){
		fprintf(stderr, "Couldnt identify extension\n");
		exit(EXIT_FAILURE);
	}

	Mime2Rdf4C_ParserConfig* parser;
	char *tmp;
	parser = Mime2Rdf4C_get_parser_from_ext(ext+1);
	if(parser == NULL){
		fprintf(stderr, "Mime2Rdf4C_get_parser_from_ext(\"%s\") returned NULL\n", ext+1);
		exit(EXIT_FAILURE);
	}
	tmp = load_into_memory(input);
	if(tmp == NULL){
		fprintf(stderr, "ERROR failed to load input data\n");
		exit(EXIT_FAILURE);
	}
	switch(Mime2Rdf4C_util_use_file_as_base(parser, input, NULL, 0)){
		case 0:
			break;
		default:
			fprintf(stderr, "util_use_file_as_base failed\n");
			exit(EXIT_FAILURE);
	}

	err = Mime2Rdf4C_parse(tmp, myhandler, NULL, parser);
	free(tmp);
	free_Mime2Rdf4CParserConfig(parser);
	if(err != 0){
		fprintf(stderr, "ERROR parsing error\n");
		exit(EXIT_FAILURE);
	} else {
		exit(EXIT_SUCCESS);
	}
			/*
	g_input

	free_global_values();
	int err;
	const char* baseiri = "http://example.com/jsonldcontext/";
	const char* datairi = "http://example.com/jsonldcontext/data00";
	char* qq;
	const char* q = strrchr(input, '/');
	size_t end_dirpath = q - input;
	qq = malloc(end_dirpath + 1);
	memcpy(qq, input, end_dirpath);
	qq[end_dirpath] = '\0';

	fprintf(stderr, "brubru %s %s %s\n", baseiri, qq, datairi);
	err = Mime2Rdf4C_enable_LoadDocumentCallback_for_relativefiles(parser, 
			baseiri, qq);
	err = Mime2Rdf4C_set_baseiri(parser, datairi);

	*/
}

static int parse_args(int argc, char* argv[]){
	if(argc < 1){
		fprintf(stderr, "ERROR wrong input. Expected import "
				"location\n");
		return 1;
	}
	input = argv[1];
	return 0;
}
static int parse_input(){
}


int8_t myhandler(
                const char* subject, uint8_t subject_type,
                const char* predicate,
                const char* object, const char* object_suffix,
                uint8_t object_type,
                const char* graphid, uint8_t graph_type,
                void* user)
{
	fprintf(stderr, "%s %s %s.\n", subject, predicate, object);
	return 0;
}

static char* load_into_memory(const char* filepath){
        char *ret;
        long fsize;
        FILE *f = fopen(filepath, "rb");
	fprintf(stderr, "brubru %s\n", filepath);
        if (f == NULL) return NULL;
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        rewind(f);
        //fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        ret = malloc(fsize + 1);
        fread(ret, fsize, 1, f);
        ret[fsize] = 0;
        fclose(f);
        return ret;
}
