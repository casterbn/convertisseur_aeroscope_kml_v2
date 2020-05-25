/** Class previsualisation
 * Permet d'afficher le tableau */
#include "previsualisation.h"

/** Constructeur */
previsualisation::previsualisation(QVector <lecture_donnees> donnees_drone, QVector <QString> model_drone, QVector <QString> id_drone, int index_row)
{
    /* Transformation des données en local */
    m_donnees_drone = donnees_drone;
    m_model_drone = model_drone;
    m_id_drone = id_drone;

    /* Création d'un modèle */
    model = new QStandardItemModel(this);

    /* Création du header du tableau */
    QStringList horizontalHeader;
    horizontalHeader << "Longitude" << "Latitude" << "Altitude" << "Temps";

    /* Mise en place des lignes et des colonnes du tableau */
    model->setRowCount(1);
    model->setColumnCount(4);
    model->setHorizontalHeaderLabels(horizontalHeader);
    ligneTable = 0;

    /* Ajout de chaque ligne */
    for (int j = 0; j < donnees_drone.size(); j++)
    {

        if (id_drone[index_row] == donnees_drone[j].id)
        {
            /* Longitude */
            longitude.push_back(nullptr);
            longitude[ligneTable] = new QStandardItem(donnees_drone[j].longitude);
            model->setItem(ligneTable, 0, longitude[ligneTable]);

            /* Latitude */
            latitude.push_back(nullptr);
            latitude[ligneTable] = new QStandardItem(donnees_drone[j].latitude);
            model->setItem(ligneTable, 1, latitude[ligneTable]);

            /* Altitude */
            altitude.push_back(nullptr);
            altitude[ligneTable] = new QStandardItem(donnees_drone[j].altitude);
            model->setItem(ligneTable, 2, altitude[ligneTable]);

            /* Temps */
            time.push_back(nullptr);
            time[ligneTable] = new QStandardItem(donnees_drone[j].time);
            model->setItem(ligneTable, 3, time[ligneTable]);

            ligneTable++;
        }
    }
}

/** Destructeur */
previsualisation::~previsualisation()
{
    for (int i = 0; i < longitude.size(); i++)
        delete longitude[i];
    for (int i = 0; i < latitude.size(); i++)
        delete latitude[i];
    for (int i = 0; i < altitude.size(); i++)
        delete altitude[i];
    for (int i = 0; i < time.size(); i++)
        delete time[i];

    delete model;
}

/** Fonction permettant de placer le modèle dans la vue actuelle */
void previsualisation::setModel(QTableView *view) {

    view->setModel(model);
    view->horizontalHeader()->setMinimumSectionSize(150);
}

/** Permet de transformer le tableau actuel en fichier CSV. */
void previsualisation::to_csv(const QModelIndex &index)
{
    /* Formattage des données */
    QString format;

    /* En-tête du CSV */
    format.push_back("Longitude,Latitude,Altitude,Temps\n");

    /* Ecriture des données */
    for (int i = 0; i < m_id_drone.size(); i++)
    {
        if (m_id_drone[i] == m_id_drone[index.row()])
        {
            for (int j = 0; j < m_donnees_drone.size(); j++)
            {
                if (m_donnees_drone[j].id == m_id_drone[i])
                {
                    format.push_back(m_donnees_drone[j].longitude);
                    format.push_back(",");
                    format.push_back(m_donnees_drone[j].latitude);
                    format.push_back(",");
                    format.push_back(m_donnees_drone[j].altitude);
                    format.push_back(",");
                    format.push_back(m_donnees_drone[j].time);
                    format.push_back("\n");
                    nameFile = m_id_drone[i] + "_" + m_model_drone[i];
                }
            }
        }
    }

    /* Enregistrement des données */
    QString urlFileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Sauvegarde du fichier tableur"), QDir::homePath() + "/Documents/" + nameFile, QObject::tr("Tableur (*.csv)"));

    if (urlFileName.isEmpty())
        return;
    else
    {
        QFile file(urlFileName);

        /* Test du fichier */
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox erreur;
            erreur.setIcon(QMessageBox::Warning);
            erreur.setWindowTitle("Erreur fichier");
            erreur.setText("Impossible d'enregistrer le fichier.");
            erreur.setStandardButtons(QMessageBox::Ok);
            erreur.exec();
            return;
        }

        QTextStream out(&file);
        out << format;
    }
}
