
#ifndef ENUMS_H
#define ENUMS_H

enum SCANMODE { STEP, QCONT, RCONT };
enum DIRECTION { FORWARD, BACKWARD };

enum MCASTARTRESUME { MCA_START, MCA_RESUME };
// START : 次に「開始」ボタンを押された時にスタートすべき状態(停止か、一時停止)
// START : 次に「開始」ボタンを押された時にスタートすべき状態(停止か、一時停止)

enum ENCORPM { XENC, XPM };
enum OLDNEW { OLD, NEW };

enum RELABS { REL, ABS };

enum ASType { EACHSCAN, SUMUPOFSCANS };   // after save type


#endif

