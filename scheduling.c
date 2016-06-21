#include <stdio.h>
#include <stdlib.h>

typedef struct BSTNode{
  int i;
  int r;
  int c;
  int p;
  struct BSTNode* left;
  struct BSTNode* right;
  struct BSTNode* parent;
}BSTNode;

BSTNode* answer = NULL;

BSTNode* insert(BSTNode* root, int i, int r, int c, int p){
  BSTNode* new_node = (BSTNode*)malloc(sizeof(BSTNode));
  new_node->i = i;
  new_node->r = r;
  new_node->c = c;
  new_node->p = p;
  if(root == NULL){
    root = new_node;
    return root;
  }
  BSTNode* temp = root;
  while(temp!= NULL && temp->left!= NULL || temp->right!=NULL){
    if(new_node->p <= temp->p){
      if(temp->left==NULL)
        break;
      temp = temp->left;
    }
    else{
      if(temp->right==NULL)
        break;
      temp = temp->right;
    }
  }
  if(new_node->p <= temp->p){
    temp->left = new_node;
    new_node->parent = temp;
  }
  else{
    temp->right = new_node;
    new_node->parent = temp;
  }
  return root;
}

BSTNode* delete(BSTNode* root){
  if(root==NULL){
    answer = NULL;
    return NULL;
  }
  BSTNode* temp = root;
  if(root->right == NULL){
    answer = root;
    root = root->left;
    return root;
  }
  while(temp->right!=NULL){
    temp = temp->right;
  }
  answer = temp;
  if(temp && temp->left)
    temp->left->parent = temp->parent;
  temp->parent->right = temp->left;
  return root;
}

void preOrder(BSTNode* root){
  if(root==NULL)
    return;
  printf("%d-->", root->i);
  preOrder(root->left);
  preOrder(root->right);
}

int main(){
  char buff[100000];
  int arr[100000][4];
  int i=0, r=0, c=0, p=0;
  int j = 0, count = 0, curr_time = 0;
  BSTNode *root = NULL, *process = NULL;
  int flag = 0;
  while(fscanf(stdin, "%d%d%d%d", &i, &r, &c, &p)!=EOF){
    arr[count][0] = i;
    arr[count][1] = r;
    arr[count][2] = c;
    arr[count][3] = p;
    count++;
  }
  while(j < count || root != NULL || process != NULL){
    while(j < count && arr[j][1] == curr_time){
      root = insert(root, arr[j][0], arr[j][1], arr[j][2], arr[j][3]);
      j++;
    }
    if(j == 0)
      printf("0 ");
    if(process == NULL){
      // if(root!=NULL){
        root = delete(root);
        process = answer;
      // }
    }
    else if(process != NULL /*&& root != NULL*/){
      root =  delete(root);
      if(answer!=NULL){
        if(answer->p > process->p){
          root = insert(root, process->i, process->r, process->c, process->p);
          process = answer;
        }else{
          root = insert(root, answer->i, answer->r, answer->c, answer->p);
        }
      }
    }
    if(process != NULL){
      printf("%d ", process->i);
      if(process->c > 0){
        process->c -= 1;
      }
      if(process->c == 0)
        process = NULL;
    }
    curr_time++;
  }
  return 0;
}
