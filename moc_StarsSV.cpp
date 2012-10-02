/****************************************************************************
** Meta object code from reading C++ file 'StarsSV.h'
**
** Created: Fri Mar 9 14:20:21 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StarsSV.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StarsSV.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StarsSV[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,    9,    8,    8, 0x05,
      42,   37,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   62,    8,    8, 0x08,
      89,   62,    8,    8, 0x08,
     111,   62,    8,    8, 0x08,
     138,   62,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_StarsSV[] = {
    "StarsSV\0\0newA\0SSVNewAddress(QString)\0"
    "newP\0SSVNewPort(QString)\0item\0"
    "SSVAnewIndex(QString)\0SSVPnewIndex(QString)\0"
    "RecordSSVHistoryA(QString)\0"
    "RecordSSVHistoryP(QString)\0"
};

const QMetaObject StarsSV::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_StarsSV,
      qt_meta_data_StarsSV, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StarsSV::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StarsSV::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StarsSV::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StarsSV))
        return static_cast<void*>(const_cast< StarsSV*>(this));
    return QDialog::qt_metacast(_clname);
}

int StarsSV::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SSVNewAddress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: SSVNewPort((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: SSVAnewIndex((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: SSVPnewIndex((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: RecordSSVHistoryA((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: RecordSSVHistoryP((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void StarsSV::SSVNewAddress(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void StarsSV::SSVNewPort(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
