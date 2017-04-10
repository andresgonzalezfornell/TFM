/****************************************************************************
** Meta object code from reading C++ file 'ChannelsConfiguration.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/interface/ChannelsConfiguration.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChannelsConfiguration.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChannelsConfiguration_t {
    QByteArrayData data[12];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChannelsConfiguration_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChannelsConfiguration_t qt_meta_stringdata_ChannelsConfiguration = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ChannelsConfiguration"
QT_MOC_LITERAL(1, 22, 9), // "setNumber"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 6), // "number"
QT_MOC_LITERAL(4, 40, 8), // "setLevel"
QT_MOC_LITERAL(5, 49, 5), // "level"
QT_MOC_LITERAL(6, 55, 8), // "setLabel"
QT_MOC_LITERAL(7, 64, 5), // "label"
QT_MOC_LITERAL(8, 70, 9), // "setActive"
QT_MOC_LITERAL(9, 80, 5), // "state"
QT_MOC_LITERAL(10, 86, 8), // "loadFile"
QT_MOC_LITERAL(11, 95, 8) // "openInfo"

    },
    "ChannelsConfiguration\0setNumber\0\0"
    "number\0setLevel\0level\0setLabel\0label\0"
    "setActive\0state\0loadFile\0openInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChannelsConfiguration[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       4,    1,   47,    2, 0x08 /* Private */,
       6,    1,   50,    2, 0x08 /* Private */,
       8,    1,   53,    2, 0x08 /* Private */,
      10,    0,   56,    2, 0x08 /* Private */,
      11,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ChannelsConfiguration::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChannelsConfiguration *_t = static_cast<ChannelsConfiguration *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setNumber((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->loadFile(); break;
        case 5: _t->openInfo(); break;
        default: ;
        }
    }
}

const QMetaObject ChannelsConfiguration::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ChannelsConfiguration.data,
      qt_meta_data_ChannelsConfiguration,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ChannelsConfiguration::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChannelsConfiguration::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ChannelsConfiguration.stringdata0))
        return static_cast<void*>(const_cast< ChannelsConfiguration*>(this));
    return QObject::qt_metacast(_clname);
}

int ChannelsConfiguration::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
