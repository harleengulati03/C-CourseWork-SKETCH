// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// Allocate memory for a drawing state and initialise it
state *newState() {
state *theNewState = malloc(sizeof(state)
);
theNewState->x = 0;
theNewState->y = 0;
theNewState->tx = 0;
theNewState->ty = 0;
theNewState->tool = LINE;
theNewState->start = 0;
theNewState->data = 0;
theNewState->end = 0;
return theNewState;
}
// Release all memory associated with the drawing state
void freeState(state *s) {
free(s);
}
// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
// check first bit and if it is 1 this means the opcode is a tool or data
if (
(b & (1<<7)
)
) {
// checks second bit and if it is 1 returns data else tool
if (b & (1<<6)
) {return DATA;}
return TOOL;
}
// if it is a 0 then this means it is DY or DX
// checks second bit and if it is 1 then opcode is DY
else if (b & (1 << 6)
) {return DY;}
// else the two MSB are 0 and opcode is DX
return DX;
}
// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
int signDetermine = 0x20;
int signExpansionHelper = 0x3F;
// get sign
int sign = b & signDetermine;
// if sign negative then do sign extension i.e. set all higher bits
if(sign) {return (b | ~signExpansionHelper);}
// otherwise, remove all higher bits
return (b & signExpansionHelper);
}
// HELPER FUNCTIONS FOR OBEY
void dataCommand(int operand, state *s) {
// shifts bits of current data fields six positions to left
s->data = s->data << 6;
// (operand & 63) masks 6 bits off the operand
// then we combine 6 bits of data with 6 bits of operand
s->data = (s->data | (operand & 63)
);
}
void carryOutTool(display *d, int operand, state *s) {
// selects tool based on value of operand
// and clears data once tool command executed
if (operand == LINE) {
s->tool = LINE;
s->data = 0;
}
else if (operand == BLOCK) {
s->tool = BLOCK;
s->data = 0;
}
else if (operand == COLOUR) {
colour(d, (int)s->data);
s->data = 0;
}
else if (operand == TARGETX) {
s->tx = (int)s->data;
s->data = 0;
}
else if (operand == TARGETY) {
s->ty = (int)s->data;
s->data = 0;
}
else if (operand == SHOW) {
show(d);
s->data = 0;
}
else if (operand == PAUSE) {
pause(d, (int)s->data);
s->data = 0;
}
else if (operand == NEXTFRAME) {
s->data = 0;
}
else if (operand == NONE) {
s->tool = NONE;
s->data = 0;
}
}
void DXCommand(int operand, state *s) {
// shifts location of target by dx axis along x-axis
s->tx = s->tx + operand;
}
void DYCommand(display *d, int operand, state *s) {
// shifts location of target by dy axis along y-axis
// if tool is a line then call line function
s->ty = s->ty + operand;
if (s->tool == LINE) {
line(d, s->x, s->y, s->tx, s->ty);
}
else if (s->tool == BLOCK) {
block(d, s->x, s->y, (s->tx - s->x) , (s->ty - s->y)
);
}
// update current x and y values
s->x = s->tx;
s->y = s->ty;
}
// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
// gets opcode and operands
int opCode = getOpcode(op);
int operand = getOperand(op);
// if statements to call appropriate function
if (opCode == TOOL) { carryOutTool(d, operand, s);
}
else if (opCode == DX) { DXCommand(operand,s);}
else if (opCode == DATA) {
dataCommand(operand, s);
}
else {
DYCommand(d, operand, s);
}
}
void isNextFrame(byte c, void *data, unsigned int count) {
// checks if next frame is called
state *s = (state*) data;
int opCode = getOpcode(c);
int operand = getOperand(c);
if (
(opCode == TOOL) & (operand == NEXTFRAME)
) {
// if called, updates start to point to next command after next frame
s->start = count + 1;
// updates end to true indicating no further bytes should be read
s->end = true;
}
}
// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, void *data, const char pressedKey) {
if (data == NULL) {return (pressedKey == 27);}
state *s = (state*) data;
char *fileName = getName(d);
FILE *in = fopen(fileName, "rb");
// get contents of file from starting position to end
fseek(in, (s->start), SEEK_SET);
unsigned char c = fgetc(in);
unsigned int count = s->start;
// iterates until end is false or end of file reached
while (
(
! feof(in)
) && (s->end == false)
) {
obey(d, s, c);
// checks if command is next frame command
isNextFrame(c, s, count);
count = count + 1;
c = fgetc(in);
}
// sets start to 0 if end of file reached, otherwise test.c repeats tests
if (feof(in)
) {s->start = 0;}
fclose(in);
show(d);
s->x = 0;
s->y = 0;
s->tx = 0;
s->ty = 0;
s->end = 0;
s->data = 0;
return (pressedKey == 27);
}
// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
display *d = newDisplay(filename, 200, 200);
state *s = newState();
run(d, s, processSketch);
freeState(s);
freeDisplay(d);
}
// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]
) {
if (n != 2) { // return usage hint if not exactly one argument
printf("Use ./sketch file\n");
exit(1);
} else view(args[1]
); // otherwise, view sketch file in argument
return 0;
}
#endif
