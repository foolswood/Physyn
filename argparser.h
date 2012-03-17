#ifndef ARGPARSERHEADER
#define ARGPARSERHEADER

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

typedef struct args_t {
	char *fpath;
	char *io_module;
} args_t;


int argparse(int argc, char **argv, args_t *args) ;

#endif
