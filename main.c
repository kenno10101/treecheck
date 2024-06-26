#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure of a node for a binary (search) tree
typedef struct tnode
{
    int key;
    struct tnode *left;
    struct tnode *right;
} tnode;

// Structure of a node for a linked list
typedef struct node
{
    int value;
    struct node *next;
} node;

// Function to add a Node to the last position (used in nodeSearch)
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

// Function to print the elements of a linked list (used in nodeSearch)
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

// Function to insert a value to a binary search tree
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

// Function to insert values to a binary search tree from a textfile
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

// traverse tree in reverse-postorder, calculate balance, store value for min/max/avg later
void postOrder(tnode *root, int *avlViolation, int *min, int *max, float *sum, float *counter)
{
    if (root == NULL)
    {
        return;
    }
    postOrder(root->right, avlViolation, min, max, sum, counter);
    postOrder(root->left, avlViolation, min, max, sum, counter);

    // calculate balance
    printf("bal(%d)=%d", root->key, bal(root));
    // set AVL violation flag if balance is greater than 1 or less than -1
    if (bal(root) > 1 || bal(root) < -1)
    {
        *avlViolation = 1;
        printf(" (AVL violation!)");
    }
    printf("\n");

    // calculate statistics
    (*counter)++;
    *sum += root->key;
    if (root->key < *min)
    {
        *min = root->key;
    }
    else if (root->key > *max)
    {
        *max = root->key;
    }
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

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[0], "treecheck") == 0)
    {
        // part 1
        tnode *root = NULL;
        char *filename = argv[1];

        root = inputFile(filename, root);

        int avlViolation = 0;
        int min = root->key, max = root->key;
        float counter = 0, sum = 0;
        postOrder(root, &avlViolation, &min, &max, &sum, &counter);

        if (avlViolation == 0)
        {
            printf("AVL: yes\n");
        }
        else
        {
            printf("AVL: no\n");
        }
        printf("min: %d, max: %d, avg: %.1f\n", min, max, sum / counter);

        deleteTree(root); // free memory of tree (root)
    }
    else if (argc == 3 && strcmp(argv[0], "treecheck") == 0)
    {
        // part 2
        tnode *root = NULL;
        tnode *subRoot = NULL;
        char *filename = argv[1];
        char *subtreeFilename = argv[2];

        root = inputFile(filename, root);              // create a tree from the searchtree file
        subRoot = inputFile(subtreeFilename, subRoot); // create a tree from the subtree file

        if (subRoot != NULL && subRoot->left == NULL && subRoot->right == NULL)
        {
            // search for a single key
            node *subtreeHead = NULL; // linked list to store the subtree of searchKey
            int searchKey = subRoot->key;
            nodeSearch(root, searchKey, &subtreeHead);

            deleteList(subtreeHead);
        }
        else if (subRoot != NULL && (subRoot->left != NULL || subRoot->right != NULL))
        {
            // search for a subtree
            subtreeSearch(root, subRoot);
        }

        deleteTree(subRoot); // free memory
        deleteTree(root);
    }
    else
    {
        printf("invalid input\ntype: treecheck <input filename> <subtree filename (optional)>");
    }
    return 0;
}