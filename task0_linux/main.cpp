#include <iostream>
#include <cstring>
#include <fstream>
#include <cctype>

using namespace std;

#define NORW    13
#define NMAX    14
#define AL      10

char word[40][12] = {"", "", "", "+", "-", "*", "/", "=", "<>", "<", "<=", ">", ">=",
                     "(", ")", ",", ";", ".", ":=", "begin", "call", "const", "do",
                     "end", "if", "odd", "procedure", "read",
                     "then", "var", "while", "write"};
char symstr[40][12] = {"NUL", "IDENT", "NUMBER", "PLUS", "MINUS", "TIMES", "SLASH", "EQL", "NEQ", "LSS",
                       "LEQ", "GTR", "GEQ", "LPAREN", "RPAREN", "COMMA", "SEMICOLON", "PERIOD", "BECOMES",
                       "BEGINSYM", "CALLSYM", "CONSTSYM", "DOSYM", "ENDSYM", "IFSYM", "ODDSYM", "PROCSYM",
                       "READSYM", "THENSYM", "VARSYM", "WHILESYM", "WRITESYM"};
enum symbol {
    NUL, IDENT, NUMBER, PLUS, MINUS, TIMES, SLASH, EQL, NEQ, LSS,
    LEQ, GTR, GEQ, LPAREN, RPAREN, COMMA, SEMICOLON, PERIOD, BECOMES,
    BEGINSYM, CALLSYM, CONSTSYM, DOSYM, ENDSYM, IFSYM, ODDSYM, PROCSYM,
    READSYM, THENSYM, VARSYM, WHILESYM, WRITESYM
};


symbol sym;
char ch;
int cc = 0;
int num;
int ll;
char a[AL + 1];
char id[AL + 1];
char line[100];
symbol wsym[40];
symbol ssym[255];


void getch(ifstream &in);

void getsym(ifstream &in);

int main() {
//    char filename[100];
//    cout << "please input source program file name : " << endl;
//    cin >> filename;
//    ifstream in(filename);
    ifstream in("../16061040_test.txt");
    if (!in.is_open()) {
        cout << "Error opening file";
        exit(1);
    }

    wsym[19] = BEGINSYM;
    wsym[20] = CALLSYM;
    wsym[21] = CONSTSYM;
    wsym[22] = DOSYM;
    wsym[23] = ENDSYM;
    wsym[24] = IFSYM;
    wsym[25] = ODDSYM;
    wsym[26] = PROCSYM;
    wsym[27] = READSYM;
    wsym[28] = THENSYM;
    wsym[29] = VARSYM;
    wsym[30] = WHILESYM;
    wsym[31] = WRITESYM;

    ssym['+'] = PLUS;
    ssym['-'] = MINUS;
    ssym['*'] = TIMES;
    ssym['/'] = SLASH;
    ssym['('] = LPAREN;
    ssym[')'] = RPAREN;
    ssym['='] = EQL;
    ssym[','] = COMMA;
    ssym['.'] = PERIOD;
    ssym['<'] = LSS;
    ssym['>'] = GTR;
    ssym[';'] = SEMICOLON;


    cc = 0;
    ll = 0;
    ch = ' ';
    int count = 0;
    while (!in.eof()) {
        getsym(in);
        if (sym == IDENT) {
            cout << count << " " << symstr[sym] << " " << id << endl;
        } else if (sym == NUMBER) {
            cout << count << " " << symstr[sym] << " " << num << endl;
        } else {
            cout << count << " " << symstr[sym] << " " << word[sym] << endl;
        }
        count++;
    }

    getsym(in);
    return 0;
}

void getch(ifstream &in) {
    if (cc == ll) {
        if (in.eof()) {
            cout << "program incomplete" << endl;
            exit(1);
        }
        ll = 0;
        cc = 0;
        in.getline(line, 1000);
        ll = (int) strlen(line);
    }

    ch = line[cc];
    cc += 1;
}


void getsym(ifstream &in) {
    int i, j, k;
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        getch(in);
    }
    if (isalpha(ch)) {
        k = 0;
        while (isalnum(ch)) {
            if (k < AL) {
                a[k] = ch;
                k++;
            }
            getch(in);
        }
        a[k] = '\0';
        strcpy(id, a);
        i = 19;
        j = NORW+18;
        do {
            k = (i + j) / 2;
            if (strcmp(id, word[k]) <= 0) {
                j = k - 1;
            }
            if (strcmp(id, word[k]) >= 0) {
                i = k + 1;
            }
        } while (i <= j);
        if (i - 1 > j) {
            sym = wsym[k];
        } else {
            sym = IDENT;
        }
    } else if (isdigit(ch)) {
        k = 0;
        num = 0;
        sym = NUMBER;
        do {
            num = 10 * num + ch - '0';
            k++;
            getch(in);
        } while (isdigit(ch));
        if (k > NMAX) {
            cout << "Integer constant expected";
            exit(1);
        }
    } else if (ch == ':') {
        getch(in);
        if (ch == '=') {
            sym = BECOMES;
            getch(in);
        } else {
            sym = NUL;
        };
    } else if (ch == '<') {
        getch(in);
        if (ch == '=') {
            sym = LEQ;
            getch(in);
        } else if (ch == '>') {
            sym = NEQ;
            getch(in);
        } else {
            sym = LSS;
        }
    } else if (ch == '>') {
        getch(in);
        if (ch == '=') {
            sym = GEQ;
            getch(in);
        } else {
            sym = GTR;
        }
    } else {
        sym = ssym[ch];
        getch(in);
    }
}
