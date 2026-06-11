#include <stdlib.h>
#include "Mime2Rdf4C.h"

int main(int argc, char *argv[]){
	const char* type;
	char* ret;
	for (int i = 0; Mime2Rdf4C_Parser_List[i].type != NULL; i++){
		type = Mime2Rdf4C_Parser_List[i].type;
		fprintf(stderr, "test empty: \"%s\"\n", type);
		ret = Mime2Rdf4C_finish(
				Mime2Rdf4C_get_serializer_from_mediatype(type)
				);
		if (ret != NULL) free(ret);
	}
}
