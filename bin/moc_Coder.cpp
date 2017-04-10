/****************************************************************************
** Meta object code from reading C++ file 'Coder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/interface/Coder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Coder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Coder_t {
    QByteArrayData data[7];
    char stringdata0[43];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Coder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Coder_t qt_meta_stringdata_Coder = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Coder"
QT_MOC_LITERAL(1, 6, 9), // "loadInput"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 5), // "reset"
QT_MOC_LITERAL(4, 23, 5), // "apply"
QT_MOC_LITERAL(5, 29, 6), // "cancel"
QT_MOC_LITERAL(6, 36, 6) // "submit"

    },
    "Coder\0loadInput\0\0reset\0apply\0cancel\0"
    "submit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Coder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Coder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Coder *_t = static_cast<Coder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadInput(); break;
        case 1: _t->reset(); break;
        case 2: _t->apply(); break;
        case 3: _t->cancel(); break;
        case 4: _t->submit(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Coder::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Coder.data,
      qt_meta_data_Coder,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Coder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Coder::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Coder.stringdata0))
        return static_cast<void*>(const_cast< Coder*>(this));
    return QDialog::qt_metacast(_clname);
}

int Coder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
