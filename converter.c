#include <stdio.h>
#include <stdlib.h>
#include "displayfull.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#define WIDTH 200
#define HEIGHT 200
// struct to store 2d array
// each value is a greyscale value
typedef struct elements {
unsigned char data[WIDTH]
[HEIGHT];
} elements;
// gets data from file and stores it into elements
void getData(char *file, void *data) {
// casts data to struct elements
elements *PGMData = (elements *) (data);
FILE *imagein;
int row, col;
char greyScaleVal;
imagein = fopen(file,"rb");
// checks if file actually exists
if(imagein == NULL) {
fprintf(stderr, "Error opening first file\n");
exit(1);
}
// get first line so we can access the rest
char firstLine[100];
fgets(firstLine, 100, imagein);
// get each byte from the file
// store it in a 2D array
for (row=0; row<HEIGHT; row++)
{
for (col=0; col < WIDTH; col++) {
greyScaleVal = fgetc(imagein);
PGMData->data[row]
[col] = greyScaleVal;
}
}
fclose(imagein);
}
// converts the int values into a series of s->data commands
void readIntVals(unsigned int i, FILE*sketch) {
// we know s->data works by shifting bits left by 6 and adding operand to end
// so we shift the bits right by enough spaces
// start by 30 to get bits in first byte available
// get last 6 bits of them and write them to the file to be added to s->data
// then subtract offset by 6 because want to get bits from next byte section
// and repeat
for(int offset = 30; offset >=0; offset -= 6) {
const unsigned char val = (i >> offset) & 0x3f;
// this gets 11 at the front of command
// as opcode is 11 of data command
fprintf(sketch, "%c", (3 << 6) | val);
}
}
// converts colour values to RGBA values
unsigned int setRGBAVals(unsigned char colour) {
// ors the vals to get value of form AAA0 where A is 8 bits each
unsigned int val2 = (colour << 8);
unsigned int val3 = (val2 << 8);
unsigned int val4 = (val3 << 8);
unsigned int final = (val2 | val3 | val4);
// passes to func to write to file
return final;
}
void writeColourCommands(unsigned int colourVal, FILE *sketch) {
unsigned char colourCommand = 131;
readIntVals(colourVal, sketch);
fwrite(&colourCommand, 1, sizeof(unsigned char), sketch);
}
// converts data from 2D array into commands for sk binary files
void convertData(void *data, char *file) {
// casts data to elements
elements *PGMData = (elements *) (data);
FILE *sketch = fopen(file, "w");
// each command has a unique number based on opcode and operand
// these have been assigned here
unsigned char incrementSTX = 1;
unsigned char setToolBlock = 130;
unsigned char incrementSTY = 65;
unsigned char callDY = 64;
unsigned char setToolDX = 132;
unsigned char setToolDY = 133;
// goes through every element in the 2D array
// and assigns relevant command to it by writing command to file
// only calls a colour command if change in colour
// to reduce possible file size
for(int i=0; i<HEIGHT; i++) {
for(int j =0; j<WIDTH; j++) {
unsigned int final = setRGBAVals(PGMData->data[i]
[j]
);
// setting colour statement
if (
(i == 0) && (j == 0)
) {
writeColourCommands(final, sketch);
}
// the new row case
else if (j == 0) {
unsigned int finalBefore = setRGBAVals(PGMData->data[i-1]
[WIDTH - 1]
);
if (final != finalBefore) {
writeColourCommands(final, sketch);
}
}
// the general each element in a row case
else if (j > 0) {
unsigned int finalBefore = setRGBAVals(PGMData->data[i]
[j-1]
);
if (final != finalBefore) {
writeColourCommands(final, sketch);
}
}
readIntVals(i, sketch);
fwrite(&setToolDY, 1, sizeof(unsigned char), sketch);
fwrite(&callDY, 1, sizeof(unsigned char), sketch);
fwrite(&incrementSTX, 1, sizeof(unsigned char), sketch);
fwrite(&setToolBlock, 1, sizeof(unsigned char), sketch);
fwrite(&incrementSTY, 1, sizeof(unsigned char), sketch);
}
// before moving onto next row of pixels
// set relevant coordinates back to where they need to be
// to be able to do the next row
fwrite(&setToolDX, 1, sizeof(unsigned char), sketch);
fwrite(&setToolDY, 1, sizeof(unsigned char), sketch);
fwrite(&callDY, 1, sizeof(unsigned char), sketch);
}
fclose(sketch);
}
// changes extension from pmg to sk
char *changeExtension(char *file) {
// allocates memory on heap for storing new file name
// this memory is freed in main()
char *newFileName = malloc(strlen(file)
);
// set null terminator
newFileName[strlen(file)-1] = '\0';
int count = 0;
// stores everything before the dot in the file
while (
(file[count] != '
.
'
)
) {
newFileName[count] = file[count];
count = count + 1;
}
// stores .sk
newFileName[count] = '
.
';
count = count + 1;
newFileName[count] = 's';
count = count + 1;
newFileName[count] = 'k';
// renames the file in directory
return newFileName;
}
// prints message that conversion has been done
void finalMessage(char *final) {
// stores final in result
char result[strlen(final)+1];
result[strlen(final)
] = '\0';
int count = 0;
while (
(final[count] != '\0'
)
) {
result[count] = final[count];
count = count + 1;
}
// set message to be shown
// use concatenation to output the message
char message1[200] = "File ";
char message3[100] = " has been written";
strcat(message1, result);
strcat(message1, message3);
printf("%s\n", message1);
}
void testRGBAVals() {
assert(setRGBAVals(255) == 0xffffff00);
assert(setRGBAVals(0) == 0x00000000);
assert(setRGBAVals(15) == 0x0f0f0f00);
assert(setRGBAVals(21) == 0x15151500);
assert(setRGBAVals(128) == 0x80808000);
}
void testChangeExtension() {
char *testVal1 = changeExtension("bands.pgm");
assert(
(strcmp(testVal1, "bands.sk") == 0)
);
free(testVal1);
char *testVal2 = changeExtension("fractal.pgm");
assert(
(strcmp(testVal2, "fractal.sk") == 0)
);
free(testVal2);
char *testVal3 = changeExtension("myImage.pgm");
assert(
(strcmp(testVal3, "myImage.sk") == 0)
);
free(testVal3);
}
void test() {
testRGBAVals();
testChangeExtension();
}
int main(int n , char **args)
{
if (n == 1) {
test();
printf("All tests pass.\n");
return 0;
}
else if (n == 2) {
elements* PGMData = malloc(sizeof(elements)
);
getData(args[1], PGMData);
convertData(PGMData, args[1]
);
char *final = changeExtension(args[1]
);
rename(args[1], final);
finalMessage(final);
free(PGMData);
free(final);
return 0;
}
else {
printf("Please enter ./converter or ./converter a where a is a file.\n");
return 1;
}
