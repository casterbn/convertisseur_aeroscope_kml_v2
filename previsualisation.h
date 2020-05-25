#ifndef PREVISUALISATION_H
#define PREVISUALISATION_H

/** Classes Qt */
#include <QHeaderView>
#include <QTableWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QRadioButton>
#include <QStandardItemModel>
#include <QTextStream>

/** Structures */
#include "structure.h"

class previsualisation : public QObject
{
    Q_OBJECT

public:
    /** Contructeur */
    explicit previsualisation(QVector <lecture_donnees> donnees_drone,
                              QVector <QString> model_drone,
                              QVector <QString> id_drone,
                              int index_row);

    /** Destructeur */
    ~previsualisation();

    /** Fonction permettant de placer le modèle dans la vue actuelle */
    void setModel(QTableView *view);

    /** Permet de transformer le tableau actuel en fichier CSV. */
    void to_csv(const QModelIndex &index);

private:

    /* Liste des données de chaque drones (local) */
    QVector <lecture_donnees> m_donnees_drone;

    /* Liste des modèles de drones (local) */
    QVector <QString> m_model_drone;

    /* Liste des ID de drones */
    QVector <QString> m_id_drone;

    /* Données qui seront affichés sur les ignes du tableau */
    QVector <QStandardItem *> longitude;
    QVector <QStandardItem *> latitude;
    QVector <QStandardItem *> altitude;
    QVector <QStandardItem *> time;

    /* Nom du fichier pour l'export en CSV */
    QString nameFile;

    /* Modèle qui sera utilisé pour la vue */
    QStandardItemModel *model;

    /* Nombre de lignes pour le tableau en fonction des éléments de celui-ci */
    int ligneTable;
};

#endif // PREVISUALISATION_H
