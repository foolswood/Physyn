#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

typedef struct args_t {
	char *fpath;
	char *io_module;
} args_t;

static char *usage = "usage is: physyn [-o output_device] filepath\n";

int argparse(int argc, char **argv, args_t *args) {
	//options
	static struct option long_options[] = {
		{"help",     no_argument,       0, 'h'},
		{"output",  required_argument, 0, 'o'}
	};
	int c;
	int option_index = 0;
	//defaults
	args->fpath = NULL;
	args->io_module = "jack";
	while (1) {
		/* getopt_long stores the option index here. */
		c = getopt_long(argc, argv, "ho:", long_options, &option_index);
		/* Detect the end of the options. */
		if (c == -1)
			break;
		switch (c) {
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf("\n");
				break;
			case 'h':
				puts(usage);
				break;
			case 'o':
				args->io_module = optarg;
				break;
			case '?':
				/* getopt_long already printed an error message. */
				return 1;
			default:
				return 1;
		}
	}
	//non option arguments
	if (argc - optind == 1) {
		args->fpath = argv[optind];
	} else {
		puts("no file (or too many files) specified, error!");
		return 1;
	}
	return 0;
}
