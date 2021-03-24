#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
int _acc = 0, _cmp;
char _byte;
cin >> noskipws;

// === Declarations === 

int C = 0;
int Divisor = 0;
int Num = 0;

// === Operations === 

_acc = 0;
Num = _acc;
cin >> _byte;
_acc = (int) _byte;
cout << (char) _acc;
Loop1: _cmp = _acc - 32;
if (_cmp == 0) goto Next1;
_cmp = _acc - 13;
if (_cmp == 0) goto Next1;
_acc -= 48;
C = _acc;
_acc = Num;
_acc *= 10;
_acc += C;
Num = _acc;
cin >> _byte;
_acc = (int) _byte;
cout << (char) _acc;
goto Loop1;
Next1: _acc = Num;
_acc *= 2;
Num = _acc;
_acc = 1;
Divisor = _acc;
Loop2: _acc = Divisor;
_acc *= 10;
_cmp = _acc - Num;
if (_cmp >= 0) goto Loop3;
Divisor = _acc;
goto Loop2;
Loop3: _acc = Num;
_acc /= Divisor;
_acc += 48;
cout << (char) _acc;
_acc = Num;
_acc %= Divisor;
Num = _acc;
_acc = Divisor;
_acc /= 10;
Divisor = _acc;
_acc = Num;
_cmp = _acc - 0;
if (_cmp != 0) goto Loop3;
exit(0);

// === HALT Bypassed === 

exit(1);
}
