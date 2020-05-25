/** Class conversion
 * Permet de convertir le fichier en KML */
#include "conversion.h"

/** Constructeur */
conversion::conversion(QVector <lecture_donnees> donnees_drone, QVector <QString> model_drone, QVector <QString> id_drone, QString nomFichier, QVector <custom_drones> customDrones, QVector <QString> colorDrones)
{
    /* Variables de mise en place */
    m_nomFichier = nomFichier;
    m_donnes_drone = donnees_drone;
    m_id_drone = id_drone;
    m_model_dronne = model_drone;
    m_customDrones = customDrones;
    m_color_drones = colorDrones;

    /* Appel des fonctions */
    replaceName();
    miseEnPage();
    sauvegarder();

}

/** Fonction qui remplace l'extension ".txt" en ".kml" */
void conversion::replaceName()
{
    while (m_nomFichier.indexOf("/") != -1)
    {
        while (m_nomFichier[0] != "/")
        {
            m_nomFichier.remove(0, 1);
        }
        m_nomFichier.remove(0, 1);
    }
    m_nomFichier.replace(".txt", ".kml");
}

/** Fonction qui génère le texte du fichier KML */
void conversion::miseEnPage()
{
    /* En-tête */
    format.push_back("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                     "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n"
                     "<Document>\n"
                     "  <name>");
    format.push_back(m_nomFichier);
    format.push_back("</name>\n");

    /* Création des styles */
    for(int i = 0; i < m_id_drone.size(); i++) {
        format.push_back("  <Style id=\"");
        format.push_back(QString::number(i));
        format.push_back("\">\n"
                         "      <LineStyle>\n"
                         "          <color>");
        int opacite = (255 * m_customDrones[i].opacite) / 100;
        format.push_back(QString::number(opacite, 16));
        format.push_back(m_color_drones[m_customDrones[i].indexCouleur]);
        format.push_back("</color>\n"
                         "          <width>");
        format.push_back(QString::number(m_customDrones[i].largeur));
        format.push_back("</width>\n"
                         "      </LineStyle>\n"
                         "  </Style>\n");
    }
    format.push_back("  <Style id=\"Depart\">\n"
                     "      <IconStyle>\n"
                     "          <scale>1</scale>\n"
                     "      </IconStyle>\n"
                     "      <LabelStyle>\n"
                     "          <scale>1</scale>\n"
                     "      </LabelStyle>\n"
                     "  </Style>\n"
                     "  <Style id=\"Arrivee\">\n"
                     "      <IconStyle>\n"
                     "          <scale>1</scale>\n"
                     "      </IconStyle>\n"
                     "      <LabelStyle>\n"
                     "          <scale>1</scale>\n"
                     "      </LabelStyle>\n"
                     "  </Style>\n");


    /* Ajout des données */
    for (int j = 0; j < m_id_drone.size(); j++)
    {
        if (m_customDrones[j].isSelected == true)
        {
            bool depart = true;
            while (depart == true)
            {
                for (int i = 0; i < m_donnes_drone.size(); i++)
                {
                    if (m_id_drone[j] == m_donnes_drone[i].id && depart == true)
                    {
                        format.push_back("  <Folder>\n"
                                         "      <name>");
                        format.push_back(m_model_dronne[j]);
                        format.push_back(" - ");
                        format.push_back(m_id_drone[j]);

                        /* Premier point */
                        format.push_back("</name>\n"
                                         "      <Placemark>\n"
                                         "          <name>Premier point"
                                         "</name>\n");
                        format.push_back("          <description>");
                        format.push_back(m_donnes_drone[i].time);
                        format.push_back("          </description>\n");
                        format.push_back("          <styleUrl>#Depart</styleUrl>\n"
                                         "          <Style>\n"
                                         "              <IconStyle>\n"
                                         "                  <Icon>\n"
                                         "                      <href>http://thibma.fr/images/d%C3%A9part.png</href>\n"
                                         "                  </Icon>\n"
                                         "              </IconStyle>\n"
                                         "          </Style>\n"
                                         "          <Point>\n"
                                         "              <altitudeMode>absolute</altitudeMode>\n"
                                         "              <coordinates>");
                        format.push_back(m_donnes_drone[i].longitude);
                        format.push_back(",");
                        format.push_back(m_donnes_drone[i].latitude);
                        format.push_back(",");
                        format.push_back(m_donnes_drone[i].altitude);
                        format.push_back("</coordinates>\n"
                                         "          </Point>\n"
                                         "      </Placemark>\n");
                        depart = false;
                    }
                }

            }

            /* Tracé du drone */
            format.push_back("      <Placemark>\n"
                             "          <name>");
            format.push_back(m_customDrones[j].name);
            format.push_back("</name>\n"
                             "          <description>");
            format.push_back(m_customDrones[j].description);
            format.push_back("</description>\n"
                             "          <styleUrl>#");
            format.push_back(QString::number(j));
            format.push_back("</styleUrl>\n"
                             "          <LineString>\n"
                             "              <altitudeMode>absolute</altitudeMode>\n"
                             "              <coordinates>\n");
            for (int i = 0; i < m_donnes_drone.size(); i++)
            {
                if (m_id_drone[j] == m_donnes_drone[i].id)
                {
                    format.push_back(m_donnes_drone[i].longitude);
                    format.push_back(",");
                    format.push_back(m_donnes_drone[i].latitude);
                    format.push_back(",");
                    format.push_back(m_donnes_drone[i].altitude);
                    format.push_back(" ");
                }
            }
            format.push_back("\n"
                             "              </coordinates>\n"
                             "          </LineString>\n"
                             "      </Placemark>\n");


            /* Dernier point */
            QString buffLong, buffLat, buffAlt, buffTime;
            for (int i = 0; i < m_donnes_drone.size(); i++)
            {
                if (m_id_drone[j] == m_donnes_drone[i].id)
                {
                    buffLong = m_donnes_drone[i].longitude;
                    buffLat = m_donnes_drone[i].latitude;
                    buffAlt = m_donnes_drone[i].altitude;
                    buffTime = m_donnes_drone[i].time;
                }
            }
            format.push_back("      <Placemark>\n"
                             "          <name>Dernier point</name>\n"
                             "          <description>");
            format.push_back(buffTime);
            format.push_back("</description>\n"
                             "          <styleUrl>#Arrivee</styleUrl>\n"
                             "          <Style>\n"
                             "              <IconStyle>\n"
                             "                  <Icon>\n"
                             "                      <href>http://thibma.fr/images/arriv%C3%A9e.png</href>\n"
                             "                  </Icon>\n"
                             "              </IconStyle>\n"
                             "          </Style>\n"
                             "          <Point>\n"
                             "              <altitudeMode>absolute</altitudeMode>\n"
                             "              <coordinates>");
            format.push_back(buffLong);
            format.push_back(",");
            format.push_back(buffLat);
            format.push_back(",");
            format.push_back(buffAlt);
            format.push_back("</coordinates>\n"
                             "          </Point>\n"
                             "      </Placemark>\n");


            format.push_back("  </Folder>\n");
        }
    }

    /* Pied du fichier */
    format.push_back("</Document>\n"
                     "</kml>");
}

/** Fonction qui enregistre le fichier dans l'endroit spécifié par l'utilisateur */
void conversion::sauvegarder()
{
    /* Ouverture de la boîte de dialogue pour sauvegarder */
    urlFileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Sauvegarde du fichier KML"), QDir::homePath() + "/Documents/" + m_nomFichier, QObject::tr("Google Earth (*.kml)"));
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

        /* Ecriture dans le fichier */
        QTextStream out(&file);
        out << format;
    }
}
