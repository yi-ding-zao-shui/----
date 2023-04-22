/****************************************************/
/* File: main.c                                     */
/* Main program for TINY compiler                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "GLOBALS.H"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
/* 将NO_PARSE设置为TRUE，表示以获得一个仅扫描的编译器 */
#define NO_PARSE TRUE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
/* 将NO_ANALYZE设置为TRUE，表示以获得一个仅分析的编译器 */
#define NO_ANALYZE TRUE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
/* 将NO_CODE设置为TRUE，表示以获得一个不生成代码的编译器 */
#define NO_CODE FALSE

#include "UTIL.H"
#include "SCAN.H"

/* allocate global variables */
/* 分配全局变量 */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

/* allocate and set tracing flags */
/* 分配和设置跟踪标志 */
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;

int main(int argc, char *argv[])
{
  TreeNode *syntaxTree;
  char pgm[120]; /* source code file name */
  /* 要求执行必须为2个参数，第2个参数为源代码文件名 */
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    exit(1);
  }
  // 拷贝源代码文件名或路径
  strcpy(pgm, argv[1]);
  // 如果源代码文件名无后缀，则加上后缀.tny
  if (strchr(pgm, '.') == NULL)
    strcat(pgm, ".tny");

  /* 以只读方式打开文件 */
  source = fopen(pgm, "r");
  if (source == NULL)
  {
    fprintf(stderr, "File %s not found\n", pgm);
    exit(1);
  }

  listing = stdout; /* send listing to screen */
  fprintf(listing, "\nTINY COMPILATION: %s\n\n", pgm);

  /* 不断地查找下一个语法单元，直到文件末尾 */
  while (getToken() != ENDFILE)
    ;

  fclose(source);
  system("pause");
  return 0;
}
