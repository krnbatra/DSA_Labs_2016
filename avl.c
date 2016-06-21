#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct node{
  char data[1000];
  struct node* left;
  struct node* right;
  struct node* parent;
  int height;
}node;

int find_max(int a, int b){
  if(a > b)
    return a;
  else
    return b;
}

node* rotate_right(node* temp){
  node* l = temp->left; 
  node* r = l->right; 
  l->right = temp;
  temp->left = r;
  if(temp->parent){
    if(temp->parent->right==temp){
      temp->parent->right = l;
    }else
      temp->parent->left = l;
  }
  l->parent = temp->parent;
  temp->parent = l;
  if(r)
    r->parent = temp;
  int a, b;
  if(temp->left!=NULL){
    a = temp->left->height;
  }else
    a = 0;
  if(temp->right!=NULL){
    b = temp->right->height;
  }else{
    b = 0;
  }
  temp->height = find_max(a, b)+1;

  if(l->left!=NULL){
    a = l->left->height;
  }else
    a = 0;
  if(l->right!=NULL){
    b = l->right->height;
  }else{
    b = 0;
  }
  l->height = find_max(a, b)+1;
  return l;
}

node* rotate_left(node* temp){
  node* l = temp->right; //l becomes y
  node* r = l->left; //r becomes t3
  l->left = temp;
  temp->right = r;
  if(r)
    r->parent = temp;
  if(temp->parent){
    if(temp->parent->right==temp){
      temp->parent->right = l;
    }else
      temp->parent->left = l;
  }
  l->parent = temp->parent;
  temp->parent = l;
  //update heights
  int a, b;
  if(temp->left!=NULL){
    a = temp->left->height;
  }else
    a = 0;
  if(temp->right!=NULL){
    b = temp->right->height;
  }else{
    b = 0;
  }
  temp->height = find_max(a, b)+1;

  if(l->left!=NULL){
    a = l->left->height;
  }else
    a = 0;
  if(l->right!=NULL){
    b = l->right->height;
  }else{
    b = 0;
  }
  l->height = find_max(a, b)+1;
  return l;
}
void inorder(node* root){
  if(root==NULL)
    return;
  inorder(root->left);
  if(root->left!= NULL && root->right!=NULL)
    printf("%s %d %d\n", root->data, root->height, root->left->height - root->right->height);
  else if(root->left == NULL && root->right!=NULL)
    printf("%s %d -1\n", root->data, root->height);
  else if(root->right == NULL && root->left!=NULL)
    printf("%s %d 1\n", root->data, root->height);
  else
    printf("%s %d 0\n", root->data, root->height);
  inorder(root->right);
}
node* insert(node* root, char data[]){
  node* new_node = (node*)malloc(sizeof(node));
  strcpy(new_node->data, data);
  new_node->height = 1;
  if(root == NULL){
    root = new_node;
    return root;
  }
  node* temp = root;
  while(temp->left != NULL || temp->right != NULL){
    if(strcmp(new_node->data, temp->data) >= 0){
      if(temp->right!=NULL){
        temp = temp->right;
      }
      else
        break;
    }
    else{
      if(temp->left!=NULL){
        temp = temp->left;
      }
      else
        break;
    }
  }
  if(strcmp(new_node->data, temp->data) < 0){
    temp->left = new_node;
    new_node->parent = temp;
  }else{
    temp->right = new_node;
    new_node->parent = temp;
  }
  temp = new_node;
  while(temp != NULL){
    int lh=0, rh = 0;
    if(temp->left != NULL)
      lh = temp->left->height;
    if(temp->right != NULL)
      rh = temp->right->height;
    if(abs(lh-rh) > 1){
      if((lh-rh) > 1 && temp->left && strcmp(new_node->data, temp->left->data) < 0){
        temp = rotate_right(temp);
        break;
      }
      if((lh-rh) < -1 && temp->right && strcmp(new_node->data, temp->right->data) >= 0){
        temp = rotate_left(temp);
        break;
      }
      if((lh-rh) > 1 && temp->left && strcmp(new_node->data, temp->left->data) >= 0){
        temp->left = rotate_left(temp->left);
        temp = rotate_right(temp);
        break;
      }

      else if((lh-rh) < -1 && temp->right && strcmp(new_node->data, temp->right->data) < 0){
        // printf("%s\n\n", temp->data);
        temp->right = rotate_right(temp->right);
        temp = rotate_left(temp);
        break;
      }
    }else{
      temp->height = find_max(lh, rh)+1;
    }
    temp = temp->parent;
  }
  temp = new_node;

  while(temp->parent!=NULL){
    temp = temp->parent;
  }
  root = temp;
  return root;
}
void preorder(node* root){
  if(root==NULL)
    return;
  if(root->left!= NULL && root->right!=NULL)
    printf("%s %d %d\n", root->data, root->height, root->left->height - root->right->height);
  else if(root->left == NULL && root->right!=NULL)
    printf("%s %d -1\n", root->data, root->height);
  else if(root->right == NULL && root->left!=NULL)
    printf("%s %d 1\n", root->data, root->height);
  else
    printf("%s %d 0\n", root->data, root->height);
  preorder(root->left);
  preorder(root->right);
}
int main(){
  char buff[1000];
  char input[1000][100];
  int count = 0;
  node* root = NULL;
  while(fscanf(stdin,"%s", buff)!=EOF){
      strcpy(input[count], buff);
      count++;
  }
  // printf("%d\n", count);
  int j = 0;
  int i= 0;
  for(i = 0;i < count; i++){
    root = insert(root, input[i]);
  }
  inorder(root);
  preorder(root);
  return 0;
}
