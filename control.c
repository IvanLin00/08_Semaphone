#include "control.h"

int main(int argc, char *argv[]){
  if(!strcmp(argv[1], "-c")){
    crea();
  }
  else if(!strcmp(argv[1], "-r")){
    remo();
  }
  else if(!strcmp(argv[1], "-v")){
    view();
  }
  else{
    printf("huh\n");
  }
  return 0;
}

void crea(){
  int semd;
  int v, r;
  char input[3];

  semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if(semd < 0){
    semd = semget(KEY, 1, 0);
    v = semctl(semd, 0, GETVAL, 0);
  }
  else{
    union semun us;
    us.val = 1;
    r = semctl(semd, 0, SETVAL, us);
  }
  printf("Semaphore created\n");
  int shmd = shmget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  printf("Shared memory created\n");
  int fd = open("semaphone.txt", O_CREAT | O_TRUNC, 0644);
  if (fd < 0){
    printf("error %d: %s\n",errno,strerror(errno));
    return;
  }
  printf("File created\nSuccess!\n");
  close(fd);
}


void remo(){
  view();
  int shmd = shmget(KEY,1,0);
  int semd = semget(KEY,1,0);
  shmctl(shmd, IPC_RMID, 0);
  printf("Shared memory removed\n");
  semctl(semd, IPC_RMID, 0);
  printf("Semaphore removed\n");
  int status = remove("semaphone.txt");
  if (status < 0){
    printf("error %d: %s\n",errno,strerror(errno));
    return;
  }
  printf("File removed\nSuccess!\n");
}

void view(){
  int fd = open("semaphone.txt", O_RDONLY);
  if (fd < 0){
    printf("error %d: %s",errno,strerror(errno));
    return;
  }
  char buffer[256];
  int r = read(fd,buffer,256);
  if (r < 0){
    printf("error %d: %s",errno,strerror(errno));
    return;
  }
  printf("The story so far:\n%s\n",buffer);
  close(fd);
}
