#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

class stat_line{

public:
  int s_pid;
  std::string s_name;
  char s_status;
  int s_parentpid;
};

int main(){

  int pid;
  int c;
  std::string str;

  struct dirent *ep;
  DIR* dp;

  std::ifstream read_piddir;
  dp = opendir ("/proc");

  std::vector<stat_line> stat_buffer;

  int temp_count = 0;
  int temp_pid;
  std::string temp_s_name;
  char junkchar;
  int temp_s_parentpid;


  if(dp != NULL){//  this block opens up the /proc dir scans through it
    while(ep = readdir (dp)){
      pid = strtol (ep->d_name, NULL, 10);
      if((ep->d_type == DT_DIR) && (pid > 0)){

        char path_str[100] = { '/','p','r','o','c','/','\0'};
        char add_stat[100] = { '/','s','t','a','t','\0'};

        strcat(path_str, ep->d_name);
        strcat(path_str, add_stat);
        //  we open up the stat file for each pid in /proc that has a stat file
        read_piddir.open(path_str);

        if(read_piddir.is_open()){//  each pids coorisponding stat file is read and inserted into the array stat_buff

          stat_buffer.resize(((stat_buffer.size())+1));

          read_piddir >> temp_pid;
          read_piddir >> temp_s_name;
          read_piddir >> junkchar;
          read_piddir >> temp_s_parentpid;

          stat_buffer[temp_count].s_pid       = temp_pid;
          stat_buffer[temp_count].s_name      = temp_s_name;
          stat_buffer[temp_count].s_parentpid = temp_s_parentpid;

          read_piddir.close();
          temp_count++;
        }
      }
    }
    closedir(dp);
  }else{
    perror ("Couldn't open the directory");
  }

  std::cout<<std::endl<<"---> dummy root(0)"<<std::endl;

  int min; int temp;
  for(int i = 0; i < stat_buffer.size(); i++){//  here i sort the stat_buff list from smallest parent pid to largest to make printing easier
    min = i;
      for(int j = i+1; j < stat_buffer.size(); j++)
        if(stat_buffer[j].s_parentpid < stat_buffer[min].s_parentpid)
          min = j;

    temp_pid         = stat_buffer[min].s_pid;
    temp_s_name      = stat_buffer[min].s_name;
    temp_s_parentpid = stat_buffer[min].s_parentpid;

    stat_buffer[min] = stat_buffer[i];

    stat_buffer[i].s_pid       = temp_pid;
    stat_buffer[i].s_name      = temp_s_name;
    stat_buffer[i].s_parentpid = temp_s_parentpid;
  }

  std::vector<int> ppid_list;
  ppid_list.resize(1);
  ppid_list[0] = stat_buffer[0].s_parentpid;

  for(int i = 0; i < stat_buffer.size(); i++){// here i create a seperate verctor list of each unique parent pid contained in stat_buffer
    if(ppid_list.back() != stat_buffer[i].s_parentpid){
      ppid_list.resize(ppid_list.size()+1);
      ppid_list[ppid_list.size()-1] = stat_buffer[i].s_parentpid;
    }
  }

  std::cout<<std::endl;
  std::cout<<std::endl<<"   |"<<std::endl<<"   |";
  for(int i = 0; i < ppid_list.size(); i++){//  sorry, i couldn't figure out in time how to print parent-child tree style list like in the example so this is the best example i could come up wiht before the due date
    std::cout<<std::endl<<"   PARIENT PID: "<<ppid_list[i];
    for(int j = 0; j < stat_buffer.size(); j++){
      if(stat_buffer[j].s_parentpid == ppid_list[i]){
        std::cout<<std::endl<<"    |"<<std::endl<<"    |"<<std::endl<<"    |"<<std::endl<<"    |---> ";
        std::cout<<stat_buffer[j].s_pid<<stat_buffer[j].s_name<<")";
      }
    }
    std::cout<<std::endl;
  }

  return 0;
}
