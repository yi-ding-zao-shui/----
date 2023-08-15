/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode *stmt_sequence(void);
static TreeNode *statement(void);
static TreeNode *if_stmt(void);
static TreeNode *repeat_stmt(void);
static TreeNode *assign_stmt(void);
static TreeNode *read_stmt(void);
static TreeNode *write_stmt(void);
static TreeNode *exp(void);
static TreeNode *simple_exp(void);
static TreeNode *term(void);
static TreeNode *factor(void);

// Tiny+
static TreeNode *program(void);
static TreeNode *decl(void);
static TreeNode *varlist(void);
static TreeNode *do_while_stmt(void);

static void syntaxError(char *message)
{
  fprintf(listing, "\n>>> ");
  fprintf(listing, "Syntax error at line %d: %s", lineno, message);
  Error = TRUE;
}

static void match(TokenType expected)
{
  if (token == expected)
    token = getToken();
  else
  {
    syntaxError("unexpected token -> ");
    printToken(token, tokenString);
    fprintf(listing, "      ");
  }
}

TreeNode *stmt_sequence(void)
{
  TreeNode *t = statement();
  TreeNode *p = t;
  while ((token != ENDFILE) && (token != END) &&
         (token != ELSE) && (token != UNTIL) && (token != WHILE))
  {
    TreeNode *q;
    match(SEMI);
    q = statement();
    if (q != NULL)
    {
      if (t == NULL)
        t = p = q;
      else /* now p cannot be NULL either */
      {
        p->sibling = q;
        p = q;
      }
    }
  }
  return t;
}

TreeNode *statement(void)
{
  TreeNode *t = NULL;
  switch (token)
  {
  case IF:
    t = if_stmt();
    break;
  case REPEAT:
    t = repeat_stmt();
    break;
  case ID:
    t = assign_stmt();
    break;
  case READ:
    t = read_stmt();
    break;
  case WRITE:
    t = write_stmt();
    break;
  // tiny+
  case DO:
    t = do_while_stmt();
    break;
  default:
    syntaxError("unexpected token1 -> ");
    printToken(token, tokenString);
    token = getToken();
    break;
  } /* end case */
  return t;
}

TreeNode *if_stmt(void)
{
  TreeNode *t = newStmtNode(IfK);
  match(IF);
  if (t != NULL)
    t->child[0] = exp();
  match(THEN);
  if (t != NULL)
    t->child[1] = stmt_sequence();
  if (token == ELSE)
  {
    match(ELSE);
    if (t != NULL)
      t->child[2] = stmt_sequence();
  }
  match(END);
  return t;
}

TreeNode *repeat_stmt(void)
{
  TreeNode *t = newStmtNode(RepeatK);
  match(REPEAT);
  if (t != NULL)
    t->child[0] = stmt_sequence();
  match(UNTIL);
  if (t != NULL)
    t->child[1] = exp();
  return t;
}

TreeNode *assign_stmt(void)
{
  TreeNode *t = newStmtNode(AssignK);
  if ((t != NULL) && (token == ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(token);
  if (t != NULL)
    t->child[0] = exp();
  return t;
}

TreeNode *read_stmt(void)
{
  TreeNode *t = newStmtNode(ReadK);
  match(READ);
  if ((t != NULL) && (token == ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  return t;
}

TreeNode *write_stmt(void)
{
  TreeNode *t = newStmtNode(WriteK);
  match(WRITE);
  if (t != NULL)
    t->child[0] = simple_exp();
  return t;
}

TreeNode *exp(void)
{
  TreeNode *t = simple_exp();
  if ((token == LT) || (token == EQ) || (token == LTE))
  {
    TreeNode *p = newExpNode(OpK);
    if (p != NULL)
    {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
    }
    match(token);
    if (t != NULL)
      t->child[1] = simple_exp();
  }
  return t;
}

TreeNode *simple_exp(void)
{
  TreeNode *t = term();
  while ((token == PLUS) || (token == MINUS))
  {
    TreeNode *p = newExpNode(OpK);

    p->op_char = token == PLUS ? '+' : '-';
    if (p != NULL)
    {
      p->child[0] = t;
      p->attr.op = token;
      // p->attr.name = copyString(tokenString);
      t = p;
      match(token);
      t->child[1] = term();
    }
  }
  return t;
}

TreeNode *term(void)
{
  TreeNode *t = factor();
  while ((token == TIMES) || (token == OVER))
  {
    TreeNode *p = newExpNode(OpK);

    p->op_char = token == TIMES ? '*' : '/';

    if (p != NULL)
    {
      p->child[0] = t;
      p->attr.op = token;
      // p->attr.name = copyString(tokenString);
      t = p;
      match(token);
      p->child[1] = factor();
    }
  }
  return t;
}

TreeNode *factor(void)
{
  TreeNode *t = NULL;
  switch (token)
  {
  case STR:
    t = newExpNode(ConstK);
    if ((t != NULL) && (token == STR))
    {
      t->attr.name = copyString(tokenString);
      t->type = String;
    }
    match(STR);
    break;
  case NUM:
    t = newExpNode(ConstK);
    if ((t != NULL) && (token == NUM))
    {
      t->attr.val = atoi(tokenString);
      t->type = Integer;
    }
    match(NUM);
    break;
  case ID:
    t = newExpNode(IdK);
    if ((t != NULL) && (token == ID))
      t->attr.name = copyString(tokenString);
    match(ID);
    break;
  case LPAREN:
    match(LPAREN);
    t = exp();
    match(RPAREN);
    break;
  default:
    syntaxError("unexpected token -> ");
    printToken(token, tokenString);
    token = getToken();
    break;
  }
  return t;
}

// Tiny+
TreeNode *program(void)
{
  TreeNode *t = newStmtNode(ProgramK);
  int i = 0;
  while (t != NULL && i < MAXCHILDREN && (token == INT || token == BOOL || token == STRING || token == FLOAT || token == DOUBLE) && (token != ENDFILE))
  {
    t->child[i] = decl();
    i++;
    match(SEMI);
  }
  if (t != NULL && i < MAXCHILDREN && (token != ENDFILE))
  {
    t->child[i] = stmt_sequence();
    i++;
  }
  return t;
}
// TreeNode *declarations(void)
// {
//   TreeNode *t = newStmtNode(DeclsK);
//   int i = 0;
//   while (i < MAXCHILDREN && (token == INT || token == BOOL || token == STRING || token == FLOAT || token == DOUBLE))
//   {
//     t->child[i] = decl();
//     i++;
//     match(SEMI);
//   }
//   return t;
// }
TreeNode *decl(void)
{
  TreeNode *t = newStmtNode(DeclK);
  if (token == INT || token == BOOL || token == STRING ||
      token == FLOAT || token == DOUBLE)
  {
    t->attr.name = copyString(tokenString);
    switch (token)
    {
    case INT:
      t->type = Integer;
      break;
    case BOOL:
      t->type = Boolean;
      break;
    case STRING:
      t->type = String;
      break;
    case FLOAT:
      t->type = Float;
      break;
    case DOUBLE:
      t->type = Double;
      break;
    default:
      fprintf(listing, "gg\n");
      break;
    }
    match(token);
  }
  if (t != NULL)
  {
    t->child[0] = varlist();
  }
  return t;
}

// TreeNode* type_specifier(void) {
//   TreeNode* t = newStmtNode(DeclK);
//   if (token == INT || token == BOOL || token == STRING || token == FLOAT || token == DOUBLE) {
//     t->attr.name = copyString(tokenString);
//     switch (token)
//     {
//     case INT:
//       t->type = Integer;
//       break;
//     case BOOL:
//       t->type = Boolean;
//       break;
//     case STRING:
//       t->type = String;
//       break;
//     case FLOAT:
//       t->type = Float;
//       break;
//     case DOUBLE:
//       t->type = Double;
//       break;
//     default:
//       break;
//     }
//     match(token);
//     return t;
//   }
//   return NULL;
// }

TreeNode *varlist(void)
{
  TreeNode *t = newExpNode(IdK);
  TreeNode *p = t;
  t->attr.name = copyString(tokenString);
  match(ID);

  while (strcmp(tokenString, ",") == 0)
  {
    match(COMMA);
    t->child[0] = newExpNode(IdK);
    t = t->child[0];
    t->attr.name = copyString(tokenString);
    match(ID);
  }
  return p;
}

TreeNode *do_while_stmt(void)
{
  TreeNode *t = newStmtNode(WhileK);
  match(DO);
  if (t != NULL)
  {
    t->child[0] = stmt_sequence();
  }
  match(WHILE);
  if (t != NULL)
  {
    t->child[1] = exp();
  }
  return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode *parse(void)
{
  TreeNode *t;
  token = getToken();
  t = stmt_sequence();
  // t = program();
  if (token != ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}
