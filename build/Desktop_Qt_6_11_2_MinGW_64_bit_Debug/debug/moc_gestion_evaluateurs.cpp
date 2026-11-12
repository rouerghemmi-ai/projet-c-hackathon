/****************************************************************************
** Meta object code from reading C++ file 'gestion_evaluateurs.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../gestion_evaluateurs.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gestion_evaluateurs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
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
struct qt_meta_tag_ZN18GestionEvaluateursE_t {};
} // unnamed namespace

template <> constexpr inline auto GestionEvaluateurs::qt_create_metaobjectdata<qt_meta_tag_ZN18GestionEvaluateursE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GestionEvaluateurs",
        "demandeEmployes",
        "",
        "demandeSalles",
        "demandeDeconnexion",
        "ajouterEvaluateur",
        "modifierEvaluateur",
        "supprimerEvaluateur",
        "annulerFormulaire",
        "rechercherEvaluateur",
        "trierEvaluateurs",
        "exporterEvaluateurs",
        "selectionnerLigne",
        "lancerTechChallenge",
        "lancerAnalyseProjet"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'demandeEmployes'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'demandeSalles'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'demandeDeconnexion'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'ajouterEvaluateur'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'modifierEvaluateur'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'supprimerEvaluateur'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'annulerFormulaire'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'rechercherEvaluateur'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'trierEvaluateurs'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'exporterEvaluateurs'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'selectionnerLigne'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'lancerTechChallenge'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'lancerAnalyseProjet'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GestionEvaluateurs, qt_meta_tag_ZN18GestionEvaluateursE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GestionEvaluateurs::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18GestionEvaluateursE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18GestionEvaluateursE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18GestionEvaluateursE_t>.metaTypes,
    nullptr
} };

void GestionEvaluateurs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GestionEvaluateurs *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->demandeEmployes(); break;
        case 1: _t->demandeSalles(); break;
        case 2: _t->demandeDeconnexion(); break;
        case 3: _t->ajouterEvaluateur(); break;
        case 4: _t->modifierEvaluateur(); break;
        case 5: _t->supprimerEvaluateur(); break;
        case 6: _t->annulerFormulaire(); break;
        case 7: _t->rechercherEvaluateur(); break;
        case 8: _t->trierEvaluateurs(); break;
        case 9: _t->exporterEvaluateurs(); break;
        case 10: _t->selectionnerLigne(); break;
        case 11: _t->lancerTechChallenge(); break;
        case 12: _t->lancerAnalyseProjet(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GestionEvaluateurs::*)()>(_a, &GestionEvaluateurs::demandeEmployes, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GestionEvaluateurs::*)()>(_a, &GestionEvaluateurs::demandeSalles, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (GestionEvaluateurs::*)()>(_a, &GestionEvaluateurs::demandeDeconnexion, 2))
            return;
    }
}

const QMetaObject *GestionEvaluateurs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GestionEvaluateurs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18GestionEvaluateursE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int GestionEvaluateurs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void GestionEvaluateurs::demandeEmployes()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GestionEvaluateurs::demandeSalles()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GestionEvaluateurs::demandeDeconnexion()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
