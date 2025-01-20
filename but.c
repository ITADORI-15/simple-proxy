#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// Define a structure for tree nodes
struct Node {
    char data;
    struct Node* left;
    struct Node* right;
};

// Function to create a new tree node
struct Node* newNode(char data) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = data;
    temp->left = temp->right = NULL;
    return temp;
}

// Stack structure for tree nodes
struct Stack {
    struct Node* treeNode;
    struct Stack* next;
};

// Push a node onto the stack
void push(struct Stack** top, struct Node* node) {
    struct Stack* temp = (struct Stack*)malloc(sizeof(struct Stack));
    temp->treeNode = node;
    temp->next = *top;
    *top = temp;
}

// Pop a node from the stack
struct Node* pop(struct Stack** top) {
    if (*top == NULL) {
        printf("Stack underflow\n");
        return NULL;
    }
    struct Node* temp = (*top)->treeNode;
    struct Stack* oldTop = *top;
    *top = (*top)->next;
    free(oldTop);
    return temp;
}

// Build the expression tree from a postfix expression
struct Node* buildExpressionTree(char postfix[]) {
    struct Stack* stack = NULL;

    for (int i = 0; postfix[i] != '\0'; i++) {
        // Create a new node for each character
        struct Node* node = newNode(postfix[i]);

        // If the character is an operand, push it onto the stack
        if (isalnum(postfix[i])) {
            push(&stack, node);
        }
        // If the character is an operator, pop two nodes, set them as children, and push the new subtree
        else {
            struct Node* right = pop(&stack);
            struct Node* left = pop(&stack);
            node->right = right;
            node->left = left;
            push(&stack, node);
        }
    }

    // The last node in the stack is the root of the expression tree
    return pop(&stack);
}

// Evaluate the expression tree recursively
int evaluate(struct Node* root) {
    // Base case: if the node is a leaf (operand), return its value
    if (root->left == NULL && root->right == NULL) {
        return root->data - '0'; // Convert character to integer
    }

    // Recursively evaluate the left and right subtrees
    int leftValue = evaluate(root->left);
    int rightValue = evaluate(root->right);

    // Perform the operation based on the operator in the current node
    switch (root->data) {
        case '+': return leftValue + rightValue;
        case '-': return leftValue - rightValue;
        case '*': return leftValue * rightValue;
        case '/': return leftValue / rightValue;
        case '^': return pow(leftValue, rightValue);
        default: return 0;
    }
}

// Inorder traversal of the expression tree (prints the infix expression)
void inorder(struct Node* root) {
    if (root != NULL) {
        if (!isalnum(root->data)) printf("(");
        inorder(root->left);
        printf("%c", root->data);
        inorder(root->right);
        if (!isalnum(root->data)) printf(")");
    }
}

int main() {
    char postfix[100];

    // Input postfix expression
    printf("Enter a postfix expression: ");
    scanf("%s", postfix);

    // Build the expression tree
    struct Node* root = buildExpressionTree(postfix);

    // Print the infix expression (optional)
    printf("Infix expression: ");
    inorder(root);
    printf("\n");

    // Evaluate the expression tree
    int result = evaluate(root);
    printf("Result of evaluation: %d\n", result);

    return 0;
}