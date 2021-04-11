#ifndef PREFERENCESETTINGSDIALOG_H
#define PREFERENCESETTINGSDIALOG_H

#include <QWidget>

namespace Ui {
class PreferenceSettingsDialog;
}

class PreferenceSettingsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit PreferenceSettingsDialog(QWidget *parent = nullptr);
    ~PreferenceSettingsDialog();

private slots:
    void on_comboBox_activated(const QString &arg1);

private:
    Ui::PreferenceSettingsDialog *ui;
};

#endif // PREFERENCESETTINGSDIALOG_H
