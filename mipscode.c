#include "mipscode.h"
int arg_num = 0;
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
void get_reg(){
}
void release_reg(){
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
				temp->assign.left = opm2;
				temp->assign.right = opm1;
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
			tem->code = temp;
			MipsCodes_link(head,tem);
			break;
		case RET:
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCode(MIP_MOVE);
			opm1 = new_reg(2);
			temp->assign.left = opm1;
			//return #k
			if(IC_code->onlyop.op->kind == CONSTANT){
				opm2 = new_Operand_M(0,IC_code->onlyop.op->value);
			}
			else {
				reg_no = get_reg();
				opm2 = new_reg(reg_no);
			}
			temp->assign.right = opm2;
			tem->code = temp;
			MipsCodes_link(head,tem);
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
			MipsCodes tem = MipsCodes_init();
			MipsCode temp = new_MipsCodes(MIP_FUNC);
			opm1 = new_Operand_M(MIP_FUNC_op,0);
			strcmp(opm1->func,IC_code->onlyop.op->func);
			temp->onlyop.op = opm1;
			tem->code = temp;
			MipsCodes_link(head,tem);
			break;
		case DEC:
			break;
		case READ:
			MipsCodes tem = MipsCodes_init();
                        MipsCode temp = new_MipsCode(MIP_READ);
			tem->code = temp;
			reg_no = get_reg();
			opm1 = new_reg(reg_no);
			temp->onlyop.op = opm1;
			MipsCodes_link(head,tem);
			break;
		case WRITE:
                        MipsCodes tem = MipsCodes_init();
                        MipsCode temp = new_MipsCode(MIP_WRITE);
                        tem->code = temp;
                        reg_no = get_reg();
                        opm1 = new_reg(reg_no);
                        temp->onlyop.op = opm1;
                        MipsCodes_link(head,tem);
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
			MipsCodes_link(head,tem);
			int i;
			for(i = 0;i<arg_num;i++){
				MipsCodes tem1 = MipsCodes_init();
				MipsCode temp1 = new_MipsCode(MIP_SW);
				temp1->assign.left = new_reg(i+4);
				temp1->assign.right = new_addr(29,4*i);
				//temp1->binop.op2 = new_Operand_M(4*i);
				tem1->code = temp1;
				MipsCodes_link(head,tem1);
			}
                        MipsCodes tem2 = MipsCodes_init();
                        MipsCode temp2 = new_MipsCode(MIP_SW);
                        temp2->assign.left = new_reg(31);
                        temp2->assign.right = new_addr(29,4*i);
                        //temp1->binop.op2 = new_Operand_M(4*i);
			tem2->code = temp2;
			MipsCodes_link(head,tem2);

			MipsCodes tem3 = MipsCodes_init();
			MipsCode temp3 = new_MipsCode(MIP_JAL);
			temp3->onlyop.op = new_Operand_M(MIP_FUNC_op);
			strcpy(temp3->onlyop.op->func,IC_code->binop.op2->func);
			tem3->code = temp3;
                        MipsCodes_link(head,tem3);
			
                        for(i = 0;i<arg_num;i++){
                                MipsCodes tem1 = MipsCodes_init();
                                MipsCode temp1 = new_MipsCode(MIP_LW);
                                temp1->assign.left = new_reg(i+4);
                                temp1->assign.right = new_addr(29,4*i);
                                //temp1->binop.op2 = new_Operand_M(4*i);
                                tem1->code = temp1;
                                MipsCodes_link(head,tem1);
                        }

                        MipsCodes tem4 = MipsCodes_init();
                        MipsCode temp4 = new_MipsCode(MIP_LW);
                        temp4->assign.left = new_reg(31);
                        temp4->assign.right = new_addr(29,4*i);
                        //temp1->binop.op2 = new_Operand_M(4*i);
                        tem4->code = temp4;
                        MipsCodes_link(head,tem4);

                        MipsCodes tem5 = MipsCodes_init();
                        MipsCode temp5 = new_MipsCode(MIP_ADDI);
                        opm1 = new_reg(29);
                        opm2 = new_reg(29);
                        opm3 = new_Operand_M(0,4*(arg_num+1));
                        temp5->binop.result = opm1;
                        temp5->binop.op1 = opm2;
                        temp5->binop.op2 = opm3;
                        tem5->code = temp5;
                        MipsCodes_link(head,tem5);

			arg_num = 0;
			break;
		case ARG:
			arg_num++;
			break;
		case PARAM_I:
                        MipsCodes tem = MipsCodes_init();
                        MipsCode temp = new_MipsCode(MIP_LI);
                        reg_no = get_reg();
			opm1 = new_reg(reg_no);
                 	opm2 = new_Operand_M(0,1);
                        temp->assign.left = opm1;
			temp->assign.right = opm2;
                        tem->code = temp;
                        MipsCodes_link(head,tem);
			break;
		default:
			printf("Oh,no! FORGET!\n");
			break;
	}
	return temp;
}
