#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QRegularExpression>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QApplication>
#include <QClipboard>

#include "xmudialog.h"
#include "xmudata.h"


const double Invalid = -1.0;


XmuDialog::XmuDialog(QWidget *parent) : QDialog(parent)
{
    lineEdit = new QLineEdit(this);
    connect(lineEdit, &QLineEdit::textChanged,
            this, &XmuDialog::onTextChange);

    QLabel *helpLabel = new QLabel(
        tr("Please input formula(s) below ([m]Xn for <big><sup>m</sup>X<sub>n</sub></big>):<br>"
           "&nbsp;&nbsp;&nbsp;&nbsp;<i>e.g.</i> [13]CH3 D2<br>"
           "<font color='gray'>Formulas are separated in spaces.<br>"
           "Please use D and T for heavy isotopes of hydrogen.</font>"),
        this);

    xmuLabel = new QLabel(this);
    formulaLabel = new QLabel(this);

    copyButton = new QPushButton(tr("Copy to Clipboard"));
    connect(copyButton, &QPushButton::clicked,
            this, &XmuDialog::copyToClipboard);

    buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal);
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &QDialog::accept);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &QDialog::reject);

    QHBoxLayout *editLayout = new QHBoxLayout;
    editLayout->addWidget(lineEdit);
    editLayout->addWidget(xmuLabel);

    QHBoxLayout *formulaLayout = new QHBoxLayout;
    formulaLayout->addWidget(formulaLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(copyButton);
    buttonLayout->addSpacing(50);
    buttonLayout->addStretch();
    buttonLayout->addWidget(buttonBox);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(helpLabel);
    mainLayout->addSpacing(25);
    mainLayout->addLayout(editLayout);
    mainLayout->addLayout(formulaLayout);
    mainLayout->addSpacing(25);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("Reduced Mass Calculator"));
    onTextChange(lineEdit->text());
    setFixedHeight(sizeHint().height());

    xmuData = new XmuData(":/nist-atom-weight");
}


XmuDialog::~XmuDialog() {}


double XmuDialog::getXmu() const
{
    return xmu;
}


void XmuDialog::onTextChange(const QString &text)
{
    if (text.isEmpty())
    {
        xmuLabel->setText(tr("<b>0</b>"));
        formulaLabel->setText("<big>Waiting for input...</big>");
        return;
    }
    if (calculateReducedMass(text) != Invalid)
    {
        xmuLabel->setText(tr("<b>%1</b>").arg(QString::number(xmu, 'g', 12)));
        formulaLabel->setText(tr("<big>Formulas: %1</big>").
            arg(displayFormula));
    }
    else
    {
        xmuLabel->setText(tr("<b>0</b>"));
        formulaLabel->setText("<big>Invalid input.</big>");
    }
}


double XmuDialog::calculateMass(const QString &formula, QString &disp)
{
    disp = "";
    if (formula.isEmpty()) return 0.0;
    static QRegularExpression isotopeRegExp(
        "(\\[[1-9][0-9]{0,2}\\])?([A-Z][a-z]{0,2})([0-9]+)?"
    );
    int pos = 0, imass, count;
    double mass = 0.0, massIsotope;
    QString symbol, imassString;
    QRegularExpressionMatch match;

    while (pos < formula.size())
    {
        if (formula.indexOf(isotopeRegExp, pos, &match) != pos) {
            return Invalid;
        }

        pos += match.capturedLength();
        symbol = match.captured(2);

        imassString = match.captured(1);
        if (imassString.isEmpty()) {
            imass = xmuData->getDefaultImass(symbol);
        } else {
            imassString = imassString.mid(1, imassString.size() - 2);
            imass = imassString.toInt();
        }

        if (match.captured(3).isEmpty()) {
            count = 1;
        } else {
            count = match.captured(3).toInt();
        }

        massIsotope = xmuData->getXmu(imass, symbol);
        if (massIsotope == 0.0) return Invalid;
        mass += massIsotope * count;
        disp.append(tr("<sup>%1</sup>%2").arg(imass).arg(symbol));
        if (count != 1) disp.append(tr("<sub>%3</sub>").arg(count));
    }
    return mass;
}


double XmuDialog::calculateReducedMass(const QString &input)
{
    QStringList formulas = input.split(' ');
    QString currentDisplayFormula;
    double *masses = new double[formulas.size()];
    xmu = 0.0;
    displayFormula = "";

    for (int i = 0; i < formulas.size(); ++i)
    {
        masses[i] = calculateMass(formulas[i], currentDisplayFormula);
        if (masses[i] == Invalid)
        {
            xmu = Invalid;
            break;
        }
        else if (masses[i] != 0.0)
        {
            xmu += 1.0 / masses[i];
            if (!displayFormula.isEmpty())
                displayFormula.append(tr(" <font color='gray'>and</font> "));
            displayFormula.append(currentDisplayFormula);
        }
    }

    if (xmu != Invalid && xmu != 0.0) xmu = 1.0 / xmu;
    delete[] masses;
    return xmu;
}


void XmuDialog::copyToClipboard()
{
    QApplication::clipboard()->setText(QString::number(xmu, 'g', 12));
}
