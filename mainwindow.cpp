/* MainWindow class
 * Classe gérant la fenêtre principale de l'application
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

/** Constructeur de MainWindow*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /* Initialisations des classes pour la liste des drones et le tableau */
    pr = new personnaliser(model_drone, id_drone, &customDrones);
    p = new previsualisation(donnes_drone, model_drone, id_drone, 0);

    /* Configuration de la fenêtre de base */
    ui->setupUi(this);
    ui->convertir_button->setEnabled(false);
    ui->listeDrones->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* Créations des couleurs de bases pour la personnalisation des tracés */
    setColorComboBox();
    colorDrone.append("NULL");
    colorDrone.append("0000ff"); // Rouge
    colorDrone.append("ff0000"); // Bleu
    colorDrone.append("00ff00"); // Vert
    colorDrone.append("00ffff"); // Jaune
    colorDrone.append("ff00ff"); // Magenta
    colorDrone.append("ffff00"); // Cyan
    colorDrone.append("a4a0a0"); // Grey
    colorDrone.append("000000"); // Noir
    colorDrone.append("ffffff"); // Blanc

    /* Connection des boutons aux fonctions */
    connect(ui->button_parcourir, SIGNAL(clicked()), this, SLOT(rechercher_fichier()));
    connect(ui->leave_button, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->convertir_button, SIGNAL(clicked()), this, SLOT(convert()));
    connect(ui->csvButton, SIGNAL(clicked()), this, SLOT(csvAction()));

    /* Connexion de la liste à la fonction */
    connect(ui->listeDrones, SIGNAL(pressed(const QModelIndex &)), this, SLOT(selectDrone(const QModelIndex &)));

    /* Connexion des sliders, textEdit, spins, etc... de la partie personnalisation */
    connect(ui->comboColor, SIGNAL(currentIndexChanged(int)), this, SLOT(changedIndexColor(int)));
    connect(ui->lineName, SIGNAL(textEdited(const QString &)), this, SLOT (changedLineName(const QString &)));
    connect(ui->textDescription, SIGNAL(textChanged()), this, SLOT(changedTextDescription()));
    connect(ui->spinOpacity, SIGNAL(valueChanged(int)), this, SLOT(changedOpacity(int)));
    connect(ui->sliderOpacity, SIGNAL(valueChanged(int)), this, SLOT(changedOpacitySlide()));
    connect(ui->spinLargeur, SIGNAL(valueChanged(double)), this, SLOT(changedLargeur(double)));
    connect(ui->sliderLargeur, SIGNAL(valueChanged(int)), this, SLOT(changedLargeurSlide(int)));


}

/** Destructeur de MainWindow*/
MainWindow::~MainWindow()
{
    fichier.close();
    delete ui;
    delete pr;
}


/** SLOTS */

/** [Slot] Fonction qui récupère un fichier aéroscope  */
void MainWindow::rechercher_fichier()
{
    /* On sauvegarde l'ancien fichier au cas où le nouveau est incorrect */
    QString oldName = nomfichier;

    /* Fenêtre pour l'importation du fichier aéroscope */
    nomfichier = QFileDialog::getOpenFileName(this, tr("Ouvrir le fichier"), QDir::homePath() + "/Documents", tr("Fichiers texte (*.txt)"));
    fichier.setFileName(nomfichier);

    /* Test d'ouverture du fichier pour éviter qu'il n'y ai pas d'erreurs */
    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* Sinon, on récup l'ancien fichier */
        fichier.setFileName(oldName);
    }

    /* Si pas d'erreurs, on continue */
    else
    {
        ui->file_name->setText(nomfichier);
        scan_id();
    }
}

/** [Slot] Affiche la classe "personnaliser"
 * Permet d'afficher la liste des drones et débloque l'accès au tableau
 * et à la partie personnalisation. */
void MainWindow::fenetre_id_drone()
{
    /* On delete la donnée de l'ancien fichier ou la donnée par défaut (voir contructeur) */
    delete pr;

    /* On créer la nouvelle donnée et on l'affiche dans la liste des drones */
    pr = new personnaliser(model_drone, id_drone, &customDrones);
    ui->listeDrones->setEnabled(true);
    ui->groupBoxListeDrone->setEnabled(true);
    pr->setModel(ui->listeDrones);
}

/** [Slot] Affiche la classe "conversion"
 * Permet de convertir les données en fichier KML.*/
void MainWindow::convert()
{
    /* On rescan les données pour préparer à la conversion */
    scan_all();

    /* On converti le fichier dans une autre classe */
    conversion *e = new conversion(donnes_drone, model_drone, id_drone, nomfichier, customDrones, colorDrone);
    delete e;
}

/** [Slot] Classe "previsualisation"
 * Permet de convertir le tableau du drone sélectionné en fichier CSV */
void MainWindow::csvAction()
{
    p->to_csv(ui->listeDrones->currentIndex());
}

/** [Slot] Se déclanche quand un drone est sélectionné */
void MainWindow::selectDrone(const QModelIndex &index)
{
    /* On affiche le tableau */
    previsualiser(index);

    /* On active la zone de personnalisation */
    ui->groupBoxPerso->setEnabled(true);

    /* On mets à jour l'affichage de la zone de personnalisation */
    ui->comboColor->setCurrentIndex(customDrones[index.row()].indexCouleur);
    ui->lineName->setText(customDrones[index.row()].name);
    ui->textDescription->setText(customDrones[index.row()].description);
    ui->spinOpacity->setValue(customDrones[index.row()].opacite);
    ui->sliderOpacity->setValue(customDrones[index.row()].opacite);
    ui->spinLargeur->setValue(customDrones[index.row()].largeur);
    double largeur = customDrones[index.row()].largeur * 10;
    ui->sliderLargeur->setValue(static_cast<int>(largeur));
}

/** [Slot] Se déclanche quand on choisis une autre couleur */
void MainWindow::changedIndexColor(int index) {
    customDrones[ui->listeDrones->currentIndex().row()].indexCouleur = index;
}

/** [Slot] Se déclanche quand on change le titre du tracé */
void MainWindow::changedLineName(const QString &name)
{
    customDrones[ui->listeDrones->currentIndex().row()].name = name;
}

/** [Slot] Se déclanche quand on change la description du tracé */
void MainWindow::changedTextDescription()
{
    customDrones[ui->listeDrones->currentIndex().row()].description = ui->textDescription->toPlainText();
}

/** [Slot] Se déclanche quand on change l'opacité du tracé (spin) */
void MainWindow::changedOpacity(int i)
{
    customDrones[ui->listeDrones->currentIndex().row()].opacite = i;
    ui->sliderOpacity->setValue(i);
}

/** [Slot] Se déclanche quand on change l'opacité du tracé (slide) */
void MainWindow::changedOpacitySlide()
{
    customDrones[ui->listeDrones->currentIndex().row()].opacite = ui->sliderOpacity->value();
    ui->spinOpacity->setValue(ui->sliderOpacity->value());
}

/** [Slot] Se déclanche quand on change la largeur du tracé (spin) */
void MainWindow::changedLargeur(double i)
{
    customDrones[ui->listeDrones->currentIndex().row()].largeur = i;
    ui->sliderLargeur->setValue(static_cast<int>(i*10));
}

/** [Slot] Se déclanche quand on change l'opacité (slide) */
void MainWindow::changedLargeurSlide(int i)
{
    double cast = static_cast<double>(i/10.0);
    customDrones[ui->listeDrones->currentIndex().row()].largeur = cast;
    ui->spinLargeur->setValue(cast);
}

/* WORK IN PROGRESS */
void MainWindow::menuClicDroit(const QPoint &point)
{
    QMenu menu(this);

    QAction *copy = new QAction("Copier", this);

    menu.addAction(copy);
    menu.popup(point);
}


/** FONCTIONS */

/** Détermine les différents ID des drones du fichier aéroscope*/
void MainWindow::scan_id()
{
    /* Test directement du fichier pour vérifier que c'est le bon */
    QTextStream in(&fichier);

    /* Initialisation à 0 */
    QString ligne = "";
    QString id_temp = "";
    QString model_temp = "";
    int index_id = 0;
    int index_model = 0;
    id_drone.clear();
    id_drone.append("0");
    model_drone.clear();
    model_drone.append("0");
    customDrones.clear();

    /* Lecture du fichier jusqu'à la fin */
    while(!fichier.atEnd())
    {
        /* Lecture d'une ligne */
        ligne = in.readLine();

        /* Recherche du mot clé "sn" -> ID du drone */
        index_id = ligne.indexOf("\"sn\"") + 6;

        /* Recherche du mot clé "productType" -> Type du drone */
        index_model = ligne.indexOf("\"productType\"") + 15;

        /* Variables temporaires */
        id_temp = "";
        model_temp = "";

        /* Sécurité de vérification du fichier */
        if (index_id != 5)
        {
            /* Lecture de la donnée "ID du drone" */
            do
            {
                id_temp.append(ligne[index_id]);
                index_id++;
            } while(ligne[index_id] != "\"");

            /* Sécurité de vérification du fichier */
            if (index_model != 14)
            {
                /* Lecture de la donnée "Type du drone" */
                do
                {
                    model_temp.append(ligne[index_model]);
                    index_model++;
                } while(ligne[index_model] != "\"");
             }

            /* Premier drone de la liste détecté */
            if (id_drone[0] == "0" && model_drone[0] == "0")
            {
                /* On ajout l'ID et le type du drone */
                id_drone[0] = id_temp;
                model_drone[0] = model_temp;

                /* Données temporaire de la customisation par défaut de chaque drone pour être ajoutée à la fin */
                custom_drones temp;
                temp.name = "Tracé du " + model_temp;
                temp.description = "ID : " + id_temp;
                temp.opacite = 100;
                temp.indexCouleur = 1;
                temp.largeur = 3.0;
                temp.isSelected = true;

                /* Ajout de ces données dans la liste */
                customDrones.append(temp);
            }

            /* Sinon, on ajoute le drone à la suite dans la liste */
            else
            {
                    /* On vérifie que le drone qu'on ajoute n'est pas déjà dans la liste */
                    if (id_drone.contains(id_temp) == false && index_model != 14)
                    {
                        /* On ajout l'ID et le type du drone */
                        id_drone.append(id_temp);
                        model_drone.append(model_temp);

                        /* Données temporaire de la customisation par défaut de chaque drone pour être ajoutée à la fin */
                        custom_drones temp;
                        temp.name = "Tracé du " + model_temp;
                        temp.description = "ID : " + id_temp;
                        temp.opacite = 100;
                        temp.largeur = 3.0;
                        temp.isSelected = true;

                        /* Seulement 9 couleurs dispo, on reboucle si plus de 9 drones dans la liste */
                        if (id_drone.size() < 9)
                            temp.indexCouleur = id_drone.size();
                        else {
                            int tempIndex = id_drone.size();
                            while (tempIndex > 9)
                                tempIndex -= 9;
                            temp.indexCouleur = tempIndex;
                        }

                        /* Ajout de ces données dans la liste */
                        customDrones.append(temp);

                    }
            }
        }
        /* Si donnée incorecte : on passe */
        else
            break;
    }

    /* Si on a réussi a rentrer au moins une donnée de drone */
    if (id_drone[0] != "0")
    {
        /* On affiche le nombre de drone détectés */
        ui->sn_label->setText(QString::number(id_drone.size()) + " drone(s) différent(s) détecté(s).\n");
        ui->convertir_button->setEnabled(true);

        /* Go to */
        fenetre_id_drone();
    }

    /* Sinon, fenêtre d'erreur si le fichier est invalide */
    else
    {
        QMessageBox erreur;
        erreur.setIcon(QMessageBox::Warning);
        erreur.setWindowTitle("Fichier incompatible");
        erreur.setStandardButtons(QMessageBox::Ok);
        erreur.setText("Erreur : fichier incorrect, veuillez importer un fichier Aéroscope valide.");
        erreur.exec();
        ui->sn_label->setText("Erreur lors de l'importation du fichier");
        ui->listeDrones->setEnabled(false);
        ui->listeDrones->reset();
        ui->groupBoxListeDrone->setEnabled(false);
        ui->convertir_button->setEnabled(false);
        ui->file_name->setText("");
    }

    /* Fermeture du fichier, fin utilisaton */
    fichier.close();

}

/** Affiche la classe "previsualisation"
 * Permet d'afficher un tableau récapitulatif de chaque donnée pour un drone sélectionné dans
 * une liste */
void MainWindow::previsualiser(const QModelIndex &index)
{
    /* On récupère chaque donnée de chaque drone */
    scan_all();

    /* On delete la donnée de l'ancien fichier ou la donnée par défaut (voir contructeur) */
    delete p;

    /* On créer la nouvelle donnée pour afficher le tableau */
    p = new previsualisation(donnes_drone, model_drone, id_drone, index.row());
    ui->groupTableView->setEnabled(true);
    ui->tableView->setEnabled(true);
    p->setModel(ui->tableView);

}

/** Récupère toutes les données intéréssentes du fichier aéroscope */
void MainWindow::scan_all()
{
    /* On ouvre à nouveau le fichier */
    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        /* Erreur si fichier déplacé ou supprimé */
        QMessageBox erreur;
        erreur.setIcon(QMessageBox::Critical);
        erreur.setWindowTitle("Fichier déplacé");
        erreur.setStandardButtons(QMessageBox::Ok);
        erreur.setText("Attention ! Veuillez ne pas déplacer où supprimer le fichier lors de l'utilisation de celui-ci !");
        erreur.exec();
        ui->sn_label->setText("Fichier déplacé ou supprimé");
        ui->listeDrones->setEnabled(false);
        ui->listeDrones->reset();
        ui->groupBoxListeDrone->setEnabled(false);
        ui->convertir_button->setEnabled(false);
        ui->file_name->setText("");
        return;
    }

    /* Création des variables pour récupérer les données */
    QTextStream in(&fichier);
    QString ligne = "";
    QString id_temp, longitude_temp, latitude_temp, altitude_temp, time_temp;
    int index_id, index_longitude, index_latitude, index_altitude, index_time;

    /* On vide le précédent tableau au cas ou */
    donnes_drone.clear();

    /* Lecture du fichier jusu'à la fin */
    while(!fichier.atEnd())
    {
        /* Lecture de chaque ligne, on détecte chaque élément intéressent comme précédemment */
        ligne = in.readLine();
        index_id = ligne.indexOf("\"sn\"") + 6;
        index_longitude = ligne.indexOf("\"longitude\"") + 12;
        index_latitude = ligne.indexOf("\"latitude\"") + 11;
        index_altitude = ligne.indexOf("\"absoluteHeight\"") + 17;
        index_time = ligne.indexOf("\"lastDronePushUpdateTime\"") + 26;
        id_temp = ""; longitude_temp = ""; latitude_temp = ""; altitude_temp = ""; time_temp = "";

        /* Lecture de la donnée "ID du drone" */
        do
        {
            id_temp.append(ligne[index_id]);
            index_id++;
        } while(ligne[index_id] != "\"");

        /* Lecture de la donnnée "longitude" */
        do
        {
            longitude_temp.append(ligne[index_longitude]);
            index_longitude++;
        } while(ligne[index_longitude] != ",");

        /* Lecture de la donnée "latitude" */
        do
        {
            latitude_temp.append(ligne[index_latitude]);
            index_latitude++;
        } while(ligne[index_latitude] != ",");

        /* Lecture de la donnée "altitude" */
        do
        {
            altitude_temp.append(ligne[index_altitude]);
            index_altitude++;
        } while(ligne[index_altitude] != ",");

        /* Lecture de la donnnée "temps" */
        do
        {
            time_temp.append(ligne[index_time]);
            index_time++;
        } while(ligne[index_time] != ",");

        /* Filtrage des données au cas où si les coordonnées n'ont pas pu être récupérés à un certain moment du vol d'un drone */
        if (longitude_temp != "0.0" || latitude_temp != "0.0")
        {
            /* On créer une variable temporaire qui va ensuite écrire dans le tableau final */
            lecture_donnees temporaire;
            temporaire.longitude = longitude_temp;
            temporaire.latitude = latitude_temp;
            temporaire.altitude = altitude_temp;
            temporaire.id = id_temp;
            temporaire.time = transformTime(time_temp);

            /* Ecriture dans le tableau final */
            donnes_drone.append(temporaire);
        }
    }

    /* Fermeture du fichier à la fin */
    fichier.close();
}

/** Fonction qui transforme le temps brut en temps formaté */
QString MainWindow::transformTime(QString timestamp)
{
    qint64 timeInt = timestamp.toLongLong();
    QDateTime time;
    time.setMSecsSinceEpoch(timeInt);

    return time.toString("dd.MM.yyyy hh:mm:ss");
}

/** Fonction qui permet de définir les couleurs au lancement de l'app */
void MainWindow::setColorComboBox()
{
    QPixmap px(10, 10);
    QIcon icon(px);

    /* Transparent */
    px.fill(QColor(Qt::transparent));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Sélectionnez une couleur...");
    QVariant v(0);
    ui->comboColor->setItemData(0, v, Qt::UserRole - 1);

    /* ROUGE */
    px.fill(QColor(Qt::red));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Rouge");

    /* Bleu */
    px.fill(QColor(Qt::blue));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Bleu");

    /* Vert */
    px.fill(QColor(Qt::green));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Vert");

    /* Jaune */
    px.fill(QColor(Qt::yellow));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Jaune");

    /* Magenta */
    px.fill(QColor(Qt::magenta));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Magenta");

    /* Cyan */
    px.fill(QColor(Qt::cyan));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Cyan");

    /* Gris */
    px.fill(QColor(Qt::gray));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Gris");

    /* Noir */
    px.fill(QColor(Qt::black));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Noir");

    /* Blanc */
    px.fill(QColor(Qt::white));
    icon.addPixmap(px);
    ui->comboColor->addItem(icon, " Blanc");
}
