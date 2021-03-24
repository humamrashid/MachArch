// C++ code.
// Translated using M86Asm.
// Dated: 14:52:46, 10/24/2014.
// Approx. translating time: 15 ms.
// Number of operations: 13.
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

int I = 1;
int SQRT = 0;
int X = 16;

// === Operations === 

LOOP: _acc = X;
_cmp = _acc - 0;
if (_cmp <= 0) goto DONE;
_acc -= I;
X = _acc;
_acc = SQRT;
_acc += 1;
SQRT = _acc;
_acc = I;
_acc += 2;
I = _acc;
goto LOOP;
DONE: exit(0);

// === HALT Bypassed === 

exit(1);
}

// === EOF === //
