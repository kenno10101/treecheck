#include <stdio.h>
#include <stdlib.h>

typedef struct tnode
{
    int key;
    struct tnode *left;
    struct tnode *right;
} tnode;

typedef struct node
{
    int value;
    struct node *next;
} node;

node *addNode(node *head, int value)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->value = value;
    newNode->next = NULL;
    if (head == NULL)
    {
        return newNode;
    }
    newNode->next = head;
    return newNode;
}

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

// calculate max depth of a tree
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

// calculate balance of a node (right depth - left depth)
int bal(tnode *root)
{
    int balance = maxDepth(root->right) - maxDepth(root->left);
    return balance;
}

// reverse-postorder (right then left), calculate balance, store value for min/max/avg later
void postOrder(tnode *root, node **head, int *avlViolation)
{
    if (root == NULL)
    {
        return;
    }
    postOrder(root->right, head, avlViolation);
    postOrder(root->left, head, avlViolation);
    printf("bal(%d)=%d", root->key, bal(root));
    // set AVL violation flag if balance is greater than 1 or less than -1
    if (bal(root) > 1 || bal(root) < -1)
    {
        *avlViolation = 1;
        printf(" (AVL violation!)");
    }
    printf("\n");
    // add number to list for min/max/avg
    *head = addNode(*head, root->key);
}

// print if avl and min/max/avg
void printStats(node *head, int *avlViolation)
{
    int min = head->value, max = head->value;
    float counter = 0, sum = 0;

    if (head == NULL)
    {
        return;
    }
    while (head != NULL)
    {
        sum += head->value;
        counter++;
        if (head->value < min)
        {
            min = head->value;
        }
        if (head->value > max)
        {
            max = head->value;
        }
        head = head->next;
    }
    float avg = sum / counter;
    if (*avlViolation == 0)
    {
        printf("AVL: yes\n");
    }
    else
    {
        printf("AVL: no\n");
    }
    printf("min: %d, max: %d, avg: %.1f\n", min, max, avg);
}

void nodeSearch(tnode *root, int searchKey)
{
    if (root == NULL)
    {
        printf("%d not found!\n", searchKey);
        return;
    }
    if (searchKey == root->key)
    {
        printf("%d found ", searchKey);
        // implement function to print the subtree of the searchKey

        printf("\n");
        return;
    }
    if (searchKey < root->key)
    {
        nodeSearch(root->left, searchKey);
    }
    else
    {
        nodeSearch(root->right, searchKey);
    }
}
int main()
{

    tnode *root = NULL;
    node *head = NULL;
    int avlViolation = 0;
    char *filename = "input.txt";
    root = inputFile(filename, root);
    postOrder(root, &head, &avlViolation);
    printStats(head, &avlViolation);
    nodeSearch(root, 7);
    nodeSearch(root, 30);
    nodeSearch(root, 29);
    nodeSearch(root, 16);
    nodeSearch(root, 17);

    return 0;
}