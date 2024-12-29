#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#define GREEN "\e[0;92m"
#define WHITE "\e[0;37m"
#define YELLOW "\e[0;93m"


void ForkExec(char** arg)
{
    pid_t pid = fork();
    if (pid==-1)
    {
        perror("error");
        printf("\n");
        return;
    }
    else if (pid==0)
    {
        if (execvp(arg[0], arg)<0)
            printf("'%s': Nie znaleziono takiej komendy\n", *arg);
        exit(0);
    }
    else
    {
        wait(NULL);
        return;
    }
}
void CD(char *arg[],char y[])
{

    if (arg[1]==NULL || strcmp(arg[1], "~")==0)
    {
        chdir(getenv("HOME"));
    }
    else if(strcmp(arg[1], "..")==0)
    {
        chdir("..");
    }
    else if (strcmp(arg[1], "-")==0)
    {
        chdir(y);
    }
    else if (arg[1] !=NULL)
    {
        int x = chdir(arg[1]);
        if(x==-1)
        {
            errno = 2;
            perror("error(2)");
            printf("\n");
        }
    }
}
int main()
{
    printf("----> **** MICROSHELL **** <----\n");
    printf("Wpisz 'help', aby dowiedziec wiecej na temat tego programu\n");

    char komenda[128];
    int errno;
    char prevdir[512];
    while(1)
    {

        char p[256];
        char *path = getcwd(p, 256);
        char *com[64];
        printf(GREEN"[%s]"YELLOW"$ "WHITE, path);

        fgets(komenda, sizeof(komenda), stdin);
        if (komenda[strlen(komenda)-1]=='\n')
            komenda[strlen(komenda)-1]=0;
        int i=0;
        com[0] = strtok(komenda, " ");

        while (com[i] != NULL) {
            i++;
            com[i] = strtok(NULL, " ");
        }
        com[i] = NULL;

        if (!com[0])
        {
            printf("error: Nalezy wpisac komende");
            continue;
        }

        if (strcmp(com[0], "help")==0)
        {
            printf("Jest to prosty program powloki, wzorowany na powloce Bash.\n"
                   "Znajduja sie w nim takie komendy jak:\n"
                   "> cd - zmienianie katalogu roboczego\n"
                   "> pwd - wyswietlanie sciezki biezacego katalogu roboczego\n"
                   "> pozostale komendy powloki\n"
                   "> exit - wyjscie z programu powloki\n");
        }
        else if (strcmp(com[0], "cd")==0)
        {
            if (strcmp(com[1], "-")!=0) {
                strcpy(prevdir,getcwd(p, 256));
            }
            CD(com, prevdir);
        }
        else if (strcmp(com[0], "pwd")==0)
            printf("%s\n", getcwd(p, 256));

        else if (strcmp(com[0], "exit")==0)
            break;

        else
            ForkExec(&com[0]);
    }
    exit(EXIT_SUCCESS);
}