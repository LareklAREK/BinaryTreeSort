#include "bst.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

static int insert(Node **root, int value) {
    if (*root == 0) {
        *root = (Node*)malloc(sizeof(Node));
        if (!*root) {
            fprintf(stderr, "Ошибка: недостаточно памяти для создания узла дерева\n");
            return -1;
        }
        (*root)->key = value;
        (*root)->left = 0;
        (*root)->right = 0;
        return 0;
    } else if (value < (*root)->key) {
        return insert(&((*root)->left), value);
    } else {
        return insert(&((*root)->right), value);
    }
}

static void inorder(Node *root, int *arr, int *index) {
    if (root == 0) return;
    inorder(root->left, arr, index);
    arr[(*index)++] = root->key;
    inorder(root->right, arr, index);
}

static void freeTree(Node *root) {
    if (root == 0) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void binary_tree_sort(int *arr, int n) {
    if (n <= 1) return;

    Node *root = 0;

    for (int i = 0; i < n; i++) {
        if (insert(&root, arr[i]) != 0) {
            freeTree(root);
            fprintf(stderr, "Ошибка: сортировка прервана из-за недостатка памяти\n");
            return;
        }
    }

    int idx = 0;
    inorder(root, arr, &idx);
    freeTree(root);
}
