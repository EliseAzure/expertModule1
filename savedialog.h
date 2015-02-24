#ifndef SAVEDIALOG
#define SAVEDIALOG

#include <QDialog>
#include <QLineEdit>

class SaveDialog : public QDialog
{
	Q_OBJECT
private:
	QLineEdit* m_ptxtFirstName;
	QLineEdit* m_ptxtLastName;

public:
	SaveDialog(QWidget* pwgt = 0);

	QString firstName() const;
	QString lastName () const;
};

#endif // SAVEDIALOG

