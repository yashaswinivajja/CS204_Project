#include <stdlib.h>
#include <stdio.h>
int PC;

    // Register file
    static unsigned int X[32];
// flags
// memory
static char MEM[4000];

// intermediate datapath and control path signals
static unsigned int instruction_word;
static unsigned int operand1;
static unsigned int operand2;

int hex(char letter)
{
    int x;
    switch (letter)
    {
    case '1':
        x = 1;
        break;
    case '2':
        x = 2;
        break;
    case '3':
        x = 3;
        break;
    case '4':
        x = 4;
        break;
    case '5':
        x = 5;
        break;
    case '6':
        x = 6;
        break;
    case '7':
        x = 7;
        break;
    case '8':
        x = 8;
        break;
    case '9':
        x = 9;
        break;
    case 'A':
        x = 10;
        break;
    case 'B':
        x = 11;
        break;
    case 'C':
        x = 12;
        break;
    case 'D':
        x = 13;
        break;
    case 'E':
        x = 14;
        break;
    case 'F':
        x = 15;
        break;
    default:
        break;
    }
    return x;
}

void hextobin(char Instruction_word[],int *arry)
{
    int b[5] , j = 0, num, rem;
    for (int i = 0; i < 8; i++)
    {
        num = hex(Instruction_word[i]);
        while (j < 4)
        {
            rem = num % 2;
            num = num / 2;
            b[j] = rem;
            j++;
        }
        for (int j = 0; j < 4; j++)
        {
            arry[31 - (4 * i) - j] = b[3 - j];
        }
    }
}

void bintodec(char bin[],int size)
{
    int dec=0,j=0;
    for(int i=size-1;i>=0;i--){
        dec=dec+(bin[i]*(2^j));
        j++;
    }
}

void dectobin(int num,int *b,int size)
{
    int rem, i;
    if (num >= 0)
    {
        int array[size+1];
        while (i < size)
        {
            rem = num % 2;
            num = num / 2;
            array[i] = rem;
            i++;
        }
        for (i = size-1; i >= 0; i--)
        {
            b[size-1-i]=array[i];
        }
    }
    else
    {
        int a1[size+1], a2[size+1], carry;
        num = num * (-1);
        while (i < size)
        {
            rem = num % 2;
            num = num / 2;
            if (rem == 0)
            {
                a2[i] = 1;
            }
            else
            {
                a2[i] = 0;
            }
            i++;
        }
        carry = (a2[0] + 1) / 2;
        a1[0] = (a2[0] + 1) % 2;
        for (i = 1; i < size; i++)
        {
            a1[i] = (a2[i] + carry) % 2;
            carry = (carry + a2[i]) / 2;
        }
        for (i = size-1; i >= 0; i--)
        {
            b[size-1-i]=a1[i];
        }
    }
}

void run_riscvsim()
{
    while (1)
    {
        fetch();
        decode();
        execute();
        mem();
        write_back();
    }
}

// it is used to set the reset values
// reset all registers and memory content to 0
void reset_proc()
{
}

int read_word(char *mem, unsigned int address);
void write_word(char *mem, unsigned int address, unsigned int data);

// load_program_memory reads the input memory, and pupulates the instruction
//  memory
void load_program_memory(char *file_name)
{
    FILE *fp;
    unsigned int address, instruction;
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("Error opening input mem file\n");
        exit(1);
    }
    while (fscanf(fp, "%x %x", &address, &instruction) != EOF)
    {
        write_word(MEM, address, instruction);
    }
    fclose(fp);
}

// writes the data memory in "data_out.mem" file
void write_data_memory()
{
    FILE *fp;
    unsigned int i;
    fp = fopen("data_out.mem", "w");
    if (fp == NULL)
    {
        printf("Error opening dataout.mem file for writing\n");
        return;
    }

    for (i = 0; i < 4000; i = i + 4)
    {
        fprintf(fp, "%x %x\n", i, read_word(MEM, i));
    }
    fclose(fp);
}

// should be called when instruction is swi_exit
void swi_exit()
{
    write_data_memory();
    exit(0);
}

// reads from the instruction memory and updates the instruction register
void fetch()
{
}
// reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode()
{
}
// executes the ALU operation based on ALUop
void execute()
{
}
// perform the memory operation
void mem()
{
}
// writes the results back to register file
void write_back()
{
}

int read_word(char *mem, unsigned int address)
{
    int *data;
    data = (int *)(mem + address);
    return *data;
}

void write_word(char *mem, unsigned int address, unsigned int data)
{
    int *data_p;
    data_p = (int *)(mem + address);
    *data_p = data;
}
