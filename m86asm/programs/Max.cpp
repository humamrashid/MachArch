// C++ code.
// Translated using M86Asm.
// Dated: 14:52:25, 10/24/2014.
// Approx. translating time: 16 ms.
// Number of operations: 8.
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

int MAX = 0;
int X = 10;
int Y = 8;

// === Operations === 

_acc = X;
_cmp = _acc - Y;
if (_cmp >= 0) goto X_BIG;
_acc = Y;
MAX = _acc;
exit(0);
X_BIG: MAX = _acc;
exit(0);

// === HALT Bypassed === 

exit(1);
}

// === EOF === //
