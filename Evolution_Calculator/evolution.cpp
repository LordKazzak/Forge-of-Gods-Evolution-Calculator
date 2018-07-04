#include "evolution.h"
#include "ui_evolution.h"
#include "qdebug.h"

Evolution::Evolution(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Evolution)
{
    ui->setupUi(this);

    ui->groupBox_s2c2->setEnabled(false);
    ui->groupBox_s3c2->setEnabled(false);
    ui->label_EM1->hide();
    ui->comboBox_c1evo1->hide();
    ui->label_EM2->hide();
    ui->comboBox_c2evo1->hide();
    ui->label_EM3->hide();
    ui->comboBox_evo2->hide();
    //ui->groupBox_c2->setEnabled(false);
    //ui->groupBox_c3->setEnabled(false);
    //ui->groupBox_c4->setEnabled(false);
}

Evolution::~Evolution()
{
    delete ui;
}

void Evolution::calculateStats()
{
    int elite[4];
    elite[0] = 30;
    elite[1] = 60;
    // Wolves require less to become elite, because they don't need a secondary monster in evolution from
    if(ui->checkBox_wolves->isChecked())
    {
        elite[2] = 121;
        elite[3] = 271;
    }
    else
    {
        elite[2] = 151;
        elite[3] = 331;
    }

    int start = ui->spinBox_from->value();
    int end = ui->spinBox_to->value();

    QList<QSpinBox*> main_evol = ui->groupBox_s2c1->findChildren<QSpinBox*>();
    main_evol.append(ui->groupBox_s2c2->findChildren<QSpinBox*>());
    main_evol.append(ui->groupBox_s2c3->findChildren<QSpinBox*>());
    main_evol.append(ui->groupBox_s2c4->findChildren<QSpinBox*>());

    QList<QComboBox*> evo_status = ui->groupBox_s3c1->findChildren<QComboBox*>();
    evo_status.append(ui->groupBox_s3c2->findChildren<QComboBox*>());
    evo_status.append(ui->groupBox_s4->findChild<QComboBox*>());

    QList<bool> max_level;
    max_level.append(ui->checkBox_c1megaEvolve->isChecked());
    max_level.append(ui->checkBox_c2megaEvolve->isChecked());
    max_level.append(ui->checkBox_c3megaEvolve->isChecked());
    max_level.append(ui->checkBox_c4megaEvolve->isChecked());

    foreach(QSpinBox *box, main_evol)
    {
        qDebug() << box;
    }

    foreach(QComboBox *box, evo_status)
    {
        qDebug() << box;
    }

    int creatures[4];
    if(1 == start)
    {
        // Completes first stage of evolution.
        for(int i=0; i<pow(2,end-start-1); i++)
        {
            creatures[i] = main_evol[2*i]->value() + main_evol[2*i+1]->value();
            if(max_level[i])
            {
                creatures[i] += 10;
                if(ui->checkBox_evolveEvent->isChecked())
                    creatures[i] += 5;
            }
        }
        if(end > 2) // If it's calculating more than one step.
        {
            for(int i=0; i<pow(2,end-start-2); i++)
            {
                creatures[i] = creatures[2*i] + creatures[2*i+1];
                if(!ui->checkBox_wolves->isChecked()) // Check if I'm calculating for wolves, which don't use extra material.
                {
                    // Add Megaevolved, Elite and Max level bonuses.
                    if(evo_status[i]->currentText() == "Megaevolved")
                        creatures[i] += 20;
                    else if(evo_status[i]->currentText() == "Elite")
                        creatures[i] += 40;
                    else
                        creatures[i] += 10;

                    if(ui->checkBox_evolveEvent->isChecked())
                        creatures[i] += 5;
                }
                else if(ui->checkBox_megaEvolve->isChecked()) // TODO Customized option for each wolf double.
                {
                    creatures[i] += 10;
                    if(ui->checkBox_evolveEvent->isChecked())
                        creatures[i] += 5;
                }
            }
        }
        if(end > 3) // Evolution from scratch.
        {
            creatures[0] = creatures[0] + creatures[1] + 10;
            if(evo_status.first()->currentText() == "Megaevolved")
                creatures[0] += 10;
            else if(evo_status.first()->currentText() == "Elite")
                creatures[0] += 30;

            if(ui->checkBox_evolveEvent->isChecked())
                creatures[0] += 5;
        }

    }
    else if(2 == start)
    {
        for(int i=0; i<pow(2,end-start-1); i++)
        {
            creatures[i] = main_evol[2*i]->value() + main_evol[2*i+1]->value();
            if(!ui->checkBox_wolves->isChecked())
            {
                // Add Megaevolved, Elite and Max level bonuses.
                if(evo_status[i]->currentText() == "Megaevolved")
                    creatures[i] += 20;
                else if(evo_status[i]->currentText() == "Elite")
                    creatures[i] += 40;
                else
                    creatures[i] += 10;

                if(ui->checkBox_evolveEvent->isChecked())
                    creatures[i] += 5;
            }
            else if(ui->checkBox_megaEvolve->isChecked()) // TODO Customized option for each wolf double.
            {
                creatures[i] += 10;
                if(ui->checkBox_evolveEvent->isChecked())
                    creatures[i] += 5;
            }
        }
        if(end > 3)
        {
            creatures[0] = creatures[0] + creatures[1] + 10;
            if(evo_status.first()->currentText() == "Megaevolved")
                creatures[0] += 10;
            else if(evo_status.first()->currentText() == "Elite")
                creatures[0] += 30;

            if(ui->checkBox_evolveEvent->isChecked())
                creatures[0] += 5;
        }
    }
    else if(3 == start)
    {
        creatures[0] = main_evol[6]->value() + main_evol[7]->value() + 10;
        if(evo_status.first()->currentText() == "Megaevolved")
            creatures[0] += 10;
        else if(evo_status.first()->currentText() == "Elite")
            creatures[0] += 30;

        if(ui->checkBox_evolveEvent->isChecked())
            creatures[0] += 5;
    }
    else
    {
        qDebug() << "Error. Attempted to start at " << start;
    }

    /*for(int i=0; i<pow(2,end-start); i++)
    {
        if(start == 1 && i == 0) // If the calculation starts at first stage that doesn't require special materials/max level creatures.
        {

        }
    }

    for(int i=start; i<end; i++)
    {
        for(int j=0; j<pow(2,end-start); j++)
        result += main1 + main2 + evo1;

        // If creatures are max level, apply megaevolve bonus and event bonus, if it's active.
        if(i==1 && ui->checkBox_megaEvolve->isChecked() || i!=1)
        {
            result += 10;
            if(ui->checkBox_evolveEvent->isChecked())
                result += 5;
        }
        main1 = main2 = result; // Result from previous stage is material in the next.
    }*/

    QString status;
    if(creatures[0] >= elite[end-1]) // If creature has enough % for elite status.
        status = "Elite";
    else if(creatures[0] == 0)
        status = "Basic";
    else
        status = "Megaevolved";

    ui->label_missing->setText("Evolves to " + QString::number(creatures[0]) + "% (" + status + "). Requirement for elite is " + QString::number(elite[end-1]) + "%.");
}

void Evolution::on_pushButton_pressed()
{
    calculateStats();
}

void Evolution::on_spinBox_from_valueChanged(int start)
{
    int end = ui->spinBox_to->value();

    // Megaevolve is irrelevant for non-wolf stage 2 evolutions, since all creatures must be maximum level.
    if(start == 1 || start == 2 && ui->checkBox_wolves->isChecked())
        ui->checkBox_megaEvolve->setEnabled(true);
    else
        ui->checkBox_megaEvolve->setEnabled(false);

    // Keep value of "to" valid when changing "from".
    if(end <= start)
        ui->spinBox_to->setValue(start+1);

    QList<QCheckBox*> megaEvolve = ui->groupBox_s4->findChildren<QCheckBox*>();
    if(start != 1)
    {
        foreach(QCheckBox *box, megaEvolve)
            box->setEnabled(false);
    }
    else
    {
        foreach(QCheckBox *box, megaEvolve)
            box->setEnabled(true);
    }

    // Disable unnecessary groupBoxes.
    if(end - start == 3) // Full evoulution. Everything enabled.
    {
        ui->groupBox_s2c2->setEnabled(true);
        ui->groupBox_s3c2->setEnabled(true);
        ui->label_EM1->show();
        ui->comboBox_c1evo1->show();
        ui->label_EM2->show();
        ui->comboBox_c2evo1->show();
        ui->label_EM3->show();
        ui->comboBox_evo2->show();
    }
    else if(end - start == 2) // 2 Stages of evolution.
    {
        ui->groupBox_s2c2->setEnabled(true);
        ui->groupBox_s3c2->setEnabled(false);
        ui->label_EM1->show();
        ui->comboBox_c1evo1->show();
        ui->label_EM2->hide();
        ui->comboBox_c2evo1->hide();
        ui->label_EM3->hide();
        ui->comboBox_evo2->hide();
    }
    else if(end - start == 1) // A single stage.
    {
        ui->groupBox_s2c2->setEnabled(false);
        ui->groupBox_s3c2->setEnabled(false);
        if(start == 1 || start == 2 && ui->checkBox_wolves->isChecked())
        {
            ui->label_EM1->hide();
            ui->comboBox_c1evo1->hide();
        }
        else
        {
            ui->label_EM1->show();
            ui->comboBox_c1evo1->show();
        }
        ui->label_EM2->hide();
        ui->comboBox_c2evo1->hide();
        ui->label_EM3->hide();
        ui->comboBox_evo2->hide();
    }
}

void Evolution::on_spinBox_to_valueChanged(int end)
{
    int start = ui->spinBox_from->value();

    // Megaevolve is irrelevant for non-wolf stage 2 evolutions, since all creatures must be maximum level.
    if(start == 1 || start == 2 && ui->checkBox_wolves->isChecked())
        ui->checkBox_megaEvolve->setEnabled(true);
    else
        ui->checkBox_megaEvolve->setEnabled(false);

    // Keep value of "to" valid when changing "from".
    if(end <= start)
        ui->spinBox_from->setValue(end-1);

    // Disable unnecessary groupBoxes.
    if(end - start == 3) // Full evoulution. Everything enabled.
    {
        ui->groupBox_s2c2->setEnabled(true);
        ui->groupBox_s3c2->setEnabled(true);
        ui->label_EM1->show();
        ui->comboBox_c1evo1->show();
        ui->label_EM2->show();
        ui->comboBox_c2evo1->show();
        ui->label_EM3->show();
        ui->comboBox_evo2->show();
    }
    else if(end - start == 2) // 2 Stages of evolution.
    {
        ui->groupBox_s2c2->setEnabled(true);
        ui->groupBox_s3c2->setEnabled(false);
        ui->label_EM1->show();
        ui->comboBox_c1evo1->show();
        ui->label_EM2->hide();
        ui->comboBox_c2evo1->hide();
        ui->label_EM3->hide();
        ui->comboBox_evo2->hide();
    }
    else if(end - start == 1) // A single stage.
    {
        ui->groupBox_s2c2->setEnabled(false);
        ui->groupBox_s3c2->setEnabled(false);
        if(start == 1 || start == 2 && ui->checkBox_wolves->isChecked())
        {
            ui->label_EM1->hide();
            ui->comboBox_c1evo1->hide();
        }
        else
        {
            ui->label_EM1->show();
            ui->comboBox_c1evo1->show();
        }
        ui->label_EM2->hide();
        ui->comboBox_c2evo1->hide();
        ui->label_EM3->hide();
        ui->comboBox_evo2->hide();
    }
}

void Evolution::on_checkBox_wolves_toggled(bool checked)
{
    if(checked)
    {
        ui->comboBox_c1evo1->setEnabled(false);
        ui->comboBox_c2evo1->setEnabled(false);
    }
    else
    {
        ui->comboBox_c1evo1->setEnabled(true);
        ui->comboBox_c2evo1->setEnabled(true);
    }
}
