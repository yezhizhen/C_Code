#include <stdio.h>
#include <unistd.h>    // fork(), getpid()
#include <sys/wait.h>  // wait()

//use sys command
//ps aux |grep defunct
//to find the zombie
//Linux will label zombie processes as “<defunct>”.
int main(void) {
		int pid;
			if( (pid = fork()) != 0 ) 
			{
					printf("Look at the status of the process %d\n", pid);
					while( getchar() != '\n' );		// block stdin until a newline.
					//before wait, child is zombie
					wait(NULL);
					//then, zombie removed
					printf("Look again!\n");
					while( getchar() != '\n' );
			}
				return 0;
}
