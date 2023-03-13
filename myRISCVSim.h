#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Register file
static unsigned int X[32];
// flags
// memory
static int MEM[4000];
int Machinecode[1000][32];
int leninst;
int IR[32];
int PC;
int arguments[5];
int ALU_output;

// intermediate datapath and control path signals
static unsigned int instruction_word;
static unsigned int operand1;
static unsigned int operand2;

int bintodec(int bin[], int size, bool k)
{
    int dec = 0, j = 1;
    for (int i = size - 1; i > 0; i--)
    {
        dec = dec + (bin[i] * (j));
        j=j*2;
    }
    if(k==1)
    dec=dec+((-1)*j*bin[0]);
    else
    dec = dec+(j*bin[0]);
    return dec;
}

void dectobin(int num, int *b, int size)
{
    int rem, i = 0;
    if (num >= 0)
    {
        int array[size + 1];
        while (i < size)
        {
            rem = num % 2;
            num = num / 2;
            array[i] = rem;
            i++;
        }
        for (i = size - 1; i >= 0; i--)
        {
            b[size - 1 - i] = array[i];
        }
    }
    else
    {
        int a1[size + 1], a2[size + 1], carry;
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
        for (i = size - 1; i >= 0; i--)
        {
            b[size - 1 - i] = a1[i];
        }
    }
}

void fetch(int *IR);
void decode(int IR[],int *arguments);
void execute(int arguments[]);
void memory_access(int arguments[]);
void write_back();
int read_word(char *mem, unsigned int address);
void write_word(char *mem, unsigned int address, unsigned int data);

void run_riscvsim()
{
    while (PC <= ((leninst - 1) * 4))
    {
        int IR[32];
        fetch(IR);
        decode(IR,arguments);
        execute(arguments);
        memory_access(arguments);
        write_back();
    }
}

// it is used to set the reset values
// reset all registers and memory content to 0
void reset_proc()
{
    for (int i = 0; i < 4000; i++)
    {
        if (i < 32)
        {
            X[i] = 0;
            MEM[i] = 0;
        }
        else
            MEM[i] = 0;
    }
}

// load_program_memory reads the input memory, and pupulates the instruction
//  memory
void load_program_memory(char *file_name)
{
    FILE *fp;
    int address, instruction;
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("Error opening input mem file\n");
        exit(1);
    }
    int i = 0;
    while (fscanf(fp, "%X %X", &address, &instruction) != EOF)
    {
        int Arr[32];
        dectobin(instruction, Arr, 32);
        for (int j = 0; j < 32; j++)
            Machinecode[i][j] = Arr[j];
        write_word(MEM, address, instruction);
        i++;
    }
    fclose(fp);
    leninst = i;
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
        fprintf(fp, "%X %X\n", i, read_word(MEM, i));
    }
    fclose(fp);
}

// should be called when instruction is swi_exit
void swi_exit()
{
    write_data_memory();
    exit(0);
}

int branch(int taken, int imm)
{
    if (taken == 1)
        printf("Branch is taken");
    else
        printf("Branch is not taken");
    PC = PC + imm;
    return PC;
}

// reads from the instruction memory and updates the instruction register
void fetch(int *IR)
{
    for (int i = 0; i < 32; i++)
        IR[i] = Machinecode[PC / 4][31 - i];
}

// reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode(int IR[],int *arguments)
{
    int opcode[8], func3[4], func7[8], rs1[6], rs2[6], rd[6], imm[21];
    int RS1, RS2, RD, Opcode, fun7, fun3,operation,Imm=0;
    // opcode=IR[6:0]    func3=IR[14:12] rd=IR[11:7] rs1=IR[19:15]   rs2=IR[24:20]   func7=IR[31:25]
    for (int i = 0; i <= 6; i++)
    {
        opcode[i] = IR[6 - i];
        func7[i] = IR[31 - i];
        if (i <= 2)
            func3[i] = IR[14 - i];
    }
    Opcode = bintodec(opcode, 7,0);
    fun7 = bintodec(func7, 7,0);
    fun3 = bintodec(func3, 3,0);
    for (int i = 0; i <= 4; i++)
    {
        rs1[i] = IR[19 - i];
        rs2[i] = IR[24 - i];
        rd[i] = IR[11 - i];
    }
    RS1 = bintodec(rs1, 5,0);
    RS2 = bintodec(rs2, 5,0);
    RD = bintodec(rd, 5,0);
    if(Opcode==51)
    {
       if(fun3==0 && fun7==0)
        {//add
            operation=1;
            printf("operation: add rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
        else if(fun3==0 && fun7==32)
        {//sub
            operation=2;
            printf("operation: sub rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
        else if(fun3==1)
        {//sll
            operation=3;
            printf("operation: sll rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
        else if(fun3==2)
        {//slt
            operation=4;
            printf("operation: slt rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
        else if(fun3==4)
        {//xor
            operation=5;
            printf("operation: xor rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
        else if(fun3==6)
        {//or
            operation=6;
            printf("operation: or rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
        else if(fun3==7)
        {//and
            operation=7;
            printf("operation: and rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
        else if(fun3==5 && fun7==0)
        {//srl
            operation=8;
            printf("operation: srl rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
        else if(fun3==5 && fun7==32)
        {//sra
            operation=9;
            printf("operation: sra rs1: %d rs2: %d rd: %d",RS1,RS2,RD);
        }
    }
    else if(Opcode==19)
    {
        for(int i=11;i>=0;i--)
        {
            imm[i]=IR[20+i];
        }
        Imm = bintodec(imm,12,1);       
        if(fun3==0)
        {//addi
            operation=10;
            printf("Operation: addi rs1: %d rd: %d imm: %d",RS1,RD,Imm);
        }
        else if(fun3==7)
        {//andi
            operation=11;
            printf("Operation: andi rs1: %d rd: %d imm: %d",RS1,RD,Imm);
        }
        else if(fun3==6)
        {//ori
            operation=12;
            printf("Operation: ori rs1: %d rd: %d imm: %d",RS1,RD,Imm);
        } 
    }
   else if(Opcode==3)
    {
        for(int i=11;i>=0;i--)
        {
            imm[i]=IR[20+i];
        }
        Imm=bintodec(imm,12,1); 
        if(fun3==0)
        {//lb
            operation=13;
            printf("Operation: load rs1: %d rd: %d imm: %d",RS1,RD,Imm);  
        }
        else if(fun3==1)
        {//lh
            operation=14;
            printf("Operation: load rs1: %d rd: %d imm: %d",RS1,RD,Imm);
        }
        else if(fun3==2)
        {//lw
            operation=15;
            printf("Operation: load rs1: %d rd: %d imm: %d",RS1,RD,Imm);
        }
    }
    else if(Opcode==103)
    {
        for(int i=11;i>=0;i--)
        {
            imm[i]=IR[20+i];
        }
        Imm = bintodec(imm,12,1);
        if(fun3==0)
        {//jalr
            operation=16;
            printf("Operation: jalr rs1: %d rd: %d imm: %d",RS1,RD,Imm);
        }
    }
    else if(Opcode==35)
    {
        for(int i=11;i>=0;i--)
        {
            if(i<=11 && i>=5){
                imm[i]=IR[20+i];
            }
            else if(i<=4 && i>=0){
                imm[i]=IR[i+7];
            }
        }
        Imm=bintodec(imm,12,1);
        if(fun3==0)
        {//sb
            operation=17;
            printf("Operation: store rs1: %d rs2: %d imm: %d",RS1,RS2,Imm);            
        }
        else if(fun3==1)
        {//sh
            operation=18;
            printf("Operation: store rs1: %d rs2: %d imm: %d",RS1,RS2,Imm);       
        }
        else if(fun3==2)
        {//sw
            operation=19;
            printf("Operation: store rs1: %d rs2: %d imm: %d",RS1,RS2,Imm);       
        }
    }
    else if(Opcode==101)
    {
        for(int i=11;i>=0;i--)
        {
            imm[11]=IR[31];
            imm[10]=IR[7];
            if(i<=9 && i>=4)
            imm[i]=IR[21+i];
            else if(i<=3 && i>=0)
            imm[i]=IR[8+i];
        }
        Imm=bintodec(imm,12,1);
        if(fun3==0)
        {//beq
            operation = 20;
            printf("Operation: beq rs1: %d rs2: %d imm: %d",RS1,RS2,Imm);
        }
        else if(fun3==1)
        {//bne
            operation = 21;
            printf("Operation: bne rs1: %d rs2: %d imm: %d",RS1,RS2,Imm);
        }
        else if(fun3==5)
        {//bge
            operation = 22;
            printf("Operation: bge rs1: %d rs2: %d imm: %d",RS1,RS2,Imm);
        }
        else if(fun3==4)
        {//blt
            operation = 23;
            printf("Operation: blt rs1: %d rs2: %d imm: %d",RS1,RS2,Imm);   
        }
    }
    else if(Opcode==111)
    {//jal
        operation = 24;
        printf("Operation: jal rd: %d imm: %d",RD,Imm);
    }
    else if(Opcode==55)
    {//lui
        for(int i=19;i>=0;i--)
        {
            imm[i]=IR[12+i];
        }
        Imm = bintodec(imm ,20,1);
        operation = 25;
        printf("Operation: lui rd: %d imm: %d",RD,Imm);
    }
    else if(Opcode==23)
    {//auipc
        for(int i=19;i>=0;i--)
        {
            imm[i]=IR[12+i];
        }
        Imm = bintodec(imm ,20,1);
        operation = 26;
        printf("Operation: auipc rd: %d imm: %d",RD,Imm);
    }
    arguments[0]=operation;
    arguments[1]=RS1;
    arguments[2]=RS2;
    arguments[3]=RD;
    arguments[4]=Imm;
}

// executes the ALU operation based on ALUop
void execute(int arguments[])
{
    int operation = arguments[0];
    int rs1_ = arguments[1];
    int rs2_ = arguments[2];
    int rd_ = arguments[3];
    int imm_ = arguments[4];
    int ALU_output;
    PC=PC+4;
    if (operation == 1)
    { // add
        ALU_output = X[rs1_] + X[rs2_];
        printf("ALU output of the add instruction is %d", ALU_output);
    }
    else if (operation == 2)
    { // sub
        ALU_output = X[rs1_] - X[rs2_];
        printf("ALU output of the sub instruction is %d", ALU_output);
    }
    else if (operation == 3)
    { // sll
        ALU_output = X[rs1_] << X[rs2_];
        printf("ALU output of the sll instruction is %d", ALU_output);
    }
    else if (operation == 4)
    { // slt
        ALU_output = X[rs1_] << X[rs2_];
        printf("ALU output of the slt instruction is %d", ALU_output);
    }
    else if (operation == 5)
    { // xor
        ALU_output = X[rs1_] ^ X[rs2_];
        printf("ALU output of the xor instruction is %d", ALU_output);
    }
    else if (operation == 6)
    { // or
        ALU_output = X[rs1_] | X[rs2_];
        printf("ALU output of the or instruction is %d", ALU_output);
    }
    else if (operation == 7)
    { // and
        ALU_output = X[rs1_] & X[rs2_];
        printf("ALU output of the sll instruction is %d", ALU_output);
    }
    else if (operation == 8)
    { // srl
        ALU_output = X[rs1_] << X[rs2_];
        printf("ALU output of the srl instruction is %d", ALU_output);
    }
    else if (operation == 9)
    { // sra
        ALU_output = X[rs1_] << X[rs2_];
        printf("ALU output of the sra instruction is %d", ALU_output);
    }
    else if (operation == 10)
    { // addi
        ALU_output = X[rs1_] + imm_;
        printf("ALU output of the addi instruction is %d", ALU_output);
    }
    else if (operation == 11)
    { // andi
        ALU_output = X[rs1_] & imm_;
        printf("ALU output of the andi instruction is %d", ALU_output);
    }
    else if (operation == 12)
    { // ori
        ALU_output = X[rs1_] | imm_;
        printf("ALU output of the ori instruction is %d", ALU_output);
    }
    else if (operation > 12 && operation < 20)
    { // lb lh lw jalr sb sh sw
        ALU_output = X[rs1_] + imm_;
        printf("ALU output of the instruction is %d", ALU_output);
    }
    else if (operation == 20)
    {//beq
        if (X[rs1_] == X[rs2_])
            ALU_output = 1;
        else
            ALU_output = 0;
        PC = branch(ALU_output, imm_);
    }
    else if (operation == 21)
    {//bne
        if (X[rs1_] != X[rs2_])
            ALU_output = 1;
        else
            ALU_output = 0;
        PC = branch(ALU_output, imm_);
    }
    else if (operation == 22)
    {//bge
        if (X[rs1_] >= X[rs2_])
            ALU_output = 1;
        else
            ALU_output = 0;
        PC = branch(ALU_output, imm_);
    }
    else if (operation == 23)
    {//blt
        if (X[rs1_] < X[rs2_])
            ALU_output = 1;
        else
            ALU_output = 0;
        PC = branch(ALU_output, imm_);
    }
    else if (operation == 24)
    {//jal
        ALU_output = PC + 4;
        PC = PC + imm_;
        printf("jump to %X",PC);
    }
    else if (operation == 25)
    {//lui
        ALU_output = imm_ << 12;
        printf("imm is shifted by 12")
    }
    else if (operation == 26)
    {//auipc
        ALU_output = PC + (imm_ << 12);
        printf("operation: PC+(imm<<12)")
    }
}

// perform the memory operation
void memory_access(int arguments[])
{
    if(arguments[0]==13||arguments[0]==14||arguments[0]==15)
    {
        
    }
    else if(arguments[0]==17||arguments[0]==18||arguments[0]==19);
}

// writes the results back to register file
void write_back()
{
}

int read_word(int *mem, unsigned int address)
{
    int *data;
    data = (int *)(mem + address);
    return *data;
}

void write_word(int *mem, unsigned int address, unsigned int data)
{
    int *data_p;
    data_p = (int *)(mem + address);
    *data_p = data;
}
