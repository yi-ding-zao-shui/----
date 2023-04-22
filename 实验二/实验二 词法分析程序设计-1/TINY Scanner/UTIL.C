/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char *tokenString)
{
  switch (token)
  {
  case IF:
  case THEN:
  case ELSE:
  case END:
  case REPEAT:
  case UNTIL:
  case READ:
  case WRITE:
  // Tiny+
  case True:
  case False:
  case OR:
  case AND:
  case NOT:
  case BOOL:
  case KEY_STRING:
  case INT:
  case FLOAT:
  case DOUBLE:
  case DO:
  case WHILE:
  case INCLUDE:
  case BREAK:
  case CONTINUE:
    fprintf(listing,
            "(KEY, %s)\n", tokenString);
    break;
  case ASSIGN:
    fprintf(listing, "(SYM, :=)\n");
    break;
  case LT:
    fprintf(listing, "(SYM, <)\n");
    break;
  case EQ:
    fprintf(listing, "(SYM, =)\n");
    break;
  case LPAREN:
    fprintf(listing, "(SYM, ()\n");
    break;
  case RPAREN:
    fprintf(listing, "(SYM, ))\n");
    break;
  case SEMI:
    fprintf(listing, "(SYM, ;)\n");
    break;
  case PLUS:
    fprintf(listing, "(SYM, +)\n");
    break;
  case MINUS:
    fprintf(listing, "(SYM, -)\n");
    break;
  case TIMES:
    fprintf(listing, "(SYM, *)\n");
    break;
  case OVER:
    fprintf(listing, "(SYM, /))\n");
    break;
  case ENDFILE:
    fprintf(listing, "EOF\n");
    break;
  case NUM:
    fprintf(listing,
            "(NUM, %s)\n", tokenString);
    break;
  case ID:
    fprintf(listing,
            "(ID, %s)\n", tokenString);
    break;
  case ERROR:
    fprintf(listing,
            "ERROR: %s\n", tokenString);
    break;
  // Tiny+
  case GT:
    fprintf(listing, "(SYM, >)\n");
    break;
  case LET:
    fprintf(listing, "(SYM, <=)\n");
    break;
  case GET:
    fprintf(listing, "(SYM, >=)\n");
    break;
  case COMMA:
    fprintf(listing, "(SYM, ,)\n");
    break;
  case SQM:
    fprintf(listing, "(SYM, ')\n");
    break;
  case PERCENT:
    fprintf(listing, "(SYM, %%)\n");
    break;
  case POWER:
    fprintf(listing, "(SYM, **)\n");
    break;
  case STRING:
    fprintf(listing, "(STR, %s)\n", tokenString);
    break;
  case ERROR_INVAILD_LETTER:
    fprintf(listing, "ERROR: Found invalid letter '%s'\n", tokenString);
    break;
  case ERROR_STRING_SINGLE_MISSING:
    fprintf(listing, "ERROR: Single quotes missing\n");
    break;
  case ERROR_COMMENT_LEFT_MISSING:
    fprintf(listing, "ERROR: Comment left brace missing\n");
    break;
  case ERROR_COMMENT_RIGHT_MISSING:
    fprintf(listing, "ERROR: Comment right brace missing\n");
    break;
  case ERROR_ASSIGN_SYMBOL:
    fprintf(listing, "ERROR: The assign symbol are not complete\n");
    break;
  case ERROR_ID_DEFINE:
    fprintf(listing, "EROOR: Incorrect definition of identifier '%s'\n", tokenString);
    break;
  case ERROR_COMMENT_NEST:
    fprintf(listing, "EROOR: Comments canned not nested\n");
    break;
  default: /* should never happen */
    fprintf(listing, "Unknown token: %d\n", token);
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode *newStmtNode(StmtKind kind)
{
  TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
  int i;
  if (t == NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else
  {
    for (i = 0; i < MAXCHILDREN; i++)
      t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode *newExpNode(ExpKind kind)
{
  TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
  int i;
  if (t == NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else
  {
    for (i = 0; i < MAXCHILDREN; i++)
      t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char *copyString(char *s)
{
  int n;
  char *t;
  if (s == NULL)
    return NULL;
  n = strlen(s) + 1;
  t =(char*) malloc(n);
  if (t == NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else
    strcpy(t, s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno += 2
#define UNINDENT indentno -= 2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
  int i;
  for (i = 0; i < indentno; i++)
    fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode *tree)
{
  int i;
  INDENT;
  while (tree != NULL)
  {
    printSpaces();
    if (tree->nodekind == StmtK)
    {
      switch (tree->kind.stmt)
      {
      case IfK:
        fprintf(listing, "If\n");
        break;
      case RepeatK:
        fprintf(listing, "Repeat\n");
        break;
      case AssignK:
        fprintf(listing, "Assign to: %s\n", tree->attr.name);
        break;
      case ReadK:
        fprintf(listing, "Read: %s\n", tree->attr.name);
        break;
      case WriteK:
        fprintf(listing, "Write\n");
        break;
      default:
        fprintf(listing, "Unknown ExpNode kind\n");
        break;
      }
    }
    else if (tree->nodekind == ExpK)
    {
      switch (tree->kind.exp)
      {
      case OpK:
        fprintf(listing, "Op: ");
        printToken(tree->attr.op, "\0");
        break;
      case ConstK:
        fprintf(listing, "Const: %d\n", tree->attr.val);
        break;
      case IdK:
        fprintf(listing, "Id: %s\n", tree->attr.name);
        break;
      default:
        fprintf(listing, "Unknown ExpNode kind\n");
        break;
      }
    }
    else
      fprintf(listing, "Unknown node kind\n");
    for (i = 0; i < MAXCHILDREN; i++)
      printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
