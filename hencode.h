#ifndef HENCODE_H
#define HENCODE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
typedef struct binTreeNode *tNode, binTreeNode;
struct binTreeNode{
        tNode left;
        tNode right;
        int value;
        char key;
        int isLeaf;
};

typedef struct Tree{
        tNode head;
}*tree, Tree;

typedef struct linkedListNode *lNode, linkedListNode;
struct linkedListNode{
        lNode next;
        tNode t;
};


typedef struct List{
        lNode head;
}*list, List;
int delete_front(list l){
  if(l->head != NULL){
    lNode tmp = l->head;
    l->head = l->head->next;
    free(tmp);
    return 0;
  }
  else{
    return 1;
  }
}
list create_list(){
        list l = (list)malloc(sizeof(List));
        l->head = NULL;
        return l;
}
void delete_tNode(tNode n){
     if (n != NULL) {
        delete_tNode(n->right);
        delete_tNode(n->left);
        free(n);
     }
     return;
}
void delete_lNode(lNode n){
        if(n->next == NULL){
                delete_tNode(n->t);
                free(n);
                return;
        }
        delete_lNode(n->next);
        delete_tNode(n->t);
        free(n);
        return;
}
void delete_list(list l){
        if(l->head != NULL){
        delete_lNode(l->head);
        }
        free(l);
        return;
}
void insert_sorted_lNode(list l, tNode t){
        lNode currNode, newNode;
        currNode = l->head;
        if(currNode == NULL){
                l->head = (lNode)(malloc(sizeof(linkedListNode)));
                l->head->next = NULL;
                l->head->t = t;
                return;
        }

        if(currNode->t->value >= t->value){
                newNode = (lNode)(malloc(sizeof(linkedListNode)));
                newNode->next = l->head;
                newNode->t = t;
                l->head = newNode;
                return;
        }
        while(currNode->next != NULL && (currNode->next->t->value < t->value)){
                currNode = currNode->next;
        }
        newNode = (lNode)(malloc(sizeof(linkedListNode)));
        newNode->next = currNode->next;
        newNode->t = t;
        currNode->next = newNode;
        return;
}
tNode create_tree(tNode node1, tNode node2){
        tNode node3;
        node3 = (tNode)(malloc(sizeof(binTreeNode)));
        node3->right = node2;
        node3->left = node1;
        node3->value = node1->value + node2->value;
        node3->key = node1 -> key;
        node3->isLeaf = 0;
        return node3;
}

tree make_tree(list l){
        tree newTree;
        tNode newNode;
        newTree = (tree)malloc(sizeof(Tree));
        if(l->head == NULL){
                return NULL;
        }
        while(l->head->next != NULL){
                newNode = create_tree(l->head->t, l->head->next->t);
                delete_front(l);
                delete_front(l);
                insert_sorted_lNode(l, newNode);
        }
        newTree->head = l->head->t;
        return newTree;
}
void rec_get_codes(tNode n, int *binList[], int codes[], int codeSize){
        int i;

        if (n->left){
                codes[codeSize] = 0;
                rec_get_codes(n->left, binList, codes, codeSize +1);
        }
        if(n->right){
                codes[codeSize] = 1;
                rec_get_codes(n->right, binList, codes, codeSize+1);
        }
        if (n->isLeaf){
                binList[(int)(n->key)] = (int*) malloc((codeSize+1) * sizeof(int));
                for(i=0;i<codeSize; i++){
                        binList[(int)(n->key)][i] = codes[i];
                }
                binList[(int)(n->key)][i] = -1;
                return;
        }
}
void make_bin_list(tree t, int *binList[]){
        int codes[255];
        if(t == NULL){
                return;
        }
        rec_get_codes(t->head, binList, codes, 0);
}
#endif
