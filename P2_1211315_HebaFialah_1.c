/*
 - Student Name:Heba Fialah
 - Id:1211315
 - Section:1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_SIZE 50
typedef struct Node *ptr;
struct Node {
    char data[30];
    ptr left;
    ptr right;
};
struct node {
    char eq[100];
    char postfix[200];
    int result;
    char problem[68];
    int valid;
    struct node *next;
};

typedef struct node *list;
typedef struct node *position;

// -----------------------------Function in this project ------------------
void display_menu();

int readFile(list L);

void printToConsole(list L);

int printToFile(list L);

void appendCharWithoutSpace(char *str, char c);

position findPrevious(list L, position p);

char nextChar(char *str, short i);

void insert(list L, char *eq);

void isValid(position p);

int is_operator(char ch);

int is_bracket(char ch);

int open_bracket(char ch);

int isInteger(double num);

void infixToPostfix(position p);

ptr expTree(char postfix[]);

int precedence(char operator);

ptr createNode(char val[]);

int eval(ptr root);

int toInt(char s[]);
//--------------------------------------------------------------------------

int main() {     //_______________________Main Function______________________________
    list L = (list) malloc(sizeof(struct node));
    L->next = NULL;
    ptr root;
    position p;
    position m;
    position n;
    char *postfix;
    int operation;
    printf("\n\t\t_____________________________Welcome to project#2_________________________________\n");
    do {
        display_menu();
        printf("Enter Operation:\n");
        scanf("%d", &operation);
        switch (operation) {
            case 1:  // read the equation from file
                readFile(L);
                break;
            case 2:  //print infix equation on the screen
                p = L->next;
                while (p != NULL) {
                    isValid(p);
                    p = p->next;
                }
                printToConsole(L);
                break;
            case 3: //print infix equation & postfix equation & result of expression tree on the screen
                n = L->next;
                printf("%-20s\t\t%-20s\t\t%-20s\n", "Equation infix", " Equation postfix ", "Result");
                printf("----------------------------------------------------------------------\n");
                while (n != NULL) {
                    isValid(n);
                    if (n->valid == 1) {
                        infixToPostfix(n);
                        ptr root = expTree(&n->postfix);
                        n->result = eval(root);
                        printf("%-20s\t\t%-20s\t\t%d\n", n->eq, n->postfix, n->result);
                    } else if (n->valid == -1) {
                        infixToPostfix(n);
                        ptr root = expTree(&n->postfix);
                        // n->result = eval(root);
                        printf("%-20s\t\t%-20s\t\tMath ERROR (division by zero)\n", n->eq, n->postfix);
                    }


                    n = n->next;
                }


                break;
            case 4: //print postfix expression  on the screen
                m = L->next;
                printf("%-20s\t\t%-20s\t\t\n", "Equation infix", " Equation postfix ");
                printf("--------------------------------------------------------\n");
                while (m != NULL) {
                    isValid(m);

                    if (m->valid == 1 || m->valid == -1) {
                        infixToPostfix(m);
                        printf("%-20s\t\t%-20s\t\t\n", m->eq, m->postfix);
                    }
                    m = m->next;


                }

                break;
            case 5:    // print in the file
                printToFile(L);
                printf("\t\t Write to file Successfully\t\t\n");
                break;
            case 6: //Exit from program
                printf("\t\t Thanks for Select My Program \t\t\n");
                break;

            default:
                printf("Try again because number of option does not exist");
        }

    } while (operation != 6);

    return 0;

}

void display_menu() {//Main Menu
    printf("\t\t-----------MENU----------\n\n");
    printf("\t\t Choose from the list : \n\n");
    printf("\n\t\t 1) Read from the input file.\n");
    printf("\n\t\t 2) Print equation.\n");
    printf("\n\t\t 3) Evaluate using Expression Tree.\n");
    printf("\n\t\t 4) Print postfix expression.\n");
    printf("\n\t\t 5) Print to output file all result.\n");
    printf("\n\t\t 6) Exit.\n");
    printf("\t\t-----------------------------");
    printf("\n\t\t ::=>");
}

int readFile(list L) {  // read the equation from the file

    FILE *in;  // pointer of file to read


    in = fopen("input.txt", "r");

    if (in == NULL)
        return 0;

    char eq[100] = "";
    char c;

    while (fscanf(in, "%c", &c) != EOF) {

        if (c != '\n') {
            appendCharWithoutSpace(eq, c);
        } else {
            // if we reach '\n' append a null and insert the expression, and clear eq
            appendCharWithoutSpace(eq, '\0');
            insert(L, eq);         //read the expressions in this file and save them in the  linked list
            eq[0] = '\0';
        }
    }

    if (c != '\n') {        /* c != '\n' after this loop, then this means that the last equation is at the last line,
                            that is EOF is immediately after it */
        appendCharWithoutSpace(eq, '\0');
        insert(L, eq);
    }

    position p = L->next;

    while (p != NULL) {

        if (nextChar(p->eq, 0) == '\0') {       // if the line is empty then delete its node
            position q = findPrevious(L, p);
            q->next = p->next;
            free(p);
            p = q->next;
            continue;
        }
        p = p->next;
    }

    fclose(in);
    printf("Reading is finished successfully.\n");
    return 1; //it will return 1 if the file opened successfully, otherwise it will return zero
}

void printToConsole(list L) { //function to print on the screen

    position p = L->next;


    printf("%-20s\t\t%-20s\n", "Equation infix", "Validity");
    printf("---------------------------------------------\n");

    while (p != NULL) {


        printf("%-20s\t\t", p->eq);

        if (p->valid == 1 || p->valid == -1) {
            printf("valid");
        } else {
            printf("invalid");
        }

        p = p->next;
        printf("\n");
    }

}

int printToFile(list L) {
    char path[100];
    FILE *out = fopen("output.txt", "w");  // pointer of file to write

    if (out == NULL)
        return 0;

    position p = L->next;

    while (p != NULL) {

        fprintf(out, "%-20s ", p->eq);

        if (p->valid == 1) {
            fprintf(out, "\t valid \t ");
            fprintf(out, " \t ");
            fprintf(out, "postfix: %-20s ", p->postfix);

            if (isInteger(p->result))
                fprintf(out, "\t result = %d\n\n", (int) (p->result));
            else
                fprintf(out, "%-10s   result: %.2f\n\n", p->eq, p->result);
        } else if (p->valid == -1) {

            fprintf(out, "\t valid \t ");
            fprintf(out, " \t ");
            fprintf(out, "postfix: %-20s ", p->postfix);
            fprintf(out, "\t result = division by zero\n\n");
        } else {
            fprintf(out, "\t invalid \t ");

        }

        p = p->next;
        fprintf(out, "\n");
    }

    fclose(out);
    return 1;
}

void appendCharWithoutSpace(char *str, char c) {

    char Char[2];
    if (c >= '0' && c <= '9' || c == '*' || c == '-' || c == '+' || c == '/' || c == '%' || c == ')' || c == '(') {
        Char[0] = c;
        Char[1] = '\0';

        strcat(str, Char);              // appending the character to the string
    }
}

position findPrevious(list L, position p) {

    position previous = L;

    while ((previous->next != NULL) && (previous->next != p))
        previous = previous->next;

    return previous;
}

char nextChar(char *str, short i) {     // it must not be the index of null char
    i++;
    while (str[i] == ' ')
        i++;
    return str[i];
}

void insert(list L, char *eq) {

    position temp = (position) malloc(sizeof(struct node));

    if (temp != NULL) {

        position p = L;

        while (p->next != NULL)
            p = p->next;

        p->next = temp;
        temp->next = NULL;
        strcpy(temp->eq, eq);
        //is full
    } else {
        printf("Out of memory!");
        exit(0);
    }
}

void isValid(position p) {  // this function will check if the equation is valid or invalid.

    char *eq = p->eq;
    char *problem = p->problem;
    //this counter iterate the equation
    short i = 0;
    //this counter to count the number of parentheses,square,and curly brackets
    short count_parentheses = 0;
    short count_square_brackets = 0;
    short count_curly_brackets = 0;

    while (eq[i] != '\0') {
        //check if the character is not space & is not operator & is not bracket & is not digit,for example(#,@,$...),it's invalid equation
        if (eq[i] != ' ' && !is_operator(eq[i]) && !is_bracket(eq[i]) && !isdigit(eq[i])) {
            char ch[4];
            ch[0] = '"';
            ch[1] = eq[i];
            ch[2] = '"';
            ch[3] = '\0';

            strcpy(problem, "Error Usage :Not Valid Equation");
            strcat(problem, ch);// ch = ch + problem
            p->valid = 0;
            return;
        }
        //if the character is '(',increase counter by one
        if (eq[i] == '(')
            count_parentheses++;
            //if the character is ')',decrease counter by one
        else if (eq[i] == ')')
            count_parentheses--;
        //if the character is '[',increase counter by one
        if (eq[i] == '[')
            count_square_brackets++;
            //if the character is '[',decrease counter by one
        else if (eq[i] == ']')
            count_square_brackets--;
        //if the character is '{',increase counter by one
        if (eq[i] == '{')
            count_curly_brackets++;
            //if the character is '}',decrease counter by one
        else if (eq[i] == '}')
            count_curly_brackets--;

        if (count_parentheses < 0) {

            p->valid = 0;
            return;
        }
        if (count_square_brackets < 0) {

            p->valid = 0;
            return;
        }
        if (count_curly_brackets < 0) {

            p->valid = 0;
            return;
        }



        //if there is two parentheses like this ( )
        if (eq[i] == '(' && nextChar(eq, i) == ')') {

            p->valid = 0;
            return;
        }
        //if there is two square bracket like this [ ]
        if (eq[i] == '[' && nextChar(eq, i) == ']') {

            p->valid = 0;
            return;
        }
        //if there is two curly bracket like this { }
        if (eq[i] == '{' && nextChar(eq, i) == '}') {

            p->valid = 0;
            return;
        }

        if (eq[i] == ')' && nextChar(eq, i) == '(') {

            p->valid = 0;
            return;
        }

        if (eq[i] == ']' && nextChar(eq, i) == '[') {

            p->valid = 0;
            return;
        }
        if (eq[i] == '}' && nextChar(eq, i) == '{') {

            p->valid = 0;
            return;
        }



        //like this 3 2,missing operator between them
        if (isdigit(eq[i]) && eq[i + 1] == ' ') {
            char nextCharacter = nextChar(eq, i);
            if (isdigit(nextCharacter)) {

                p->valid = 0;
                return;
            }
            //like this 2 [,3 {,9 (,missing operation between them
            if (is_bracket(nextCharacter)) {

                p->valid = 0;
                return;
            }
        }
        //like this 2{
        if (isdigit(eq[i]) && open_bracket(eq[i + 1])) {

            p->valid = 0;
            return;

        }
        if (eq[i] == '/' && eq[i + 1] == '0') {
            p->valid = -1;
            return;
        }


        if (eq[i] == '+' || eq[i] == '-') {

            char nextCharacter = nextChar(eq, i);
            //like this +'\0',-'\0'
            if (nextCharacter == '\0') {

                p->valid = 0;
                return;
            }
            //like this +*,-*,+/,-/
            if (nextCharacter == '*' || nextCharacter == '/') {
                char op1[2];
                op1[0] = eq[i];
                op1[1] = '\0';

                char op2[2];
                op2[0] = nextCharacter;
                op2[1] = '\0';

                problem[0] = '\0';              // clearing the string first

                strcat(problem, op1);

                strcat(problem, op2);

                p->valid = 0;
                return;
            }
            //like this ++,--,-+.+-
            if (nextCharacter == '+' || nextCharacter == '-') {
                char op1[2];
                op1[0] = eq[i];
                op1[1] = '\0';

                char op2[2];
                op2[0] = nextCharacter;
                op2[1] = '\0';

                problem[0] = '\0';              // clearing the string first

                strcat(problem, op1);
                strcat(problem, op2);

                p->valid = 0;
                return;
            }


        }

        if (eq[i] == '*' || eq[i] == '/') {

            char nextCharacter = nextChar(eq, i);
            //like this *'\0',/'\0'
            if (nextCharacter == '\0') {

                p->valid = 0;
                return;
            }
            //like this **,//,*/,/*
            if (nextCharacter == '*' || nextCharacter == '/') {
                char op1[2];
                op1[0] = eq[i];
                op1[1] = '\0';

                char op2[2];
                op2[0] = nextCharacter;
                op2[1] = '\0';

                problem[0] = '\0';              // clearing the string first

                strcat(problem, op1);
                strcat(problem, op2);

                p->valid = 0;
                return;
            }
        }

        i++;
    }


    //Equation is valid if count_curly_brackets,parentheses,and square bracket equal to zero
    if (count_parentheses != 0) {

        p->valid = 0;
        return;
    }
    if (count_square_brackets != 0) {

        p->valid = 0;
        return;
    }
    if (count_curly_brackets != 0) {

        p->valid = 0;
        return;
    }

    p->problem[0] = '\0';
    p->valid = 1;
}

int is_operator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%');
}

int is_bracket(char ch) {
    return (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}');
}

int open_bracket(char ch) {
    return (ch == '(' || ch == '[' || ch == '{');
}

int isInteger(
        double num) {         // this function will the if the given double number can be replaced with an integer number

    int n = (int) num;

    if (n == num)
        return 1;
    else
        return 0;
}

void infixToPostfix(position p) {   // convert from infix to postfix
    int i, j;
    char *infix = p->eq;
    int len = strlen(infix);
    // char *postfix = (char *) malloc(sizeof(char) * (len + 2));
    char stack[MAX_EXPR_SIZE];
    int top = -1;

    for (i = 0, j = 0; i < len; i++) {
        if (infix[i] == ' ' || infix[i] == '\t')
            continue;

        // If the scanned character is operand
        // add it to the postfix expression
        if (infix[i] >= '0' && infix[i] <= '9') {

            p->postfix[j++] = infix[i];
            if (i < strlen(infix) - 1 && (infix[i + 1] < '0' || infix[i + 1] > '9')) {
                p->postfix[j] = ' ';
                j++;
            }
            if (i == strlen(infix) - 1) {
                p->postfix[j] = ' ';
                j++;
            }

        }



            // if the scanned character is '('
            // push it in the stack
        else if (infix[i] == '(') {
            stack[++top] = infix[i];

        }

            // if the scanned character is ')'
            // pop the stack and add it to the
            // output string until empty or '(' found
        else if (infix[i] == ')') {
            while (top > -1 && stack[top] != '(')
                p->postfix[j++] = stack[top--];
            p->postfix[j++] = ' ';

            top--;
        }

            // If the scanned character is an operator
            // push it in the stack
        else if (is_operator(infix[i])) {
            while (top > -1 && precedence(stack[top]) >= precedence(infix[i]))
                p->postfix[j++] = stack[top--];
            p->postfix[j++] = ' ';
            stack[++top] = infix[i];


        }
    }

    // Pop all remaining elements from the stack
    while (top > -1) {
        if (stack[top] == '(') {


        }

        p->postfix[j++] = stack[top--];
        p->postfix[j++] = ' ';
    }
    p->postfix[j] = '\0';

}

int precedence(char operator) {  // value of all operation
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;

        default:
            return -1;
    }
}

ptr expTree(char postfix[]) {  //function to build expression tree
//    //char *postfix = p;
    int top = -1;
    ptr stack[100];//stack of node

    char int_string[100];
    int num = 0;
    for (int i = 0, j = 0; i < strlen(postfix); i++) {
        if (postfix[i] == ' ')
            continue;
        if (postfix[i] >= '0' && postfix[i] <= '9') {
            j = i;
            while (postfix[j] >= '0' && postfix[j] <= '9' && postfix[j] != ' ') {
                num = num * 10 + postfix[j] - '0';
                j++;
            }

            itoa(num, int_string, 10);
            ptr newNode = createNode(int_string);
            stack[++top] = newNode;
            num = 0;
            if (i != j)
                i = j - 1;
        } else {

            char index[2] = "\0";
            index[0] = postfix[i];
            ptr nodeNew = createNode(&index);
            nodeNew->right = stack[top--];
            nodeNew->left = stack[top--];
            stack[++top] = nodeNew;
        }
    }

    return stack[top--];
}

ptr createNode(char val[]) { //function to create node
    ptr newNode = (ptr) malloc(sizeof(struct Node));
    strcpy(newNode->data, val);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int eval(ptr root) { //function to evaluation expression tree
    // empty tree
    position p;
    if (!root)
        return 0;

    // leaf node i.e, an integer
    if (!root->left && !root->right)
        return toInt(root->data);

    // Evaluate left subtree
    int l_val = eval(root->left);

    // Evaluate right subtree
    int r_val = eval(root->right);

    // Check which operator to apply
    // if (root->data=="+")
    if (strcmp(root->data, "+") == 0)
        return l_val + r_val;

    //  if (root->data=="-")
    if (strcmp(root->data, "-") == 0)
        return l_val - r_val;

    //if (root->data=="*")
    if (strcmp(root->data, "*") == 0)
        return l_val * r_val;
    //if(root->data=="%")
    if (strcmp(root->data, "%") == 0)
        return l_val % r_val;

    return l_val / r_val;
}

int toInt(char s[]) {   // function to return the integer value of a given string

    int num = 0;

    // Check if the integral value is
    // negative or not
    // If it is not negative, generate the number
    // normally
    if (s[0] != '-')
        for (int i = 0; i < strlen(s); i++)
            num = num * 10 + s[i] - 48;
        // If it is negative, calculate the +ve number
        // first ignoring the sign and invert the
        // sign at the end
    else {
        for (int i = 1; i < strlen(s); i++)
            num = num * 10 + s[i] - 48;
        num = num * -1;
    }

    return num;
}




