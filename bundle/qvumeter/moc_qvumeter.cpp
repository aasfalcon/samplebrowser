/****************************************************************************
** Meta object code from reading C++ file 'qvumeter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qvumeter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qvumeter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QVUMeter_t {
    QByteArrayData data[13];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVUMeter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVUMeter_t qt_meta_stringdata_QVUMeter = {
    {
QT_MOC_LITERAL(0, 0, 8), // "QVUMeter"
QT_MOC_LITERAL(1, 9, 13), // "valueLChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 13), // "valueRChanged"
QT_MOC_LITERAL(4, 38, 10), // "setColorBg"
QT_MOC_LITERAL(5, 49, 13), // "setColorValue"
QT_MOC_LITERAL(6, 63, 12), // "setColorHigh"
QT_MOC_LITERAL(7, 76, 11), // "setColorLow"
QT_MOC_LITERAL(8, 88, 11), // "setValueDim"
QT_MOC_LITERAL(9, 100, 12), // "setLeftValue"
QT_MOC_LITERAL(10, 113, 13), // "setRightValue"
QT_MOC_LITERAL(11, 127, 11), // "setMinValue"
QT_MOC_LITERAL(12, 139, 11) // "setMaxValue"

    },
    "QVUMeter\0valueLChanged\0\0valueRChanged\0"
    "setColorBg\0setColorValue\0setColorHigh\0"
    "setColorLow\0setValueDim\0setLeftValue\0"
    "setRightValue\0setMinValue\0setMaxValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVUMeter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       3,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   75,    2, 0x0a /* Public */,
       5,    1,   78,    2, 0x0a /* Public */,
       6,    1,   81,    2, 0x0a /* Public */,
       7,    1,   84,    2, 0x0a /* Public */,
       8,    1,   87,    2, 0x0a /* Public */,
       9,    1,   90,    2, 0x0a /* Public */,
      10,    1,   93,    2, 0x0a /* Public */,
      11,    1,   96,    2, 0x0a /* Public */,
      12,    1,   99,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void QVUMeter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QVUMeter *_t = static_cast<QVUMeter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueLChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->valueRChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setColorBg((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 3: _t->setColorValue((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 4: _t->setColorHigh((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 5: _t->setColorLow((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 6: _t->setValueDim((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setLeftValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setRightValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->setMinValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setMaxValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QVUMeter::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QVUMeter::valueLChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (QVUMeter::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QVUMeter::valueRChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject QVUMeter::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QVUMeter.data,
      qt_meta_data_QVUMeter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QVUMeter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVUMeter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QVUMeter.stringdata0))
        return static_cast<void*>(const_cast< QVUMeter*>(this));
    return QWidget::qt_metacast(_clname);
}

int QVUMeter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void QVUMeter::valueLChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QVUMeter::valueRChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
