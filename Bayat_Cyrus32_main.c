
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>
// buffer size according to the assignment
#define bufferSize 102


// To make the shell screen more friendly (Creativity)
// Clean the screen and have a header on the top
void screenFriendly(){
        system("clear");
        printf("******************************************************************\n");
        printf("*** Welcome to my shell ***\n");
        printf("******************************************************************\n");
}

// char array for the input, with a size of the 102 byte
char bufferArray [bufferSize];
// In this assignment, we need to have two argument vector, one for child, and other one for the main as usual
// Space make the character to be argument. So 102/2 means 51 argument 
// Becuse of the NUll, need to add 1 to 51 ==> 52 
char * childArgumentVector [(bufferSize/2)+1];
// main 
int main(int argc, char *argv[]){
        // Calling the screenFriendly() to clean the screen, first thing after run the program
        screenFriendly();

        
        // First element in argv is the prompt if the argc is more than one.
        char * prompt = "~>";
        // In Makefile we have RUNOPTIONS="Prompt> ", so we can use it.
        if (argc > 1){
               prompt = argv[1];
        }
        // To store the fgets() return value
        char * buff;
        // leave is the key to leave the while loop
        int leave = 0;
        // Iterate until user wants to leave the shell.
        // If user wants to exit, change the value of Leave to 1.
        while(leave == 0){

                // Showing user for the input, Getting input
                printf("%s",prompt);
                // get a string from a stream
                // fgets(): pointer to an element of a char array on success, null pointer on failure.
                // stdin is the standard input stream.
                // Reference: https://www.man7.org/linux/man-pages/man3/fgets.3p.html
                buff = fgets(bufferArray, bufferSize,stdin);

                // If buff is NUll,it means End Of File or an error happened, so exit (leave the while loop).
                if(buff == NULL){
                printf("End Of File Reached.\n");
                //perror(errno);
                leave = 1;
                // Skips the current iteration and moves on to check the loop condition for the next iteration.
                continue;
                }

                // Breaking the string to the small pieces so program can execute them.
                // Using delimiter to ignore the space, tap and next line in strtok().
                char * delimiter = " \t\n";
                // return a pointer to the next token, or NULL if there are no more tokens.
                // Refrence: https://man7.org/linux/man-pages/man3/strtok.3.html
                char * token;
                token = strtok (bufferArray, delimiter);
                // To keep track of the argument Number in child Argument Vector.
                int childArgumentCount = 0;
                // Iterate untill there are no more tokens.
                while (token != NULL){
                        // If there is a token, add it to the child argument vector.
                        childArgumentVector[childArgumentCount]=token;
                        // Increment the child Argument Count by one.
                        childArgumentCount++;
                        //printf("%s\n", token);
                        // Instruct the strtok() function to stop the parsing and return null and leave the loop.
                        token = strtok(NULL, delimiter);
                }
                // Add a NUll Pointer to the end.
                childArgumentVector[childArgumentCount]=NULL;

                // handle the case of not entring enythings (or just entring delimiter).
                if (childArgumentCount==0){
                        printf("Error, Empty String Was Enter.\n");
                        continue;
                }
                
                // handle the case of entring exit (leave the while loop)
                // Compares the two strings.
                // It will return 0, if if user input is equal to "exit".
                // Refrence: https://man7.org/linux/man-pages/man3/strcmp.3.html
                if(strcmp(childArgumentVector[0], "exit")==0){
                        leave=1;
                }
                
                // command to execute
                else{
                        
                        // To store the execvp() return value.
                        int error;
                        // Wait Status
                        int status;
                        // create a Child (child PID)
                        pid_t childPID = fork();
                        // Different Process IDs, have different meaning.
                        /* Here I could use the if statement, But I chose the switch statement for practice (same result)*/
                        switch(childPID){
                        /* fork failed (process ID = -1)*/
                        case -1 :
                                printf("Fork() failed");
                                exit(-1);
                        break;

                        /* Child (process ID = 0)*/
                        case 0:
                                // call execvp(). 
                                // If an error has occurred for some reason, execvp() will return -1
                                // Refrence: https://www.man7.org/linux/man-pages/man3/exec.3.html
                                // Refrence: https://www.geeksforgeeks.org/exec-family-of-functions-in-c/
                                error = execvp (childArgumentVector[0],childArgumentVector);
                                // if execute failed, print the Error number.
                                if(error<0){
                                        // Refrence: https://www.tutorialspoint.com/cprogramming/c_error_handling.htm
                                        printf("execvp() failed. Error: %d\n",errno);
                                        // leaving the child, without it shel to shel will happen
                                        exit(errno);
                                }
                        break;
                        // Any positive value, (process ID > 0)
                        /* Parent */
                        default:
                                // Wait for its child process to complete
                                //sleep(1);
                                // Refrence: https://man7.org/linux/man-pages/man2/wait.2.html
                                wait(&status);
                                // Print the child PID and the return result from the child process.
                                printf("child %d finished with status: %d \n", childPID, WEXITSTATUS(status));
                                //printf("%d\n",childPID);
                        break;
                        }
                }
         
        }
        return 0;

}
