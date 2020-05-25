#ifndef CONVERSION_H
#define CONVERSION_H

/* Struct */
#include "structure.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

class conversion
{
public:
    /** Constructeur de "conversion */
    conversion(QVector <lecture_donnees> donnees_drone,
               QVector <QString> model_drone,
               QVector <QString> id_drone,
               QString nomFichier,
               QVector <custom_drones> customDrones,
               QVector <QString> colorDrones);

    /** Fonction qui génère le texte du fichier KML */
    void miseEnPage();

    /** Fonction qui remplace l'extension ".txt" en ".kml" */
    void replaceName();

    /** Fonction qui enregistre le fichier dans l'endroit spécifié par l'utilisateur */
    void sauvegarder();

private:
    /* Texte généré pour le KML */
    QString format;

    /* == nomFichier */
    QString m_nomFichier;

    /* URL locale pour enregistrer le fichier */
    QString urlFileName;

    /* == donnnes_drone */
    QVector <lecture_donnees> m_donnes_drone;

    /* == modele_drone */
    QVector <QString> m_model_dronne;

    /* == id_drone */
    QVector <QString> m_id_drone;

    QVector <custom_drones> m_customDrones;

    QVector <QString> m_color_drones;

};


#endif // CONVERSION_H
