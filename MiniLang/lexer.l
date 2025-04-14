%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"

void yyerror(const char *s);
extern int interactive_mode;  /* Declare as extern to reference the variable defined in parser.y */
%}

%option noyywrap
%option yylineno

%%

[ \t\r]+           /* Skip whitespace except newlines */
\n                 { if (interactive_mode) { return SEMICOLON; } /* In interactive mode, treat newlines as statement separators */ }
"="                { return ASSIGN; }
";"                { return SEMICOLON; }
"print"            { return PRINT; }
"if"               { return IF; }
"else"             { return ELSE; }
"while"            { return WHILE; }
"for"              { return FOR; }
"min"              { return MIN; }
"max"              { return MAX; }
"+"                { return PLUS; }
"-"                { return MINUS; }
"*"                { return MULTIPLY; }
"/"                { return DIVIDE; }
"=="               { return EQUALS; }
"!="               { return NEQUALS; }
">"                { return GREATER; }
"<"                { return LESS; }
">="               { return GREATEREQ; }
"<="               { return LESSEQ; }
","                { return COMMA; }
"("                { return '('; }
")"                { return ')'; }
"{"                { return '{'; }
"}"                { return '}'; }

[0-9]+             { 
                     yylval.fval = atof(yytext); 
                     return NUMBER; 
                   }

[0-9]+\.[0-9]+     { 
                     yylval.fval = atof(yytext); 
                     return NUMBER; 
                   }

[a-zA-Z][a-zA-Z0-9_]* { 
                     yylval.sval = strdup(yytext);
                     if (yylval.sval == NULL) {
                         yyerror("Memory allocation failed");
                         exit(1);
                     }
                     return IDENTIFIER; 
                   }

\"[^\"]*\"         {
                     if (strlen(yytext) > 2) {
                         /* Remove the quotes and copy the string content */
                         yylval.sval = strdup(yytext + 1);
                         if (yylval.sval == NULL) {
                             yyerror("Memory allocation failed");
                             exit(1);
                         }
                         yylval.sval[strlen(yylval.sval) - 1] = '\0';
                     } else {
                         /* Empty string */
                         yylval.sval = strdup("");
                         if (yylval.sval == NULL) {
                             yyerror("Memory allocation failed");
                             exit(1);
                         }
                     }
                     return STRING;
                   }

<<EOF>>            { return 0; }

.                  { 
                      if (yytext[0] == 4 || yytext[0] == 26) { /* Ctrl+D or Ctrl+Z */
                         return 0; /* Signal end of input */
                      } else {
                         printf("Unrecognized character: %s at line %d\n", yytext, yylineno);
                      }
                   }
%%