#include "mipscode.h"

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
		q = translate_MipsCode(p->code);
		p = p->next;
		q = q->next;
	}
	Mips_head = q;
}
void get_reg(){
}
void release_reg(){
}

MipsCode translate_MipsCode(InterCode IC_code){
	//MipsCode temp = new_MipsCode();
	Operand_M opm1 = NULL,opm2 = NULL,opm3 = NULL;
	MipsCodes head = NULL;
	int reg_no;
	switch(IC_code){
		case ASSIGN:
			MipsCode temp = new_MipsCode(MIP_ASSIGN);
			//x:=#k
			if(IC_code->assign.right->kind == CONSTANT){
				temp->assign.left->kind = MIP_CONSTANT;
				opm1 = new_Operand_M(0,IC_code->assign.right->value);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				temp->assign.right = opm1;
				temp->assign.left = opm2;
				temp->kind = MIP_LI;
				MipsCodes_link(head,temp);
				return 
			}
			//x:=y
			else if(IC_code->assign.right->kind == VARIABLE || IC_code->assign.right->kind == TEMP){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				temp->kind = MIP_MOVE;
				MipsCodes_link(head,temp);
			}
			//x:=*y
			else if(IC_code->assign.right->kind == ADDR_op && IC_code->assign.left->kind != ADDR_op){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_addr(reg_no,0);
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				temp->assign->kind = MIP_LW;
				MipsCodes_link(head,temp);
			}
			//*x:=y
			else if(IC_code->assign.left->kind == ADDR_op && IC_code->assign.rigt->kind != ADDR_op){
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				reg_no = get_reg();
				opm1 = new_addr(reg_no,0);
				temp->assign.left = opm1;
				temp->assign.right = opm2;
				temp->assign->kind = MIP_SW;
				MipsCodes_link(head,temp);
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
				temp->kind = MIP_ADDI;
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;				
			}
			//x:=#k+y
			if(IC_code->binop.op2->kind != ADDR_op && IC_code->binop.op2->kind != CONSTANT&&IC_code->binop.op1->kind == CONSTANT){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm3 = new_reg(reg_no);
				opm2 = new_Operand_M(0,IC_code->binop.op2->value);
				temp->kind = MIP_ADDI;
				temp->binop->result = opm1;
				temp->binop->left = opm2;
				temp->binop->right = opm3;
			}
			//x:=y+*z
			if(){
				reg_no = get_reg();
				opm1 = new_reg(reg_no);
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
				
			}
			//x:=*y+z
			//x:=*y+*z
			//x:=y+&z
			//x:=&y+z
			//x:=*y+#k
			//x:=#k+*y
			//x:=#k+#k
			break;
		case SUB:
			break;
		case MUL:
			break;
		case DIVI:
			break;
		case LAB:
			MipsCode temp = new_MipsCode(MIP_LAB);
			temp->onlyop.op = new_Operand_M(MIP_LABEL,IC_code->onlyop.op->label_no);
			break;
		case RET:
			break;
		case GOTO:
			break;
		case ADDR:
			break;
		case COND:
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
