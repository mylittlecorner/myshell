#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



static int exec_prog(char **argv)
{
    pid_t   my_pid;
    int     status, timeout;
    char *text[]={"/usr/local/sbin","/usr/local/bin","/usr/sbin","/usr/bin","/sbin","/bin","/usr/local/games","/usr/games"};

    if (0 == (my_pid = fork())) {
            if (-1 == execvpe(argv[0],argv, text)) {
                    perror("child process execve failed [%m]");
                    return -1;
            }
    }

#ifdef WAIT_FOR_COMPLETION
    timeout = 1000;

    while (0 == waitpid(my_pid , &status , WNOHANG)) {
            if ( --timeout < 0 ) {
                    perror("timeout");
                    return -1;
            }
            sleep(1);
    }

    printf("%s WEXITSTATUS %d WIFEXITED %d [status %d]\n",
            argv[0], WEXITSTATUS(status), WIFEXITED(status), status);

    if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
            perror("%s failed, halt system");
            return -1;
    }

#endif
    return 0;
}

int main() {

  char cwd[1024];
  getcwd(cwd, sizeof(cwd));

  char input[1024];

  while (1) {
    printf("[%s]> ", cwd);
    fflush(stdin);
    fgets(input,1024,stdin);

    char *tokenized_input = strtok(input, " ");
    char *arguments[1024];
    int i =0;
    arguments[i] = tokenized_input;
    printf("%s\n",arguments[i]);

    while (tokenized_input != NULL)
    {
       i++;
       tokenized_input = strtok(NULL, " ");
       if(tokenized_input == NULL) break;
       arguments[i] = tokenized_input;
       printf("%s\n",arguments[i]);
    }

    exec_prog(arguments);
    fflush(stdin);
  }

  return 0;
}
