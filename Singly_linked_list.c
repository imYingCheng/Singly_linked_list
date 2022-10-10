#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct listNode *listPointer;
struct listNode{
    int data;
    listPointer link;
};
int totalNode = 0;
void add(listPointer *, int);
bool del(listPointer *, int);
bool ins(listPointer *, int, int);
void mul(listPointer *, int, int);
void invert(listPointer *, listPointer);                    //invert the substring
listPointer rev(listPointer *, int);
void show(listPointer *);
FILE *input_file;
FILE *output_file;
int main(){
    listPointer head, tail;
    int input,tmp1,tmp2;
    char str[5],c;
    bool flag = 0;                                          //record if the operation is success(is used in modify totalNode)
    input_file = fopen("input_1.txt", "r");
    output_file=fopen("output_1.txt","w");
    head = NULL;
    tail = NULL;
    while(1){                                               //build the original list until it reads '\n'
        fscanf(input_file, "%d%c", &input, &c);
        listPointer node;
        node = (listPointer)malloc(sizeof(struct listNode));
        node->data = input;
        node->link = NULL;
        totalNode++;
        if(!head)
            head = node;
        else
            tail->link = node;
        tail = node;
        if(c=='\n')
            break;
    }
    fscanf(input_file, "%d ", &input);                      //input is the number of operations
    for (int i = 0; i < input;i++){
        flag = 0;
        fscanf(input_file, "%s", str);
        if(!strcmp(str,"add")){
            fscanf(input_file, " %d\n", &tmp1);
            add(&head,tmp1);
            totalNode++;
        }
        else if(!strcmp(str,"del")){
            fscanf(input_file, " %d\n", &tmp1);
            flag=del(&head,tmp1);                           //if the operation of deletion is success, substract 1 from totalNode
            if(flag)
                totalNode--;
        }
        else if(!strcmp(str,"ins")){
            fscanf(input_file, " %d %d\n", &tmp1, &tmp2);   //if the operation of insertion is success, add 1 from totalNode
            flag=ins(&head,tmp1, tmp2);
            if(flag)
                totalNode++;
        }
        else if(!strcmp(str,"mul")){
            fscanf(input_file, " %d %d\n", &tmp1, &tmp2);
            mul(&head,tmp1, tmp2);
        }
        else if(!strcmp(str,"rev")){
            fscanf(input_file, " %d\n", &tmp1);
            head=rev(&head,tmp1);
        }
        else if(!strcmp(str,"show"))
            show(&head);
    }
    free(head);
    free(tail);
    fclose(input_file);
    fclose(output_file);
}
void add(listPointer *head,int i){
    listPointer node,tmp;
    node = (listPointer)malloc(sizeof(struct listNode));//build the new node
    node->data = i;
    node->link = NULL;                                  
    if (!(*head))                                       //if the head doesn't exist, the new node is the head
        *head = node;
    else{
        for (tmp = *head; tmp->link;tmp=tmp->link);     //else add the new node to the tail of the original list
        tmp->link = node;
    }
    return;
}
bool del(listPointer *head,int i){
    if(i==0||i>totalNode)                               //if i exceeds the range of list node, the operation remains unchanged
        return false;                                   //i.e. the operaiton of deletion is failed
    else if(i==1){                                      //delete the head node
        listPointer target;
        target = *head;
        *head = (*head)->link;
        free(target);
    }
    else{                                               //delete the other node
        listPointer preTarget, target;
        preTarget = *head;
        int j;
        for (j = 0; j < i - 2;j++){
            preTarget = preTarget->link;
            if(preTarget==NULL)
                break;
        }
        if(j==i-2){
            target = preTarget->link;
            if(target)
                preTarget->link = target->link;
            free(target);
        }
        else
            return false;
    }
    return true;
}
bool ins(listPointer *head,int i,int j){
    listPointer node,target;
    node = (listPointer)malloc(sizeof(struct listNode));//build the new node
    node->data = j;
    node->link = NULL;
    if(!head&&i==0)                                     //if the head doesn't exist, the new node is the head
        *head = node;
    else if(i==0){                                      //if i==0, insert the new node in front of the head node
        node->link = (*head);
        *head = node;                                   //and then set the head node to the new node
    }
    else{                                               //else find the right position to insert new node
        target = *head;
        int k;
        for (k = 0; k < i - 1;k++){
            target = target->link;
            if(!target)
                break;
        }
        if(k==i-1){                                     //if it find the right position, insert the new node in the list
            node->link = target->link;
            target->link = node;
        }
        else                                            //if it reaches the end of the list before finding the right position, the operation of insertion is failed
            return false;
    }
    return true;
}
void mul(listPointer *head,int i,int j){
    if (i > totalNode || i <= 0)                        //if i exceeds the range of the number of node, return
        return;
    listPointer target;
    target = *head;
    for (int k = 0; k < totalNode-i; k++)               //else find the target node
        target = target->link;
    target->data *= j;                                  //multiply the data of target node by j
    return;
}
listPointer rev(listPointer *head,int k){
    listPointer tail,tmp;
    tail = *head;
    if(!tail)
        return *head;                                   //if the list doesn't exist, return
    for (int i = 0; i < k-1;i++){                       //find the sublist that needs to be inverted
        if(!(tail->link))
            break;
        tail = tail->link;
    }
    tmp = tail->link;                                   //record the head node of next sublist
    invert(head, tail);                                 //invert the sublist
    (*head)->link = rev(&(tmp),k);
    *head = tail;
    return *head;
}
void show(listPointer *head){
    listPointer target;
    for (target = *head; target; target = target->link)
        fprintf(output_file,"%d ", target->data);
    fprintf(output_file,"\n");
    return;
}
void invert(listPointer *head,listPointer tail){        //invert the list
    listPointer first, middle, last;
    first = *head;
    middle = NULL;
    while(first!=tail){
        last = middle;
        middle = first;
        first = first->link;
        middle->link = last;
    }
    first->link = middle;
    return;
}