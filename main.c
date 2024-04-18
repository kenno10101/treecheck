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

node *addLastNode(node *head, int value)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->value = value;
    newNode->next = NULL;
    if (head == NULL)
    {
        return newNode;
    }
    node *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
    return head;
}

// to prevent memory leak
void deleteList(node *head)
{
    node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void printList(node *head)
{
    if (head == NULL)
    {
        return;
    }
    while (head != NULL)
    {
        if (head->next == NULL)
        {
            printf("%d", head->value);
            break;
        }
        printf("%d, ", head->value);
        head = head->next;
    }
    return;
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

// Function to delete an AVL tree
void deleteTree(tnode *root)
{
    if (root == NULL)
    {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

tnode *inputFile(char *filename, tnode *root)
{
    int value;
    FILE *f = fopen(filename, "r");
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
    fclose(f);
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

// preorder traversal search and store the each key for the subtree
void nodeSearch(tnode *root, int searchKey, node **subtreeHead)
{
    if (root == NULL)
    {
        // free memory of linked list (subtreeHead)
        printf("%d not found!\n", searchKey);
        return;
    }
    // add root->key to linked list
    *subtreeHead = addLastNode(*subtreeHead, root->key);
    if (searchKey == root->key)
    {
        printf("%d found ", searchKey);
        // print the subtree of the searchKey (linked list)
        printList(*subtreeHead);
        // free memory of linked list (subtreeHead)
        deleteList(*subtreeHead);
        printf("\n");
        return;
    }
    if (searchKey < root->key)
    {
        nodeSearch(root->left, searchKey, subtreeHead);
    }
    else
    {
        nodeSearch(root->right, searchKey, subtreeHead);
    }
}

// Function to check if the given subtree is present in the main tree
int checkSubtree(tnode *mainRoot, tnode *subRoot)
{
    if (subRoot == NULL)
    {
        return 1;
    }
    if (mainRoot == NULL)
    {
        return 0;
    }

    if (mainRoot->key == subRoot->key)
    {
        return checkSubtree(mainRoot->left, subRoot->left) && checkSubtree(mainRoot->right, subRoot->right);
    }

    return checkSubtree(mainRoot->left, subRoot) || checkSubtree(mainRoot->right, subRoot);
}

// Function to print the result if the given subtree is present in the main tree
void subtreeSearch(tnode *mainRoot, tnode *subRoot)
{
    if (checkSubtree(mainRoot, subRoot))
    {
        printf("Subtree found\n");
    }
    else
    {
        printf("Subtree not found!\n");
    }
}

int main()
{
    // part 1
    tnode *root = NULL;
    // linked list for storing treenodes for printing stats
    node *head = NULL;
    int avlViolation = 0;
    char *filename = "input.txt";
    root = inputFile(filename, root);
    postOrder(root, &head, &avlViolation);
    printStats(head, &avlViolation);

    // part 2
    // linked list to store the subtree of searchKey
    node *subtreeHead = NULL;
    // search for a single key
    int searchKey = 7;
    nodeSearch(root, searchKey, &subtreeHead);

    // search for a subtree
    tnode *subRoot = NULL;
    char *subtreeFilename = "subtree.txt";
    subRoot = inputFile(subtreeFilename, subRoot);
    subtreeSearch(root, subRoot);

    // free linked list (head) and tree (root)
    deleteList(head);
    deleteTree(root);

    return 0;
}