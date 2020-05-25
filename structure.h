#ifndef STRUCTURE_H
#define STRUCTURE_H

/* Classe Qt */
#include <QString>

/* Données standards des drones */
struct lecture_donnees
{
    QString longitude;
    QString latitude;
    QString altitude;
    QString id;
    QString time;
};

/* Données de la personnalisation des drones */
struct custom_drones {
    int indexCouleur;
    int opacite;
    double largeur;
    QString name;
    QString description;
    bool isSelected;
};

#endif // STRUCTURE_H
