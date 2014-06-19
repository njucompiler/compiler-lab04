#include "mipscode.h"
#include <string.h>
#include <assert.h>
<<<<<<< HEAD
#include <stdio.h>
=======
#include "intercode.h"
>>>>>>> 4e86b57391e0c79b72784bfc3590cd165f736fb2

#define REG_TABLE_SIZE 14
FILE *fp;
int arg_num = 0;
int reg_t,reg_a,reg_v;
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

void print_Operand_M(Operand_M p){
        assert(p != NULL);
        switch(p->kind){
                case MIP_CONSTANT:
                      fprintf(fp, "#%d", p->value);
                      break;
                case MIP_LABEL:
                      fprintf(fp, "label%d:", p->label_no);
                      break;
                case MIP_FUNC_op:
                      fprintf(fp, "%s:" ,p->func);
                      break;
                case MIP_ADDR_op:
                      fprintf(fp, "%d(%s)", p->addr.addr_reg_no, p->addr.offset); 
                      break;
                case MIP_REG:
                      fprintf(fp, "%s", reg[p->reg_no]);
                      break;
                default:
                        break;
        }
}

void print_MIP_LAB(MipsCodes p){
	print_Operand_M(p->code->onlyop.op);
}

void print_MIP_LI(MipsCodes p){
	fputs("li ",fp);
	print_Operand_M(p->code->assign.left);
	fputs(", ",fp);
	print_Operand_M(p->code->assign.right);
}

void print_MIP_LA(MipsCodes p){
	fputs("la ",fp);
	print_Operand_M(p->code->assign.left);
	fputs(", ",fp);
	print_Operand_M(p->code->assign.right);
}

void print_MIP_MOVE(MipsCodes p){
	fputs("move ",fp);
	print_Operand_M(p->code->assign.left);
	fputs(", ",fp);
	print_Operand_M(p->code->assign.right);
}

void print_MIP_ADDI(MipsCodes p){
	fputs("addi ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_ADD(MipsCodes p){
	fputs("add ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_SUB(MipsCodes p){
	fputs("sub ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_MUL(MipsCodes p){
	fputs("mul ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_DIV(MipsCodes p){
	fputs("div ",fp);
	print_Operand_M(p->code->assign.right);
	fputs(", ",fp);
	print_Operand_M(p->code->assign.left);
}

void print_MIP_MFLO(MipsCodes p){
	fputs("mflo ",fp);
	print_Operand_M(p->code->onlyop.op);
}

void print_MIP_LW(MipsCodes p){
	fputs("lw ",fp);
	print_Operand_M(p->code->assign.right);
	fputs(", ",fp);
	print_Operand_M(p->code->assign.left);
}

void print_MIP_SW(MipsCodes p){
	fputs("sw ",fp);
	print_Operand_M(p->code->assign.right);
	fputs(", ",fp);
	print_Operand_M(p->code->assign.left);
}

void print_MIP_J(MipsCodes p){
	fputs("j ",fp);
	print_Operand_M(p->code->onlyop.op);
}

void print_MIP_JAL(MipsCodes p){
	fputs("jal ",fp);
	print_Operand_M(p->code->onlyop.op);
}

void print_MIP_JR(MipsCodes p){
	fputs("jr ",fp);
	print_Operand_M(p->code->onlyop.op);
}

void print_MIP_BEQ(MipsCodes p){
	fputs("beq ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_BNE(MipsCodes p){
	fputs("bne ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_BGT(MipsCodes p){
	fputs("bgt ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_BLT(MipsCodes p){ 
	fputs("blt ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_BGE(MipsCodes p){
	fputs("bge ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_BLE(MipsCodes p){
	fputs("ble ",fp);
	print_Operand_M(p->code->binop.result);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op1);
	fputs(", ",fp);
	print_Operand_M(p->code->binop.op2);
}

void print_MIP_READ(MipsCodes p){
     	fputs("addi $sp, $sp, -4\n", fp);
     	fputs("sw $ra, 0($sp)\n", fp);
	fputs("jal read\n",fp);
	fputs("lw $ra, 0($sp)\n",fp);
	fputs("addi $sp, $sp, 4\n",fp);
	fputs("move ",fp);
	print_Operand_M(p->code->assign.right);
	fputs(", $v0",fp);
}

void print_MIP_WRITE(MipsCodes p){
     	fputs("move $a0, ",fp);
	print_Operand_M(p->code->assign.left);
	fputs("addi $sp, $sp, -4\n", fp);
     	fputs("sw $ra, 0($sp)\n", fp);
	fputs("jal write\n",fp);
	fputs("lw $ra, 0($sp)\n",fp);
	fputs("addi $sp, $sp, 4",fp);
}

void print_MipsCodes(char* output){
	fp = fopen(output,"w");
	if ( !fp )
	{
		perror(output);
		return ;
	}
	MipsCodes p = Mips_head->next;assert(p->code!=NULL);
	while(p!=NULL){
		switch(p->code->kind){
			case MIP_LAB:
				printf_MIP_LAB(p);
				break;
			case MIP_LI:
				printf_MIP_LI(p);
				break;
			case MIP_LA:
				printf_MIP_LA(p);
				break;
			case MIP_MOVE:
				printf_MIP_MOVE(p);
				break;
			case MIP_ADDI:
				printf_MIP_ADDI(p);
				break;
			case MIP_ADD:
				printf_MIP_ADD(p);
				break;
			case MIP_SUB:
				printf_MIP_SUB(p);
				break;
			case MIP_MUL:
				printf_MIP_MUL(p);
				break;
			case MIP_DIV:
				printf_MIP_DIV(p);
				break;
			case MIP_MFLO:
				printf_MIP_MFLO(p); 
                                break; 
                        case MIP_LW:
				printf_MIP_LW(p);
				break;
			case MIP_SW:
				printf_MIP_SW(p);
				break;
			case MIP_J:
				printf_MIP_J(p);
				break;
			case MIP_JAL:
				printf_MIP_JAL(p);
				break;
			case MIP_JR:
				printf_MIP_JR(p);
				break; 
			case MIP_BEQ: 
				printf_MIP_BEQ(p);
				break;
			case MIP_BNE:
				printf_MIP_BNE(p);
				break;
			case MIP_BGT:
				printf_MIP_BGT(p);
				break;
			case MIP_BLT:
				printf_MIP_BLT(p);
				break;
			case MIP_BGE:
				printf_MIP_BGE(p);
				break;
			case MIP_BLE:
				printf_MIP_BLE(p);
				break;
			case MIP_READ:
				printf_MIP_READ(p);
				break;
			case MIP_WRITE:
				printf_MIP_WRITE(p);
				break;
			default:
				break;

		}
		//if(p->code->kind!=COND)
		//	fputs("\n",fp);
		p=p->next;
	}
	fclose(fp);
}

void regtable_init(){
	reg_t = -1; reg_a = -1; reg_v = -1;
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
	Mips_head = MipsCodes_init();
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
	if(kind == 0)//constant
		op->value = value;
	else if(kind == MIP_FUNC_op)
		;
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
	Operand_M op = (Operand_M)malloc(sizeof(Operand_M_));
	op->kind = MIP_ADDR_op;
	op->addr.addr_reg_no = no;
	op->addr.offset = offset;
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
int get_reg(int kind){//0 $t0 ,1 $a0 ,2 $v0
	if(kind == 0){
                int i;
        	for(i = 0; i < 8; i++){
        		if(regtable[i]->kind == NO_USE){
			        regtable[i] == REG_USE;
                                return i;

		        }
	        }
                reg_t ++;
                return (reg_t % 8 + 8);
        }
        else if(kind == 1){
                int i;
                for(i = 8; i < 12; i++){
                        if(regtable[i]->kind == NO_USE){
                                regtable[i]->kind = REG_USE;
                                return i-4; 
                        }
                }
                reg_a ++;
                return (reg_a % 4 + 4);
        }
        else if(kind == 2){
                int i;
                for(i = 12; i < 14; i++){
                        if(regtable[i]->kind == NO_USE){
                                regtable[i]->kind = REG_USE;
                                return i-10;
                        }
                }
                reg_v++;
                return (reg_v % 2 + 2);
        }
}

/*int get_reg_name(char* name, int kind){
	if(kind == 0){
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
        else if(kind == 1){
                int i, j = 0;
                for(i = 8; i < 12; i++){
                        if(regtable[i]->kind == NO_USE){
                                regtable[i]->kind = REG_INT;
                                return i-4; 
                        }
                }
                return 4;
        }
        else if(kind == 2){
                int i, j = 0;
                for(i = 12; i < 14; i++){
                        if(regtable[i]->kind == NO_USE){
                                regtable[i]->kind = REG_INT;
                                return i-10;
                        }
                }
                return 12;
        }
}*/

MipsCode translate_MipsCode(InterCodes IC_codes){
	InterCode IC_code = IC_codes->code->kind;
	//MipsCode temp = new_MipsCode();
	Operand_M opm1 = NULL,opm2 = NULL,opm3 = NULL;
	MipsCodes Mips_head = NULL;
	int reg_no;
	switch(IC_code){
		case ASSIGN:
			//x:=#k
			if(IC_code->assign.right->kind == CONSTANT){
				//temp->assign.left->kind = MIP_CONSTANT;
				opm1 = new_Operand_M(0,IC_code->assign.right->value);
				reg_no = get_reg(0);
				opm2 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.right = opm1;
				temp->assign.left = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				return 
			}
			//x:=y
			else if(IC_code->assign.right->kind == VARIABLE || IC_code->assign.right->kind == TEMP){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_MOVE);
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=*y
			else if(IC_code->assign.right->kind == ADDR_op && IC_code->assign.left->kind != ADDR_op){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//*x:=y
			else if(IC_code->assign.left->kind == ADDR_op && IC_code->assign.rigt->kind != ADDR_op){
				reg_no = get_reg(0);
				opm2 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm1 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_SW);
				temp->assign.left = opm2;
				temp->assign.right = opm1;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			break;
		case ADD:
			//x:=y+#k
			if(IC_code->binop.op1->kind != ADDR_op && IC_code->binop.op1->kind != CONSTANT&&IC_code->binop.op2->kind == CONSTANT){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_reg(reg_no);
				opm3 = new_Operand_M(0,IC_code->binop.op1->value);
				MipsCode temp = new_MipsCode(MIP_ADDI);
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;	
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);			
			}
			//x:=#k+y
			if(IC_code->binop.op2->kind != ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op1->kind == CONSTANT){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm3 = new_reg(reg_no);
				opm2 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCode temp = new_MipsCode(MIP_ADDI);
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=y+*z
			if(IC_code->binop.op2->kind == ADDR_op && IC_code->binop.op1->kind != CONSTANT&&IC_code->binop.op1->kind != ADDR_op){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(z)
				temp->assign.left = opm1;
				temp->assign.right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADD);		//add reg(x) reg(y) reg(x)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm2;
				temp1->binop.op2 = opm1;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=*y+z
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op2->kind != ADDR_op){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				opm3 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADD);		//add reg(x) reg(x) reg(z)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = opm3;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=*y+*z
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind == ADDR_op){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp2 = new_MipsCode(MIP_LW);		//lw reg(x) reg(z)
				temp2->assign.left = opm1;
				temp2->assign.right = opm3;
				MipsCodes tem2 = MipsCodes_init();
				tem2->code = temp2;
				MipsCodes_link(Mips_head,tem2);
				MipsCode temp1 = new_MipsCode(MIP_ADD);		//add reg(x) reg(x) reg(z)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = opm3;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=y+&z
			//x:=&y+z
			//x:=*y+#k
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind == CONSTANT){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				//reg_no = get_reg();
				//opm3 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADDI);		//addi reg(x) reg(x) k
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op3 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
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
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADDI);		//addi reg(x) k reg(x)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=#k+#k
			break;
		case SUB:
			//x:=y-#k
			if(IC_code->binop.op1->kind != ADDR_op && IC_code->binop.op1->kind != CONSTANT&&IC_code->binop.op2->kind == CONSTANT){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_reg(reg_no);
				opm3 = new_Operand_M(0,IC_code->binop.op1->value);
				MipsCode temp = new_MipsCode(MIP_ADDI);
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;	
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);			
			}
			//x:=#k-y
			if(IC_code->binop.op2->kind != ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op1->kind == CONSTANT){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm3 = new_reg(reg_no);
				opm2 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCode temp = new_MipsCode(MIP_ADDI);
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=y-*z
			if(IC_code->binop.op2->kind == ADDR_op && IC_code->binop.op1->kind != CONSTANT&&IC_code->binop.op1->kind != ADDR_op){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(z)
				temp->assign.left = opm1;
				temp->assign.right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp1 = new_MipsCode(MIP_SUB);		//add reg(x) reg(y) reg(x)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm2;
				temp1->binop.op2 = opm1;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=*y-z
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op2->kind != ADDR_op){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				opm3 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp1 = new_MipsCode(MIP_SUB);		//SUB reg(x) reg(x) reg(z)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = opm3;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=*y-*z
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind == ADDR_op){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp2 = new_MipsCode(MIP_LW);		//lw reg(x) reg(z)
				temp2->assign.left = opm1;
				temp2->assign.right = opm3;
				MipsCodes tem2 = MipsCodes_init();
				tem2->code = temp2;
				MipsCodes_link(Mips_head,tem2);
				MipsCode temp1 = new_MipsCode(MIP_SUB);		//SUB reg(x) reg(x) reg(z)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = opm3;
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=y-&z
			//x:=&y-z
			//x:=*y-#k
			if(IC_code->binop.op1->kind == ADDR_op && IC_code->binop.op2->kind == CONSTANT){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				//reg_no = get_reg();
				//opm3 = new_reg(reg_no);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADDI);		//addi reg(x) reg(x) k
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op3 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=#k-*y				//?????????????????????????
			if(IC_code->binop.op1->kind == CONSTANT && IC_code->binop.op2->kind == ADDR_op){
				reg_no = get_reg(0);
				opm1 = new_reg(reg_no);
				//reg_no = get_reg();
				//opm2 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				opm3 = new_addr(reg_no,0);
				MipsCode temp = new_MipsCode(MIP_LW);		//lw reg(x) reg(y)
				temp->assign.left = opm1;
				temp->assign.right = opm3;
				MipsCodes tem = MipsCodes_init();
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
				MipsCode temp1 = new_MipsCode(MIP_ADDI);		//addi reg(x) k reg(x)
				temp1->binop.result = opm1;
				temp1->binop.op1 = opm1;
				temp1->binop.op2 = new_Operand_M(0,IC_code->binop.op2->value);
				MipsCodes tem1 = MipsCodes_init();
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
			//x:=#k+#k
			break;
		case MUL:
			reg_no = get_reg(0);
			opm1 = new_reg(reg_no);
			//x:= y * #k
			if(IC_code->binop.op2->kind == CONSTANT){
				opm3 = new_Operand_M(0,IC_code->binop.op2->value);
				reg_no = get_reg(0);
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:= #k * y
			if(IC_code->binop.op1->kind == CONSTANT){
				opm2 = new_Operand_M(0,IC_code->binop.op1->value);
				reg_no = get_reg(0);
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=*y*z
			if(IC_code->binop.op1->kind == CONSTANT){		// temp:=*y
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=y**z
			if(IC_code->binop.op2->kind == CONSTANT){		// temp:=*z
				reg_no = get_reg(0);
				opm3 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=y*z
			MipsCodes tem1 = MipsCodes_init();
			MipsCode temp1 = new_MipsCode(MIP_MUL);
			temp1->binop.result = opm1;
			temp1->binop.op1 = opm2;
			temp1->binop.op2 = opm3;
			tem1->code = temp1
			MipsCodes_link(Mips_head,tem1);
			break;
		case DIVI:
			reg_no = get_reg(0);
			opm1 = new_reg(reg_no);
			//x:= y / #k
			if(IC_code->binop.op2->kind == CONSTANT){
				opm3 = new_Operand_M(0,IC_code->binop.op2->value);
				reg_no = get_reg(0);
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:= #k / y
			if(IC_code->binop.op1->kind == CONSTANT){
				opm2 = new_Operand_M(0,IC_code->binop.op1->value);
				reg_no = get_reg(0);
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LI);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=*y/z
			if(IC_code->binop.op1->kind == ADDR_op){		// temp:=*y
				reg_no = get_reg(0);
				opm2 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm2;
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=y/*z
			if(IC_code->binop.op2->kind == ADDR_op){		// temp:=*z
				reg_no = get_reg(0);
				opm3 = new_addr(reg_no,0);
				reg_no = get_reg(0);
				Operand_M opm_tem = new_reg(reg_no);
				MipsCodes tem = MipsCodes_init();
				MipsCode temp = new_MipsCode(MIP_LW);
				temp->assign.left = opm_tem;
				temp->assign.right = opm3;
				tem->code = temp;
				MipsCodes_link(Mips_head,tem);
			}
			//x:=y/z
			MipsCodes tem1 = MipsCodes_init();
			MipsCode temp1 = new_MipsCode(MIP_DIV);
			temp1->assign.left = opm2;
			temp1->assign.right = opm3;
			tem1->code = temp1
			MipsCodes_link(Mips_head,tem1);
			MipsCodes tem2 = MipsCodes_init();
			MipsCode temp2 = new_MipsCode(MIP_MFLO);
			temp->onlyop.op = opm1;
			tem2->code = temp2
			MipsCodes_link(Mips_head,tem2);
			break;
		case LAB:
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCode(MIP_LAB);
			temp->onlyop.op = new_Operand_M(MIP_LABEL,IC_code->onlyop.op->label_no);
			tem->code = temp;
			MipsCodes_link(Mips_head,tem);
			break;
		case RET:
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCode(MIP_MOVE);
                        reg_no = get_reg(2)
			opm1 = new_reg(reg_no);
			temp->assign.left = opm1;
			//return #k
			if(IC_code->onlyop.op->kind == CONSTANT){
				opm2 = new_Operand_M(0,IC_code->onlyop.op->value);
			}
			else {
				reg_no = get_reg(0);
				opm2 = new_reg(reg_no);
			}
			temp->assign.right = opm2;
			tem->code = temp;
			MipsCodes_link(Mips_head,tem);
			break;
		case GOTO:
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCode(MIP_J);
			temp->onlyop.op = new_Operand_M(MIP_LABEL,IC_code->onlyop.op->label_no);
			tem-.code = temp;
			MipsCodes_link(Mips_head,tem);
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
				MipsCodes_link(Mips_head,tem);
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
				MipsCodes_link(Mips_head,tem);
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
			reg_no = get_reg(0);
			Operand_M opm1 = new_reg(reg_no);
			reg_no = get_reg(0);
			Operand_M opm2 = new_reg(reg_no);
			temp->binop.result = opm1;
			temp->binop.op1 = opm2;
			opm3 = new_Operand_M(MIP_LABEL,IC_codes->next->code->onlyop.op->label_no);
			temp->binop.op2 = opm3;
			tem->code = temp;
			MipsCodes_link(Mips_head,tem);
			break;
		case FUNC_I:
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCodes(MIP_FUNC);
			opm1 = new_Operand_M(MIP_FUNC_op,0);
			strcmp(opm1->func,IC_code->onlyop.op->func);
			temp->onlyop.op = opm1;
			tem->code = temp;
			MipsCodes_link(Mips_head,tem);
			break;
		case DEC:
			break;
		case READ:
			MipsCodes tem = MipsCodes_init();
                        MipsCode temp = new_MipsCode(MIP_READ);
			tem->code = temp;
			reg_no = get_reg(0);
			opm1 = new_reg(reg_no);
			temp->onlyop.op = opm1;
			MipsCodes_link(Mips_head,tem);
			break;
		case WRITE:
                        MipsCodes tem = MipsCodes_init();
                        MipsCode temp = new_MipsCode(MIP_WRITE);
                        tem->code = temp;
                        reg_no = get_reg(0);
                        opm1 = new_reg(reg_no);
                        temp->onlyop.op = opm1;
                        MipsCodes_link(Mips_head,tem);
			break;
		case CALL:
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCode(MIP_ADDI);
			opm1 = new_reg(29);
			opm2 = new_reg(29);
			opm3 = new_Operand_M(0,0-4*(arg_num+1));
			temp->binop.result = opm1;
			temp->binop.op1 = opm2;
			temp->binop.op2 = opm3;
			tem->code = temp;
			MipsCodes_link(Mips_head,tem);
			int i;
			for(i = 0;i<arg_num;i++){
				MipsCodes tem1 = MipsCodes_init();
				MipsCode temp1 = new_MipsCode(MIP_SW);
				temp1->assign.left = new_reg(i+4);
				temp1->assign.right = new_addr(29,4*i);
				//temp1->binop.op2 = new_Operand_M(4*i);
				tem1->code = temp1;
				MipsCodes_link(Mips_head,tem1);
			}
                        MipsCodes tem2 = MipsCodes_init();
                        MipsCode temp2 = new_MipsCode(MIP_SW);
                        temp2->assign.left = new_reg(31);
                        temp2->assign.right = new_addr(29,4*i);
                        //temp1->binop.op2 = new_Operand_M(4*i);
			tem2->code = temp2;
			MipsCodes_link(Mips_head,tem2);

			MipsCodes tem3 = MipsCodes_init();
			MipsCode temp3 = new_MipsCode(MIP_JAL);
			temp3->onlyop.op = new_Operand_M(MIP_FUNC_op);
			strcpy(temp3->onlyop.op->func,IC_code->binop.op2->func);
			tem3->code = temp3;
                        MipsCodes_link(Mips_head,tem3);
			
                        for(i = 0;i<arg_num;i++){
                                MipsCodes tem1 = MipsCodes_init();
                                MipsCode temp1 = new_MipsCode(MIP_LW);
                                temp1->assign.left = new_reg(i+4);
                                temp1->assign.right = new_addr(29,4*i);
                                //temp1->binop.op2 = new_Operand_M(4*i);
                                tem1->code = temp1;
                                MipsCodes_link(Mips_head,tem1);
                        }

                        MipsCodes tem4 = MipsCodes_init();
                        MipsCode temp4 = new_MipsCode(MIP_LW);
                        temp4->assign.left = new_reg(31);
                        temp4->assign.right = new_addr(29,4*i);
                        //temp1->binop.op2 = new_Operand_M(4*i);
                        tem4->code = temp4;
                        MipsCodes_link(Mips_head,tem4);

                        MipsCodes tem5 = MipsCodes_init();
                        MipsCode temp5 = new_MipsCode(MIP_ADDI);
                        opm1 = new_reg(29);
                        opm2 = new_reg(29);
                        opm3 = new_Operand_M(0,4*(arg_num+1));
                        temp5->binop.result = opm1;
                        temp5->binop.op1 = opm2;
                        temp5->binop.op2 = opm3;
                        tem5->code = temp5;
                        MipsCodes_link(Mips_head,tem5);

			arg_num = 0;
			break;
		case ARG:
			arg_num++;
			break;
		case PARAM_I:
                        MipsCodes tem = MipsCodes_init();
                        MipsCode temp = new_MipsCode(MIP_LI);
                        reg_no = get_reg(0);
			opm1 = new_reg(reg_no);
                 	opm2 = new_Operand_M(0,1);
                        temp->assign.left = opm1;
			temp->assign.right = opm2;
                        tem->code = temp;
                        MipsCodes_link(Mips_head,tem);
			break;
		default:
			printf("Oh,no! FORGET!\n");
			break;
	}
	return temp;
}

void cal_MipsCodes(char* output){
        printf_init();
        Mips_head_init();
        translate_MipsCodes(intercodes_head->next);
        print_MipsCodes(output);
}
