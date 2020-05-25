/** personnaliser class
 * Classe gérant l'affichage de la liste des drones */
#include "personnaliser.h"

/** Constructeur de personnaliser */
personnaliser::personnaliser(QVector <QString> model_drone, QVector <QString> id_drone, QVector <custom_drones> *customDrones)
{
    /* Transfert des variables du constructeur à la classe */
    m_id_drone = id_drone;
    m_customDrones = customDrones;

    /* Création d'un modèle pour la liste des drones */
    model = new QStandardItemModel();

    /* On créer un espace libre dans le tableau pour chaque drone détecté */
    for (int i = 0; i < id_drone.size(); i++)
    {
        /* On créer un objet que l'on va personnalisé */
        item.append(new QStandardItem());

        /* On défini le drone checkable et on lui associe son nom/ID */
        item[i]->setCheckable(true);
        item[i]->setText(id_drone[i] + " - " + model_drone[i]);
        item[i]->setCheckState(Qt::Checked);

        /* On insère l'objet dans le modèle */
        model->setItem(i, item[i]);
    }

    /* Connexion à une méthode lorsque que l'on clique sur la comboBox d'une ligne */
    connect(model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(checkBoxList(QStandardItem *)));
}

/** Destructeur de la classe */
personnaliser::~personnaliser()
{

}

/** Fonction qui applique le modèle à la vue */
void personnaliser::setModel(QListView *view) {
    view->setModel(model);
}

/** [Slot] Change l'état du drone en "Activé" ou "Désactivé" quand
 * on clique sur la comboBox */
void personnaliser::checkBoxList(QStandardItem *item) {
    if (item->checkState()) {
        (*m_customDrones)[item->row()].isSelected = true;
    }
    else {
        (*m_customDrones)[item->row()].isSelected = false;
    }

}

