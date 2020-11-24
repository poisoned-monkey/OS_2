//вариант 8

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void sortBooble(char*massIn)
{
//сортировка пузырьком
    int tmp;
    int noSwap;
    int i;
    int N =strlen(massIn);
    for ( i= N - 1; i >= 0; i--)
    {
        noSwap = 1;
        for (int j = 0; j < i; j++)
        {
            if (massIn[j] > massIn[j + 1])
            {
                tmp = massIn[j];
                massIn[j] = massIn[j + 1];
                massIn[j + 1] = tmp;
                noSwap = 0;
            }
        }
        if (noSwap == 1)
            break;
    }

}



int main()
{

    pid_t pid;//идентификатор потока
    char fname1[50];
    char fname2[50];


  printf("Make sure all files are in the same folder.\nEnter the name of the first file\n");
   gets(fname1);
//    fname1="/Users/macbook/Desktop/OS_2/file1.txt"


 printf("\nEnter the name of the second file\n");
 gets(fname2);
//    fname2="/Users/macbook/Desktop/OS_2/file2.txt"



    char *filename;// указатель на путь к нужному файлу

    int file_pipes[2];//массив для хранения файловых дескрипторов

    char buffer[BUFSIZ + 1];//массив для чтения данных из канала

    char dataFile[100];//массив для хранения данных


if (pipe(file_pipes) == -1)//если канал обмена не создан
 {
    perror("pipe");
    exit(EXIT_FAILURE);
 }

 pid = fork();//создаем новый поток

  if (pid == -1)//поток не создан
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }

    if(pid ==0)//ребенок
    {
        filename = fname2;//будем читать данные со второго файла

    }
    else//главный поток
    {

        filename =fname1;//читаем данные из первого файла

    }

    printf("%s\n",filename);

    //--------------читаем данные из файла
    FILE * myFile;
    myFile = fopen(filename,"r");
    int i=0;
    fscanf(myFile, "%s", dataFile);
    //------------------------------------

    if(pid==0)//если ребенок
    {
        close(file_pipes[0]);//закрываем выход для чтения
        sortBooble(dataFile);//сортируем данные
        write(file_pipes[1], dataFile, strlen(dataFile));//записываем данные в канал обмена, dataFile -буффер
         _exit(EXIT_SUCCESS);//выходим из потока
    }
    else
    {

        close(file_pipes[1]);//закрываем выход для записи
        //-------------------------------------выводим данные о завершении дочернего потока
        int status;
        waitpid(pid, &status, 0);
        printf("exit normally? %s\n", (WIFEXITED(status) ? "true" : "false")); //не равно нулю, если дочерний процесс успешно завершился
        printf("child exitcode = %i\n", WEXITSTATUS(status));
        //---------------------------------------------------------------------------------

        read(file_pipes[0], buffer, BUFSIZ);//читаем данные из канала обмена
        sortBooble(dataFile);//сортируем текущие даннные родительского потока

        int s_data= strlen(dataFile);
        int s_buf = strlen(buffer);

        int p;
        if(s_data>s_buf){
            p=s_data;
        } else {
            p=s_buf;
        }

        for( int i=0; i<p; i++)
        {
            if(i<s_data)
            printf("file1->%c\n",dataFile[i]);
            if(i<s_buf)
            printf("file2->%c\n",buffer[i]);
        }

        wait(NULL);                /* Ожидание потомка */
        fclose(myFile);
        _exit(EXIT_SUCCESS); // все дескрипторы файлов, принадлежащие процессу, закрываются
    }
}