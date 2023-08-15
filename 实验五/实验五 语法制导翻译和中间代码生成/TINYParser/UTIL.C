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
  case T_TRUE:
  case T_FALSE:
  case OR:
  case AND:
  case NOT:
  case INT:
  case BOOL:
  case STRING:
  case DO:
  case WHILE:
  case FLOAT:
  case DOUBLE:
    fprintf(listing,
            "reserved word: %s\n", tokenString);
    break;
  case ASSIGN:
    fprintf(listing, ":=\n");
    break;

  case LT:
    fprintf(listing, "<\n");
    break;
  case EQ:
    fprintf(listing, "=\n");
    break;
  case GT:
    fprintf(listing, ">\n");
    break;
  case LTE:
    fprintf(listing, "<=\n");
    break;
  case GTE:
    fprintf(listing, ">=\n");
    break;

  case LPAREN:
    fprintf(listing, "(\n");
    break;
  case RPAREN:
    fprintf(listing, ")\n");
    break;
  case SEMI:
    fprintf(listing, ";\n");
    break;
  case COMMA:
    fprintf(listing, ",\n");
    break;
  case SQM:
    fprintf(listing, "\'\n");
    break;
  case PLUS:
    fprintf(listing, "+\n");
    break;
  case MINUS:
    fprintf(listing, "-\n");
    break;
  case TIMES:
    fprintf(listing, "*\n");
    break;
  case OVER:
    fprintf(listing, "/\n");
    break;
  case ENDFILE:
    fprintf(listing, "EOF\n");
    break;
  case NUM:
    fprintf(listing,
            "NUM, val= %s\n", tokenString);
    break;
  case ID:
    fprintf(listing,
            "ID, name= %s\n", tokenString);
    break;
  case STR:
    fprintf(listing, "STR,name= %s\n", tokenString);
    break;
  case ERROR:
  {

    fprintf(listing,
            "ERROR %s :%s\n", errorMsg[errorCode], tokenString);
  }
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

// TreeNode* newVarNode(VarKind kind) {
//   TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
//   int i;
//   if (t == NULL)
//     fprintf(listing, "Out of memory error at line %d\n", lineno);
//   else
//   {
//     for (i = 0; i < MAXCHILDREN; i++)
//       t->child[i] = NULL;
//     t->sibling = NULL;
//     t->nodekind = VarKey;
//     t->kind.var = kind;
//     t->lineno = lineno;
//   }
//   return t;
// }

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
  t = (char *)malloc(n);
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
      case WhileK:
        fprintf(listing, "While\n");
        break;
      case ProgramK:
        fprintf(listing, "Program\n");
        break;
      case DeclK:
        fprintf(listing, "Type: %s\n", tree->attr.name);
        break;
      default:
        fprintf(listing, "Unknown ExpNode kind1\n");
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
        switch (tree->type)
        {
        case Integer:
          fprintf(listing, "Const: Integer: %d\n", tree->attr.val);
          break;
        case String:
          fprintf(listing, "Const: String: %s\n", tree->attr.name);
          break;
        }
        break;
      case IdK:
        fprintf(listing, "Id: %s\n", tree->attr.name);
        break;
      default:
        fprintf(listing, "Unknown ExpNode kind2\n");
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

int isLegalChar(char c)
{

  return (isalnum(c) ||
          isspace(c) ||
          c == '>' ||
          c == '<' ||
          c == '=' ||
          c == ',' ||
          c == ';' ||
          c == '\'' ||
          c == '{' ||
          c == '}' ||
          c == '+' ||
          c == '-' ||
          c == '*' ||
          c == '/' ||
          c == '(' ||
          c == ')');
}

// 实验五
int id;          // 临时变量序号
char id_str[20]; // 临时变量字符串
void generateMiddleCode(TreeNode *tree)
{
  printf("\nSyntax-directed translation and intermediate code generation:\n");
  while (tree != NULL)
  {
    // 只有检查到赋值语句时才进行语法制导翻译与中间代码生成
    if (tree->nodekind == StmtK && tree->kind.stmt == AssignK)
    {
      printf("-----------------------\n");
      id = 1;
      printThreeAddressCode(tree->child[0]);
      fprintf(listing, "%s := %s\n", tree->attr.name, tree->child[0]->temp_id);
    }
    tree = tree->sibling;
  }
}

void printThreeAddressCode(TreeNode *tree)
{
  if (tree->nodekind == ExpK)
  {
    sprintf(id_str, "t%d", id++);
    tree->temp_id = copyString(id_str);
    if (tree->kind.exp == OpK)
    {
      if (tree->child[0]->kind.exp == IdK && tree->child[1]->kind.exp == IdK)
      {
        // 左边为ID，右边为ID
        fprintf(listing, "%s := %s %c %s\n", tree->temp_id, tree->child[0]->attr.name, 
          tree->op_char, tree->child[1]->attr.name);
      }
      else if (tree->child[0]->kind.exp == IdK && tree->child[1]->kind.exp == ConstK)
      {
        // 左边为ID，右边为常量
        if (tree->child[1]->type == Integer)
        {
          fprintf(listing, "%s := %s %c %d\n", tree->temp_id, tree->child[0]->attr.name, 
            tree->op_char, tree->child[1]->attr.val);
        }
        else
        {
          fprintf(listing, "%s := %s %c %s\n", tree->temp_id, tree->child[0]->attr.name, 
            tree->op_char, tree->child[1]->attr.name);
        }
      }
      else if (tree->child[0]->kind.exp == ConstK && tree->child[1]->kind.exp == IdK)
      {
        // 左边为常量，右边为ID
        if (tree->child[0]->type == Integer)
        {
          fprintf(listing, "%s := %d %c %s\n", tree->temp_id, tree->child[0]->attr.val, 
            tree->op_char, tree->child[1]->attr.name);
        }
        else
        {
          fprintf(listing, "%s := %s %c %s\n", tree->temp_id, tree->child[0]->attr.name,
            tree->op_char, tree->child[1]->attr.name);
        }
      }
      else if (tree->child[0]->kind.exp == ConstK && tree->child[1]->kind.exp == ConstK)
      {
        // 左边为常量，右边为常量
        if (tree->child[0]->type != tree->child[1]->type) {
          fprintf(listing, "error! type mismatch\n");
          return;
        }

        if (tree->child[0]->type == Integer)
        {
          fprintf(listing, "%s := %d %c %d\n", tree->temp_id, tree->child[0]->attr.val, 
            tree->op_char, tree->child[1]->attr.val);
        }
        else
        {
          fprintf(listing, "%s := %s %c %s\n", tree->temp_id, tree->child[0]->attr.name, 
            tree->op_char, tree->child[1]->attr.name);
        }
      }
      else if (tree->child[0]->kind.exp == IdK)
      {
        // 只有左边一个ID
        printThreeAddressCode(tree->child[1]);
        fprintf(listing, "%s := %s %c %s\n", tree->temp_id, tree->child[0]->attr.name, 
          tree->op_char, tree->child[1]->temp_id);
      }
      else if (tree->child[1]->kind.exp == IdK)
      {
        // 只有右边一个ID
        printThreeAddressCode(tree->child[0]);
        fprintf(listing, "%s := %s %c %s\n", tree->temp_id, tree->child[0]->temp_id, 
          tree->op_char, tree->child[1]->attr.name);
      }
      else if (tree->child[0]->kind.exp == ConstK)
      {
        // 只有左边一个常量
        printThreeAddressCode(tree->child[1]);
        if (tree->child[0]->type == Integer)
        {
          fprintf(listing, "%s := %d %c %s\n", tree->temp_id, tree->child[0]->attr.val, 
            tree->op_char, tree->child[1]->temp_id);
        }
        else
        {
          fprintf(listing, "%s := %d %c %s\n", tree->temp_id, tree->child[0]->attr.name, 
            tree->op_char, tree->child[1]->temp_id);
        }
      }
      else if (tree->child[1]->kind.exp == ConstK)
      {
        // 只有右边一个常量
        printThreeAddressCode(tree->child[0]);
        if (tree->child[1]->type == Integer)
        {
          fprintf(listing, "%s := %s %c %d\n", tree->temp_id, tree->child[0]->temp_id, 
            tree->op_char, tree->child[1]->attr.val);
        }
        else
        {
          fprintf(listing, "%s := %s %c %s\n", tree->temp_id, tree->child[0]->temp_id, 
            tree->op_char, tree->child[1]->attr.name);
        }
      }
      else
      {
        // 两边均需要进行分解
        printThreeAddressCode(tree->child[0]);
        printThreeAddressCode(tree->child[1]);
        fprintf(listing, "%s := %s %c %s\n", tree->temp_id, tree->child[0]->temp_id, 
          tree->op_char, tree->child[1]->temp_id);
      }
    }
    else if (tree->kind.exp == IdK || (tree->kind.exp == ConstK && tree->type == String))
    {
      fprintf(listing, "%s := %s\n", tree->temp_id, tree->attr.name);
    }
    else if (tree->kind.exp == ConstK && tree->type == Integer)
    {
      fprintf(listing, "%s := %d\n", tree->temp_id, tree->attr.val);
    }
  }
}
