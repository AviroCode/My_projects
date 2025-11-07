// //Reading files
// #include <stdio.h>
// #include <stdlib.h>
// #define add(A, B) ((A) + (B))
// #define min(A, B) ((A) < (B) ? (A) : (B))
// int main() {
//     FILE *file = fopen("../hello.txt", "r");
//     if (file == NULL) {
//         perror("fopen");
//         return 1;
//     }
//     int ch;
//     int digit_array[10]; // to store up to 10 digits
//     int digit_count = 0;
//     while ((ch = fgetc(file)) != EOF) { // check end of file
//         fprintf(stdout, "%c", ch); // print character
//         if ( ch >= '0' && ch <= '9') {
//             if (digit_count < 10) {
//                 digit_array[digit_count] = ch - '0'; // convert char to int
//                 digit_count++;
//             }
//         }
        
//     }
//     fclose(file);
//     printf("\nDigits found in the file: ");
//     if (digit_count == 2) {
//         int sum = add(digit_array[0], digit_array[1]);
//         printf("\nSum of the two digits: %d\n", sum);
//     } else {
//         printf("Less than two digits found.\n");
//     }
//     printf("Minimum digit found: ");
//     if (digit_count > 0) {
//         int min_digit = digit_array[0];
//         for (int i = 1; i < digit_count; i++) {
//             min_digit = min(min_digit, digit_array[i]);
//         }
//         printf("%d\n", min_digit);
//     } else {
//         printf("No digits found.\n");
//     }
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strcpy and strtok
#define MAX_STUDENTS 50

typedef struct {
    int student_id;
    char name[50];
    float score;
} Student;
void add_data_to_file(char name[], int id,float scores, FILE* file_pointer){
    if (file_pointer == NULL){
        printf("Error! File not found");
        return;
    }
    fprintf(file_pointer, "%s,%d,%.2f\n", name, id, scores);

}
int main(){
    Student class_roaster[MAX_STUDENTS];
    int student_count = 0;
    while(1){
        printf("1. Add a New Student\n");
        printf("2. List All Students\n");
        printf("3. Calculate Class Average\n");
        printf("4. Exit\n");
        printf("Enter your Choice :");
        int choice;
        if (scanf("%d", &choice) != 1){ //Read into choice and check if valid
            // CLEAR INVALID INPUT
            
            while(getchar() != '\n'); // Clear invalid input
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            continue;
        }
        switch (choice) {
            case 1: {
                if (student_count < MAX_STUDENTS) {
                    char name[30];
                    int ID;
                    float score;

                    printf("Enter your Name: ");
                    scanf("%s", &name);
                    printf("Enter your ID: ");
                    scanf("%d", &ID);
                    printf("Enter your Score: ");
                    scanf("%f", &score);
                
                    
                    
                    class_roaster[student_count].student_id = ID;
                    strcpy(class_roaster[student_count].name, name); //strcpy to copy string
                    class_roaster[student_count].score = score;
                    student_count++;
                    FILE *fptr = fopen("../data.csv", "a");
                    if (fptr == NULL){
                        perror("Error Opening file");
                        
                    }
                    else{
                        add_data_to_file(name, ID, score, fptr);
                        fclose(fptr);
                        printf("Data added successfully!\n");
                    }
                    
                }
                else {
                    printf("Maximum student limit reached.\n");
                }
                break;
            }
            case 2:{
                int ch;
                FILE *fptr = fopen("../data.csv", "r");
                if (fptr == NULL){
                    perror("Error Opening file");
                    break;
                }
                printf("----- Student List -----\n");
                while ((ch = fgetc(fptr)) != EOF){
                    putchar(ch);
                }
                fclose(fptr);
                printf("------------------------\n");
                break;
            }
            case 3:{
                FILE *file = fopen("../data.csv", "r");
                if (file == NULL){
                    perror("Error Opening file");
                    break;
                }
                char line[1024];
                float total = 0.0;
                int count = 0;
                while (fgets(line, sizeof(line), file) != NULL){
                    char* name = strtok(line, ",");
                    char* id_str = strtok(NULL, ",");
                    char* score_str = strtok(NULL, ",");
                    if (score_str != NULL){
                        float score = atof(score_str); // atof to convert string to float
                        total += score;

                        count++;
                    }
                }
                if (count > 0){
                    float average = total / count;
                    printf("Class Average Score: %.2f\n", average);
                }
                fclose(file);
                break;
            }
            case 4:{
                return 0;
            }
            default:{
                printf("Invalid choice. Please try again.\n");
                break;
        }
        }
    }
    return 0;
}