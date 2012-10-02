/****************************************************************************
** Meta object code from reading C++ file 'Stars.h'
**
** Created: Fri Mar 9 14:20:21 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Stars.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Stars.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Stars[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,    7,    6,    6, 0x05,
      43,   38,    6,    6, 0x05,
      70,   38,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
      97,   38,    6,    6, 0x0a,
     122,   38,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Stars[] = {
    "Stars\0\0msg,time\0aMessage(QString,int)\0"
    "item\0RecordSSVHistoryA(QString)\0"
    "RecordSSVHistoryP(QString)\0"
    "SetNewSVAddress(QString)\0SetNewSVPort(QString)\0"
};

const QMetaObject Stars::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Stars,
      qt_meta_data_Stars, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Stars::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Stars::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Stars::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Stars))
        return static_cast<void*>(const_cast< Stars*>(this));
    return QObject::qt_metacast(_clname);
}

int Stars::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: aMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: RecordSSVHistoryA((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: RecordSSVHistoryP((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: SetNewSVAddress((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: SetNewSVPort((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Stars::aMessage(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Stars::RecordSSVHistoryA(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Stars::RecordSSVHistoryP(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
