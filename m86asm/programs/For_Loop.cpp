// C++ code.
// Translated using M86Asm.
// Dated: 14:51:11, 10/24/2014.
// Approx. translating time: 16 ms.
// Number of operations: 15.
// Number of memory units allocated: 2.

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
int X = 0;

// === Operations === 

_acc = 24;
X = _acc;
_acc = 1;
I = _acc;
LOOP: _acc = I;
_cmp = _acc - 5;
if (_cmp > 0) goto NEXT;
_acc = X;
_acc *= 2;
X = _acc;
_acc = I;
_acc += 1;
I = _acc;
goto LOOP;
NEXT: exit(0);

// === HALT Bypassed === 

exit(1);
}

// === EOF === //
