#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <QMainWindow>

namespace Ui {
class Evolution;
}

class Evolution : public QMainWindow
{
    Q_OBJECT

public:
    explicit Evolution(QWidget *parent = 0);
    ~Evolution();

private slots:
    void on_pushButton_pressed();

    void on_spinBox_from_valueChanged(int arg1);

    void on_checkBox_wolves_toggled(bool checked);

    void on_spinBox_to_valueChanged(int arg1);

private:
    Ui::Evolution *ui;
    void calculateStats();
};

#endif // EVOLUTION_H
