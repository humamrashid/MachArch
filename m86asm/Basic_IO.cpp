#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
int _acc = 0, _cmp;
char _byte;
cin >> noskipws;

// === Declarations === 

int C = 0;
int I = 0;

// === Operations === 

cin >> _byte;
_acc = (int) _byte;
cout << (char) _acc;
C = _acc;
_acc = 1;
I = _acc;
LOOP: _acc = I;
_cmp = _acc - 5;
if (_cmp > 0) goto DONE;
_acc = C;
cout << (char) _acc;
_acc += 1;
C = _acc;
_acc = I;
_acc += 1;
I = _acc;
goto LOOP;
DONE: exit(0);

// === HALT Bypassed === 

exit(1);
}
