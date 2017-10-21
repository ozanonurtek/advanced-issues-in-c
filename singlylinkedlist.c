#import <stdio.h>
#import <stdlib.h>
#import <string.h>

struct Employee{
    int age;
    char name[100];
    struct Employee *next;
};
//Functions signatures
void addNewEmployee(int newEmployeeAge, char newEmployeeName[100], struct Employee **head);
void iterateLinkedList(struct Employee *employee);
void deleteEmployeeWithName(char employeeName[100], struct Employee **employee);
void addToEndOfList(int newEmployeeAge, char newEmployeeName[100], struct Employee **head);
void addAfterEmployee(int newEmployeeAge, char newEmployeeName[100],char afterEmployee[100], struct Employee **head);	
void deleteAllList(struct Employee **head);

int main(){
    struct Employee *head = NULL;
	addToEndOfList(20, "Mehmet", &head);
	addNewEmployee(22, "Ozan", &head);
	addNewEmployee(22, "Mert", &head);
	addNewEmployee(16, "Veli", &head);
	addToEndOfList(13, "Ali", &head);
	addToEndOfList(14, "Ahmet", &head);
	addAfterEmployee(21, "Berfin", "Ozan", &head);
	iterateLinkedList(head);
	printf("----DELETED LIST-----\n");
	deleteEmployeeWithName("Ahmet", &head);	
	deleteEmployeeWithName("Ozan", &head);	
	
	iterateLinkedList(head);
	deleteAllList(&head);
	printf("----DELETE ALL LIST-----\n");
	iterateLinkedList(head);
    
	return 1;
}


//Adding to the front of list O(1)
void addNewEmployee(int newEmployeeAge, char newEmployeeName[100], struct Employee **head){

    //New employee is created.
    struct Employee *newEmployee = malloc(sizeof(struct Employee));
    //Data insertion
    strcpy(newEmployee->name, newEmployeeName);
    newEmployee->age  = newEmployeeAge;
    newEmployee->next  = (*head);
    //Head presents new added node, since we are pushing to front of the list
    (*head) = newEmployee;

}

void iterateLinkedList(struct Employee *employee){
	int counter = 0;
    while(employee != NULL){
        printf("Employee name: %s and age: %d, \n", employee->name, employee->age);
        employee = employee->next;
		counter++;
    }
	printf("Number of employees in list: %d \n", counter);
}

void deleteEmployeeWithName(char employeeName[100], struct Employee **employee){
	struct Employee *iterator = *employee;	
	//Check for first element
	if(*employee != NULL && strcmp(iterator->name,employeeName) == 0){
		(*employee) = iterator->next;
	}
	
	while(iterator->next != NULL){
		if(strcmp(iterator->next->name,employeeName) == 0){
				iterator->next = iterator->next->next;
				printf("%s is deleted from list. \n---\n", employeeName);
				break;
		}
		iterator = iterator->next;
	}

}
void addToEndOfList(int newEmployeeAge, char newEmployeeName[100], struct Employee **head){
    struct Employee *newEmployee = malloc(sizeof(struct Employee));
	struct Employee *iterator = *head;
	
    strcpy(newEmployee->name, newEmployeeName);
    newEmployee->age  = newEmployeeAge;
	
	if((*head) == NULL){
		(*head) = &(*newEmployee);
		return;
	}
	while(iterator->next != NULL){
		iterator = iterator->next;
	}
	iterator->next = newEmployee;
	

}
void addAfterEmployee(int newEmployeeAge, char newEmployeeName[100],char afterEmployee[100], struct Employee **head){		
	struct Employee *iterator = *head;
	
	if((*head) == NULL){
		return;
	}
	while(iterator -> next != NULL){
		if(strcmp(iterator->name, afterEmployee) == 0){
		    struct Employee *newEmployee = malloc(sizeof(struct Employee));
		    strcpy(newEmployee->name, newEmployeeName);
		    newEmployee->age  = newEmployeeAge;
			newEmployee->next = iterator->next;
			iterator->next = newEmployee;
			break;
		}
		iterator = iterator->next;
	}
	
}

void deleteAllList(struct Employee **head){
	*head = NULL;
}

