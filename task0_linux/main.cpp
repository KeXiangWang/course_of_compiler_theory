#include <iostream>
#include <cstring>
#include <fstream>
#include <cctype>

using namespace std;

#define NORW    13
#define TXMAX   100
#define NMAX    14
#define AL      10
#define AMAX    2047
#define LEVMAX  3
#define CXMAX   200

char word[NORW][AL];
enum symbol {
    nul, ident, number, plus, minus, times, slash, oddsym, eql, neq, lss,
    leq, gtr, geq, lparen, rparen, comma, semicolon, period, becomes,
    beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym,
    varsym, procsym, readsym, writesym
};


symbol sym;
char ch;
int cc = 0;
int num;
int ll;
char line[100];
//ifstream in;


void getch(ifstream &in);

void getsym();

int main() {
//    char filename[100];
//    cout << "please input source program file name : " << endl;
//    cin >> filename;
//    ifstream in(filename);
    ifstream in("/home/dywkx/CLionProjects/course_of_compiler_theory/pl0code.txt");
    if (!in.is_open()) {
        cout << "Error opening file";
        exit(1);
    }
    cc = 0;
    ll = 0;
    ch = ' ';


//    while (!in.eof()) {
//        getch(in);
//        cout << ch ;
//    }

    getsym();
    return 0;
}

void getch(ifstream &in) {
//    cout << "cc: " << cc << " ;ll:" << ll;
    if (cc == ll) {
        if (in.eof()) {
            cout << "program incomplete" << endl;
            exit(1);
        }
        ll = 0;
        cc = 0;
        in.getline(line, 1000);
        cout << line;
        ll = (int)strlen(line);
    }

    ch = line[cc];
    cc += 1;
//    cout << line[cc] ;
}


void getsym() {

}
