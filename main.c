#include <stdio.h>
#include <stdlib.h>

typedef struct tnode
{
    int key;
    struct tnode *left;
    struct tnode *right;
} tnode;

tnode *insertValue(tnode *root, int value)
{
    // if root is null, create a new node (exit clause)
    if (root == NULL)
    {
        tnode *newNode = (tnode *)malloc(sizeof(tnode));
        newNode->key = value;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    // if value is equal to root key, do nothing
    if (value == root->key)
    {
        return root;
    }
    // if value is less than root key, insert to left recursively
    if (value < root->key)
    {
        root->left = insertValue(root->left, value);
    }
    // if value is greater than root key, insert to right recursively
    else
    {
        root->right = insertValue(root->right, value);
    }
}

void inputFile(char *filename, tnode *root)
{
    int value;
    FILE *f;
    f = fopen("input.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
    }
    else
    {
        while (fscanf(f, "%d", &value) == 1)
        {
            root = insertValue(root, value);
        }
    }
}

int maxDepth(tnode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    int left = maxDepth(root->left);
    int right = maxDepth(root->right);
    if (left > right)
    {
        return (left + 1);
    }
    else
    {
        return (right + 1);
    }
}

int bal(tnode *root)
{
    int balance = maxDepth(root->right) - maxDepth(root->left);
    return balance;
}

int main()
{

    tnode *root = NULL;
    char *filename = "input.txt";

    // inputFile(filename, root);
    root = insertValue(root, 5);
    root = insertValue(root, 3);
    root = insertValue(root, 17);
    root = insertValue(root, 9);
    root = insertValue(root, 23);
    root = insertValue(root, 54);
    root = insertValue(root, 11);
    root = insertValue(root, 79);
    root = insertValue(root, 30);
    root = insertValue(root, 12);

    int depth = maxDepth(root);
    printf("depth of root: %d\n", depth);
    printf("balance factor of %d: %d", root->right->right->right->right->key, bal(root->right->right->right->right));

    return 0;
}