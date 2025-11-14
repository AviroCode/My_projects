#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TASK_FILE "../tasks.txt"
#define MAX_NAME_LEN 256

typedef struct{
    char task_name[MAX_NAME_LEN];
    int is_completed;
}Task;

void add_task(Task **task_list, int *task_count, char name[]){
    Task *new_list = realloc(*task_list, (*task_count + 1) * sizeof(Task));
    if (!new_list) {
        printf("Memory allocation failed!\n");
        return;
    }
    *task_list = new_list;
    strncpy((*task_list)[*task_count].task_name, name, MAX_NAME_LEN-1);
    (*task_list)[*task_count].task_name[MAX_NAME_LEN-1] = '\0';
    (*task_list)[*task_count].is_completed = 0; // mark new task as not completed
    (*task_count)++; //[task_count] is pointer to int, so we need to dereference it
    printf("Task added successfully!\n");
}

// new helper: save tasks to file (overwrite)
int save_tasks_to_file(Task *task_list, int task_count){
    FILE *f = fopen(TASK_FILE, "w");
    if (!f) {
        perror("Failed to open task file for writing");
        return 0;
    }
    for (int i = 0; i < task_count; i++){
        // format: is_completed TAB task_name newline
        fprintf(f, "%d\t%s\n", task_list[i].is_completed, task_list[i].task_name);
    }
    fclose(f);
    return 1;
}

// new helper: load tasks from file
int load_tasks_from_file(Task **task_list, int *task_count){
    FILE *f = fopen(TASK_FILE, "r");
    if (!f) {
        return 0; // no file to load
    }
    char line[1024];
    int count = 0;
    Task *list = NULL;
    while (fgets(line, sizeof(line), f)){
        // remove newline
        char *p = strchr(line, '\n');
        if (p) *p = '\0';
        // find separator
        char *sep = strchr(line, '\t');
        if (!sep) sep = strchr(line, ' ');
        if (!sep) continue;
        *sep = '\0';
        int completed = atoi(line);
        char *name = sep + 1;
        // trim leading spaces
        while (*name == ' ') name++;
        Task *tmp = realloc(list, (count + 1) * sizeof(Task));
        if (!tmp) {
            free(list);
            fclose(f);
            return 0;
        }
        list = tmp;
        list[count].is_completed = completed ? 1 : 0;
        strncpy(list[count].task_name, name, MAX_NAME_LEN-1);
        list[count].task_name[MAX_NAME_LEN-1] = '\0';
        count++;
    }
    fclose(f);
    *task_list = list;
    *task_count = count;
    return 1;
}

// new helper: delete a task (1-based index)
int delete_task(Task **task_list, int *task_count, int index){
    if (index < 1 || index > *task_count) return 0;
    for (int i = index - 1; i < (*task_count) - 1; i++){
        (*task_list)[i] = (*task_list)[i+1];
    }
    if (*task_count - 1 == 0){
        free(*task_list);
        *task_list = NULL;
        *task_count = 0;
        return 1;
    }
    Task *new_list = realloc(*task_list, (*task_count - 1) * sizeof(Task));
    if (new_list) *task_list = new_list; // if realloc fails, old pointer still valid
    (*task_count)--;
    return 1;
}

// new helper: print tasks with numbers (shows message if empty)
void print_tasks(Task *task_list, int task_count){
    if (task_count == 0) {
        printf("No tasks available.\n");
        return;
    }
    printf("----- Task List -----\n");
    for (int i = 0; i < task_count; i++){
        printf("%d. %s [%s]\n", i + 1, task_list[i].task_name,
               task_list[i].is_completed ? "Completed" : "Pending");
    }
    printf("---------------------\n");
}

int prompt_yes_no(const char *msg){
    char buf[8];
    printf("%s (y/n): ", msg);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    return (buf[0] == 'y' || buf[0] == 'Y');
}

int read_int_from_stdin(const char *prompt){
    char buf[64];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    int val;
    if (sscanf(buf, "%d", &val) != 1) return -1;
    return val;
}

int main(){
    Task *task_list = NULL; //pointer 
    int task_count = 0;
    int choice;

    // Ask to load previous session
    if (prompt_yes_no("Load previous session")) {
        if (load_tasks_from_file(&task_list, &task_count)) {
            printf("Loaded %d task(s) from %s\n", task_count, TASK_FILE);
        } else {
            printf("No previous session found or failed to load.\n");
        }
    }
    int running = 1;
    while (running){

        printf("\n1. Add A Task\n");
        printf("2. Complete a Task\n");
        printf("3. Delete a Task\n");
        printf("4. List All Tasks\n");
        printf("5. Exit\n");
        printf("Type the number to choose the task: ");

        char input[16];
        if (!fgets(input, sizeof(input), stdin)) continue;
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice){
            case 1:{
                char name[MAX_NAME_LEN];
                printf("Enter task name: ");
                if (!fgets(name, sizeof(name), stdin)) {
                    printf("Failed to read task name.\n");
                    continue;
                }
                // trim newline
                char *p = strchr(name, '\n');
                if (p) *p = '\0';
                if (strlen(name) == 0) {
                    printf("Task name cannot be empty.\n");
                    continue;
                }
                add_task(&task_list, &task_count, name);
                if (!save_tasks_to_file(task_list, task_count)) {
                    printf("Warning: failed to save tasks to file.\n");
                }
                // show updated list after adding
                print_tasks(task_list, task_count);
                continue;
            }
            case 2:{
                if (task_count == 0) {
                    printf("No tasks to complete.\n");
                    continue;
                }
                // show tasks with numbers so user can pick
                print_tasks(task_list, task_count);
                int task_number = read_int_from_stdin("Enter task number to complete: ");
                if (task_number < 1 || task_number > task_count){
                    printf("Invalid task number!\n");
                }
                else{
                    task_list[task_number - 1].is_completed = 1;
                    printf("Task marked as completed!\n");
                    if (!save_tasks_to_file(task_list, task_count)) {
                        printf("Warning: failed to save tasks to file.\n");
                    }
                }
                continue;
            }
            case 3:{
                if (task_count == 0) {
                    printf("No tasks to delete.\n");
                    continue;
                }
                // show tasks with numbers so user can pick
                print_tasks(task_list, task_count);
                int task_number = read_int_from_stdin("Enter task number to delete: ");
                if (task_number < 1 || task_number > task_count){
                    printf("Invalid task number!\n");
                } else {
                    if (delete_task(&task_list, &task_count, task_number)) {
                        printf("Task deleted.\n");
                        if (!save_tasks_to_file(task_list, task_count)) {
                            printf("Warning: failed to save tasks to file.\n");
                        }
                    } else {
                        printf("Failed to delete task.\n");
                    }
                    // show updated list after deletion
                    print_tasks(task_list, task_count);
                }
                continue;
            }
            case 4:{
                print_tasks(task_list, task_count);
                running = 0; // just to break the loop after listing
                break;

            }
            case 5:{
                if (!save_tasks_to_file(task_list, task_count)) {
                    printf("Warning: failed to save tasks to file on exit.\n");
                }
                free(task_list);
                printf("Exiting the program.\n");
                return 0;
            }
            default:{
                printf("Invalid choice! Please try again.\n");
            }
        }
        /* loop continues */
    }

    return 0;

}
