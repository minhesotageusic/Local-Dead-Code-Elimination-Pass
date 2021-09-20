/*
 *********************************************
 *  415 Compilers                            *
 *  Spring 2021                              *
 *  Students                                 *
 *********************************************
 */


#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "Instr.h"
#include "InstrUtils.h"

typedef struct contributionNode {
	int f1;
	int f2;
	struct contributionNode* next;
} ContributionNode;

int exists(ContributionNode* head, int field1, int field2) {
	if (head == NULL) return 0;
	ContributionNode* ptr = head;
	while (ptr != NULL) {
		if (ptr->f1 == field1 && ptr->f2 == field2) {
			return 1;
		}
		ptr = ptr->next;
	}
	return 0;
}

void add(ContributionNode* head, int field1, int field2) {
	if (head == NULL) return;
	if (exists(head, field1, field2) == 1) return;
	ContributionNode* ptr = (ContributionNode*)malloc(sizeof(ContributionNode));
	if (ptr == NULL) exit(1);
	ptr->f1 = field1;
	ptr->f2 = field2;
	ptr->next = NULL;
	while (head->next != NULL) {
		head = head->next;
	}
	head->next = ptr;
}

void removeInstr(Instruction * ptr) {
	if (ptr == NULL) return;
	Instruction* prev_instr_ptr = ptr->prev;
	Instruction* next_instr_ptr = ptr->next;
	if (prev_instr_ptr != NULL) {
		prev_instr_ptr->next = next_instr_ptr;
	}
	if (next_instr_ptr != NULL) {
		next_instr_ptr->prev = prev_instr_ptr;
	}
	ptr->next = NULL;
	ptr->prev = NULL;
	free(ptr);
}

int main(int argc, char *argv[])
{
        Instruction *InstrList = NULL;
	
	if (argc != 1) {
  	    fprintf(stderr, "Use of command:\n  deadcode  < ILOC file\n");
		exit(-1);
	}

	fprintf(stderr,"------------------------------------------------\n");
	fprintf(stderr,"        Local Deadcode Elimination\n               415 Compilers\n                Spring 2021\n");
	fprintf(stderr,"------------------------------------------------\n");

        InstrList = ReadInstructionList(stdin);

 
        /* HERE IS WHERE YOUR CODE GOES */
		Instruction* instr_ptr = NULL;
		Instruction* prev_instr_ptr = NULL;
		ContributionNode* head = NULL;
		if (InstrList != NULL) {
			head = (ContributionNode*)malloc(sizeof(ContributionNode));
			if (head == NULL) exit(1);
			head->f1 = 0;
			head->f2 = -1;
			head->next = NULL;
			instr_ptr = LastInstruction(InstrList);
			while (instr_ptr != NULL) {
				prev_instr_ptr = instr_ptr->prev;
				//outputAI
				if (instr_ptr->opcode == OUTPUTAI) {
					add(head, instr_ptr->field1, instr_ptr->field2);
				}
				//loadI
				else if (instr_ptr->opcode == LOADI) {
					if (exists(head, instr_ptr->field2, -1) == 0) {
						removeInstr(instr_ptr);
					}
				}
				//loadAI
				else if (instr_ptr->opcode == LOADAI) {
					if (exists(head, instr_ptr->field3, -1) == 0) {
						removeInstr(instr_ptr);
					}
					else {
						add(head, instr_ptr->field1, -1);
						add(head, instr_ptr->field1, instr_ptr->field2);
					}
				}
				//storeAI
				else if (instr_ptr->opcode == STOREAI) {
					if (exists(head, instr_ptr->field2, instr_ptr->field3) == 0) {
						removeInstr(instr_ptr);
					}
					else {
						add(head, instr_ptr->field2, -1);
						add(head, instr_ptr->field1, -1);
					}
				}
				//add
				else if (instr_ptr->opcode == ADD || instr_ptr->opcode == SUB || instr_ptr->opcode == MUL) {
					if (exists(head, instr_ptr->field3, -1) == 0) {
						removeInstr(instr_ptr);
					}
					else {
						add(head, instr_ptr->field1, -1);
						add(head, instr_ptr->field2, -1);
					}
				}
				//other
				else {}
				instr_ptr = prev_instr_ptr;
			}
		}

        PrintInstructionList(stdout, InstrList);

	fprintf(stderr,"\n-----------------DONE---------------------------\n");
	
	return 0;
}
