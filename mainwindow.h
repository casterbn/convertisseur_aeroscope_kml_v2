#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Classes Qt */
#include <QFile>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <QTextStream>
#include <QDebug>
#include <QColor>
#include <QDateTime>
#include <QModelIndex>
#include <QVariant>
#include <QPixmap>
#include <QIcon>

/* Classes */
#include "previsualisation.h"
#include "conversion.h"
#include "personnaliser.h"

/* Struct */
#include "structure.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

/** Méthodes */
public:
    /** Constructeur/Destructeur de MainWindow*/
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /** Détermine les différents ID des drones du fichier aéroscope*/
    void scan_id();

    /** Récupère toutes les données intéréssentes du fichier aéroscope */
    void scan_all();

    /** Fonction qui transforme le temps brut en temps formaté */
    QString transformTime(QString timestamp);

    /** Affiche la classe "previsualisation"
     * Permet d'afficher un tableau récapitulatif de chaque donnée pour un drone sélectionné dans
     * une liste */
    void previsualiser(const QModelIndex &index);

    /** Fonction qui permet de définir les couleurs au lancement de l'app */
    void setColorComboBox();

/** Slots */
public slots:

    /* WIP */
    void menuClicDroit(const QPoint &point);

    /** [Slot] Fonction qui récupère un fichier aéroscope  */
    void rechercher_fichier();

    /** [Slot] Affiche la classe "personnaliser"
     * Permet d'afficher la liste des drones et débloque l'accès au tableau
     * et à la partie personnalisation. */
    void fenetre_id_drone();

    /** [Slot] Affiche la classe "conversion"
     * Permet de convertir les données en fichier KML.*/
    void convert();

    /** [Slot] Classe "previsualisation"
     * Permet de convertir le tableau du drone sélectionné en fichier CSV */
    void csvAction();

    /** [Slot] Se déclanche quand un drone est sélectionné */
    void selectDrone(const QModelIndex &index);

    /** [Slot] Se déclanche quand on choisis une autre couleur */
    void changedIndexColor(int index);

    /** [Slot] Se déclanche quand on change le titre du tracé */
    void changedLineName(const QString &name);

    /** [Slot] Se déclanche quand on change la description du tracé */
    void changedTextDescription();

    /** [Slot] Se déclanche quand on change l'opacité du tracé (spin) */
    void changedOpacity(int i);

    /** [Slot] Se déclanche quand on change l'opacité du tracé (slide) */
    void changedOpacitySlide();

    /** [Slot] Se déclanche quand on change la largeur du tracé (spin) */
    void changedLargeur(double i);

    /** [Slot] Se déclanche quand on change l'opacité (slide) */
    void changedLargeurSlide(int i);

/** Attributs */
private:
    /* QTDesign */
    Ui::MainWindow *ui;

    /* Nom du fichier */
    QString  nomfichier;

    /* Fichier */
    QFile fichier;

    /* Tableau des id de chaque drone détectés */
    QVector <QString> id_drone;

    /* Tableau des différennts modèles de chaque drones détectés */
    QVector <QString> model_drone;

    /* Tableau des données de chaque drones détectés */
    QVector <lecture_donnees> donnes_drone;

    /* Tableau de la personnalisation du tracé de chaque drones */
    QVector <custom_drones> customDrones;

    /* Tableaux de différentes couleur pour la personnalisation de l'affichage des données sur Earth. */
    QVector <QColor> couleur;

    /* Liste des couleurs du drone */
    QVector <QString> colorDrone;

    /* Classe personnalisée définie par défaut */
    personnaliser *pr;

    /* Classe prévisualisation définie par */
    previsualisation *p;
};

#endif // MAINWINDOW_H
