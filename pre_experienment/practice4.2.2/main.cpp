#include <iostream>
#include <cstring>

using namespace std;

#define MAX 1024

void call_error(int i) {
    cout << "Error Occur at position " << i << endl;
    exit(1);
};

void parse(string word) {
//    cout << strlen(word);
    int i = 0;
    if (word[i] == '(') {
        i++;
        if (word[i] != 'c') call_error(i);
        while (word[i] == 'c' && i < word.size()) i++;
        if (word[i] != ')') call_error(i);
        i++;
        if (i != word.size()) call_error(i);
    } else if (word[i] == 'd') {
        i++;
        if (word[i] != 'c') call_error(i);
        while (word[i] == 'c' && i < word.size()) i++;
        if (word[i] != 'e') call_error(i);
        i++;
        if (i != word.size()) call_error(i);
    } else {
        call_error(i);
    }
}

int main() {
    string sentence;
    getline(cin, sentence);
    parse(sentence);

    cout << sentence;
    return 0;
}