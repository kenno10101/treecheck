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
    // if root is null, create a new node (base case)
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

tnode *inputFile(char *filename, tnode *root)
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
            // printf("read value: %d\n", value);
            root = insertValue(root, value);
        }
    }
    return root;
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

// postorder (right then left), calculate balance, store value for min/max/avg later
void postOrder(tnode *root)
{
    if (root == NULL)
    {
        return;
    }
    postOrder(root->right);
    postOrder(root->left);
    printf("bal(%d)=%d", root->key, bal(root));
    if (bal(root) > 1 || bal(root) < -1)
    {
        printf(" (AVL violation!)");
    }
    printf("\n");
    // add number to list for min/max/avg
}

int main()
{

    tnode *root = NULL;
    char *filename = "input.txt";
    root = inputFile(filename, root);
    postOrder(root);

    // int depth = maxDepth(root);
    // printf("depth of root: %d\n", depth);
    // printf("balance factor of %d: %d", root->key, bal(root));

    return 0;
}