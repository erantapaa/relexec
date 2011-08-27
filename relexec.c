
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#define program_name "relexec"
#define EXIT_CANCELED 1

int main(int argc, char** argv)
{
  // argv[1] will
  if (argc <= 2) {
    fprintf(stderr, "usage: %s program args...\n", program_name);
    exit(EXIT_CANCELED);
  }

  char *final_path;

  char *abs_script_path;

  char path[PATH_MAX+1];

  char *interp_path = argv[1];
  char *script_path = argv[2];

  char *abs_interp_path;

  if (interp_path[0] == '\0') {
    fprintf(stderr, "%s: zero-length interpreter path\n", program_name);
    exit(EXIT_CANCELED);
  } else if (interp_path[0] == '/') {
    abs_interp_path = interp_path;
  } else {
    char *abs_script_path;

    char tmp_path[PATH_MAX+1];

    if (script_path[0] == '\0') {
      fprintf(stderr, "%s: zero-length script path\n", program_name);
      exit(1);
    } else if (script_path[0] == '/') {
      abs_script_path = argv[1];
    } else {
      abs_script_path = realpath(script_path, tmp_path);
      if (abs_script_path == NULL) {
        perror("realpath");
        exit(EXIT_CANCELED);
      }
    }

    // abs_script_path = absolute path of the script

    // find the dirname of abs_script_path

    char *p = abs_script_path + strlen(abs_script_path) -1;
    while (p >= abs_script_path && *p != '/') { p--; }

    // p points to the end of dirname(abs_script_path)

    *p = '\0';

    // set abs_interp_path = realpath(abs_script_path + '/' + interp_path)

    int len = strlen(abs_script_path) + 1 + strlen(interp_path);
    if (len > PATH_MAX) {
      fprintf(stderr, "%s: path too long\n", program_name);
      exit(EXIT_CANCELED);
    }

    path[0] = '\0';
    strcat(path, abs_script_path);
    strcat(path, "/");
    strcat(path, interp_path);

    abs_interp_path = path;
  }

  execv(abs_interp_path, argv+1);
  perror("exec");
}
