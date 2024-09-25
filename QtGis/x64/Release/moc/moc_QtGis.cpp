/****************************************************************************
** Meta object code from reading C++ file 'QtGis.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../QtGis.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtGis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtGis_t {
    QByteArrayData data[45];
    char stringdata0[766];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtGis_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtGis_t qt_meta_stringdata_QtGis = {
    {
QT_MOC_LITERAL(0, 0, 5), // "QtGis"
QT_MOC_LITERAL(1, 6, 15), // "slot_fileSaveAs"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 12), // "slot_fileNew"
QT_MOC_LITERAL(4, 36, 13), // "slot_fileOpen"
QT_MOC_LITERAL(5, 50, 13), // "slot_fileSave"
QT_MOC_LITERAL(6, 64, 14), // "slot_fileClose"
QT_MOC_LITERAL(7, 79, 13), // "slot_fileExit"
QT_MOC_LITERAL(8, 93, 14), // "openRecentFile"
QT_MOC_LITERAL(9, 108, 4), // "file"
QT_MOC_LITERAL(10, 113, 23), // "updateRecentFileActions"
QT_MOC_LITERAL(11, 137, 17), // "projectCrsChanged"
QT_MOC_LITERAL(12, 155, 20), // "canvasRefreshStarted"
QT_MOC_LITERAL(13, 176, 21), // "canvasRefreshFinished"
QT_MOC_LITERAL(14, 198, 12), // "userRotation"
QT_MOC_LITERAL(15, 211, 12), // "showRotation"
QT_MOC_LITERAL(16, 224, 9), // "showScale"
QT_MOC_LITERAL(17, 234, 16), // "refreshMapCanvas"
QT_MOC_LITERAL(18, 251, 15), // "redrawAllLayers"
QT_MOC_LITERAL(19, 267, 9), // "markDirty"
QT_MOC_LITERAL(20, 277, 20), // "onActiveLayerChanged"
QT_MOC_LITERAL(21, 298, 12), // "QgsMapLayer*"
QT_MOC_LITERAL(22, 311, 5), // "layer"
QT_MOC_LITERAL(23, 317, 22), // "onLayerTreeViewClicked"
QT_MOC_LITERAL(24, 340, 20), // "autoSelectAddedLayer"
QT_MOC_LITERAL(25, 361, 19), // "QList<QgsMapLayer*>"
QT_MOC_LITERAL(26, 381, 6), // "layers"
QT_MOC_LITERAL(27, 388, 28), // "updateNewLayerInsertionPoint"
QT_MOC_LITERAL(28, 417, 18), // "slot_addRaterLayer"
QT_MOC_LITERAL(29, 436, 19), // "slot_addVectorLayer"
QT_MOC_LITERAL(30, 456, 18), // "slot_addImageLayer"
QT_MOC_LITERAL(31, 475, 26), // "slot_ImageJinzitaTriggered"
QT_MOC_LITERAL(32, 502, 22), // "slot_ImageCutTriggered"
QT_MOC_LITERAL(33, 525, 25), // "slot_ImageLashenTriggered"
QT_MOC_LITERAL(34, 551, 23), // "slot_ImageLvboTriggered"
QT_MOC_LITERAL(35, 575, 28), // "slot_ImageZhifangtuTriggered"
QT_MOC_LITERAL(36, 604, 29), // "slot_ImageZengqianguTriggered"
QT_MOC_LITERAL(37, 634, 14), // "slot_rgbDetect"
QT_MOC_LITERAL(38, 649, 14), // "slot_sarDetect"
QT_MOC_LITERAL(39, 664, 18), // "slot_rgbsarBianshi"
QT_MOC_LITERAL(40, 683, 17), // "slot_rgbsarDetect"
QT_MOC_LITERAL(41, 701, 19), // "slot_rgbsarirDetect"
QT_MOC_LITERAL(42, 721, 16), // "slot_rgbirDetect"
QT_MOC_LITERAL(43, 738, 15), // "slot_JinjingLoc"
QT_MOC_LITERAL(44, 754, 11) // "slot_UAVLoc"

    },
    "QtGis\0slot_fileSaveAs\0\0slot_fileNew\0"
    "slot_fileOpen\0slot_fileSave\0slot_fileClose\0"
    "slot_fileExit\0openRecentFile\0file\0"
    "updateRecentFileActions\0projectCrsChanged\0"
    "canvasRefreshStarted\0canvasRefreshFinished\0"
    "userRotation\0showRotation\0showScale\0"
    "refreshMapCanvas\0redrawAllLayers\0"
    "markDirty\0onActiveLayerChanged\0"
    "QgsMapLayer*\0layer\0onLayerTreeViewClicked\0"
    "autoSelectAddedLayer\0QList<QgsMapLayer*>\0"
    "layers\0updateNewLayerInsertionPoint\0"
    "slot_addRaterLayer\0slot_addVectorLayer\0"
    "slot_addImageLayer\0slot_ImageJinzitaTriggered\0"
    "slot_ImageCutTriggered\0slot_ImageLashenTriggered\0"
    "slot_ImageLvboTriggered\0"
    "slot_ImageZhifangtuTriggered\0"
    "slot_ImageZengqianguTriggered\0"
    "slot_rgbDetect\0slot_sarDetect\0"
    "slot_rgbsarBianshi\0slot_rgbsarDetect\0"
    "slot_rgbsarirDetect\0slot_rgbirDetect\0"
    "slot_JinjingLoc\0slot_UAVLoc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtGis[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  204,    2, 0x08 /* Private */,
       3,    0,  205,    2, 0x08 /* Private */,
       4,    0,  206,    2, 0x08 /* Private */,
       5,    0,  207,    2, 0x08 /* Private */,
       6,    0,  208,    2, 0x08 /* Private */,
       7,    0,  209,    2, 0x08 /* Private */,
       8,    1,  210,    2, 0x08 /* Private */,
      10,    1,  213,    2, 0x08 /* Private */,
      11,    0,  216,    2, 0x08 /* Private */,
      12,    0,  217,    2, 0x08 /* Private */,
      13,    0,  218,    2, 0x08 /* Private */,
      14,    0,  219,    2, 0x08 /* Private */,
      15,    0,  220,    2, 0x08 /* Private */,
      16,    0,  221,    2, 0x08 /* Private */,
      17,    1,  222,    2, 0x08 /* Private */,
      17,    0,  225,    2, 0x28 /* Private | MethodCloned */,
      19,    0,  226,    2, 0x08 /* Private */,
      20,    1,  227,    2, 0x08 /* Private */,
      23,    0,  230,    2, 0x08 /* Private */,
      24,    1,  231,    2, 0x08 /* Private */,
      27,    0,  234,    2, 0x08 /* Private */,
      28,    0,  235,    2, 0x0a /* Public */,
      29,    0,  236,    2, 0x0a /* Public */,
      30,    0,  237,    2, 0x0a /* Public */,
      31,    0,  238,    2, 0x0a /* Public */,
      32,    0,  239,    2, 0x0a /* Public */,
      33,    0,  240,    2, 0x0a /* Public */,
      34,    0,  241,    2, 0x0a /* Public */,
      35,    0,  242,    2, 0x0a /* Public */,
      36,    0,  243,    2, 0x0a /* Public */,
      37,    0,  244,    2, 0x0a /* Public */,
      38,    0,  245,    2, 0x0a /* Public */,
      39,    0,  246,    2, 0x0a /* Public */,
      40,    0,  247,    2, 0x0a /* Public */,
      41,    0,  248,    2, 0x0a /* Public */,
      42,    0,  249,    2, 0x0a /* Public */,
      43,    0,  250,    2, 0x0a /* Public */,
      44,    0,  251,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QtGis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtGis *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slot_fileSaveAs(); break;
        case 1: _t->slot_fileNew(); break;
        case 2: _t->slot_fileOpen(); break;
        case 3: _t->slot_fileSave(); break;
        case 4: _t->slot_fileClose(); break;
        case 5: _t->slot_fileExit(); break;
        case 6: _t->openRecentFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->updateRecentFileActions((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 8: _t->projectCrsChanged(); break;
        case 9: _t->canvasRefreshStarted(); break;
        case 10: _t->canvasRefreshFinished(); break;
        case 11: _t->userRotation(); break;
        case 12: _t->showRotation(); break;
        case 13: _t->showScale(); break;
        case 14: _t->refreshMapCanvas((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->refreshMapCanvas(); break;
        case 16: _t->markDirty(); break;
        case 17: _t->onActiveLayerChanged((*reinterpret_cast< QgsMapLayer*(*)>(_a[1]))); break;
        case 18: _t->onLayerTreeViewClicked(); break;
        case 19: _t->autoSelectAddedLayer((*reinterpret_cast< QList<QgsMapLayer*>(*)>(_a[1]))); break;
        case 20: _t->updateNewLayerInsertionPoint(); break;
        case 21: _t->slot_addRaterLayer(); break;
        case 22: _t->slot_addVectorLayer(); break;
        case 23: _t->slot_addImageLayer(); break;
        case 24: _t->slot_ImageJinzitaTriggered(); break;
        case 25: _t->slot_ImageCutTriggered(); break;
        case 26: _t->slot_ImageLashenTriggered(); break;
        case 27: _t->slot_ImageLvboTriggered(); break;
        case 28: _t->slot_ImageZhifangtuTriggered(); break;
        case 29: _t->slot_ImageZengqianguTriggered(); break;
        case 30: _t->slot_rgbDetect(); break;
        case 31: _t->slot_sarDetect(); break;
        case 32: _t->slot_rgbsarBianshi(); break;
        case 33: _t->slot_rgbsarDetect(); break;
        case 34: _t->slot_rgbsarirDetect(); break;
        case 35: _t->slot_rgbirDetect(); break;
        case 36: _t->slot_JinjingLoc(); break;
        case 37: _t->slot_UAVLoc(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QgsMapLayer* >(); break;
            }
            break;
        case 19:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QgsMapLayer*> >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QtGis::staticMetaObject = { {
    QMetaObject::SuperData::link<DemoRibbonWindow::staticMetaObject>(),
    qt_meta_stringdata_QtGis.data,
    qt_meta_data_QtGis,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QtGis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtGis::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtGis.stringdata0))
        return static_cast<void*>(this);
    return DemoRibbonWindow::qt_metacast(_clname);
}

int QtGis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DemoRibbonWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
