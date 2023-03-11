#include <stdio.h>
#include <stdlib.h>





void Fetch(char line);
{

}
void Decode();
void Executes();
void Memory();
void WriteBack();





int main()
{
    
    
    char name[100], line[15];
    FILE *file;
    char *buffer;

    printf("Enter the name of the file to be read(with .mem): ");
    gets(name);

    file = fopen(name, "r"); // opens the file in read mode

    if (file == NULL) // if the file is empty then it prints error
    {
        printf("Error in opening the File\n");
        return 1; // indicates it has some error to terminal
    }

    else
    {
        fgets(line, 15, file);  // gets the information in the file
        buffer = strtok(line, " "); // divides the numbers by spacing

        int i = 0;
        while (buffer != NULL)
        {
            M[i] = atoi(buffer);     // converts the characters to integers
            buffer = strtok(NULL, " "); // gets to next set of characters
            i++;
        }
    }
    fclose(file); // closing the file

    
    
    Fetch(line);
}
