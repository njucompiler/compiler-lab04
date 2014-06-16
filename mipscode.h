#ifndef _MIPSCODE_H_
#define _MIPSCODE_H_

typedef struct Operand_M_* Operand_M;
typedef struct MipsCode_* MipsCode;
typedef struct MipsCodes_* MipsCodes;

typedef struct Operand_M_ {
	enum {  MIP_CONSTANT, MIP_LABEL, MIP_FUNC_op, MIP_ADDR_op, MIP_REG} kind;
	int is_min;
	union {
	char op[3];//==,<=,>=,!=,>,<
	char name[20];
	int reg_no;
	int var_no;
	int value;
	int label_no;
	char func[20];
	char param[20];
	struct addr_{
		int reg_no;
		int offset;
	}addr
	} ;
}Operand_M_;
typedef struct MipsCode_
{
	enum { MIP_ASSIGN, MIP_ADD, MIP_SUB, MIP_MUL, MIP_DIVI, MIP_LAB, MIP_GOTO, MIP_RET, MIP_ADDR, MIP_COND,MIP_FUNC_I,MIP_DEC,MIP_READ,MIP_WRITE,MIP_CALL,MIP_ARG,MIP_PARAM_I} kind;
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

static char* reg[32] = {"$zero", "Sat", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7" "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$s8", "Sra"};
void translate_MipsCodes(InterCodes IC_head);
void translate_MipsCode(InterCode IC_code);
void Mips_head_init();
MipsCodes MipsCodes_init();
void MipsCodes_link(MipsCodes prev,MipsCodes next);
MipsCode new_MipsCode(int kind);
Operand_M new_operand_M(int kind,int value);
//Operand_M new_operand_name(char* name);
Operand_M new_label_M();
//Operand_M new_temp();
Operand_M new_reg(no)

#endif
