
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 100

void printDiff(const char* str1, const char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    int dp[MAX_LENGTH][MAX_LENGTH];

    // Initialize the dynamic programming table
    for (int i = 0; i <= len1; ++i) {
        for (int j = 0; j <= len2; ++j) {
            if (i == 0)
                dp[i][j] = j;  // If the first string is empty, all characters of second string are added
            else if (j == 0)
                dp[i][j] = i;  // If the second string is empty, all characters of first string are deleted
            else if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];  // Characters are same, no operation required
            else
                dp[i][j] = 1 + (dp[i - 1][j - 1] < dp[i - 1][j] ? (dp[i - 1][j - 1] < dp[i][j - 1] ? dp[i - 1][j - 1] : dp[i][j - 1]) : (dp[i - 1][j] < dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1])); // Minimum of replace, delete, and insert operations
        }
    }

    // Trace back to find the edits
    int i = len1, j = len2;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && str1[i - 1] == str2[j - 1]) {
            printf("Unchanged: %c\n", str1[i - 1]);
            i--;
            j--;
        } else if (j > 0 && (i == 0 || dp[i][j] == dp[i][j - 1] + 1)) {
            printf("Added: %c\n", str2[j - 1]);
            j--;
        } else if (i > 0 && (j == 0 || dp[i][j] == dp[i - 1][j] + 1)) {
            printf("Removed: %c\n", str1[i - 1]);
            i--;
        } else {
            printf("Replaced: %c with %c\n", str1[i - 1], str2[j - 1]);
            i--;
            j--;
        }
    }
}

int main() {
    const char* str1 = "kitten";
    const char* str2 = "onekitentwo";

    printf("String 1: %s\nString 2: %s\n", str1, str2);
    printf("Difference:\n");
    printDiff(str1, str2);

    return 0;
}