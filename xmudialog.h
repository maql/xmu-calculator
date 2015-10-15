#ifndef XMUDIALOG_H
#define XMUDIALOG_H

#include <QDialog>
#include <QString>


class XmuData;
class QLineEdit;
class QLabel;
class QPushButton;
class QDialogButtonBox;

class XmuDialog : public QDialog
{
    Q_OBJECT
    
public:
    XmuDialog(QWidget *parent = 0);
    ~XmuDialog();
    double getXmu() const;
    
private:
    void onTextChange(const QString &text);
    double calculateMass(const QString &formula, QString &disp);
    double calculateReducedMass(const QString &input);
    void copyToClipboard();
    QLineEdit *lineEdit;
    QLabel *xmuLabel, *formulaLabel;
    QPushButton *copyButton;
    QDialogButtonBox *buttonBox;
    XmuData *xmuData;
    double xmu;
    QString displayFormula;
};

#endif
