#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <float.h>

// splits a string of 1s and 0s into sections of 4
// string of 1s and 0s and length of resulting string from split passed in
char *generalNibbleSplitter(const char *result, long length) {
    char *splitResult = malloc(length+1);
    // set null terminator for the resulting string
    splitResult[length] = '\0';
    splitResult[0] = result[0];
    // count increments through each element in the original string
    int count = 1;
    // current val stores the index of the current item in the original string
    int currentVal = 1;
    // position stores where a space is required
    int position = 4;
    // spacing stores after how many points a space is required
    int spacing = 5;
    while (count < length) {
        // if count is position we need a space
        if (count == position) {
            // position is updated to point to next point where a space will be required
            position = position + spacing;
            splitResult[count] = ' ';
        }
        else {
            // if count is not position then store the current 0 or 1 of result at currentVal into split result
            splitResult[count] = result[currentVal];
            currentVal = currentVal + 1;
        }
        count = count + 1;

    }
    // split result is returned
    // note it is freed in main after being printed
    return splitResult;
}

// finds length of split string and passes into a general nibble splitter function
// takes original string of 1s and 0s as parameters
char *nibbleSplitterChar(const char *result) {
    char numberRequired = 9;
    char *finalResult = generalNibbleSplitter(result, numberRequired);
    return finalResult;
}

// does same as nibbleSplitterChar but sets a different length as this is for 64 1s and 0s
char *nibbleSplitterLong(const char *result) {
    long bitsRequired = (sizeof(result)*8) + (sizeof(result)*2) + 1;
    char *finalResult = generalNibbleSplitter(result, bitsRequired);
    // need to add extra null terminators due to way bitsRequired was defined
    finalResult[bitsRequired-1] = '\0';
    finalResult[bitsRequired-2] = '\0';
    return finalResult;
}

// same as above but for a different length of 32 1s and 0s
char *nibbleSplitterInt(const char *result) {
    long bitsRequired = (sizeof(result)*4)+(sizeof(result)-2)+1;
    char *splitResult = generalNibbleSplitter(result, bitsRequired);
    return splitResult;
}

// main function which converts a numerical value into its binary equivalent for all types except doubles
// binArr stores powers of 2^n where n is index of array in decreasing order
// result stores the final result, max is the size of the type, usersDigit is what numerical value needs to be changed
// type also passed in
char *conversionScratch(const long *binArr, char *result, int max, long usersDigit, char type[]) {
    // tempValue stores a temp value until it is equivalent to usersDigit
    long tempValue = 0;
    int count = 0;
    // if the users digit is negative then the first character in the strings of 1s and 0s is a 1
    // tempValue is updated to be this negative value
    if ((usersDigit < 0)) {
        result[count] = '1';
        tempValue = tempValue + binArr[count];
    }
    else {
        // if the users digit is positive and of type unsigned
        if ((usersDigit > 0) && ((strcmp(type, "char unsigned") == 0) || (strcmp(type, "int unsigned")) == 0)) {
            // if the current binArr value is less than or equal to the users digit
            // it would be added on to form the users digit so results at this position is set to 1
            // tempValue is updated accordingly
            if (binArr[count] <= usersDigit) {
                result[count] = '1';
                tempValue = tempValue + binArr[count];
            }
        }
    }
    // the first position has been sorted based on whether the value is negative, positive and unsigned
    // so count is incremented
    count = count + 1;
    // we keep adding 1s or 0s until temp value does not become users digit
    while ((tempValue < usersDigit)) {
        // if the current binArr value is less than or equal to users digit
        // and if users digit is greater than 0
        // and if adding tempValue to the binArr value would still result is a value smaller than usersDigit
        // this value will be added on
        if ((binArr[count] <= usersDigit) && (usersDigit > 0) && ((tempValue+binArr[count]) <=usersDigit)) {
            // appropriate variables updated accordingly
            result[count] = '1';
            tempValue = tempValue + binArr[count];
        }
            // but if the current binValue is larger than users digit and users digit is less than 0
            // this is also a potential value which can be added on
        else if ((binArr[count] >= usersDigit) && (usersDigit < 0)) {
            // if adding on tempvalue to this current binArr value results in something smaller than the users digit
            // it is a valid value to be added on and relevant variables updated accordingly
            if (tempValue + binArr[count] <= usersDigit) {
                tempValue = tempValue + binArr[count];
                result[count] = '1';
            }
            else {result[count] = '0';}
        }
        else {result[count] = '0';}
        count = count + 1;
    }
    // create an array to store the final result
    // note this is freed in conversionMain
    char * newResult = malloc(((sizeof(long) * 8))+1);
    newResult[sizeof(int)*8] = '\0';
    strcpy(newResult, result);
    return newResult;
}

// sets required binary array and result array of specific size to be passed into other functions
char *convertingLong(char usersInputChar[], char type[]) {
    // stores binary array to be passed into functions
    long *mainBinArr;
    char *usersInputAsChar;
    // converts the users input from a string to a long
    long usersInput = strtol(usersInputChar, &usersInputAsChar, 10);
    int max = sizeof(long)*8;
    // stores the result to be passed into functions
    char *mainResult = malloc(((sizeof(long)*8)+1));
    // stores result to be tampered with within this function
    char results[(sizeof(long)*8)+1];
    results[(sizeof(long)*8)] = '\0';
    // stores binArr to be tampered with within this function
    long binArr[(sizeof(long)*8)+1];
    binArr[sizeof(long)*8] = '\0';
    // as this is a binary array for longs, the first element is set to LONG_MIN
    binArr[0] = LONG_MIN;
    results[0] = '0';
    for(int i =1; i<(sizeof(long)*8); i++) {
        // bin array set to store powers of 2 where increasing index corresponds to decreasing power
        // results array currently set to a string of 0s
        binArr[i] = (long)(pow(2, max-i-1));
        results[i] = '0';
    }
    // copies binArr into mainBinArr and same for results
    mainBinArr = binArr;
    strcpy(mainResult, results);
    char* resultValue =  conversionScratch(mainBinArr, mainResult, max, usersInput, type);
    // mainResult is freed as no longer required
    free(mainResult);
    return resultValue;
}

// does same as convertingLong but this time the type and lengths are different
char *convertingInt(char usersInputChar[], char type[]) {
    long *mainBinArr;
    int usersInput = atoi(usersInputChar);
    int max = sizeof(int)*8;
    char *mainResult = malloc(((sizeof(int)*8)+1));
    char results[(sizeof(int)*8)+1];
    results[(sizeof(int)*8)] = '\0';
    long binArr[(sizeof(int)*8)+1];
    binArr[sizeof(int)*8] = '\0';
    // if the type is unsigned then the first value in bin arr is the max unsigned value
    if (strcmp(type, "int unsigned") == 0) {binArr[0] = UINT_MAX;}
        // otherwise, it is int min
    else {binArr[0] = INT_MIN;}
    results[0] = '0';
    for(int i =1; i<(sizeof(int)*8); i++) {
        binArr[i] = (long)(pow(2, max-i-1));
        results[i] = '0';
    }
    mainBinArr = binArr;
    strcpy(mainResult, results);
    char* resultValue =  conversionScratch(mainBinArr, mainResult, max, usersInput, type);
    free(mainResult);
    return resultValue;
}

// same as above two functions with only difference of size and type
char *convertingChar(char usersInputChar[], char type[]){
    long *mainBinArr;
    int usersInput = atoi(usersInputChar);
    int max = sizeof(char)*8;
    char *mainResult = malloc(((sizeof(char)*8)+1));
    char results[(sizeof(char)*8)+1];
    results[(sizeof(char)*8)] = '\0';
    long binArr[sizeof(char)*8];
    for(int i =0; i<(sizeof(char)*8); i++) {
        binArr[i] = (int)(pow(2, (sizeof(char)*8)-i-1));
        results[i] = '0';
    }
    // if it is not unsigned then the largest power of 2 in bin arr is x by -1
    // so it becomes the right power of 2 for a signed type
    if (strcmp(type, "char") == 0) {binArr[0] = -1 * binArr[0];}
    mainBinArr = binArr;
    strcpy(mainResult, results);
    char* resultValue =  conversionScratch(mainBinArr, mainResult, max, usersInput, type);
    free(mainResult);
    return resultValue;
}

// takes a double value and produces its binary equivalence
char *convertingDouble(char usersInputChar[], char type[]) {
    // allocating memory for the final result so it can be returned
    char *mainResult = malloc(((sizeof(double)*8)+1));
    // temporary storer for the results before passing into mainResult
    char results[(sizeof(double)*8)+1];
    results[(sizeof(double)*8)] = '\0';
    char *doubleNumPoint;
    // changes users num to a double
    double usersNum = strtod(usersInputChar, &doubleNumPoint);
    unsigned count;
    // produces an array of 8 bit elements which is the size of the double
    // so the array has form [00000000 , 00000000 , ... ,] in low level
    char arrOf8BitElem[sizeof(usersNum)];
    // copies the memory address of users num into the array so we can perform masking and bit comparison
    memcpy(arrOf8BitElem, &usersNum, sizeof(usersNum));
    // iterate through by 8*sizeof(usersNum) so we can access each bit
    // (sizeof(usersNum)) gives number of bytes and 1 byte is 8 bits
    for (count = 8*sizeof(usersNum); count --;) {
        // arrOf8BitElem[count/8] gives us a specific byte
        // 1 << (count%8) performs a left shift to allow us to compare two bits
        // so for example, if count = 63 then we get arrOf8BitElem[7] = 7th byte in the array
        // and 1 << count%8 = 1 << 7 = 128 = 10000000 in binary
        // so by performing an & on these two, we get either a 1 or 0 followed by all 0s
        // this allows us to determine the bit at a given position in usersNum
        if (((arrOf8BitElem[count/8]) & (1 << (count%8))) == 0) {
            // set values based on this determination accordingly
            results[63-count] = '0';
        }
        else {results[63-count] = '1';}
    }
    strcpy(mainResult, results);
    return mainResult;
}

// function for calling the required functions based on type
char *convertingMain(char type[], char usersInputChar[]) {
    if (strcmp(type, "char") == 0) {
        // finalR gives bits as a single string
        char *finalR = convertingChar(usersInputChar, type);
        // splittedFinal breaks this into nibbles
        char *splittedFinal = nibbleSplitterChar(finalR);
        // finalR freed as no longer needed
        free(finalR);
        return splittedFinal;
    }
        // same logic holds for the other types
    else if (strcmp(type, "int") == 0) {
        char *finalR = convertingInt(usersInputChar, type);
        char *splittedFinal = nibbleSplitterInt(finalR);
        free(finalR);
        return splittedFinal;
    }

    else if (strcmp(type, "long") == 0) {
        char *finalR = convertingLong(usersInputChar, type);
        char *splittedFinal = nibbleSplitterLong(finalR);
        free(finalR);
        return splittedFinal;
    }

    else if (strcmp(type, "char unsigned") == 0) {
        char *finalR = convertingChar(usersInputChar, type);
        char *splittedFinal = nibbleSplitterChar(finalR);
        free(finalR);
        return splittedFinal;
    }
    else if (strcmp(type, "int unsigned") == 0) {
        char *finalR = convertingInt(usersInputChar, type);
        char *splittedFinal = nibbleSplitterInt(finalR);
        free(finalR);
        return splittedFinal;
    }

    else if ((strcmp(type, "double")) == 0) {
        char *finalR = convertingDouble(usersInputChar, type);
        char *splittedFinal = nibbleSplitterLong(finalR);
        free(finalR);
        return splittedFinal;
    }

    return "";
}

// converts a binary string of 1s and 0s into a character
long binConvertChar(char usersBinVal[], const long* binArr) {
    // tempValue stores the changing value of the character
    long tempValue = 0;
    // iterate through the string
    for(int i =0; i< strlen(usersBinVal); i++) {
        // if the strings ith element is a 1 then we add the corresponding power of 2 to tempValue
        if (usersBinVal[i] == '1') {
            tempValue = tempValue + binArr[i];
        }
    }
    return tempValue;
}

// sets up the binary array for converting a char to a binary value
long binArrChar(char type[], char usersVal[]) {
    // stores binary array to be passed into converting function
    long *mainBinArr;
    // current binary array to be tampered with within this function
    long binArr[(sizeof(char)*8)+1];
    binArr[sizeof(char)*8] = '\0';
    // same logic as in above functions to set powers of 2
    for(int i =0; i<(sizeof(char)*8); i++) {
        binArr[i] = (int) (pow(2, (sizeof(char) * 8) - i - 1));
    }
    // same logic if unsigned or not to determine the sign of the highest power
    if (strcmp(type, "char") == 0) {binArr[0] = -1 * binArr[0];}
    // calls relevant functions and returns their value
    mainBinArr = binArr;
    long finalR = binConvertChar(usersVal, mainBinArr);
    return finalR;
}

// same concept as binArrChar but different sizes for 32 bits
long binArrInt(char type[], char usersVal[]) {
    int max = sizeof(int)*8;
    long *mainBinArr;
    long binArr[(sizeof(int)*8)+1];
    binArr[sizeof(int)*8] = '\0';
    for(int i =0; i<(sizeof(int)*8); i++) {
        binArr[i] = (long) (pow(2, max - i - 1));
    }
    if (strcmp(type, "int") == 0) {binArr[0] = -1 * binArr[0];}
    mainBinArr = binArr;
    long finalR = binConvertChar(usersVal, mainBinArr);
    return finalR;
}

// same concept as binArrChar but for a different size for 64 bits
long binArrLongDouble(char type[], char usersVal[]) {
    int max = sizeof(long)*8;
    long *mainBinArr;
    long binArr[(sizeof(long)*8)+1];
    binArr[sizeof(long)*8] = '\0';
    // if long then max power of 2 is a negative
    if (strcmp(type, "long") == 0) {binArr[0] = (long)(-1 * pow(2, max - 0 - 1));}

    for(int i =1; i<(sizeof(long)*8); i++) {
        binArr[i] = (long) (pow(2, max - i - 1));
    }
    mainBinArr = binArr;
    long finalR = binConvertChar(usersVal, mainBinArr);
    return finalR;
}

// helper function for binConvertDouble
// please see binConvertDouble first
// function calculates the exponent size based on bit pattern of exponent passed in as a parameter
double calculatingDoublesExp(const double *mainBinArr, char usersBinVal[]) {
    // tempValue stores changing exponent size
    double tempValue = 0;
    // used as an index guider in for loop
    int relevantSize = 63;
    for(int i =0; i< strlen(usersBinVal); i++) {
        // as exponent is 11 bits then to access say 10th item we do 11-1-0
        // to access 9th we do 11-1-1 hence we do index of strlen(usersBinVal)-i-1
        if (usersBinVal[strlen(usersBinVal)-i-1] == '1') {
            // though the exponent is 11 bits
            // it stems from a 64 bit array
            // hence we add onto tempValue mainBinArr[relevantSize-i]
            // to ensure we get right value from mainBinArr
            tempValue = tempValue + mainBinArr[relevantSize-i];
        }
    }
    return tempValue;
}

// converts a string of 1s and 0s into a double value
double binConvertDouble(const double *mainBinArr, const char usersBinVal[]) {
    double isNegative = 1;
    // if the first element in the string is 1 then is negative is true
    // because of the way double representation works
    if (usersBinVal[0] == '1') {isNegative = -1;}
    // the exponent is initially set to 0
    double exponent = 0;
    // 11 bits are allocated for the exponent in 64 bit representation
    int exponentSize = 11;
    // exponent sub string stores the exponent bit representation
    char exponentSubString[exponentSize+1];
    int count = 0;
    // this for loop extracts the exponent 1s and 0s from the whole string of 1s and 0s
    for(int i =0; i<exponentSize; i++) {
        exponentSubString[i] = usersBinVal[i+1];
        count = count + 1;
    }
    // set null terminator for the exponent substring
    exponentSubString[exponentSize] = '\0';
    // find value of exponent by calling a helper functions
    exponent = calculatingDoublesExp(mainBinArr, exponentSubString);
    // done with exponent so now onto mantissa
    double summingMantissa = 0;
    // from the end of the exponent to the end of the string we iterate through
    for(int i =exponentSize+1; i<8*sizeof(double); i++) {
        // if there is a 1 then we 2^n where n is the negative corresponding power
        if (usersBinVal[i] == '1') {
            summingMantissa = summingMantissa + pow(2, exponentSize-i);
        }
    }
    // then we add 1 to this mantissa value
    double totalMantissaVal = 0;
    totalMantissaVal = 1 + summingMantissa;
    // find the actual exponent from the offset (i.e. our previous exponent)
    double actualExp = (double)pow(2, (exponent-1023));
    // then multiply these values together (note this is from formula for 64 bit double representation)
    double finalVal = isNegative * totalMantissaVal * actualExp;
    // return this value
    return finalVal;
}

// sets binary array for double conversion
// same concept as before
double binConversionDouble(char usersBinVal[]) {
    int max = sizeof(long)*8;
    double *mainBinArr;
    double binArr[(sizeof(long)*8)+1];
    binArr[sizeof(long)*8] = '\0';
    for(int i =0; i<(sizeof(long)*8); i++) {
        binArr[i] = (double) (pow(2, max - i - 1));
    }
    mainBinArr = binArr;
    double finalR = binConvertDouble(mainBinArr, usersBinVal);
    return finalR;
}

// general function for wanting to convert any type except double from its binary to its numerical value
long binConversionGen(char type[], char usersBinVal[]) {
    if (strcmp(type, "char") == 0) {
        long finalVal = binArrChar(type, usersBinVal);
        return finalVal;
    }
    if ((strcmp(type, "int")) == 0) {
        long finalVal = binArrInt(type, usersBinVal);
        return finalVal;
    }
    if ((strcmp(type, "long")) == 0) {
        long finalVal = binArrLongDouble(type, usersBinVal);
        return finalVal;
    }
    if ((strcmp(type, "unsigned char")) == 0) {
        long finalVal = binArrChar(type, usersBinVal);
        return finalVal;
    }
    if ((strcmp(type, "unsigned int")) == 0) {
        long finalVal = binArrInt(type, usersBinVal);
        return finalVal;
    }
    return -1;

}

// checks if a number is a valid double
// return -1 if not otherwise 1 if is
int validNumDouble(char usersNum[]) {
    // ascii code of a negative sign
    int negativeSignCharCode = 45;
    // num of dec stores the number of decimal points detected
    int numOfDec = 0;
    // if the first element is a 0 and the string length is not 1 and the second element is not a dot
    // then we have something of the form 0a where a = any character except dot
    // this is invalid
    if ((usersNum[0] == '0') && (strlen(usersNum) != 1) && usersNum[1] != '.') {
        return -1;
    }
        // if the string length is 2 however and the first element is a - and second is a 0
        // then we only have -0 which is invalid
    else if ((strlen(usersNum) == 2) && (usersNum[0] == '-') && (usersNum[1] == '0')) {
        return -1;
    }
        // if the length is 3 and we have something of the form -0a where a = anything except dot then this is invalid
    else if ((strlen(usersNum) == 3) && (usersNum[0] == '-') && (usersNum[1] == '0') && (usersNum[2] != '.')) {
        return -1;
    }
    // iterate through each element in users num
    for(int i = 0 ; i<strlen(usersNum) ; i++) {
        // if it is not a digit then it must have to be a decimal for validity
        if (!isdigit(usersNum[i])) {
            // if it is a decimal it must not be in the first or last position and must be the only one
            if ((i < strlen(usersNum)-1) && (i != 0) && (usersNum[i] == '.') && (numOfDec == 0)) {
                numOfDec = numOfDec + 1;
            }
                // otherwise if it is anything else then return -1
            else if (!((i == 0) && usersNum[i] == negativeSignCharCode)) {
                return -1;
            }
        }
    }
    return 0;
}

// checks if a number is valid
// same concept as above function without the '.' logic however
int validNum(char usersNum[]) {
    int negativeSignCharCode = 45;

    if ((usersNum[0] == '0') && (strlen(usersNum) != 1)) {
        return -1;
    }
    else if ((strlen(usersNum) >=2) && (usersNum[0] == '-'
    ) && (usersNum[1]=='0')) {
        return -1;
    }
    for(int i = 0 ; i<strlen(usersNum) ; i++) {
        if (!isdigit(usersNum[i])) {
            if (!((i == 0) && usersNum[i] == negativeSignCharCode)) {
                return -1;
            }
        }
    }
    return 0;
}

// checks if a number is a valid char
int validChar(char usersInput[]) {
    // convert it to an int
    int theUsersDigit = atoi(usersInput);
    // check the int is in the range of a valid char
    if ((theUsersDigit < -128) || (theUsersDigit > 127)) {
        return -1;
    }
    return 0;
}

// checks if a number is a valid unsigned int
int validIntUnsigned(char usersInput[]) {
    char *longPointer;
    long resultLong;
    // converts the number to an unsigned int
    unsigned int theUsersDigit = atoi(usersInput);
    // also converts the number to a long
    resultLong = strtol(usersInput, &longPointer, 10);
    // if the int was of a valid type then it would equal the long else it would not
    // hence this comparison allows us to determine if a number is a valid unsigned int
    if (resultLong == theUsersDigit) {
        return 0;
    }
    return -1;
}

// valid int follows same logic
int validInt(char usersInput[]) {
    char *longPointer;
    long resultLong;
    resultLong = strtol(usersInput, &longPointer, 10);
    int theUsersDigit = atoi(usersInput);
    if (resultLong == theUsersDigit) {
        return 0;
    }
    else {return -1;}
}

// checking if number a valid unsigned char
int validUnsignedChar(char usersInput[]) {
    // convert number to an integer
    int theUsersDigit = atoi(usersInput);
    // check it is in the bounds of an unsigned char
    if ((theUsersDigit < 0) || (theUsersDigit > 255)) {return -1;}
    return 0;
}

// takes a double and converts it to a string
// used as a helper function in the testing functions
char *convertBoundsLong(double toConvert) {
    char str[sizeof(double)*8];
    // snprintf changes double to string and stores this in str
    snprintf(str, 64, "%f", toConvert);
    // result stores this string
    char *result = malloc(sizeof(double)*8);
    strcpy(result, str);
    return result;
}

// does the same thing however converts a long to a string
char *convertBoundsInt(long long toConvert) {
    char str[256];
    sprintf(str, "%lld", toConvert);
    char * longString = malloc(sizeof(long long) * 8);
    strcpy(longString, str);
    return longString;
}

// takes a double and makes it a string
// used for assistance in testing functions
// same logic as above
char *doubleToString(double toConvert) {
    char str[sizeof(long long)*60];
    sprintf(str, "%f", toConvert);
    char *doubleStr = malloc(sizeof(long long)*60);
    strcpy(doubleStr, str);
    return doubleStr;
}

// checks if a number is a valid long
int validLong(char usersInput[]) {
    char *doublePointer;
    double doubleUserResult;
    // changes users input to a double value
    doubleUserResult = strtod(usersInput, &doublePointer);
    // if the string version of the users input and the string version of long max are the same
    // then the users input is a valid value as it is a max long
    if (strcmp(convertBoundsInt(LONG_MAX) , usersInput) == 0) {return 0;}
    // otherwise, if the double value is less than long min or greater than or equal to long max
    // note here we do greater than or equal to because casting to double makes long max = long max + 1
    // which also explains why we checked with long max first
    if ((doubleUserResult < LONG_MIN) || (doubleUserResult >= (double)LONG_MAX)) {
        // if this is the case long not a valid long, and we return -1
        return -1;
    }
    return 0;
}

// checks if a double is valid
int validDouble(char usersInput[]) {
    char *doublePointer;
    // changes user input to a double
    double doubleUserResult = strtod(usersInput, &doublePointer);
    // checks against double boundaries
    if ((doubleUserResult > DBL_MAX) || doubleUserResult < -DBL_MAX) {
        return -1;
    }
    return 0;
}

// checks if input type correct
int isTypeValid(char usersType[]) {
    if ((strcmp(usersType, "char") == 0) || (strcmp(usersType, "int") == 0) || (strcmp(usersType, "long") == 0) || (strcmp(usersType, "double") == 0)) {
        return 0;
    }
    else {return -1;}
}

// checks if input type correct for unsigned
int isTypeValidUnsigned(char usersType1[], char usersType2[]) {
    if ((strcmp(usersType1, "unsigned") == 0) && (strcmp(usersType2, "char") == 0)) {
        return 0;
    }
    if (((strcmp(usersType1, "unsigned")) == 0) && (strcmp(usersType2, "int") ==0)) {
        return 0;
    }
    return -1;
}

// checks if a number matches its required type
int checkNumType(char type[], char usersNum[]) {
    if ((strcmp(type, "char") == 0) && (validChar(usersNum) == 0)) {
        return 0;
    }
    else if ((strcmp(type, "int") == 0) && (validInt(usersNum) == 0)) {
        return 0;
    }
    else if ((strcmp(type, "long") == 0) && (validLong(usersNum) == 0)) {
        return 0;
    }
    else if ((strcmp(type, "char unsigned") == 0) && (validUnsignedChar(usersNum)) == 0) {
        return 0;
    }

    else if ((strcmp(type, "int unsigned") == 0) && (validIntUnsigned(usersNum) == 0)) {
        return 0;
    }

    else if ((strcmp(type, "double") == 0 && (validDouble(usersNum) == 0))) {
        return 0;
    }

    else {return -1;}
}

// converts ints and char strings to actual int and chars using atoi
int convertedValueInt(char type[], char usersNum[]) {
    if ((strcmp(type, "char") == 0) || (strcmp(type, "int") == 0)) {
        return atoi(usersNum);
    }
    else {return 0;}
}

// checks a binary string is valid
// by ensuring it is all 1s and 0s
int checkAllBinVal(char usersBinVal[]) {
    // iterates through string
    for(int i =0; i< strlen(usersBinVal); i++) {
        // if reaches a point where not 1 and 0 then returns -1
        if ((usersBinVal[i] != '1') && ((usersBinVal[i]) != '0')) {
            return -1;
        }
    }
    return 0;
}

// ............... TESTING AND INTERFACE ..................

void checkValidChar() {
    assert(validChar("-128") == 0);
    assert(validChar("127") == 0);
    assert(validChar("7") == 0);
    assert(validChar("-35") == -0);
    assert(validChar("255") == -1);
    assert(validChar("-255") == -1);
    assert(validChar("-129") == -1);
    assert(validChar("128") == -1);
}

void checkValidIntUnsigned() {
    char *boundedValues;
    assert(validIntUnsigned("0") == 0);
    assert(validIntUnsigned("-128") == -1);
    assert(validIntUnsigned("127") == 0);
    assert(validIntUnsigned("7") == 0);
    assert(validIntUnsigned("-35") == -1);
    assert(validIntUnsigned("255") == 0);
    assert(validIntUnsigned("-255") == -1);
    assert(validIntUnsigned("-129") == -1);
    assert(validIntUnsigned("128") == 0);
    boundedValues = realloc(convertBoundsInt((long long)(UINT_MAX)), sizeof(long long) * 9);
    assert(validIntUnsigned(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt(((long long)(UINT_MAX)+1)), sizeof(long long) * 9);
    assert(validIntUnsigned(boundedValues) == -1);
    boundedValues = realloc(convertBoundsInt(INT_MIN), sizeof(long long) * 9);
    assert(validIntUnsigned(boundedValues) == -1);
    boundedValues = realloc(convertBoundsInt(INT_MAX), sizeof(long long) * 9);
    assert(validIntUnsigned(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt((long)(INT_MAX)+1), sizeof(long long) * 9);
    assert(validIntUnsigned(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt((long)(INT_MIN)-1), sizeof(long long) * 9);
    assert(validIntUnsigned(boundedValues) == -1);
    free(boundedValues);
}
void checkValidInt() {
    char *boundedValues;
    assert(validInt("-128") == 0);
    assert(validInt("127") == 0);
    assert(validInt("7") == 0);
    assert(validInt("-35") == -0);
    assert(validInt("255") == 0);
    assert(validInt("-255") == 0);
    assert(validInt("-129") == 0);
    assert(validInt("128") == 0);
    boundedValues = realloc(convertBoundsInt(INT_MIN), sizeof(long long) * 9);
    assert(validInt(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt(INT_MAX), sizeof(long long) * 9);
    assert(validInt(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt((long)(INT_MAX)+1), sizeof(long long) * 9);
    assert(validInt(boundedValues) == -1);
    boundedValues = realloc(convertBoundsInt((long)(INT_MIN)-1), sizeof(long long) * 9);
    assert(validInt(boundedValues) == -1);
    free(boundedValues);
}

void checkValidLong() {
    char *boundedValues;
    assert(validLong("-128") == 0);
    assert(validLong("127") == 0);
    assert(validLong("7") == 0);
    assert(validLong("-35") == -0);
    assert(validLong("255") == 0);
    assert(validLong("-255") == 0);
    assert(validLong("-129") == 0);
    assert(validLong("128") == 0);
    boundedValues = realloc(convertBoundsInt(INT_MIN), sizeof(long long) * 9);
    assert(validLong(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt(INT_MAX), sizeof(long long) * 9);
    assert(validLong(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt((long)(INT_MAX)+1), sizeof(long long) * 9);
    assert(validLong((boundedValues)) == 0);
    boundedValues = realloc(convertBoundsInt((long)(INT_MIN)-1), sizeof(long long) * 9);
    assert(validLong(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt(LONG_MIN), sizeof(long long) * 9);
    assert(validLong(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt(LONG_MAX), sizeof(long long) * 9);
    assert(validLong(boundedValues) == 0);
    boundedValues = convertBoundsLong((double)(LONG_MAX)+1);
    assert(validLong(boundedValues) == -1);
    boundedValues = convertBoundsLong((double)(LONG_MIN)*2);
    assert(validLong(boundedValues) == -1);
    free(boundedValues);
}

void checkValidDouble() {
    char *boundedValues;
    boundedValues = realloc(doubleToString(DBL_MAX), sizeof(long long) * 60);
    assert(validDouble(boundedValues) == 0);
    boundedValues = realloc(doubleToString(-DBL_MAX), sizeof(long long) * 60);
    assert(validDouble(boundedValues) == 0);
    boundedValues = doubleToString(((DBL_MAX*2)));
    assert(validDouble(boundedValues) == -1);
    boundedValues = doubleToString(((-DBL_MAX*2)));
    assert(validDouble(boundedValues) == -1);
    assert(validDouble("-128") == 0);
    assert(validDouble("127") == 0);
    assert(validDouble("7") == 0);
    assert(validDouble("-35") == -0);
    assert(validDouble("255") == 0);
    assert(validDouble("-255") == 0);
    assert(validDouble("-129") == 0);
    assert(validDouble("128") == 0);
    assert(validDouble("-1.25") == 0);
    boundedValues = realloc(convertBoundsInt(INT_MIN), sizeof(long long) * 9);
    assert(validDouble(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt(INT_MAX), sizeof(long long) * 9);
    assert(validDouble(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt((long)(INT_MAX)+1), sizeof(long long) * 9);
    assert(validDouble((boundedValues)) == 0);
    boundedValues = realloc(convertBoundsInt((long)(INT_MIN)-1), sizeof(long long) * 9);
    assert(validDouble(boundedValues) == 0);
    boundedValues = realloc(convertBoundsInt(LONG_MIN), sizeof(long long) * 9);
    assert(validDouble(boundedValues) == 0);
    boundedValues = realloc(convertBoundsLong(((double)(LONG_MAX))), sizeof(long long) * 9);
    assert(validDouble(boundedValues) == 0);
    boundedValues = convertBoundsLong(((double)((LONG_MAX))+1));
    assert(validDouble(boundedValues) == 0);
    boundedValues = convertBoundsLong(((double)((LONG_MIN))*2));
    assert(validDouble(boundedValues) == 0);
    free(boundedValues);
}

void checkValidUnsignedChar() {
    assert(validUnsignedChar("-128") == -1);
    assert(validUnsignedChar("127") == 0);
    assert(validUnsignedChar("7") == 0);
    assert(validUnsignedChar("-35") == -1);
    assert(validUnsignedChar("255") == 0);
    assert(validUnsignedChar("-255") == -1);
    assert(validUnsignedChar("-129") == -1);
    assert(validUnsignedChar("128") == 0);
}

void checkTypeValid() {
    assert(isTypeValid("char") == 0);
    assert(isTypeValid("int") == 0);
    assert(isTypeValid("long") == 0);
    assert(isTypeValid("double") == 0);
    assert(isTypeValid("0x456") == -1);
    assert(isTypeValid("Char") == -1);
    assert(isTypeValid("Int") == -1);
    assert(isTypeValid("CHAR") == -1);
    assert(isTypeValid("inT") == -1);
    assert(isTypeValid("LONG") == -1);
    assert(isTypeValid("LonG") == -1);
    assert(isTypeValid("UNSIGNED CHAR") == -1);
    assert(isTypeValid("Unsigned Char") == -1);
}

void checkTypeValidUnsigned() {
    assert(isTypeValidUnsigned("unsigned", "char") == 0);
    assert(isTypeValidUnsigned("unsigned", "int") == 0);
    assert(isTypeValidUnsigned("char", "char") == -1);
    assert(isTypeValidUnsigned("int", "int") == -1);
    assert(isTypeValidUnsigned("long", "long") == -1);
    assert(isTypeValidUnsigned("0x456", "unsigned") == -1);
    assert(isTypeValidUnsigned("unsigned" , "Char") == -1);
    assert(isTypeValidUnsigned("Unsigned", "Char") == -1);
    assert(isTypeValidUnsigned("UNSIGNED", "char") == -1);
    assert(isTypeValidUnsigned("inT", "int") == -1);
    assert(isTypeValidUnsigned("LONG" , "LONG") == -1);
}

void checkValidNumDouble() {
    assert(validNumDouble(".13") == -1);
    assert(validNumDouble("0.13") == 0);
    assert(validNumDouble("-0.13") == 0);
    assert(validNumDouble("-1234.56") == 0);
    assert(validNumDouble("-1.25") == 0);
    assert(validNumDouble("-11.3456") == 0);
    assert(validNumDouble("-128") == 0);
    assert(validNumDouble("-09") == -1);
    assert(validNumDouble("127") == 0);
    assert(validNumDouble("7") == 0);
    assert(validNumDouble("-35") == 0);
    assert(validNumDouble("255") == 0);
    assert(validNumDouble("-255") == 0);
    assert(validNumDouble("08") == -1);
    assert(validNumDouble("-x08") == -1);
    assert(validNumDouble("10000000") == 0);
    assert(validNumDouble("-10000000") == 0);

}
void checkValidNum() {
    assert(validNum("-09") == -1);
    assert(validNum("-128") == 0);
    assert(validNum("127") == 0);
    assert(validNum("7") == 0);
    assert(validNum("-35") == 0);
    assert(validNum("255") == 0);
    assert(validNum("-255") == 0);
    assert(validNum("08") == -1);
    assert(validNum("-x08") == -1);
    assert(validNum("10000000") == 0);
    assert(validNum("-10000000") == 0);
}

void checkConvertedValInt() {
    char *boundedValues;
    assert(convertedValueInt("char", "-128") == -128);
    assert(convertedValueInt("char", "127") == 127);
    assert(convertedValueInt("char", "-35") == -35);
    assert(convertedValueInt("char", "7") == 7);
    assert(convertedValueInt("int", "-128") == -128);
    assert(convertedValueInt("int", "127") == 127);
    assert(convertedValueInt("int", "-35") == -35);
    assert(convertedValueInt("int", "7") ==  7);
    boundedValues = realloc(convertBoundsInt(INT_MIN), sizeof(long long) * 9);
    assert(convertedValueInt("int", boundedValues) == INT_MIN);
    boundedValues = realloc(convertBoundsInt(INT_MAX), sizeof(long long) * 9);
    assert(convertedValueInt("int", boundedValues) == INT_MAX);
    free(boundedValues);
}

void checkConvertingMain() {
    char *boundedValues;
    assert(strcmp(convertingMain("char unsigned", "255") , "1111 1111") == 0);
    assert(strcmp(convertingMain("char unsigned", "0") , "0000 0000") == 0);
    assert(strcmp(convertingMain("char unsigned", "127") , "0111 1111") == 0);
    assert(strcmp(convertingMain("char unsigned", "7") , "0000 0111") == 0);
    assert(strcmp(convertingMain("char unsigned", "5") , "0000 0101") == 0);
    assert(strcmp(convertingMain("char", "-128") , "1000 0000") == 0);
    assert(strcmp(convertingMain("char", "127") , "0111 1111") == 0);
    assert(strcmp(convertingMain("char", "7") , "0000 0111") == 0);
    assert(strcmp(convertingMain("char", "-35") , "1101 1101") == 0);
    assert(strcmp(convertingMain("int", "127") , "0000 0000 0000 0000 0000 0000 0111 1111") == 0);
    assert(strcmp(convertingMain("int", "7") , "0000 0000 0000 0000 0000 0000 0000 0111") == 0);
    assert(strcmp(convertingMain("int", "5") , "0000 0000 0000 0000 0000 0000 0000 0101") == 0);
    boundedValues = realloc(convertBoundsInt(INT_MIN), sizeof(long long) * 8);
    assert(strcmp(convertingMain("int", boundedValues) , "1000 0000 0000 0000 0000 0000 0000 0000") == 0);
    boundedValues = realloc(convertBoundsInt(INT_MAX), sizeof(long long) * 8);
    assert(strcmp(convertingMain("int", boundedValues) , "0111 1111 1111 1111 1111 1111 1111 1111") == 0);
    assert(strcmp(convertingMain("int", "5") , "0000 0000 0000 0000 0000 0000 0000 0101") == 0);
    boundedValues = realloc(convertBoundsInt(INT_MAX), sizeof(long long) * 8);
    assert(strcmp(convertingMain("int", boundedValues) , "0111 1111 1111 1111 1111 1111 1111 1111") == 0);
    assert(strcmp(convertingMain("long", boundedValues) , "0000 0000 0000 0000 0000 0000 0000 0000 0111 1111 1111 1111 1111 1111 1111 1111") == 0);
    assert(strcmp(convertingMain("long", "5") , "0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0101") == 0);
    assert(strcmp(convertingMain("long", "0") , "0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000") == 0);
    assert(strcmp(convertingMain("long", "127") , "0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0111 1111") == 0);
    assert(strcmp(convertingMain("long", "7") , "0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0111") == 0);
    assert(strcmp(convertingMain("long", "128") , "0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1000 0000") == 0);
    boundedValues = realloc(convertBoundsInt(LONG_MIN), sizeof(long long) * 8);
    assert(strcmp(convertingMain("long", boundedValues), "1000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000") == 0);
    boundedValues = realloc(convertBoundsInt(LONG_MAX), sizeof(long long) * 8);
    assert(strcmp(convertingMain("long", boundedValues), "0111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111") == 0);
    assert(strcmp(convertingMain("int unsigned", "127") , "0000 0000 0000 0000 0000 0000 0111 1111") == 0);
    assert(strcmp(convertingMain("int unsigned", "7") , "0000 0000 0000 0000 0000 0000 0000 0111") == 0);
    boundedValues = realloc(convertBoundsInt((long long)(UINT_MAX)), sizeof(long long) * 9);
    assert(strcmp(convertingMain("int unsigned", boundedValues) , "1000 0000 0000 0000 0000 0000 0000 0000") == 0);
    assert(strcmp(convertingMain("double", "-1.25") , "1011 1111 1111 0100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000") == 0);
    boundedValues = realloc(doubleToString(DBL_MAX), sizeof(long long) * 60);
    assert(strcmp(convertingMain("double", boundedValues) , "0111 1111 1110 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111") == 0);
    boundedValues = realloc(doubleToString(-DBL_MAX), sizeof(long long) * 60);
    assert(strcmp(convertingMain("double", boundedValues) , "1111 1111 1110 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111") == 0);
    assert(strcmp(convertingMain("double", "1.25") , "0011 1111 1111 0100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000") == 0);
    free(boundedValues);
}

void checkcheckAllBinVal() {
    assert(checkAllBinVal("10000000") == 0);
    assert(checkAllBinVal("10101010") == 0);
    assert(checkAllBinVal("12345678") == -1);
    assert(checkAllBinVal("11110000111010") == 0);
    assert(checkAllBinVal("5498u7") == -1);
}

void checkGeneralBin() {
    assert(binConversionGen("char", "10000000") == -128);
    assert(binConversionGen("char", "01111111") == 127);
    assert(binConversionGen("char", "00001111") == 15);
    assert(binConversionGen("char", "00000111") == 7);
    assert(binConversionGen("char", "11011101") == -35);
    assert(binConversionGen("int", "00000000000000000000000010000000") == 128);
    assert(binConversionGen("int", "00000000000000000000000001111111") == 127);
    assert(binConversionGen("int", "00000000000000000000000000000111") == 7);
    assert(binConversionGen("int", "00000000000000000000000000000101") == 5);
    assert(binConversionGen("int", "10000000000000000000000000000000") == INT_MIN);
    assert(binConversionGen("int", "01111111111111111111111111111111") == INT_MAX);
    assert(binConversionGen("long", "0000000000000000000000000000000000000000000000000000000000000101") == 5);
    assert(binConversionGen("long", "0000000000000000000000000000000000000000000000000000000000000000") == 0);
    assert(binConversionGen("long", "0000000000000000000000000000000000000000000000000000000001111111") == 127);
    assert(binConversionGen("long", "0000000000000000000000000000000000000000000000000000000000000111") == 7);
    assert(binConversionGen("long", "0000000000000000000000000000000000000000000000000000000010000000") == 128);
    assert(binConversionGen("long", "1000000000000000000000000000000000000000000000000000000000000000") == LONG_MIN);
    assert(binConversionGen("long", "0111111111111111111111111111111111111111111111111111111111111111") == LONG_MAX);
    assert(binConversionGen("unsigned char", "10000000") == 128);
    assert(binConversionGen("unsigned char", "01111111") == 127);
    assert(binConversionGen("unsigned char", "00001111") == 15);
    assert(binConversionGen("unsigned char", "00000111") == 7);
    assert(binConversionGen("unsigned char", "11111111") == 255);
    assert(binConversionGen("unsigned int", "00000000000000000000000001111111") == 127);
    assert(binConversionGen("unsigned int", "00000000000000000000000000000111") == 7);
    assert(binConversionGen("unsigned int", "11111111111111111111111111111111") == UINT_MAX);
    assert(binConversionGen("unsigned int", "00000000000000000000000000000000") == 0);
}

void checkBinConversionDouble() {
    assert(binConversionDouble("1011111111110100000000000000000000000000000000000000000000000000") == -1.25);
    assert(binConversionDouble("0011111111110100000000000000000000000000000000000000000000000000") == 1.25);
    assert(binConversionDouble("0100000111011111111111111111111111111111110000000000000000000000") == INT_MAX);
    assert(binConversionDouble("0111111111101111111111111111111111111111111111111111111111111111") == DBL_MAX);
    assert(binConversionDouble("1111111111101111111111111111111111111111111111111111111111111111") == -DBL_MAX);
}
void test() {
    checkBinConversionDouble();
    checkGeneralBin();
    checkcheckAllBinVal();
    checkValidDouble();
    checkValidNumDouble();
    checkConvertingMain();
    checkValidIntUnsigned();
    checkTypeValidUnsigned();
    checkValidUnsignedChar();
    checkValidLong();
    checkTypeValid();
    checkValidNum();
    checkValidChar();
    checkValidInt();
    checkConvertedValInt();
    printf("All Tests pass\n");
}

// checks if user wants to find result of a combined string e.g. {int\;long}
// then it ends with } or d (could be unsigned)
int checkFormat(char *string) {
    if (string[strlen(string)-1] == '}') {return 1;}
    else if (string[strlen(string)-1] == 'd') {return 0;}
    else {return -1;}
}

// takes combined types and turns their binary value into decimal values
char *binToDec(char **values, char *types, unsigned long binVals, int elements) {
    // all results store the result in arbitrary large size
    char *allResults[500];
    char passableAllResults[500] = "";
    char *passResult=malloc(500);
    int numOfNibbles = 0;
    int countingNibbles = 0;
    char *mainVal;
    long finalVal;
    double finalValDouble;
    int storer = 0;
    // go through each type
    for(int i =0; i< strlen(types); i++) {
        // set a long to string array and initialise it to empty when go through each type
        // do same for usersBinVal array
        char longToString[256] = "";
        char usersBinVal[5000] = "";
        // set number of nibbles required based on type
        // number of nibbles = 8*sizeof(type) / 4 = 2 *sizeof(type)
        if (types[i] == 'c') {
            mainVal = "char";
            numOfNibbles = 2*sizeof(char);
        }
        if (types[i] == 'i') {
            mainVal = "int";
            numOfNibbles = 2*sizeof(int);
        }
        if (types[i] == 'l') {
            mainVal = "long";
            numOfNibbles = 2*sizeof(long);
        }
        if (types[i] == 'd') {
            mainVal = "double";
            numOfNibbles = 2*sizeof(double);
        }
        if (types[i] == 'u') {
            i = i + 1;
            if (types[i] == 'i') {
                mainVal = "unsigned int";
                numOfNibbles = 2*sizeof(int);
            }
            else {
                mainVal = "unsigned char";
                numOfNibbles = 2*sizeof(char);
            }
        }
        // for each nibble, copy it into usersBinVal
        for(int j =0; j<numOfNibbles; j++) {
            strcat(usersBinVal, values[countingNibbles]);
            // increment countingNibbles
            countingNibbles = countingNibbles + 1;
        }
        // if we are dealing with not a double
        if (types[i] != 'd') {
            // first check if binary string valid and if not return empty string to main
            int isAllValsBin = checkAllBinVal(usersBinVal);
            if (isAllValsBin == -1) {
                passResult = "";
                return passResult;
            }
            // otherwise, call general bin conversion function
            finalVal = binConversionGen(mainVal, usersBinVal);
            // change the finalVal to a string
            sprintf(longToString, "%ld", finalVal);
            // store it in allResults
            allResults[storer] = longToString;
            // concatenate it to passableAllResults then add " " for space for next value
            strcat(passableAllResults, allResults[storer]);
            strcat(passableAllResults, " ");
            // increment storer
            storer = storer + 1;

        }
            // if types is double
        else if (types[i] == 'd') {
            // same concept but call binConversionDouble
            int isAllValsBin = checkAllBinVal(usersBinVal);
            if (isAllValsBin == -1) {
                passResult = "";
                return passResult;
            }
            finalValDouble = binConversionDouble(usersBinVal);
            sprintf(longToString, "%f", finalValDouble);
            allResults[storer] = longToString;
            strcat(passableAllResults, allResults[storer]);
            strcat(passableAllResults, " ");
            storer = storer + 1;
        }
    }
    // set null terminator
    allResults[storer] = NULL;
    // copy the passableAllResults into passResult
    // return this
    // note pass result is freed in main when it is finally shown
    strcpy(passResult, passableAllResults);
    return passResult;
}

// takes a list of decimal values and converts them to their binary equivalent
char  *decToBin(char **values, char *types) {
    // all results stores the results in arbitrary large type
    int largeNum = 100;
    int largeNum2 = 500;
    char *allResults[largeNum];
    char *mainVal;
    int counter = 0;
    int arrayStorer = 0;
    char finalCombinedResult[500] = "";
    char *finalResult = malloc(largeNum2);
    // iterates through the types
    while (counter < strlen(types)) {
        // based on type set mainVal which is just full form of the type
        if (types[counter] == 'c') { mainVal = "char"; }
        else if (types[counter] == 'i') { mainVal = "int"; }
        else if (types[counter] == 'd') { mainVal = "double"; }
        else if (types[counter] == 'l') { mainVal = "long"; }
        else {
            // if type == 'u' then increment the counter and see which element is next
            // based on that mainVal is either int or char unsigned
            counter = counter + 1;
            if (types[counter] == 'i') { mainVal = "int unsigned"; }
            else if (types[counter] == 'c') { mainVal = "char unsigned"; }
        }
        counter = counter + 1;
        // if mainVal is not double
        if (strcmp(mainVal, "double")!=0) {
            // perform relevant checks as would do normally
            int isValid = validNum(values[arrayStorer]);
            if (isValid == -1) {
                finalResult = "";
                return finalResult;
            }
        }
            // otherwise perform checks on doubles
        else {
            int isValid = validNumDouble(values[arrayStorer]);
            if (isValid == -1) {
                finalResult = "";
                return finalResult;
            }
        }
        // check if num matches its type
        int doesNumMatchType = checkNumType(mainVal, values[arrayStorer]);
        if (doesNumMatchType == -1) {
            finalResult = "";
            return finalResult;
        }
        // store the resulting array from converting main into allResults[arrayStorer]
        allResults[arrayStorer] = convertingMain(mainVal, values[arrayStorer]);
        arrayStorer = arrayStorer + 1;
    }
    // set null terminator
    allResults[arrayStorer] = NULL;

    int printingCheck = 0;
    // iterate through all results until null terminator not reached
    while (allResults[printingCheck] != NULL) {
        // add the result into finalCombined result
        strcat(finalCombinedResult, allResults[printingCheck]);
        // and a space for the next result
        strcat(finalCombinedResult, " ");
        printingCheck++;
    }
    strcpy(finalResult, finalCombinedResult);
    return finalResult;
}

// main function where everything is called from
int main(int n, char *args[n]) {
    // if n is 1 call test function
    if (n==1) test();
        // if the first item after ./visualise is { this indicates we may be looking at combined types
    else if ((n>1) && (args[1][0] == '{')) {
        // check the format (ends with } or d)
        int formatTrue = 0;
        int count = 2;
        formatTrue = checkFormat(args[1]);
        if (formatTrue == -1) {
            fprintf(stderr, "Input error.");
            return -1;
        }
        // while the format is not true or we do not reach the end, then continue checking format for next item
        // this is as we may have {char\;unsigned char} so we would have
        // args[1] = {char\;unsigned
        // args[2] = char}
        while ((formatTrue != 1) && (count < n-1)) {
            formatTrue = checkFormat(args[count]);
            // if we reach a point where formatTrue is false (does not end with d or }) then return error
            if (formatTrue == -1) {
                fprintf(stderr, "Input error.");
                return -1;
            }
            count = count +1;
        }
        // if format true is not 1 i.e. it is 0 (so ends with d not }) return error
        if (formatTrue != 1) {
            fprintf(stderr, "Input error.");
            return -1;
        }
        // ..... CHECKING TYPES ........
        int elementCount = 1;
        // individual types stores an array of each type
        char individualTypes[50];
        int traversalCounter = 1;
        int elementLookingAt = 1;
        int countingEleminEach = 0;
        countingEleminEach = 0;
        int setU = 0;
        int firstIteration = 0;
        int elements = 0;
        // goes through the first args i.e. first string with combined types
        while (traversalCounter < strlen(args[elementLookingAt])) {
            elementCount = 1;
            char typeStorer[50] = "";
            int typeStorerIndex = 0;
            // whilst an element within it is not ; and end } and traversal counter is less than the length we keep iterating through
            while (((args[elementLookingAt][traversalCounter] != ';') && (args[elementLookingAt][traversalCounter] != '}')) && (traversalCounter < strlen(args[elementLookingAt]))) {
                // type storer stores the element at given index
                typeStorer[typeStorerIndex] = args[elementLookingAt][traversalCounter];
                // increment traversalCounter and typeStorerIndex
                traversalCounter = traversalCounter + 1;
                typeStorerIndex = typeStorerIndex + 1;
            }
            // if we are at our first iteration i.e. args[1] then set null terminator for type storer
            // to be traversal counter -1
            if (firstIteration == 0) {typeStorer[traversalCounter-1] = '\0';}
                // otherwise null terminator is at traversalCounter
                // this is because traversalCounter set to 0 or 1 based on at which iteration
                // so need to take this into consideration when setting the null terminator
            else {typeStorer[traversalCounter] = '\0';}
            // based on each type, add to individual types the relevant letter
            // set variables accordingly
            if ((strcmp(typeStorer, "char")) == 0) {
                individualTypes[countingEleminEach] = 'c';
                traversalCounter = traversalCounter + 1;
                individualTypes[countingEleminEach+1] = '\0';
                elements = elements + 1;
                countingEleminEach = countingEleminEach + 1;
            }
            else if ((strcmp(typeStorer, "int")) == 0) {
                individualTypes[countingEleminEach] = 'i';
                traversalCounter = traversalCounter + 1;
                individualTypes[countingEleminEach+1] = '\0';
                elements = elements + 1;
                countingEleminEach = countingEleminEach + 1;
            }
            else if ((strcmp(typeStorer, "long")) == 0) {
                individualTypes[countingEleminEach] = 'l';
                traversalCounter = traversalCounter + 1;
                individualTypes[countingEleminEach+1] = '\0';
                elements = elements + 1;
                countingEleminEach = countingEleminEach + 1;

            }
            else if ((strcmp(typeStorer, "double")) == 0) {
                individualTypes[countingEleminEach] = 'd';
                traversalCounter = traversalCounter + 1;
                individualTypes[countingEleminEach+1] = '\0';
                elements = elements + 1;
                countingEleminEach = countingEleminEach + 1;
            }
            else if ((strcmp(typeStorer, "unsigned")) == 0) {
                individualTypes[countingEleminEach] = 'u';
                traversalCounter = traversalCounter + 1;
                individualTypes[countingEleminEach+1] = '\0';
                countingEleminEach = countingEleminEach + 1;
            }
            else {fprintf(stderr, "Input error.");
                return -1;}
            // if the type is 'u'
            for(int i =0; i< strlen(individualTypes); i++) {
                if (individualTypes[i+setU] == 'u') {
                    // then move onto next iteration
                    // change element looking at so iterate through next string
                    traversalCounter = 0;
                    elementLookingAt = elementLookingAt + 1;
                    setU = setU + i+1;
                    firstIteration = firstIteration + 1;
                }
            }
        }
        // Space stores how many spaces e.g. ./visualise {char\;unsigned char}
        // this is 3 spaces i.e. n without extra stuff on end
        // set space currently to 2 because ./visualise then first string
        int spaces = 2;
        for(int i = 0; i< strlen(individualTypes); i++) {
            // if we hit an u then add a space
            if (individualTypes[i] == 'u') {spaces = spaces + 1;}
        }
        // binVals stores number of nibbles we would expect
        unsigned long binVals = 0;
        for(int i=0; i< strlen(individualTypes); i++) {
            // iterate through the types adding relevant value to binVals
            if (individualTypes[i] == 'c') {binVals = binVals+(2*sizeof(char));}
            else if (individualTypes[i] == 'i') {binVals = binVals+(2*sizeof(int));}
            else if (individualTypes[i] == 'l') {binVals = binVals+(2*sizeof(long));}
            else if (individualTypes[i] == 'd') {binVals = binVals+(2*sizeof(double));}
        }
        // storesVals stores the corresponding values which need to be manipulated
        char *storesVals[50];
        // if n = spaces + elements then we are converting from numbers to binary
        if (n == spaces + elements) {
            for(int i =spaces; i<n; i++) {
                storesVals[i-spaces] = args[i];
            }
            // set null terminator for storesVals
            storesVals[n-spaces] = (char *) (char **) '\0';
            // finds finalVal
            char  *finalVal = decToBin(storesVals, individualTypes);
            if (strcmp(finalVal, "") == 0) {
                fprintf(stderr, "Input error.");
                free(finalVal);
                return -1;
            }
            printf("%s\n", finalVal);
            // frees it as it is no longer required
            free(finalVal);
            return 0;
        }
            // if n == spaces + binVals then converting binary to numbers
        else if (n == spaces + binVals) {
            for(int i =spaces; i<n; i++) {
                storesVals[i-spaces] = args[i];
            }
            storesVals[n-spaces] = (char *) (char **) '\0';
            char *finalVal = binToDec(storesVals, individualTypes, binVals, elements);
            if (strcmp(finalVal, "") == 0) {
                fprintf(stderr, "Input error.");
                free(finalVal);
                return -1;
            }
            printf("%s\n", finalVal);
            free(finalVal);
            return 0;
        }
        else {
            fprintf(stderr, "Input error.");
            return -1;
        }

    }
        // if n = 3 then can only be converting number to binary
        // other way would require more than 3 inputs
        // even for smallest char
    else if (n==3) {
        // checks if type valid
        int determineValidType = isTypeValid(args[1]);
        if (determineValidType == -1) {
            fprintf(stderr, "Input error.");
            return -1;
        }

        else {
            // if type valid and double checks if valid num double
            if (strcmp(args[1], "double") == 0) {
                int determineValidNum = validNumDouble(args[2]);
                if (determineValidNum == -1) {
                    fprintf(stderr, "Input error.\n");
                    return -1;
                }
            }
            else {
                // otherwise, checks if valid num
                int determineValidNum = validNum(args[2]);
                if (determineValidNum == -1) {
                    fprintf(stderr, "Input error.\n");
                    return -1;
                }
            }
            // checks if the number matches its type
            int numMatchType = checkNumType(args[1], args[2]);
            if (numMatchType == -1) {
                fprintf(stderr, "Input error.\n");
                return -1;
            }
            else {
                // if all validation satisfied, converting main called
                char *result =  convertingMain(args[1], args[2]);
                printf("%s\n", result);
                // value freed once printed
                free(result);
                return 0;
            }
        }
    }
        // if n is 4 then it is converting binary char to numerical value
        // because we have of the form ./visualise char 0000 0000
    else if (n==4) {
        // check if input is indeed char
        if (strcmp(args[1], "char") == 0) {
            int charSize = 9;
            // sets array of inputs
            char usersBinInput[charSize];
            // adds the nibbles to this array
            strcpy(usersBinInput, args[2]);
            strcat(usersBinInput, args[3]);
            // checks bin string valid
            int isAllValsBin = checkAllBinVal(usersBinInput);
            if (isAllValsBin == -1) {return -1;}
            // calls conversion function
            long result =  binConversionGen("char", usersBinInput);
            printf("%ld\n", result);
            return 0;
        }
    }
        // if n is 5 then we have something of form ./visualise unsigned a 0000 0000
    else if (n==5) {
        // check this is indeed the case
        if (((strcmp(args[1] , "unsigned")) == 0) && (strcmp(args[2], "char") == 0)) {
            int charSize = 9;
            // array which stores users binary input
            char usersBinInput[charSize];
            // set null terminator
            usersBinInput[charSize-1] = '\0';
            strcpy(usersBinInput, args[3]);
            // passes nibbles into usersBinInput using a for loop
            for(int i =4; i < n; i++) {
                strcat(usersBinInput, args[i]);
            }
            // checks binary value valid
            int isAllValsBin = checkAllBinVal(usersBinInput);
            if (isAllValsBin == -1) {return -1;}
            // if valid then calls general bin converting function
            long result =  binConversionGen("unsigned char", usersBinInput);
            printf("%ld\n", result);
            return 0;
        }
        else {
            fprintf(stderr, "Input error.\n");
            return -1;
        }
    }
        // if n is 10 then by a similar logic to above we must have int binary to value
    else if (n == 10) {
        // check if indeed int
        if (strcmp(args[1], "int") != 0) {
            fprintf(stderr, "Input error.\n");
            return -1;
        }
        int size = 34;
        char usersBinInput[size];
        usersBinInput[size-1] = '\0';
        strcpy(usersBinInput, args[2]);
        // Add nibbles to usersBinInput using for loop
        for(int i =3; i < n; i++) {
            strcat(usersBinInput, args[i]);
        }
        int isAllValsBin = checkAllBinVal(usersBinInput);
        if (isAllValsBin == -1) {return -1;}
        long result =  binConversionGen("int", usersBinInput);
        printf("%ld\n", result);
        return 0;
    }
        // if n is 11 then must be unsigned int
    else if (n==11) {
        // check if this is the case
        if (((strcmp(args[1] , "unsigned")) == 0) && (strcmp(args[2], "int") == 0)) {
            int size = 34;
            char usersBinInput[size];
            usersBinInput[size-1] = '\0';
            strcpy(usersBinInput, args[3]);
            for(int i =4; i < n; i++) {
                strcat(usersBinInput, args[i]);
            }
            int isAllValsBin = checkAllBinVal(usersBinInput);
            if (isAllValsBin == -1) {return -1;}
            long result =  binConversionGen("unsigned int", usersBinInput);
            printf("%ld\n", result);
            return 0;
        }
        else {
            fprintf(stderr, "Input error.\n");
            return -1;
        }
    }
        // if n is 18 must be long or double binary
    else if (n==18) {
        // check this is the case
        if ((strcmp(args[1], "long") != 0) && (strcmp(args[1], "double") != 0)) {
            fprintf(stderr, "Input error.\n");
            return -1;
        }
        int size = 65;
        char usersBinInput[size];
        usersBinInput[size-1] = '\0';
        strcpy(usersBinInput, args[2]);
        for(int i =3; i < n; i++) {
            strcat(usersBinInput, args[i]);
        }
        int isAllValsBin = checkAllBinVal(usersBinInput);
        if (isAllValsBin == -1) {return -1;}
        if ((strcmp(args[1], "long")) == 0) {
            long result =  binConversionGen(args[1], usersBinInput);
            printf("%ld\n", result);
            return 0;
        }
        else {
            double result = binConversionDouble(usersBinInput);
            printf("%f\n", result);
            return 0;
        }
    }
    else {
        fprintf(stderr, "Input error\n");
        return -1;
    }
    return 0;
}
