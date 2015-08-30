#include <stdlib.h>
#include <stdio.h>

struct tape{    //creates a tape element with a next pointer, a previous pointer, and a value
    struct tape *next;
    struct tape *prev;
    char value;
};

typedef struct tape *Tape;

void tapeDestroy(Tape head){    //fcn to free all the linked tape elements
    Tape temp;
    if (head == NULL){
        return;
    }
    else {
        temp = head->next;
        free(head);
        tapeDestroy(temp);
    }
}

int main(int argc, char const *argv[])
{
    int state = 1;  //basically my machine
    int steps = 0;
    int ithrule;
    Tape curr;  //pointing to current position
    Tape head; //pointing to head of tape
    Tape node;  //start of my tape
    node = malloc(sizeof(struct tape));
    node->next = NULL;
    node->prev = NULL;
    node->value = 'a';
    head = node;
    curr = node;
    while (state != 0) {
        steps++;
        ithrule = curr->value - 'a';    //easier to look up the correct rule
        curr->value = argv[state][ithrule * 3]; //reassigns the value of the tape element I am currently on
        if (argv[state][ithrule * 3 + 1] == '+'){   //move right
            if (curr->next == NULL){    //check if the current tape element is the right-most; extend if yes
                Tape nodeR;
                nodeR = malloc(sizeof(struct tape));
                nodeR->next = NULL;
                nodeR->prev = curr;
                nodeR->value = 'a';
                curr->next = nodeR;
            }
            curr = curr->next;  //move current node right
        }
        else {  //move left
            if (curr->prev == NULL){    //check if the current tape element is the left-most; extend if yes
                Tape nodeL;
                nodeL = malloc(sizeof(struct tape));
                nodeL->next = curr;
                nodeL->prev = NULL;
                nodeL->value = 'a';
                curr->prev = nodeL;
                head = nodeL;
            }
            curr = curr->prev; //move current node left
        }
        state = atoi(&argv[state][ithrule * 3 + 2]);    //change to new state
    }
    tapeDestroy(head);  //free the tape
    printf("%d\n", steps); //prints how many steps we used
    return 0;
}