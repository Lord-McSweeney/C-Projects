#include <stdio.h>
#include <string.h>
#include <unistd.h>

// All box drawing characters: └	┴	┬	├	─	┼	│	┤	┐	┘	┌

char empty[8];

typedef char *pchar;

int level = 1;

int endingLevel = 2;

int keybindStatus = 0;

char orow1[16] = "/--------\\";
char orow2[16] = "|        |";
char orow3[16] = "+----    |";
char orow4[16] = "|@       |";
char orow5[16] = "\\--------/";

char input[256];

char row1[16];
char row2[16];
char row3[16];
char row4[16];
char row5[16];

pchar allRows[] = {row1, row2, row3, row4, row5};

int px = 1;
int py = 1;

char binds[256] = {'U', 'N', 'N', 'N', 'D', 'S', 'S', 'S', 'L', 'W', 'W', 'W', 'R', 'E', 'E', 'E', 'H', 'H', '\0'};

void resetRows(int playerx, int playery) {

  strcpy(row1, orow1);
  strcpy(row2, orow2);
  strcpy(row3, orow3);
  strcpy(row4, orow4);
  strcpy(row5, orow5);

  char *y = allRows[playery];

  y[playerx] = '*';
}

void resetBinds() {
  binds[0] = 'U';
  binds[1] = 'N';
  binds[2] = 'N';
  binds[3] = 'N';
  binds[4] = 'D';
  binds[5] = 'S';
  binds[6] = 'S';
  binds[7] = 'S';
  binds[8] = 'L';
  binds[9] = 'W';
  binds[10] = 'W';
  binds[11] = 'W';
  binds[12] = 'R';
  binds[13] = 'E';
  binds[14] = 'E';
  binds[15] = 'E';
  binds[16] = 'H';
  binds[17] = 'H';
  binds[18] = '\0';
}

int getCharAt(int x, int y) {
  char *ya = allRows[y];
  return ya[x];
}

void printRows() {
  int i;
  for (i = 0; i < 5; i ++) {
    printf("%s", allRows[i]);
    printf("\n");
  }
}

int main() {
  FILE *fstr = fopen("./keybinds.txt", "r");
  if (access("./keybinds.txt", F_OK)) {
      printf("Error: missing keybinds.txt file. Press Enter to exit.\n");
      fgets(empty, 8, stdin);
      return 0;
  }
  unsigned int c;
  c = fgetc(fstr);
  int i = 0;
  printf("Reading keybinds.txt...\n");
  while(c != -1) {
      if (i == 0 && c == 45) {
        printf("Found '-' at beginning of file. Switching to default keybinds.\n");
        keybindStatus = 1;
        break;
      }
      binds[i] = c;
      c = fgetc(fstr);
      if (i >= 256) {
        printf("Too many keybinds. Switching to default keybinds.\n");
        keybindStatus = 1;
        break;
      }
      i += 1;
  }
  if (keybindStatus == 0) {
    binds[i] = '\0';
  }
  if (i % 2 == 0 && keybindStatus == 0) {
    printf("Misconfigured keybinds file. Switching to default keybinds.\n");
    keybindStatus = 1;
  }
  if (keybindStatus == 1) {
    resetBinds();
  } else {
    printf("Successful reading of keybinds.txt.\n");
  }
  printf("(Resize terminal window to 180x7)\n");
  while(1) {
    resetRows(px, py);
    printf("\n");
    printRows();
    fgets(input, 256, stdin);
    for (i = 0; i < strlen(binds); i = i + 2) {
      if (input[0] == binds[i]) {
        char k = binds[i + 1];
        if (k == 'H') {
          printf("U or N to go north, R or E to go east, D or S to go south, and L or W to go west. You are the player (indicated by the '*') and you must reach the portal (indicated by the '@').");
        } else if (k == 'N') {
          if (getCharAt(px, py - 1) == 32) {
            py -= 1;
            printf("You went north.");
          } else if (getCharAt(px, py - 1) == 64) {
            printf("You won the game!\n");
            py -= 1;
            level ++;
          } else {
            printf("There was a wall to the north of you.");
          }
        } else if (k == 'S') {
          if (getCharAt(px, py + 1) == 32) {
            py += 1;
            printf("You went south.");
          } else if (getCharAt(px, py + 1) == 64) {
            printf("You won the game!\n");
            py += 1;
            level ++;
          } else {
            printf("There was a wall to the south of you.");
          }
        } else if (k == 'W') {
          if (getCharAt(px - 1, py) == 32) {
            px -= 1;
            printf("You went west.");
          } else if (getCharAt(px - 1, py) == 64) {
            printf("You won the game!\n");
            px -= 1;
            level ++;
          } else {
            printf("There was a wall to the west of you.");
          }
        } else if (k == 'E') {
          if (getCharAt(px + 1, py) == 32) {
            px += 1;
            printf("You went east.");
          } else if (getCharAt(px + 1, py) == 64) {
            printf("You won the game!");
            px += 1;
            level ++;
          } else {
            printf("There was a wall to the east of you.");
          }
        } else {
          printf("Unknown bind. Please bind keys to N (north), S (south), W (west), E (east), or H (help).");
        }
      }
    }
    if (level == endingLevel) {
      break;
    }
  }
  printf("(Enter to end game)");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  fgets(input, 256, stdin);
  return 0;
}
