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
MipsCode translate_MipsCode(InterCode IC_code){
	MipsCode temp = new_MipsCode();
	Operand_M opm1 = NULL,opm2 = NULL,opm3 = NULL;
	switch(IC_code){
		case ASSIGN:
			if(IC_code->assign.right->kind == CONSTANT){
				temp->assign.left->kind = MIP_CONSTANT;
				opm1 = new_Operand_M(0,IC_code->assign.right->value);
				opm2 = new_Operand_M();
				temp->MIP_ASSIGN.right = opm1;
				temp->MIP_ASSIGN.left = opm2;
			}
			break;
		case ADD:
			break;
		case SUB:
			break;
		case MUL:
			break;
		case DIVI:
			break;
		case LAB:
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
}