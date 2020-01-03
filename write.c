#include "control.h"

int main(){
  struct sembuf s;
  s.sem_num = 0;
  s.sem_op = 0;

  int semd = semget(KEY, 1, 0);
  int shmd = shmget(KEY, 1, 0);

  int fd = open("semaphone.txt", O_WRONLY | O_APPEND);
  char *prev = shmat(shmd, 0, 0);
  printf("Your last addition: %s\n", prev);
  char add[256];
  printf("Your next addition: ");
  fgets(add, 256, stdin);
  write(fd, add, strlen(add));
  close(fd);
  strcpy(prev,add);

  s.sem_op = 1;
  shmdt(prev);
  semop(semd, &s, 1);

  return 0;
}
