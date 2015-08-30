#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define BUFFER_SIZE (2048)  //filename will never be bigger than this
#define CONST (15)  //to start off each of my rows
#define SPACE (32)  //ASCII of ' '

struct row {    //each row has a size (how big it is), length (how many chars are in it) and c (the actual things stored in the row)
  size_t length;
  size_t size;
  char* c;
};

typedef struct row Row; //to reference struct row easier

int realIndex (Row *picture, int finalIndex) {  //gets rid of extra spaces and newlines
  int j = 0;
  j = finalIndex;
  while (picture[j].length == 0) {
    j--;
  }
  finalIndex = j;
  for (int i = 0; i<=finalIndex; i++) {
    j = picture[i].length - 1;
    while (j>= 0 && picture[i].c[j] == SPACE) {
      picture[i].length--;
      j--;
    }
  }
  return finalIndex;
}

void initRows (Row *picture, int x, int y) {  //initializes any new Rows I put in my array
  for (int i = x; i<y; i++) {
    picture[i].length = 0;
    picture[i].size = CONST;
    picture[i].c = malloc(sizeof(char) * CONST);
    assert(picture[i].c);
    for (int j = 0; j<CONST; j++) {
      picture[i].c[j] = SPACE;
    }
  }
}

void printArray (Row *picture, int finalIndex) {  //prints my array as output
  finalIndex = realIndex(picture, finalIndex);
  for (int i = 0; i<=finalIndex; i++) {
    for (int j = 0; j<picture[i].length; j++) {
      if ((j != (picture[i].length - 1)) || (picture[i].c[j] != SPACE)) {
        printf("%c", picture[i].c[j]);
      }
    }
    printf("\n");
  }
}

int main(int argc, char **argv) {
  FILE *pfile;
  int c = 0;
  int currentcol = 0;
  int currentrow = 0;
  int size = 1; //actual size of my array
  int finalIndex = 0; //final index in my array
  int row;
  int col;
  char filename[BUFFER_SIZE];
  Row *picture = malloc(sizeof(Row) * size);  //declare and initialize my array
  assert(picture);
  initRows(picture, 0, size);
  while(scanf("%d %d %s", &row, &col, filename) == 3) {
    pfile = fopen(filename, "r");
    if (pfile==0){
      perror(filename);
      exit(1);
    }
    currentrow= row;  //index of current row
    currentcol= col;  //index of current column
    while ((c = fgetc(pfile)) != EOF) {
      while (currentrow >= size) {  //makes sure array is big enough
        size *= 2;
        picture = realloc(picture, sizeof(Row) * size);
        assert(picture);
        initRows(picture, size/2, size);
      }
      while (currentcol >= picture[currentrow].size) {  //makes sure Row is big enough 
        picture[currentrow].size *= 2;
        picture[currentrow].c = realloc(picture[currentrow].c, sizeof(char) * picture[currentrow].size); 
        assert(picture[currentrow].c);
        for (size_t j = picture[currentrow].size/2; j<picture[currentrow].size; j++) {
          picture[currentrow].c[j] = SPACE;
        }
      }
      if(c == '\n') {   //go to next row in array
        currentrow++;
        currentcol = col;
      }
      else if (c != SPACE) {
        picture[currentrow].c[currentcol] = c; 
        currentcol++;
        if (currentcol >= picture[currentrow].length) { //increase the length of the Row if we use a column index bigger than length
          picture[currentrow].length = currentcol;
        }
      }
      else {  //don't print anything if c = SPACE since array is already initalized with spaces and spaces shouldn't cover non-space chars anyways 
        currentcol++;
        if (currentcol>=picture[currentrow].length) { //increase the length of the Row if we use a column index bigger than length
          picture[currentrow].length = currentcol;
        }
      }
    }
    if (finalIndex<=currentrow) {  //test if file end with \n then EOF or just EOF
      if (currentrow>size-1) {
        finalIndex = currentrow-1;
      }
      else {
        finalIndex = currentrow;
      }
    } 
    fclose(pfile);  //close file after done with it
  }
  printArray(picture, finalIndex);  //print array
  for(int k = 0; k<size; k++){ //important: free each pointer thing in each struct before freeing array
    free(picture[k].c);
  }
  free(picture);
  return 0;
}