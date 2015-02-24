#include "SaveDialog.h"
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

SaveDialog::SaveDialog(QWidget* pwgt) : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	m_ptxtFirstName = new QLineEdit;
	m_ptxtLastName  = new QLineEdit;

	QLabel* plblFirstName    = new QLabel("&First Name");
	QLabel* plblLastName     = new QLabel("&Last Name");

	plblFirstName->setBuddy(m_ptxtFirstName);
	plblLastName->setBuddy(m_ptxtLastName);

	QPushButton* pcmdOk     = new QPushButton("&Ok");
	QPushButton* pcmdCancel = new QPushButton("&Cancel");

	connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
	connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

	//Layout setup
	QGridLayout* ptopLayout = new QGridLayout;
	ptopLayout->addWidget(plblFirstName, 0, 0);
	ptopLayout->addWidget(plblLastName, 1, 0);
	ptopLayout->addWidget(m_ptxtFirstName, 0, 1);
	ptopLayout->addWidget(m_ptxtLastName, 1, 1);
	ptopLayout->addWidget(pcmdOk, 2,0);
	ptopLayout->addWidget(pcmdCancel, 2, 1);
	setLayout(ptopLayout);
}

QString SaveDialog::firstName() const
{
	return m_ptxtFirstName->text();
}

QString SaveDialog::lastName() const
{
	return m_ptxtLastName->text();
}
