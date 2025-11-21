#include <iostream>
#include <fstream>
using namespace std;

bool isOperator(const char& ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

char* fromIntToRoman(int num) {
    // Allocate a static buffer
    static char res[64];
    int pos = 0;
    while (num > 0) {
        if (num >= 1000) {
            res[pos++] = 'M';
            num -= 1000;
        }
        else if (num >= 900) {
            res[pos++] = 'C';
            res[pos++] = 'M';
            num -= 900;
        }
        else if (num >= 500) {
            res[pos++] = 'D';
            num -= 500;
        }
        else if (num >= 400) {
            res[pos++] = 'C';
            res[pos++] = 'D';
            num -= 400;
        }
        else if (num >= 100) {
            res[pos++] = 'C';
            num -= 100;
        }
        else if (num >= 90) {
            res[pos++] = 'X';
            res[pos++] = 'C';
            num -= 90;
        }
        else if (num >= 50) {
            res[pos++] = 'L';
            num -= 50;
        }
        else if (num >= 40) {
            res[pos++] = 'X';
            res[pos++] = 'L';
            num -= 40;
        }
        else if (num >= 10) {
            res[pos++] = 'X';
            num -= 10;
        }
        else if (num == 9) {
            res[pos++] = 'I';
            res[pos++] = 'X';
            num -= 9;
        }
        else if (num >= 5) {
            res[pos++] = 'V';
            num -= 5;
        }
        else if (num == 4) {
            res[pos++] = 'I';
            res[pos++] = 'V';
            num -= 4;
        }
        else {
            res[pos++] = 'I';
            num -= 1;
        }
    }
    res[pos] = '\0';
    return res;
}

int fromRomanToInt(const char* num) {
    int res = 0;
    for (int i = 0; num[i] != '\0'; ++i) {
        if (num[i] == 'I') {
            if (num[i + 1] != '\0' && (num[i + 1] == 'V' || num[i + 1] == 'X')) {
                res -= 1;
            }
            else {
                res += 1;
            }
        }
        else if (num[i] == 'V') {
            res += 5;
        }
        else if (num[i] == 'X') {
            if (num[i + 1] != '\0' && (num[i + 1] == 'L' || num[i + 1] == 'C')) {
                res -= 10;
            }
            else {
                res += 10;
            }
        }
        else if (num[i] == 'L') {
            res += 50;
        }
        else if (num[i] == 'C') {
            if (num[i + 1] != '\0' && (num[i + 1] == 'D' || num[i + 1] == 'M')) {
                res -= 100;
            }
            else {
                res += 100;
            }
        }
        else if (num[i] == 'D') {
            res += 500;
        }
        else if (num[i] == 'M') {
            res += 1000;
        }
    }
    return res;
}

int operation(int num1, int num2, char ch) {
    if (ch == '+') {
        return num2 + num1;
    }
    if (ch == '-') {
        return num2 - num1;
    }
    if (ch == '*') {
        return num2 * num1;
    }
    return num2 / num1;
}

int calculations(const char* line) {
    int stack[100];
    int index = 0;
    char romanNum[64] = "";
    int romanIndex = 0;
    bool op = false;
    for (int i = 0; line[i] != '\0'; ++i) {
        char ch = line[i];
        if (isOperator(ch)) {
            int num1 = stack[--index];
            int num2 = stack[--index];
            stack[index++] = operation(num1, num2, ch);
            if (line[i + 2] != '\0' && isOperator(line[i + 2])) {
                op = true;
            }
        }
        else if (ch == ' ') {
            if (op) {
                op = false;
                continue;
            }
            if (romanIndex != 0) {
                stack[index++] = fromRomanToInt(romanNum);
                romanIndex = 0;
                romanNum[0] = '\0';
            }
        }
        else {
            romanNum[romanIndex++] = ch;
            romanNum[romanIndex] = '\0';
        }
    }
    return stack[0];
}

int main(int argc, char* argv[]) {
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);

    int ch;
    char line[1024];  // Array
    int lineIndex = 0;
    bool firstLineCheck = true;
    while ((ch = inFile.get()) != EOF) {
        if (ch == '\n') {
            line[lineIndex] = '\0';
            if (firstLineCheck) {
                firstLineCheck = false;
            }
            else {
                outFile.put('\n');
            }
            outFile << line << " = ";
            int res = calculations(line);
            if (res < 1 || res > 3999) {
                outFile << "ERR";
            }
            else {
                outFile << fromIntToRoman(res);
            }
            lineIndex = 0;
            continue;
        }
        line[lineIndex++] = char(ch);
    }

    outFile.put('\n');

    // Last line if not empty
    if (lineIndex != 0) {
        line[lineIndex] = '\0';
        outFile << line << " = ";
        int res = calculations(line);
        if (res < 1 || res > 3999) {
            outFile << "ERR";
        }
        else {
            outFile << fromIntToRoman(res);
        }
    }

    inFile.close();
    outFile.close();

    return 0;
}
