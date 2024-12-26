#include <stdio.h> // For standard input and output operations
#include <stdlib.h> // For memory allocation and general utilities
#include <string.h> // For string manipulation functions

// Function declarations
int convertRomanToDecimal(const char* roman); // Converts a Roman numeral to its decimal equivalent
void convertNumberToWords(int num, char* output); // Converts a number to its word representation
void handleFileProcessing(const char* inputFilePath, const char* outputFilePath); // Handles file I/O and processing
void loadInputFile(const char* inputFilePath, char* lines[]); // Reads lines from an input file
void saveOutputFile(const char* outputFilePath, char* results[]); // Writes results to an output file
void appendString(char* destination, const char* source); // Appends one string to another
void copyString(char* destination, const char* source); // Copies one string to another
char* tokenizeString(char* str, const char* delimiter); // Splits a string into tokens based on a delimiter
char* duplicateString(const char* str); // Creates a duplicate of a string
int validateRomanNumeral(const char* roman); // Validates a Roman numeral string

// Number-to-word mappings
const char* units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"}; // Units place words
const char* teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"}; // Teens words
const char* tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"}; // Tens place words
const char* largeNumbers[] = {"", "Thousand", "Million", "Billion"}; // Large number words

// Main function
int main() {
    handleFileProcessing("Input.txt", "Output.txt"); // Calls the function to process the input file and save results
    printf("Check Output.txt");
    return 0; // Exit the program
}

// Converts a Roman numeral string to a decimal integer
int convertRomanToDecimal(const char* roman) {
    int value = 0; // Initialize the decimal value to zero
    for (int i = 0; roman[i] != '\0'; ++i) { // Loop through each character in the Roman numeral string
        switch (roman[i]) {
            case 'M': value += 1000; break; // Add 1000 for 'M'
            case 'D': value += 500; break; // Add 500 for 'D'
            case 'C': value += (roman[i+1] == 'D' || roman[i+1] == 'M') ? -100 : 100; break; // Handle subtraction cases for 'C'
            case 'L': value += 50; break; // Add 50 for 'L'
            case 'X': value += (roman[i+1] == 'L' || roman[i+1] == 'C') ? -10 : 10; break; // Handle subtraction cases for 'X'
            case 'V': value += 5; break; // Add 5 for 'V'
            case 'I': value += (roman[i+1] == 'V' || roman[i+1] == 'X') ? -1 : 1; break; // Handle subtraction cases for 'I'
            default: return -1; // Return -1 for invalid characters
        }
    }
    return value; // Return the computed decimal value
}

// Appends one string to another
void appendString(char* destination, const char* source) {
    while (*destination) destination++; // Move to the end of the destination string
    while (*source) *destination++ = *source++; // Copy each character from the source string
    *destination = '\0'; // Null-terminate the resulting string
}

// Copies one string to another
void copyString(char* destination, const char* source) {
    while ((*destination++ = *source++)); // Copy each character including the null terminator
}

// Splits a string into tokens based on a delimiter
char* tokenizeString(char* str, const char* delimiter) {
    static char* remaining; // Static variable to store the remaining string
    if (str) remaining = str; // If a new string is provided, set it as the remaining string
    if (!remaining) return NULL; // Return NULL if no more tokens are available

    char* tokenStart = remaining; // Mark the start of the token
    while (*remaining && !strchr(delimiter, *remaining)) remaining++; // Find the delimiter
    if (*remaining) *remaining++ = '\0'; // Null-terminate the token and move to the next part
    else remaining = NULL; // Mark end of string

    return tokenStart; // Return the extracted token
}

// Creates a duplicate of a string
char* duplicateString(const char* str) {
    size_t length = strlen(str) + 1; // Calculate the length of the string including null terminator
    char* duplicate = (char*)malloc(length); // Allocate memory for the duplicate
    if (duplicate) strcpy(duplicate, str); // Copy the string if memory allocation succeeds
    return duplicate; // Return the duplicate string
}

// Validates a Roman numeral string
int validateRomanNumeral(const char* roman) {
    while (*roman) {
        if (strchr("IVXLCDM", *roman) == NULL) return 0; // Return 0 if an invalid character is found
        roman++; // Move to the next character
    }
    return 1; // Return 1 if all characters are valid
}

// Converts a number to its word representation
void convertNumberToWords(int num, char* output) {
    output[0] = '\0'; // Initialize the output string
    if (num == 0) {
        copyString(output, "Zero"); // Handle the special case for zero
        return;
    }

    char temp[1024] = ""; // Temporary buffer for constructing the output
    if (num < 0) {
        appendString(output, "Negative "); // Handle negative numbers
        num = -num; // Convert to positive for further processing
    }

    int position = 0; // Track the position for large numbers
    while (num > 0) {
        if (num % 1000 != 0) { // Process each group of three digits
            char groupWords[128] = ""; // Buffer for the current group
            int group = num % 1000; // Extract the current group

            if (group >= 100) { // Handle hundreds place
                appendString(groupWords, units[group / 100]);
                appendString(groupWords, " Hundred ");
                group %= 100;
            }

            if (group >= 20) { // Handle tens place for numbers 20 and above
                appendString(groupWords, tens[group / 10]);
                appendString(groupWords, " ");
                group %= 10;
            } else if (group >= 10) { // Handle teens
                appendString(groupWords, teens[group - 10]);
                appendString(groupWords, " ");
                group = 0;
            }

            appendString(groupWords, units[group]); // Handle units place
            appendString(groupWords, " ");
            appendString(groupWords, largeNumbers[position]); // Add the large number suffix
            appendString(groupWords, " ");
            appendString(groupWords, temp); // Append to the temporary buffer
            copyString(temp, groupWords); // Update the temporary buffer
        }
        num /= 1000; // Move to the next group
        position++; // Increment the position
    }

    appendString(output, temp); // Copy the final result to the output
}

// Handles the processing of the input and output files
void handleFileProcessing(const char* inputFilePath, const char* outputFilePath) {
    char* inputLines[100] = {NULL}; // Array to store input lines
    char* outputResults[100] = {NULL}; // Array to store output results

    loadInputFile(inputFilePath, inputLines); // Load lines from the input file

    for (int i = 0; inputLines[i] != NULL; ++i) { // Process each input line
        char* lineCopy = duplicateString(inputLines[i]); // Create a duplicate of the line
        char* roman1 = tokenizeString(lineCopy, " "); // Extract the first Roman numeral
        char* operatorSymbol = tokenizeString(NULL, " "); // Extract the operator
        char* roman2 = tokenizeString(NULL, " "); // Extract the second Roman numeral

        if (!roman1 || !operatorSymbol || !roman2 ||
            !validateRomanNumeral(roman1) || !validateRomanNumeral(roman2)) { // Validate input
            outputResults[i] = duplicateString("Invalid input"); // Handle invalid input
            free(lineCopy); // Free allocated memory
            continue; // Skip to the next line
        }

        int value1 = convertRomanToDecimal(roman1); // Convert the first Roman numeral to decimal
        int value2 = convertRomanToDecimal(roman2); // Convert the second Roman numeral to decimal
        int resultValue;

        if (strcmp(operatorSymbol, "+") == 0) resultValue = value1 + value2; // Handle addition
        else if (strcmp(operatorSymbol, "-") == 0) resultValue = value1 - value2; // Handle subtraction
        else if (strcmp(operatorSymbol, "*") == 0) resultValue = value1 * value2; // Handle multiplication
        else if (strcmp(operatorSymbol, "/") == 0) resultValue = value2 != 0 ? value1 / value2 : 0; // Handle division
        else {
            outputResults[i] = duplicateString("Invalid operator"); // Handle invalid operator
            free(lineCopy); // Free allocated memory
            continue; // Skip to the next line
        }

        char resultWords[1024] = ""; // Buffer for the result in words
        convertNumberToWords(resultValue, resultWords); // Convert the result to words
        outputResults[i] = duplicateString(resultWords); // Store the result

        free(lineCopy); // Free allocated memory
    }

    saveOutputFile(outputFilePath, outputResults); // Save the results to the output file

    for (int i = 0; inputLines[i] != NULL; ++i) free(inputLines[i]); // Free input line memory
    for (int i = 0; outputResults[i] != NULL; ++i) free(outputResults[i]); // Free output result memory
}

// Reads lines from an input file
void loadInputFile(const char* inputFilePath, char* lines[]) {
    FILE* file = fopen(inputFilePath, "r"); // Open the file for reading
    if (!file) { // Check if the file was opened successfully
        printf("Error: Unable to open file %s\n", inputFilePath); // Print an error message
        return; // Exit the function
    }

    char buffer[256]; // Buffer for reading lines
    int lineIndex = 0; // Index for storing lines
    while (fgets(buffer, sizeof(buffer), file)) { // Read each line
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove the trailing newline character
        lines[lineIndex++] = duplicateString(buffer); // Store the line
    }
    lines[lineIndex] = NULL; // Mark the end of the lines array
    fclose(file); // Close the file
}

// Writes results to an output file
void saveOutputFile(const char* outputFilePath, char* results[]) {
    FILE* file = fopen(outputFilePath, "w"); // Open the file for writing
    if (!file) { // Check if the file was opened successfully
        printf("Error: Unable to open file %s\n", outputFilePath); // Print an error message
        return; // Exit the function
    }

    for (int i = 0; results[i] != NULL; ++i) { // Write each result to the file
        fprintf(file, "%s\n", results[i]); // Write the result followed by a newline
    }
    fclose(file); // Close the file
}
