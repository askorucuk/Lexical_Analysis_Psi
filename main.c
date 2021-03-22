#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int checkif;//if kontrol degiskeni
    char ch;
    char word[100];
    char directoryname[100];
    char currentDirectory[500];
    char fileName[100];
    char tempDirectory[500];
    const char uzanti[] = ".pmk";


    printf("Please enter the file name:  ") ;
    scanf("%s", fileName);
    strcat(fileName, uzanti);//dosya adinin sonuna .pmk uzantisini ekliyoruz.


    getcwd(currentDirectory, 100);//currentDirectory degiskenimize projemizin bulundugu yeri atadik.
    strcpy (tempDirectory , currentDirectory);//ilk basta currentDirectory'e esit olacak ama degisebilir.

    FILE*fileR;//dosya islemleri
    fileR=fopen(fileName,"r");

    ch=fgetc(fileR);//ilk chari aliyoruz
    if(fileR != NULL)//eger dosya var ise
    {
        while(!feof(fileR)){

            if(ch == ' ' || ch=='\n' || ch=='\t')//bosluklari saymamamiz gerek.
            {
                ch=fgetc(fileR);
            }

            if(ch=='<')//
            {
                ch=fgetc(fileR);
                if(ch=='/')
                {
                   printf("Operator / cannot be used at the beginning.");

                }
                while(ch!='>')
                {

                    if(ch=='*')
                    {
                        if(strcmp(word,"if")==0)//eger keyword if ' e esit ise strcmp 0 döndürür.
                        {
                            chdir("..");
                            if(getcwd(tempDirectory, 100)!=NULL)//eger bir üst dizin varsa;
                            {
                                checkif=1;
                            }
                            else
                            {

                                checkif=0;
                            }


                        }

                        if(strcmp(word,"go")==0)//eger keyword go ve <*> şeklinde ise
                        {
                            chdir("..");
                            getcwd(currentDirectory,100);//current directoryi bir üst klasöre getirdik.
                            strcpy(tempDirectory,currentDirectory);
                        }
                        if(strcmp(word,"make")==0)//eger keyword make<* seklinde ise
                        {
                            chdir("..");
                            getcwd(tempDirectory,100);
                        }
                        if(strcmp(word,"ifnot")==0)//eger keyword if ' e esit ise strcmp 0 döndürür.
                        {
                            chdir("..");
                            if(getcwd(tempDirectory, 100)!=NULL)//eger bir üst dizin varsa;
                            {
                                checkif=0;
                            }
                            else
                            {

                                checkif=1;
                            }

                        }

                    }

                    if(('a' <= ch && ch <='z') || ('A' <= ch && ch <= 'Z'))//directory name kontrolu
                    {
                        memset(directoryname, 0, sizeof(directoryname));//directory name sıfırladık
                        int counter=0; //Word counter for index.
                        while(('a' <= ch && ch <='z')|| ('A' <= ch && ch <= 'Z')|| (ch=='_') || isdigit(ch) || (ch=='/'))
                        {
                            directoryname[counter]=ch;
                            ch=fgetc(fileR);
                            counter++;
                        }
                        /*if(directoryname[counter-1]=='/')
                        {
                            printf("%sOperator / cannot be used at the end of any path!" );
                        }*/
                        if(strcmp(word,"make")==0)//make komutu ile girilen path icin directoryler olusturulur.
                        {
                            int sayac=0;//olusturulamayacak dosya sayisi
                            int sayac1=0;//token sayisi
                            char*token = strtok(directoryname , "/");
                            while( token != NULL ) {
                                    sayac1++;
                                    int control=mkdir(token);
                                    if(control!=0)//olusturalamayan zaten var olan dosyalar icin sayac arttiriliyor
                                    {
                                        sayac++;
                                    }
                                    chdir(token);
                                    getcwd(tempDirectory,100);
                                    token = strtok(NULL, "/");

                            }

                           if(sayac==sayac1)//Eger verilen klasörler zaten mevcut ise olusturulamaz hata mesaji döndürürüz
                            {
                                chdir(currentDirectory);
                                strcpy(tempDirectory,currentDirectory);
                                printf("\nThis path already created!");
                                continue;
                            }
                            chdir(currentDirectory);
                            strcpy(tempDirectory,currentDirectory);
                        }
                        if(strcmp(word,"go")==0)//currentdirectory mevcut konumumuz yapıyoruz.
                        {
                             chdir(tempDirectory);
                             char*token1 = strtok(directoryname , "/");
                             int control;
                             while( token1 != NULL ) {
                                control=chdir(token1);
                                if(control!=0)
                                {
                                    printf("\nThis directory does not exists!");
                                }
                                currentDirectory==getcwd(currentDirectory,100);
                                token1 = strtok(NULL, "/");
                            }

                        }

                        if(strcmp(word,"if")==0)
                        {
                            char*token2 = strtok(directoryname , "/");
                            int control1;
                            while( token2 != NULL ) {
                                control1=chdir(token2);
                                if(control1==0)
                                {
                                    checkif=1;
                                }
                                else
                                {
                                    checkif=0;
                                }
                                getcwd(tempDirectory,100);
                                token2 = strtok(NULL, "/");
                            }
                        }
                        if(strcmp(word,"ifnot")==0)
                        {
                            char*token3 = strtok(directoryname , "/");
                            int control2;
                            while( token3 != NULL ) {
                                control2=chdir(token3);
                                if(control2==0)
                                {
                                    checkif=0;
                                }
                                else
                                {
                                    checkif=1;
                                }
                                token3 = strtok(NULL, "/");
                            }
                            getcwd(tempDirectory,100);

                        }
                        if(ch=='*')
                        {
                            printf("\nOperator * can only be used at the beginning of path expressions.");
                            continue;
                        }
                    }
                    if(ch=='/')
                    {
                        ch=fgetc(fileR);
                        if(ch=='*')
                        {
                            ch=fgetc(fileR);
                            if(strcmp(word,"make")==0)//eger keyword make<*/* seklinde ise
                            {
                                chdir("..");
                                getcwd(tempDirectory,100);
                            }
                            if(strcmp(word,"go")==0)//eger keyword go ve <*> şeklinde ise
                            {
                                chdir("..");
                                getcwd(currentDirectory,100);//current directoryi bir üst klasöre getirdik.
                                strcpy(tempDirectory,currentDirectory);
                            }
                            if(strcmp(word,"if")==0)//eger keyword if ' e esit ise strcmp 0 döndürür.
                            {
                                chdir("..");
                                if(getcwd(tempDirectory, 100)!=NULL)//eger bir üst dizin varsa;
                                {
                                    checkif=1;
                                }
                                else
                                {

                                    checkif=0;
                                }

                            }
                            if(strcmp(word,"ifnot")==0)//eger keyword if ' e esit ise strcmp 0 döndürür.
                            {
                                chdir("..");
                                if(getcwd(tempDirectory, 100)!=NULL)//eger bir üst dizin varsa;
                                {
                                    checkif=0;
                                }
                                else
                                {

                                    checkif=1;
                                }

                            }
                            if(ch=='>')//Eðer /*> gibi bir durum söz konusu ise hata mesaji döndüreceðiz
                            {
                                printf("\nOperator * can only be used at the beginning of path expressions.");
                                return 1;
                            }

                        }
                        if(ch=='>')
                        {
                            printf("\nOperator / cannot be used at the end.") ;
                            return 1;

                        }
                        continue;

                    }
                    if(ch=='>')
                    {
                        break;
                    }

                    ch=fgetc(fileR);

                }
            }//if <
            if(ch==';')
            {
                chdir(currentDirectory);
                getcwd(currentDirectory,100);
            }
            if((strcmp(word,"if")==0 || strcmp(word,"ifnot")==0) && (ch=='>')) //Eger keyword if veya ifnot ise ve ch > ise (if calisiyorsa devam edecek calismiyorsa block veya komutu atlayacak.
            {
                chdir(currentDirectory);//currentDirectoryi degismememiz lazim.
                ch=fgetc(fileR);
                if(ch == ' ' || ch=='\n' || ch=='\t')//bosluklari saymamamiz gerek.
                {
                    ch=fgetc(fileR);
                }
                if(checkif)
                {
                    continue;
                }
                else
                {
                    if(ch=='{')
                    {
                        while(ch!='}')//if eger { ile basladiysa bitene kadar okuma cunku if sarti saglanmiyor.
                        {
                            ch=fgetc(fileR);
                        }
                    }
                    else
                    {
                        while(ch!=';')//if eger satir seklinde yazildiysa satir bitene kadar okuma cunku if sarti saglamiyor.
                        {
                            ch=fgetc(fileR);
                        }
                    }
                }
            }

            if(('a' <= ch && ch <='z'))//Keyword kontrolu.
            {
                memset(word, 0, sizeof(word));//yeni keyword geldigi icin eskisini sifirladik.
                int counter1=0; //Keyword counter for index.
                while(('a' <= ch && ch <='z')|| ('A' <= ch && ch <= 'Z')|| (ch=='_') || isdigit(ch))
                {
                    word[counter1]=ch;
                    ch=fgetc(fileR);
                    counter1++;
                }
                word[counter1]='\0';//if@ gibi bir durum olmaması için.
            }
            if(('A' <= ch && ch <= 'Z'))
            {
                printf("Keywords are case sensitive and all are lowercase!");
            }
        if(ch!='<')
        {
            ch=fgetc(fileR);
        }

        }//while
    }//if
}


