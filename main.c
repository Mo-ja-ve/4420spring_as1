#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>

int main(){

  int pid;
  int c;
  std::string str;

  struct dirent *ep;
  DIR* dp;
  FILE* read_piddir;
  dp = opendir ("/proc");

printf("   PID: COMMAND:       STATE:  PPID:                                                                            VMEMORY:");
printf("\n");
printf("---------------------------------------------------------------------------------------------------------------------------------\n");

  if(dp != NULL){
    while(ep = readdir (dp)){

      pid = strtol (ep->d_name, NULL, 10);
      if((ep->d_type == DT_DIR) && (pid > 0)){
        //puts (ep->d_name);
        char path_str[100] = { '/','p','r','o','c','/','\0'};
        char add_stat[100] = { '/','s','t','a','t','\0'};
        //char path_str[] = { "/proc/"};
        //char add_stat[] = {  "/stat"};

        strcat(path_str, ep->d_name);
        strcat(path_str, add_stat);

        read_piddir = fopen(path_str, "r");

        if(read_piddir != NULL){
        int temp_count  = 0;

        printf("      ");
        while(1){
          c = fgetc(read_piddir);
          if(c == 32){
            printf("    ");
            temp_count++;
          }

          if(temp_count > 24){

            break;
          }

          if(temp_count > 23){
            printf("%c", c);
          }

          if(temp_count >= 4){
            c = 0;
          }

          if( feof(read_piddir)){
            break;
          }
          //std::cout<<str;
          printf("%c", c);
        }
      }
        printf("\n");
      }
    }
    closedir(dp);
  }else{
    perror ("Couldn't open the directory");
  }

  return 0;
}
