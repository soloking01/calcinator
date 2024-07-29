#include "lexer.h"


void init_lexer(const char *src,lexer_t *lexer){
	lexer->src=src;
	lexer->start_pos=0;
	lexer->curr_pos=0;
}


