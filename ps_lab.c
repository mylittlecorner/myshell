#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <proc/readproc.h>

void error_arg(char **argv)
{
	printf("\nusage: %s -e\n",argv[0]);
	exit(0);
}

int main(int argc, char **argv)
{
  char *argument= "-e";

 if(argc<2)
 {
    error_arg(argv);
 }
 if((argument[0] == argv[1][0]) && (argument[1] == argv[1][1]))
 {

	  PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);

	  proc_t proc_info;
	  memset(&proc_info, 0, sizeof(proc_info));
	  printf("PID\tTTY\tTIME\tCMD\t\n");
		while (readproc(proc, &proc_info) != NULL) {
		printf("%d\t",proc_info.tid);
		printf("%d\t",proc_info.tty);
		printf("%llu\t",proc_info.utime);
		printf("%s\t\n",proc_info.cmd);

	  	}

	  closeproc(proc);

 } else { error_arg(argv); }

  return 0;
}
