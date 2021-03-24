// C++ code.
// Translated using M86Asm.
// Dated: 14:50:55, 10/24/2014.
// Approx. translating time: 16 ms.
// Number of operations: 10.
// Number of memory units allocated: 3.

// === CODE === //

#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
int _acc = 0, _cmp;
char _byte;
cin >> noskipws;

// === Declarations === 

int I = 0;
int N = 4;
int Result = 1;

// === Operations === 

_acc = 1;
Loop: _cmp = _acc - N;
if (_cmp > 0) goto Done;
I = _acc;
_acc *= Result;
Result = _acc;
_acc = I;
_acc += 1;
goto Loop;
Done: exit(0);

// === HALT Bypassed === 

exit(1);
}

// === EOF === //
