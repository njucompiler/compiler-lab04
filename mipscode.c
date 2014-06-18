#include "mipscode.h"
#include <string.h>
#include <assert.h>

#define REG_TABLE_SIZE 8
FILE *fp

RegTable regtable[REG_TABLE_SIZE];

void printf_init(){
        fprintf(fp, ".data\n");
        fprintf(fp, "_prompt: .asciiz \"Enter an integer:\"\n");
        fprintf(fp, "_ret: .asciiz \"\\n\"\n");
        fprintf(fp, ".globl main\n");
        fprintf(fp, ".text\n");
        fprintf(fp, "read:\n");
        fprintf(fp, "   li $v0, 4\n");
        fprintf(fp, "   la $a0, _prompt\n");
        fprintf(fp, "   syscall\n");
        fprintf(fp, "   li $v0, 5\n");
        fprintf(fp, "   syscall\n");
        fprintf(fp, "   jr $ra\n");
        fprintf(fp, "\n");
        fprintf(fp, "write\n");
        fprintf(fp, "   li $v0, 1\n");
        fprintf(fp, "   syscall\n");
        fprintf(fp, "   li $v0, 4\n");
        fprintf(fp, "   la $a0, _ret\n");
        fprintf(fp, "   syscall\n");
        fprintf(fp, "   move $v0, $0\n");
        fprintf(fp, "   jr $ra\n\n");
}

void printf_Operand_M(Operand_M p){
        assert(p != NULL);
        switch(p->kind){
                case: MIP_CONSTANT{
                      fprintf(fp, "#%d", p->value);
                      break;
                      }
                case: MIP_LABEL{
                      fprintf(fp, "label%d:", p->label_no);
                      break;
                      }
                case: MIP_FUNC_OP{
                      fprintf(fp, "%s:" ,func);
                      break;
                      }
                case: MIP_ADDR_OP{
                      fprintf(fp, "%d(%s)", p->addr_reg_no, p->offset); 
                      break;
                      }
                case: MIP_REG{
                      fprintf(fp, "%s", reg[p->reg_no]);
                      break;
                      }
                default:
                        break;
        }
}

void regtable_init(){
	int i;
	assert(regtable);
	for(i = 0; i < REG_TABLE_SIZE; i++){
		regtable[i] = (RegTable)malloc(sizeof(RegTable_));
		regtable[i]->kind = NO_USE;
	}
}

MipsCodes MipsCodes_init(){
	MipsCodes temp = (MipsCodes)malloc(sizeof(MipsCodes_));
	temp->prev = NULL;
	temp->next = NULL;
	return temp;
}
void MipsCodes_link(MipsCodes prev,MipsCodes next){
	if(prev == NULL){
		prev = next;
		return;
	}
	else if(next == NULL){
		return;
	}
	MipsCodes temp = prev;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = next;
	next->prev = prev;
}
void Mips_head_init(){
	MipsCodes_head = MipsCodes_init();
	var_no = 1;
}
MipsCode new_MipsCode(int kind){//init a new interCode
	MipsCode code = (MipsCode)malloc(sizeof(MipsCode_));
	code->kind = kind;
	/*if(kind == 0){//assignop
		code->assign = (assign *)malloc(sizeof(assign));
	}
	else{
		code->binop = (binop *)malloc(sizeof(binop));
	}*/
	return code;
}
Operand_M new_operand_M(int kind,int value){
	Operand_M op = (Operand_M)malloc(sizeof(Operand_M_));
	op->kind = kind;
	op->is_min = 0;
	if(kind == 0)//constant
		op->value = value;
	else 
		op->label_no = value;
	return op;
}
Operand_M new_reg(no){
	Operand_M op = (Operand_M)malloc(sizeof(Operand_M_));
	op->kind = MIP_REG;
	op->reg_no = no;
	return op;
}
Operand_M new_addr(int no, int offset){
	Operands op = (Operand_M)malloc(sizeof(Operand_M_));
	op->kind = MIPS_OP_ADDR;
	op->u.addr.reg_no = no;
	op->u.addr.offset = offset;
	return op;
}
void translate_MipsCodes(InterCodes IC_head){
	InterCodes p = IC_head;
	MipsCodes q;
	while(p!=NULL){
		q = new_MipsCodes();
		q = translate_MipsCode(p);
		if(p->code->kind == COND)
			p = p->next;
		p = p->next;
		q = q->next;
	}
	Mips_head = q;
}
int get_reg(int cst){
	int i,j = 0;
	for(i = 0; i < REG_TABLE_SIZE; i++){
		if(regtable[i]->kind == REG_INT){
			if(regtable[i]->value == cst){
				return i+8;
			}
		}
		else if(regtable[i]->kind == NO_USE){
			j = i;
		}
	}
	if(j == 0){
		free(regtable[0]);
		regtable[0] = (Reg_Table)malloc(sizeof(Reg_Table_));
		regtable[0]->kind = REG_INT;
		regtable[0]->value = cst;
		return 0+8; 
	}
	else{
		regtable[i]->kind = REG_INT;
		regtable[i]->value = cst;
		return j+8;
	}
}

int get_reg(char* name){
	int i,j = 0;
	for(i = 0; i < REG_TABLE_SIZE; i++){
		if(regtable[i]->kind == REG_NAME){
			if(strcmp(regtable[i]->name ,name) == 0){
				return i+8;
			}
		}
		else if(regtable[i]->kind == NO_USE){
			j = i;
		}
	}
	if(j == 0){
		free(regtable[0]);
		regtable[0] = (Reg_Table)malloc(sizeof(Reg_Table_));
		regtable[0]->kind = REG_NAME;
		strcpy(regtable[0]->name, name);
		return 8; 
	}
	else{
		regtable[j]->kind = REG_NAME;
		strcpy(regtable[j]->name, name);
		return j+8;
	}
}

MipsCode translate_MipsCode(InterCodes IC_codes){
	InterCode IC_code = IC_codes->code;
	//MipsCode temp = new_MipsCode();
	Operand_M opm1 = NULL,opm2 = NULL,opm3 = NULL;
	MipsCodes head = NULL;
	int reg_no;
	switch(IC_code){
		case ASSIGN:
			//x:=#k
			if(IC_code->assign.right->kind == CONSTANT){
				//temp->assign.left->kind = MIP_CONSTANT;
				opm1 = new_Operand_M(0,IC_code->assign.right->value);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.right = opm1;
				temp->assign.left = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				return 
			}
			//x:=y
			else if(IC_code->assign.right->kind == VARIABLE || IC_code->assign.right->kind == TEMP){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_MOVE);
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
			}
			//x:=*y
			else if(IC_code->assign.right->kind == ADDR_op && IC_code->assign.left->kind != ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
			}
			//*x:=y
			else if(IC_code->assign.left->kind == ADDR_op && IC_code->assign.rigt->kind != ADDR_op){
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				reg_no = get_reg();
				opm1 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_SW);
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
			}
			break;
		case ADD:
			//x:=y+#k
			if(IC_code->binop.op1->kind != ADDR_op && IC_code->binop.op1->kind != CONSTANT&&IC_code->binop.op2->kind == CONSTANT){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				opm3 = new_Operand_M(0,IC_code->binop.op1->value);
				MipsCode temp = new_MipsCode(MIP_ADDI);
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;	
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);			
			}
			//x:=#k+y
			if(IC_code->binop.op2->kind != ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op1->kind == CONSTANT){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm3 = new_reg(reg_no);
				opm2 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCode temp = new_MipsCode(MIP_ADDI);
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
			}
			//x:=y+*z
			if(IC_code->binop.op2->kind == ADDR_op && IC_code->binop.op1->kind != CONSTANT&&IC_code->binop.op1->kind != ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(z)
				temp->assign.left = opm1;
				temp->assign.right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADD);		//add reg(x) reg(y) reg(x)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm2;
				temp1->binop.op2 = opm1;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=*y+z
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op2->kind != ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				opm3 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADD);		//add reg(x) reg(x) reg(z)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = opm3;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=*y+*z
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind == ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp2 = new_MipsCode(MIP_LW);		//lw reg(x) reg(z)
				temp2->assign.left = opm1;
				temp2->assign.right = opm3;
				MipsCodes tem2 = MipsCodes_init();
				tem2->code = temp2
				MipsCodes_link(head,tem2);
				MipsCode temp1 = new_MipsCode(MIP_ADD);		//add reg(x) reg(x) reg(z)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = opm3;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=y+&z
			//x:=&y+z
			//x:=*y+#k
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind == CONSTANT){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				//reg_no = get_reg();
				//opm3 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADDI);		//addi reg(x) reg(x) k
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op3 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=#k+*y
			if(IC_code->binop.op1->kind == CONSTANT && IC_code->binop.op2->kind == ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				//reg_no = get_reg();
				//opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADDI);		//addi reg(x) k reg(x)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=#k+#k
			break;
		case SUB:
			//x:=y-#k
			if(IC_code->binop.op1->kind != ADDR_op && IC_code->binop.op1->kind != CONSTANT&&IC_code->binop.op2->kind == CONSTANT){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				opm3 = new_Operand_M(0,IC_code->binop.op1->value);
				MipsCode temp = new_MipsCode(MIP_ADDI);
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;	
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);			
			}
			//x:=#k-y
			if(IC_code->binop.op2->kind != ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op1->kind == CONSTANT){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm3 = new_reg(reg_no);
				opm2 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCode temp = new_MipsCode(MIP_ADDI);
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
			}
			//x:=y-*z
			if(IC_code->binop.op2->kind == ADDR_op && IC_code->binop.op1->kind != CONSTANT&&IC_code->binop.op1->kind != ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(z)
				temp->assign.left = opm1;
				temp->assign.right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp1 = new_MipsCode(MIP_SUB);		//add reg(x) reg(y) reg(x)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm2;
				temp1->binop.op2 = opm1;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=*y-z
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op2->kind != ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				opm3 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp1 = new_MipsCode(MIP_SUB);		//SUB reg(x) reg(x) reg(z)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = opm3;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=*y-*z
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind == ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp2 = new_MipsCode(MIP_LW);		//lw reg(x) reg(z)
				temp2->assign.left = opm1;
				temp2->assign.right = opm3;
				MipsCodes tem2 = MipsCodes_init();
				tem2->code = temp2
				MipsCodes_link(head,tem2);
				MipsCode temp1 = new_MipsCode(MIP_SUB);		//SUB reg(x) reg(x) reg(z)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = opm3;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=y-&z
			//x:=&y-z
			//x:=*y-#k
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind == CONSTANT){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				//reg_no = get_reg();
				//opm3 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADDI);		//addi reg(x) reg(x) k
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op3 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=#k-*y				//?????????????????????????
			if(IC_code->binop.op1->kind == CONSTANT && IC_code->binop.op2->kind == ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				//reg_no = get_reg();
				//opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp
				MipsCodes_link(head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADDI);		//addi reg(x) k reg(x)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(head,tem1);
			}
			//x:=#k+#k
			break;
		case MUL:
			reg_no = get_reg();
			opm1 = new_reg(reg_no);
			//x:= y * #k
			if(IC_code->binop.op2->kind == CONSTANT){
				opm3 = new_Operand_M(0,IC_code->binop.op2->value);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			//x:= #k * y
			if(IC_code->binop.op1->kind == CONSTANT){
				opm2 = new_Operand_M(0,IC_code->binop.op1->value);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			//x:=*y*z
			if(IC_code->binop.op1->kind == CONSTANT){		// temp:=*y
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			//x:=y**z
			if(IC_code->binop.op2->kind == CONSTANT){		// temp:=*z
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			//x:=y*z
			MipsCodes tem1 = MipsCodes_init();
			MipsCode temp1 = new_MipsCode(MIP_MUL);
			temp1->binop.result = opm1;
			temp1->binop.op1 = opm2;
			temp1->binop.op2 = opm3;
			tem1->code = temp1
			MipsCodes_link(head,tem1);
			break;
		case DIVI:
			reg_no = get_reg();
			opm1 = new_reg(reg_no);
			//x:= y / #k
			if(IC_code->binop.op2->kind == CONSTANT){
				opm3 = new_Operand_M(0,IC_code->binop.op2->value);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			//x:= #k / y
			if(IC_code->binop.op1->kind == CONSTANT){
				opm2 = new_Operand_M(0,IC_code->binop.op1->value);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			//x:=*y/z
			if(IC_code->binop.op1->kind == ADDR_op){		// temp:=*y
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			//x:=y/*z
			if(IC_code->binop.op2->kind == ADDR_op){		// temp:=*z
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			//x:=y/z
			MipsCodes tem1 = MipsCodes_init();
			MipsCode temp1 = new_MipsCode(MIP_DIV);
			temp1->assign.left = opm2;
			temp1->assign.right = opm3;
			tem1->code = temp1
			MipsCodes_link(head,tem1);
			MipsCodes tem2 = MipsCodes_init();
			MipsCode temp2 = new_MipsCode(MIP_MFLO);
			temp->onlyop.op = opm1;
			tem2->code = temp2
			MipsCodes_link(head,tem2);
			break;
		case LAB:
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCode(MIP_LAB);
			temp->onlyop.op = new_Operand_M(MIP_LABEL,IC_code->onlyop.op->label_no);
			tem-.code = temp;
			MipsCodes_link(head,tem);
			break;
		case RET:
			break;
		case GOTO:
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCode(MIP_J);
			temp->onlyop.op = new_Operand_M(MIP_LABEL,IC_code->onlyop.op->label_no);
			tem-.code = temp;
			MipsCodes_link(head,tem);
			break;
		case ADDR:
			break;
		case COND:
			/*if(IC_code->cond.op1->kind == CONSTANT){		// temp:=*y
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}
			if(IC_code->cond.op2->kind == CONSTANT){		// temp:=*x
				reg_no = get_reg();
				opm3 = new_addr(reg_no,0);
				reg_no = get_reg();
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(head,tem);
			}*/
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = NULL;
			//if x==y GOTO z
			if(strcmp(IC_code->cond.op->op,"==") == 0){
				temp = new_MipsCode(MIP_BEQ);
			}
			//if x!=y GOTO z
			if(strcmp(IC_code->cond.op->op,"!=") == 0){
				temp = new_MipsCode(MIP_BEQ);
			}
			//if x>y GOTO z
			if(strcmp(IC_code->cond.op->op,">") == 0){
				temp = new_MipsCode(MIP_BEQ);
			}
			//if x<y GOTO z
			if(strcmp(IC_code->cond.op->op,"<") == 0){
				temp = new_MipsCode(MIP_BEQ);
			}
			//if x>=y GOTO z
			if(strcmp(IC_code->cond.op->op,">=") == 0){
				temp = new_MipsCode(MIP_BEQ);
			}
			//if x<=y GOTO z
			if(strcmp(IC_code->cond.op->op,"<=") == 0){
				temp = new_MipsCode(MIP_BEQ);
			}
			reg_no = get_reg();
			Operand_M opm1 = new_reg(reg_no);
			reg_no = get_reg();
			Operand_M opm2 = new_reg(reg_no);
			temp->binop.result = opm1;
			temp->binop.op1 = opm2;
			opm3 = new_Operand_M(MIP_LABEL,IC_codes->next->code->onlyop.op->label_no);
			temp->binop.op2 = opm3;
			tem->code = temp;
			MipsCodes_link(head,tem);
			break;
		case FUNC_I:
			break;
		case DEC:
			break;
		case READ:
			break;
		case WRITE:
			break;
		case CALL:
			break;
		case ARG:
			break;
		case PARAM_I:
			break;

	}
	return temp;
}
