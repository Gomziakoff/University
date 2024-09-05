/****************************************************************************
** Meta object code from reading C++ file 'graph.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Graph/graph.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSgraphENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSgraphENDCLASS = QtMocHelpers::stringData(
    "graph",
    "on_addE_triggered",
    "",
    "updateEPos",
    "on_renameV_triggered",
    "on_deleteV_triggered",
    "on_save_triggered",
    "on_load_triggered",
    "save",
    "QJsonObject&",
    "json",
    "load",
    "on_information_triggered"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSgraphENDCLASS_t {
    uint offsetsAndSizes[26];
    char stringdata0[6];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[21];
    char stringdata5[21];
    char stringdata6[18];
    char stringdata7[18];
    char stringdata8[5];
    char stringdata9[13];
    char stringdata10[5];
    char stringdata11[5];
    char stringdata12[25];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSgraphENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSgraphENDCLASS_t qt_meta_stringdata_CLASSgraphENDCLASS = {
    {
        QT_MOC_LITERAL(0, 5),  // "graph"
        QT_MOC_LITERAL(6, 17),  // "on_addE_triggered"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 10),  // "updateEPos"
        QT_MOC_LITERAL(36, 20),  // "on_renameV_triggered"
        QT_MOC_LITERAL(57, 20),  // "on_deleteV_triggered"
        QT_MOC_LITERAL(78, 17),  // "on_save_triggered"
        QT_MOC_LITERAL(96, 17),  // "on_load_triggered"
        QT_MOC_LITERAL(114, 4),  // "save"
        QT_MOC_LITERAL(119, 12),  // "QJsonObject&"
        QT_MOC_LITERAL(132, 4),  // "json"
        QT_MOC_LITERAL(137, 4),  // "load"
        QT_MOC_LITERAL(142, 24)   // "on_information_triggered"
    },
    "graph",
    "on_addE_triggered",
    "",
    "updateEPos",
    "on_renameV_triggered",
    "on_deleteV_triggered",
    "on_save_triggered",
    "on_load_triggered",
    "save",
    "QJsonObject&",
    "json",
    "load",
    "on_information_triggered"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSgraphENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x08,    1 /* Private */,
       3,    0,   69,    2, 0x08,    2 /* Private */,
       4,    0,   70,    2, 0x08,    3 /* Private */,
       5,    0,   71,    2, 0x08,    4 /* Private */,
       6,    0,   72,    2, 0x08,    5 /* Private */,
       7,    0,   73,    2, 0x08,    6 /* Private */,
       8,    1,   74,    2, 0x108,    7 /* Private | MethodIsConst  */,
      11,    1,   77,    2, 0x08,    9 /* Private */,
      12,    0,   80,    2, 0x08,   11 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::QJsonObject,   10,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject graph::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSgraphENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSgraphENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSgraphENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<graph, std::true_type>,
        // method 'on_addE_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateEPos'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_renameV_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_deleteV_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_save_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_load_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'save'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QJsonObject &, std::false_type>,
        // method 'load'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'on_information_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void graph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<graph *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_addE_triggered(); break;
        case 1: _t->updateEPos(); break;
        case 2: _t->on_renameV_triggered(); break;
        case 3: _t->on_deleteV_triggered(); break;
        case 4: _t->on_save_triggered(); break;
        case 5: _t->on_load_triggered(); break;
        case 6: _t->save((*reinterpret_cast< std::add_pointer_t<QJsonObject&>>(_a[1]))); break;
        case 7: _t->load((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 8: _t->on_information_triggered(); break;
        default: ;
        }
    }
}

const QMetaObject *graph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *graph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSgraphENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int graph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
