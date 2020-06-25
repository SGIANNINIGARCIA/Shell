//Sandro Giannini 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#define INPUT_SIZE 1024

void removeChar(char *s, int c){  //remover of the freaking \n 
  
    int j; 
    int n = strlen(s); 
    int i;
    for (i = 0; i<n; i++){ 
       if (s[i] != c){ 
          s[j++] = s[i]; 
       }
    }
	s[j] = '\0'; 
}	

int main() {
   char *input = malloc(INPUT_SIZE * sizeof(char*));
   char *token;
  
  while(1) {
    
		 printf("brain-is: domesticked-cat$ ");
    
		 fgets(input, INPUT_SIZE, stdin);
		 removeChar(input, '\n');
     
     char **commands = malloc(INPUT_SIZE * sizeof(char*));
		 int index = 0;
		 token = strtok(input, " ");
		 
     while(token != NULL) {     //tokenize input
			 commands[index] = token;
			 index++;
			 token = strtok(NULL, " ");
		 } 
     commands[index] = NULL;
     
        //built in commands
     if(strcmp(commands[0], "exit") == 0) {
       break;
     }
		 if(strcmp(commands[0], "help") == 0) {
       printf("\nCuriosity killed the Cat...\n");
			 printf("It is said that a cat has nine lives, yet curiosity would wear them all out.\n");
			 printf("Sandro's bash, version 1.0, after 50000000 segment faults\n\n");
			 printf("This shell supports exit, pwd, cd, ls, and others.\n");
			 printf("if its not supported, shame.\n\n");
       printf("\n(* *)\n");
			 continue;
     }	
     if(strcmp(commands[0], "pwd") == 0) {
       char pwd[INPUT_SIZE];
       getcwd(pwd, sizeof(pwd));
     }
     if(strcmp(commands[0], "cd") == 0) {
       chdir(commands[1]);
       continue;
     } else {                                      //non built in commands
     
     int rc = fork(); 
     if(rc == 0) {
			 index = 0;
			 int redirectFlag = -1;
       
     
			 while(commands[index] != NULL) {            //delete > and return the index
				 if(strcmp(commands[index], ">") == 0) {
					 redirectFlag = index;
				 }
				 index++;
			 }
       
			 if( redirectFlag != -1) {
				 close(1);
				 open(commands[redirectFlag + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
				 index = redirectFlag;
				 while (index < INPUT_SIZE) {
           commands[index] = NULL;
           index++;
         }
		   }
       execvp(commands[0], commands); 
       
       perror("execvp");
       return -1;
     }
    
     else if (rc > 0) {
        int rc = wait(NULL);
        if (rc < 0) {
            perror("wait");
            return -1;
        }
     }
     else {
        perror("fork");
     }
	} 
 }   
  return 0;
}
