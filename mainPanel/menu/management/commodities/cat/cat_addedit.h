#ifndef CAT_ADDEDIT_H
#define CAT_ADDEDIT_H

#include <QDialog>
class DanetDbMan;
namespace Commodity
{
namespace Cat
{
	namespace Ui {
	class AddEdit;
	}

	class AddEdit : public QDialog
	{
	    Q_OBJECT

	public:
	    explicit AddEdit(DanetDbMan *db, QWidget *parent = nullptr);
	    ~AddEdit();
	    void setValues(int ID, QString CAT);

	private slots:
	    void on_okBtn_clicked();

	private:
	    Ui::AddEdit *ui;
	    int id;
	    QString cat;
	    DanetDbMan *dbMan;
	};
}
}
#endif // CAT_ADDEDIT_H
