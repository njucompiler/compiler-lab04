#ifndef _MIPSCODE_H_
#define _MIPSCODE_H_
#include "intercode.h"

typedef struct Operand_M_* Operand_M;
typedef struct MipsCode_* MipsCode;
typedef struct MipsCodes_* MipsCodes;
typedef struct RegTable_* RegTable;

typedef struct Operand_M_ {
	enum {  MIP_CONSTANT, MIP_LABEL, MIP_FUNC_op, MIP_ADDR_op, MIP_REG} kind;
	union {
	//char op[3];//==,<=,>=,!=,>,<
		int reg_no;
		int value;
		int label_no;
		char func[20];
		struct addr_{
			int addr_reg_no;
			int offset;
		}addr;
	};
}Operand_M_;

typedef struct MipsCode_
{
	enum { MIP_LAB, MIP_LI, MIP_LA, MIP_MOVE, MIP_ADDI, MIP_ADD, MIP_SUB, MIP_MUL, MIP_DIV, 
			MIP_MFLO, MIP_LW, MIP_SW, MIP_J, MIP_JAL, MIP_JR, MIP_BEQ, MIP_BNE, MIP_BGT, MIP_BLT,
			MIP_BGE, MIP_BLE, MIP_FUNC, MIP_READ, MIP_WRITE} kind;
	union {
		struct { Operand_M right, left; } assign;
		struct { Operand_M result, op1, op2; } binop;
		struct { Operand_M op; }onlyop;
		//struct { Operand op1, op, op2; }cond;
		};
}MipsCode_;

typedef struct MipsCodes_
{
	MipsCode code; 
	MipsCodes prev, next; 
}MipsCodes_;

MipsCodes Mips_head;

typedef struct RegTable_
{
	enum{NO_USE, REG_USE} kind;
	char name[20];
}RegTable_;

static char* reg[32] = {"$zero", "Sat", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7","$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7" ,"$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$s8", "$ra"};
void translate_MipsCodes(InterCodes IC_head);
void translate_MipsCode(InterCodes IC_code);
void Mips_head_init();
MipsCodes MipsCodes_init();
void MipsCodes_link(MipsCodes prev,MipsCodes next);
MipsCode new_MipsCode(int kind);
Operand_M new_operand_M(int kind,int value);
//Operand_M new_operand_name(char* name);
Operand_M new_label_M();
//Operand_M new_temp();
Operand_M new_reg(int no);
Operand_M new_addr(int no, int offset);

int get_other_reg(int kind);//1 $a,2 $v
int get_reg(Operand op);
//int get_reg_name(char* name,int kind);
//function of print
void regtable_init();

void cal_MipsCodes(char* output);
void print_MipsCodes(char* output);
void print_Operand_M(Operand_M);

void print_MIP_LAB(MipsCodes p);
void print_MIP_LI(MipsCodes p);
void print_MIP_LA(MipsCodes p);
void print_MIP_MOVE(MipsCodes p);
void print_MIP_ADDI(MipsCodes p);
void print_MIP_ADD(MipsCodes p);
void print_MIP_SUB(MipsCodes p);
void print_MIP_MUL(MipsCodes p);
void print_MIP_DIV(MipsCodes p);
void print_MIP_MFLO(MipsCodes p);
void print_MIP_LW(MipsCodes p);
void print_MIP_SW(MipsCodes p);
void print_MIP_J(MipsCodes p);
void print_MIP_JAL(MipsCodes p);
void print_MIP_JR(MipsCodes p);
void print_MIP_BEQ(MipsCodes p);
void print_MIP_BNE(MipsCodes p);
void print_MIP_BGT(MipsCodes p);
void print_MIP_BLT(MipsCodes p);
void print_MIP_BGE(MipsCodes p);
void print_MIP_BLE(MipsCodes p);
void print_MIP_FUNC(MipsCodes p);
void print_MIP_READ(MipsCodes p);
void print_MIP_WRITE(MipsCodes p);

#endif
