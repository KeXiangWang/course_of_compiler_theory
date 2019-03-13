const int alphabet_length = 26;
const char alpha = 'a';

int scale;
char start_char, end_char;
int arrangement[10];
char alphabet[26];

void swap_arrangement(int index1, int index2) {
    int int_temp;
    int_temp = arrangement[index1];
    arrangement[index1] = arrangement[index2];
    arrangement[index2] = int_temp;
}

char print_chars() {
    int start_index;
    int end_index;
    int i;
    start_index = start_char - alpha;
    end_index = end_char - alpha;
    i = start_index;
    do {
        printf(alphabet[i]);
        i = i + 1;
    } while (i <= end_index);
    printf("New Line.");
    return ((end_index - start_index) + alpha);
}

void full_arrange(int current_level) {
    int i;
    if (current_level == scale - 1) {
        for (i = 0; i < scale; i = i + 1) {
            printf(arrangement[i]);
        }
        printf("New Line.");
    } else {
        for (i = current_level; i < scale; i = i + 1) {
            swap_arrangement(i, current_level);
            full_arrange(current_level + 1);
            swap_arrangement(i, current_level);
        }
    }
}

int calc(int op, int a, char c) {
    if (op > 3) {
        return (a + c / a * (a - c) * (c + a) - ((a / (c - 2 * a))));
    } else {
        if (op >= 1) {
            return (3 * 6 * c / a - a - c);
        } else {
            if (op < -4) {
                return (a * a * c * 7);
            } else {
                if (op <= -2) {
                    return ((((((a))))) / (c + 1));
                } else {
                    if (op != 0) {
                        return (((a)));
                    } else {
                        return (c);
                    }
                }
            }
        }
    }
}

void main() {
    int i;
    int result;
    int op;
    scanf(scale);
    for (i = 0; i < scale; i = i + 1) {
        arrangement[i] = i + 1;
    }
    full_arrange(0);
    for (i = 0; i < alphabet_length; i = i + 1) {
        alphabet[i] = alpha + i;
    }
    scanf(start_char,end_char);
    result = print_chars();
    printf(result);
    printf("New Line.", end_char - start_char);
    do {
        scanf(op);
        result = calc(op, alphabet_length, alpha);
        printf("Result is ", result);
        printf(result);
    } while (op != 0)
    return;
} 
