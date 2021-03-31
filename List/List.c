#include "List.h"
#include <stdio.h>
#include <stdlib.h>

node_t * head = NULL; 
unsigned long ans;

void add_element(unsigned long riddle, unsigned long answer){
    node_t * temp;
    node_t * ptr;
    temp = (node_t *) malloc(sizeof(node_t));

    temp->riddle = riddle;
    temp->answer = answer;
    temp->next = NULL;

    if(head == NULL){
        head = temp;
    }else{
        ptr = head;
        while(ptr->next!=NULL){
            ptr=ptr->next;
        }
        ptr->next = temp;
    }
}

void delete_list(){
    node_t * tmp;
    tmp = (node_t *) malloc(sizeof(node_t));
    while(head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
    printf("Deleted\n");
}

void print_list() {
    node_t * current = head;

    while (current != NULL) {
        printf("riddle = %ld, answer = %ld\n", current->riddle, current->answer);
        current = current->next;
    }
}

unsigned long get_answer(int i){
    unsigned long val;
    node_t * ptr;
    ptr=head;
    for(int j=1;j<i;j++){
        ptr=ptr->next;
    }
    val = ptr->answer % 10;
    printf("Elem mod 10 = %lu\nElem = %lu\n", val, ptr->answer);
    return val;
}

void read_from_file(node_t * head){
    const char filename[] = "1000.txt";

    unsigned long a, c;
    char b;

    FILE *file = fopen(filename, "r");
    while (EOF != fscanf(file, "%lu %c %lu", &a,&b,&c)){
        add_element(a,c);
    }

    fclose(file);
}

int main(){
    read_from_file(head);

    print_list();

    ans = get_answer(2);
    printf("%lu",ans);

    delete_list();

    return 0;
}