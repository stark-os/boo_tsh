#include <unistd.h>
#include <sys/wait.h>






// ---------------- DEFINITIONS ----------------

//ext asm <<<<<<<<<<<<<<<<<<<<<< syscalls and get rid of includes ?
int real_waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options, void*);

//IO
#define STDIN  0
#define STDOUT 1






// ---------------- TOOLS ----------------

//str
static int streq(char* s1, char* s2, int len) {
	for(int i=0; i < len; i++){
		if(s1[i] != s2[i]){ return 0; }
	}
	return 1;
}






// ---------------- EXECUTION ----------------

//main
int _start() {
	char cmd[255];

	//main loop
	for (;;) {
		write(STDOUT, "> ", 2);

		//read
		int cnt = read(STDIN, cmd, 255);

		//special behaviors
		if(cnt >= 4){
			if(streq(cmd, "exit", 4)){ _exit(0); }
			if(streq(cmd, "heyo", 4)){ write(STDOUT, "Hej man ! What's up ?\n", 22); }
		}

		//set null terminator in cmd
		cmd[cnt-1] = '\0';

		//sub-process
		pid_t fork_result = fork();
		if(fork_result == 0){
			execve(cmd, 0, 0); //no args given
			break;
		}

		//main process (wait until end of sub-process)
		else{
			siginfo_t info;
			real_waitid(P_ALL, 0, &info, WEXITED, 0);
		}
	}

	//end
	_exit(0);
}
