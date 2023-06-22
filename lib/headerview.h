#ifndef HEADERVIEW_H
#define HEADERVIEW_H

#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QPointer>
#include <QPainter>
#include <QPainterPath>
#include <QFont>

#include <QDebug>

class HeaderSectionWidget : public QWidget
{
public:
    QLabel *lable;
    QLineEdit *lineEdit;
    QComboBox *comboBox;
    HeaderSectionWidget(QWidget *parent = 0) : QWidget(parent)
    {
        QFont font;
        font.setFamily(QStringLiteral("Tahoma"));
        font.setPixelSize(14);
        font.setBold(true);

        lable = new QLabel(this);
        lineEdit = new QLineEdit(this);
        comboBox = new QComboBox(this);
        lable->setFont(font);
        lineEdit->setFont(font);
        comboBox->setFont(font);
        lable->setStyleSheet(QStringLiteral("color: rgb(0, 0, 127);"));
        lineEdit->setStyleSheet(QStringLiteral("color: rgb(85, 0, 127);"));
        comboBox->setStyleSheet(QStringLiteral("color: rgb(85, 0, 127);"));
        comboBox->setFixedHeight(30);
        lineEdit->setFixedHeight(30);

        QVBoxLayout *vl = new QVBoxLayout(this);
        vl->addWidget(lable, 1, Qt::AlignCenter);
        vl->addWidget(lineEdit,1, Qt::AlignCenter);
        vl->addWidget(comboBox,1, Qt::AlignCenter);
        setLayout(vl);
    }
};


class HeaderView : public QHeaderView
{
    Q_OBJECT

public:
    explicit HeaderView(int colCount,int baseHeaderSizeColumnIndex, QWidget* parent):QHeaderView(Qt::Horizontal, parent)
    {
        columnCount = colCount;
        baseHeaderSizeColumn = baseHeaderSizeColumnIndex;
        for(int i = 0; i < columnCount; i++)
        {
            sections.insert(i,new HeaderSectionWidget(parent));
            connect(sections[i]->lineEdit, SIGNAL(editingFinished()),this,SIGNAL(sectionsWidgetsSignal()) );
            connect(sections[i]->comboBox, SIGNAL(currentTextChanged(QString)),this, SIGNAL(sectionsWidgetsSignal()));
        }
    }

    QVector< QPointer<HeaderSectionWidget> > sections; //vector of pointers to our widgets

    void setLableText(int section, QString label)
    {
        sections[section]->lable->setText(label);
    }

    void setSectionWidth(int section, int width)
    {
        sections[section]->lable->setFixedWidth(width);
        sections[section]->comboBox->setFixedWidth(width);
    }

    void hideWidgets(int section)
    {
        sections[section]->lable->setEnabled(false);
        sections[section]->lable->hide();
        sections[section]->comboBox->setEnabled(false);
        sections[section]->comboBox->hide();
        sections[section]->lineEdit->setEnabled(false);
        sections[section]->lineEdit->hide();
    }

    void hideLabel(int section){sections[section]->lable->setEnabled(false);sections[section]->lable->hide();}

    void hideLineEdit(int section){sections[section]->lineEdit->setEnabled(false);sections[section]->lineEdit->hide();}

    void hideComboBox(int section){sections[section]->comboBox->setEnabled(false);sections[section]->comboBox->hide();}

    void clearComboBox(const int section){sections[section]->comboBox->clear();}

    void resetHeaderWidgets()
    {
        for(int i = 0; i < columnCount; i++)
        {
            sections[i]->lineEdit->blockSignals(true);
            sections[i]->lineEdit->clear();
            sections[i]->lineEdit->blockSignals(false);
            sections[i]->comboBox->blockSignals(true);
            sections[i]->comboBox->setCurrentIndex(sections[i]->comboBox->findData(-1));
            sections[i]->comboBox->blockSignals(false);
        }
    }

    QString getLineEditText(int section){return sections[section]->lineEdit->text();}

    QVariant getComboBoxCurrentData(int section){return sections[section]->comboBox->currentData();}

    QString getComboBoxCurrentText(int section)
    {
        if(sections[section]->comboBox->currentData().toInt() == 0)//all
            return "";
        else
            return sections[section]->comboBox->currentText();
    }

    void showLabel(int section){sections[section]->lable->setEnabled(true);sections[section]->lable->show();}

    void showLineEdit(int section){sections[section]->comboBox->setEnabled(true);sections[section]->comboBox->show();}

    void showComboBox(int section){sections[section]->lineEdit->setEnabled(true);sections[section]->lineEdit->show();}

    void setLineEditAlignment(int section, Qt::Alignment alignment){sections[section]->lineEdit->setAlignment(alignment);}

    void setLineEditPlaceHolder(int section, QString PH){sections[section]->lineEdit->setPlaceholderText(PH);}

    void setLineEditText(int section, QString data){sections[section]->lineEdit->setText(data);}

    void setLineEditBlockSignal(int section, bool block){sections[section]->lineEdit->blockSignals(block);}

    void setComboBoxCurrentData(int section, QVariant data){sections[section]->comboBox->setCurrentIndex(sections[section]->comboBox->findData(data));}

    void addComboBoxItem(int section, const QString text, const QVariant data){sections[section]->comboBox->addItem(text, data);}

    void setComboBoxLayoutDirection(int section, Qt::LayoutDirection direction){sections[section]->comboBox->setLayoutDirection(direction);}

    void setComboBoxBlockSignal(int section, bool block){sections[section]->comboBox->blockSignals(block);}

    QSize sizeHint() const
    {
        QSize s = size();
        s.setHeight(sections[baseHeaderSizeColumn]->minimumSizeHint().height()+15);
        s.setWidth(sections[baseHeaderSizeColumn]->minimumSizeHint().width()+5);
        return s;
    }

signals:
    void sectionsWidgetsSignal();

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
    {
        //if(sections[logicalIndex]->geometry().x() < 1000)
        //{
            QPainterPath path;
            path.addRect(rect);
            painter->fillPath(path,QBrush(QColor(240,240,240))); //
            //        painter->setPen(QColor(0,0,0));
            QRect rec = rect;
            rec.setHeight(rect.height()-1);
            painter->drawRect(rec);
            if (!rect.isValid())
                return;
            sections[logicalIndex]->setGeometry(rect);
            sections[logicalIndex]->show();
        //}
    }

private:
    int columnCount;
    int baseHeaderSizeColumn;

};


#endif // HEADERVIEW_H
