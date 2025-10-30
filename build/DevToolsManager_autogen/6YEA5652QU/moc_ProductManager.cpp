/****************************************************************************
** Meta object code from reading C++ file 'ProductManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ProductManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProductManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
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
struct qt_meta_tag_ZN14ProductManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto ProductManager::qt_create_metaobjectdata<qt_meta_tag_ZN14ProductManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProductManager",
        "productAdded",
        "",
        "Product",
        "product",
        "productUpdated",
        "productDeleted",
        "productId",
        "productsChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'productAdded'
        QtMocHelpers::SignalData<void(const Product &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'productUpdated'
        QtMocHelpers::SignalData<void(const Product &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'productDeleted'
        QtMocHelpers::SignalData<void(const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Signal 'productsChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProductManager, qt_meta_tag_ZN14ProductManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProductManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ProductManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ProductManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ProductManagerE_t>.metaTypes,
    nullptr
} };

void ProductManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProductManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->productAdded((*reinterpret_cast< std::add_pointer_t<Product>>(_a[1]))); break;
        case 1: _t->productUpdated((*reinterpret_cast< std::add_pointer_t<Product>>(_a[1]))); break;
        case 2: _t->productDeleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->productsChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProductManager::*)(const Product & )>(_a, &ProductManager::productAdded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProductManager::*)(const Product & )>(_a, &ProductManager::productUpdated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProductManager::*)(const QString & )>(_a, &ProductManager::productDeleted, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProductManager::*)()>(_a, &ProductManager::productsChanged, 3))
            return;
    }
}

const QMetaObject *ProductManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProductManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ProductManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ProductManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ProductManager::productAdded(const Product & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ProductManager::productUpdated(const Product & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void ProductManager::productDeleted(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void ProductManager::productsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
