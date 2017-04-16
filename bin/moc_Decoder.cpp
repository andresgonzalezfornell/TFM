/****************************************************************************
** Meta object code from reading C++ file 'Decoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/interface/Decoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Decoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Decoder_t {
    QByteArrayData data[13];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Decoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Decoder_t qt_meta_stringdata_Decoder = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Decoder"
QT_MOC_LITERAL(1, 8, 15), // "toggleUpmixType"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 8), // "QAction*"
QT_MOC_LITERAL(4, 34, 4), // "item"
QT_MOC_LITERAL(5, 39, 18), // "toggleDecodingType"
QT_MOC_LITERAL(6, 58, 21), // "toggleBinauralQuality"
QT_MOC_LITERAL(7, 80, 15), // "toggleHTRFModel"
QT_MOC_LITERAL(8, 96, 15), // "setDebuggerMode"
QT_MOC_LITERAL(9, 112, 5), // "state"
QT_MOC_LITERAL(10, 118, 4), // "load"
QT_MOC_LITERAL(11, 123, 6), // "encode"
QT_MOC_LITERAL(12, 130, 6) // "decode"

    },
    "Decoder\0toggleUpmixType\0\0QAction*\0"
    "item\0toggleDecodingType\0toggleBinauralQuality\0"
    "toggleHTRFModel\0setDebuggerMode\0state\0"
    "load\0encode\0decode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Decoder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x08 /* Private */,
       5,    1,   57,    2, 0x08 /* Private */,
       6,    1,   60,    2, 0x08 /* Private */,
       7,    1,   63,    2, 0x08 /* Private */,
       8,    1,   66,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    0,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Decoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Decoder *_t = static_cast<Decoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toggleUpmixType((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->toggleDecodingType((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 2: _t->toggleBinauralQuality((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 3: _t->toggleHTRFModel((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 4: _t->setDebuggerMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->load(); break;
        case 6: _t->encode(); break;
        case 7: _t->decode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    }
}

const QMetaObject Decoder::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Decoder.data,
      qt_meta_data_Decoder,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Decoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Decoder::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Decoder.stringdata0))
        return static_cast<void*>(const_cast< Decoder*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Decoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
