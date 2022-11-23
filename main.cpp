#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void show_example() {
    printf("\
\n \
  Example:\n \
\n \
    1) if use 'filename.exe input.cnc' then program reading file\n \
       'input.cnc' and writing file 'input_out.cnc' for output data\n\n \
    2) if use 'filename.exe input.cnc output.cnc' then program\n \
       reading file 'input.cnc' and writing data to 'output.cnc' file.\n \
\n \
	");
}

void show_usage() {
    printf("\
\n \
  Usage:\n \
\n \
    'filename.exe' view this help\n \
    'filename.exe input_filename'\n \
    'filename.exe input_filename output_filename'\n \
	");
	show_example();
}

int main(int argc, char *args[]) {
    FILE *fi, *fo;

    char filename[1024]={0};
    char buf[1024];
    char *pch;

    switch (argc - 1) {
    case 2 :
        fi = fopen(args[1], "r");
        pch = args[2];
        break;
    case 1 :
        strcpy(filename, args[1]);
        pch = strrchr(filename, '.');
        if (NULL != pch)
            *pch = 0;
        pch = filename;
        strcat(pch, "_out.cnc");
        fi = fopen(args[1], "r");
        pch = filename;
        break;
    default:
        show_usage();
        return 0;
    }

    if (NULL == fi) {
        printf("Error opening input file '%s'!\nprogram end.\n", args[2]);
        show_example();
        return 1;
    }

    fo = fopen(pch, "w");

    if (NULL == fo) {
        fclose(fi);
        printf("Error opening output file '%s'!\nprogram end.\n", pch);
        show_example();
        return 1;
    }

    do {
        if (fgets(buf, sizeof(buf), fi) == NULL)
            break;

		pch = buf;

		if ((buf[0] == '\r') || (buf[0] == '\n'))
			continue;

		if (buf[0] == 'N') {
			pch = strchr(buf, ' ');
			if (pch == NULL)
				pch = buf;
			else
				pch += 1;
		}

		fprintf(fo, "%s", pch);

    } while(!feof(fi));

    fclose(fi);
    fclose(fo);

    return 0;
}
