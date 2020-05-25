#ifndef PERSONNALISER_H
#define PERSONNALISER_H

/** Classes Qt */
#include <QListView>
#include <QStandardItem>

/** Struct */
#include "structure.h"

class personnaliser : public QObject
{
    Q_OBJECT

/** Méthodes */
public:
    /** Constructeur de MainWindow */
    explicit personnaliser(QVector <QString> model_drone,
                           QVector <QString> id_drone,
                           QVector <custom_drones> *customDrones);

    /** Destructeur de la classe */
    ~personnaliser();

    /** Fonction qui applique le modèle à la vue */
    void setModel(QListView *);

/** Slots */
public slots:
    /** [Slot] Change l'état du drone en "Activé" ou "Désactivé" quand
     * on clique sur la comboBox */
    void checkBoxList(QStandardItem *item);

/** Attributs */
private:

    /* Modèle de la vue */
    QStandardItemModel *model;

    /* Un objet pour le modèle */
    QVector <QStandardItem *> item;

    /* Liste des ID des drones (local) */
    QVector <QString> m_id_drone;

    /* Liste de la personnalisation de chaque drone (local)     */
    QVector <custom_drones> *m_customDrones;

};

#endif // PERSONNALISER_H
