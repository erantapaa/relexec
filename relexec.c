
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

#define program_name "relexec"
#define EXIT_CANCELED 1

int main(int argc, char** argv)
{
  // argv[1] will
  if (argc <= 2) {
    fprintf(stderr, "usage: %s program args...\n", program_name);
    exit(EXIT_CANCELED);
  }

  char *debug_env = getenv("RELEXEC_DEBUG");
  int debug = debug_env == NULL ? 0 : atoi(debug_env);
  if (debug) {
    fprintf(stderr, "debug = %d\n", debug);
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
      abs_script_path = script_path;
    } else {
      abs_script_path = realpath(script_path, tmp_path);
      if (abs_script_path == NULL) {
        perror("realpath");
        exit(EXIT_CANCELED);
      }
    }

    // abs_interp_path = realpath(dirname(abs_script_path) + '/' + interp_path)

    char *dirname_path = dirname(abs_script_path);

    if (debug) {
      fprintf(stderr, "script_path: %s\n", script_path);
      fprintf(stderr, "abs_script : %s\n", abs_script_path);
      fprintf(stderr, "dirnam_path: %s\n", dirname_path);
      fprintf(stderr, "interp_path: %s\n", interp_path);
    }

    int len = strlen(dirname_path) + 1 + strlen(interp_path);

    if (len > PATH_MAX) {
      fprintf(stderr, "%s: path too long\n", program_name);
      exit(EXIT_CANCELED);
    }

    path[0] = '\0';
    strcat(path, dirname_path);
    strcat(path, "/");
    strcat(path, interp_path);

    abs_interp_path = path;
  }

  if (debug) {
    fprintf(stderr, "abs_interp_path: %s\n", abs_interp_path);
  }
  execv(abs_interp_path, argv+1);
  perror("exec (from relexec)");
}
