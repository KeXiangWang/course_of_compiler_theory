#include <iostream>

using namespace std;

void call_error() {
    cout << "Error Occured!";
}

int parseZ(string word, int n);

int parseA(string word, int n);

int parseB(string word, int n);


int main() {
    string sentence;
    getline(cin, sentence);
    if (parseZ(sentence, 0) < 0) call_error();
    cout << sentence;
    return 0;
}

int parseZ(string word, int n) {
    int i = n;
    int tem;
    if (parseA(word, i) >= 0) {
        i = parseA(word, i);
        if (word[i] == 'c') {
            i++;
        } else {
            return -1;
        }
        if (parseB(word, i) >= 0) {
            i = parseB(word, i);
        } else {
            return -1;
        }

        return i;
    } else if (parseB(word, i) >= 0) {
        i = parseB(word, i);
        if (word[i] == 'd') {
            i++;
        } else {
            return -1;
        }
        return i;
    } else {
        return -1;
    }
};

int parseA(string word, int n) {
    int i = n;
    if (word[i] == 'c') {
        i++;
        while (word[i] == 'a') {
            i++;
            if (parseB(word, i) >= 0) {
                i = parseB(word, i);
            } else {
                return -1;
            }
        }
        return i;
    } else {
        return -1;
    }
};

int parseB(string word, int n) {
    int i = n;
    if (word[i] == 'a') {
        i++;
        if (parseA(word, i) >= 0) {
            i = parseA(word, i);
        }
        return i;
    } else {
        return -1;
    }

};