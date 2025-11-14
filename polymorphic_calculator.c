#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
typedef double (*MathFunc)(double, double); // define a function pointer type
typedef struct OperationNode{
  
    char symbol[10]; // operation symbol
    MathFunc compute; 
    struct OperationNode *next; // pointer to next node
}OperationNode;
double add(double a, double b){
    return a + b;
}
double subtract(double a, double b){
    return a - b;
}
double multiply(double a, double b){
    return a * b;
}
double divide(double a, double b){
    if (b == 0){
        printf("Error: Division by zero!\n");
        return 0;
    }
    return a / b;
}
double power(double a, double b){
    return pow(a, b);
}
void append_operation(OperationNode **head, const char *symbol, MathFunc func){
    OperationNode *newNode = malloc(sizeof(OperationNode));
    if (!newNode){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->symbol, symbol);
    newNode->compute = func;
    newNode->next = NULL;
    if (*head == NULL){
        *head = newNode;
    }
    else{
        OperationNode *current = *head;
        while (current->next != NULL){
            current = current->next;
        }
        current->next = newNode;
    }
}
void perform_calculation(OperationNode *head, double num1, char *op_symbol , double num2){
    OperationNode *current = head;
    while ( current != NULL){
        if (strcmp(current->symbol, op_symbol) == 0){
            double result = current->compute(num1, num2);
            printf("Result: %.2f\n", result);
            return;
        }
        current = current->next;
    }
    printf("Error! Operation not Found\n");
}
int main(){
    OperationNode *op_head = NULL;
    append_operation(&op_head, "+" , add);
    append_operation(&op_head, "-", subtract);
    append_operation(&op_head, "*", multiply);
    append_operation(&op_head, "/", divide);
    append_operation(&op_head, "^", power);
    double num1, num2;
    char op_symbol[10];
    char line[128];
    printf("Enter calculation  ");
    printf("Format: number1 operator number2\n");
    printf("type 'exit' to quit\n");
    while (1){
        printf(">> ");
        int parsed = sscanf(line, "%lf %9s %lf", &num1, op_symbol, &num2); //string scan 
        if (parsed != 3){
            printf("Invalid input. Format: number operator number (e.g. 2 + 3) or 'exit'\n");
            continue;
        }
        perform_calculation(op_head, num1, op_symbol, num2);
    }
    OperationNode *current = op_head;
    while (current != NULL){
        OperationNode *temp = current;
        current = current->next;
        free (temp);
    }
    return 0;
}