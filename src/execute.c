#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct task{
    const char* task_name;
    const char* program_path;
    const char* program_args;
    const int sample_number;
    const char* log_filename;
}* Task;

void execute_task(Task task){
    char command_buf[100];
    sprintf(command_buf, "%s %s", task->program_path, task->program_args);
    __raw FILE* log_fp = fopen(task->log_filename, "a+");
    printf("execute %s\n", task->task_name);
    fprintf(log_fp, "execute %s\n", task->task_name);

    struct timespec start, end;
    timespec_get(&start, TIME_UTC);
    for(int i=0;i<task->sample_number;i++){
        printf("pass %d\n", i);
        system(command_buf);
    }
    timespec_get(&end, TIME_UTC);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Elapsed time: %.3f seconds\n", elapsed);
    fprintf(log_fp, "Elapsed time: %.3f seconds\n", elapsed);
    fclose(log_fp);
}

int main(){
    system("mkdir /tmp/lat_fs");
    system("touch /tmp/lat_fs/lmbench");
    system("cp hello /tmp/hello");
    struct task task_list[] = {
        {
            .task_name = "read",
            .program_path = "./lat_syscall",
            .program_args = "-P 1 read",
            .sample_number = 1,
            .log_filename = "./lmbench.log"
        },
        {
            .task_name = "write",
            .program_path = "./lat_syscall",
            .program_args = "-P 1 write",
            .sample_number = 1,
            .log_filename = "./lmbench.log"
        },
        {
            .task_name = "open",
            .program_path = "./lat_syscall",
            .program_args = "-P 1 open /tmp/lat_fs/lmbench",
            .sample_number = 1,
            .log_filename = "./lmbench.log"
        },
        {
            .task_name = "select",
            .program_path = "./lat_select",
            .program_args = "-n 500 -P 1 file",
            .sample_number = 1,
            .log_filename = "./lmbench.log"
        },
        {
            .task_name = "sig install",
            .program_path = "./lat_sig",
            .program_args = "-P 1 install",
            .sample_number = 1,
            .log_filename = "./lmbench.log"
        },
        {
            .task_name = "sig hndl",
            .program_path = "./lat_sig",
            .program_args = "-P 1 catch",
            .sample_number = 1,
            .log_filename = "./lmbench.log"
        },
        {
            .task_name = "fork",
            .program_path = "./lat_proc",
            .program_args = "-P 1 fork",
            .sample_number = 1,
            .log_filename = "./lmbench.log"
        },
        {
            .task_name = "shell",
            .program_path = "./lat_proc",
            .program_args = "-P 1 shell",
            .sample_number = 1,
            .log_filename = "./lmbench.log"
        },
    };
    for(int i=0;i<sizeof(task_list)/sizeof(struct task);i++){
        execute_task(&task_list[i]);
    }
    system("rm -rf /tmp/lat_fs");
    system("rm /tmp/hello");
}