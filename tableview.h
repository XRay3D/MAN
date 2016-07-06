#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QDebug>
#include <QtEvents>
#include <QTableView>

#include <QtWidgets>

#include <QShortcut>

//class ComboBoxItemDelegate : public QItemDelegate {
//    Q_OBJECT
//public:
//    ComboBoxItemDelegate(QWidget* parent = 0)
//        : QItemDelegate(parent)
//    {
//    }
//    virtual ~ComboBoxItemDelegate() {}

//    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
//    {
//        const QAbstractItemModel* model = index.model();
//        if (!model) {
//            return QItemDelegate::createEditor(parent, option, index);
//        }
//        QComboBox* box = new QComboBox(parent);
//        box->setEditable(true);
//        box->setAutoCompletion(true);
//        box->setModel(const_cast<QAbstractItemModel*>(model));
//        box->setModelColumn(index.column());
//        box->installEventFilter(const_cast<ComboBoxItemDelegate*>(this));
//        return box;
//    }

//    void setEditorData(QWidget* editor, const QModelIndex& index) const
//    {
//        QComboBox* box = qobject_cast<QComboBox*>(editor);
//        const QAbstractItemModel* model = index.model();
//        if (!box || !model) {
//            QItemDelegate::setEditorData(editor, index);
//        }
//        box->setCurrentIndex(index.row());
//    }

//    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
//    {
//        if (!index.isValid()) {
//            return;
//        }
//        QComboBox* box = qobject_cast<QComboBox*>(editor);
//        if (!box) {
//            return QItemDelegate::setModelData(editor, model, index);
//        }
//        model->setData(index, box->currentText(), Qt::DisplayRole);
//        model->setData(index, box->currentText(), Qt::EditRole);
//    }
//};

class LineEditValidatorDelegate : public QItemDelegate {
    Q_OBJECT
public:
    LineEditValidatorDelegate(QWidget* parent = 0)
        : QItemDelegate(parent)
    {
    }
    ~LineEditValidatorDelegate() {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        const QAbstractItemModel* model = index.model();
        if (!model) {
            return QItemDelegate::createEditor(parent, option, index);
        }

        /**/
        QRegExp rx("[Т][П]-?\\d{1,3}");
        QValidator* validator =new QRegExpValidator(rx, parent);
        /**/
        QLineEdit* box = new QLineEdit(parent);
        box->setValidator(validator);
        //        box->setEditable(true);
        //        box->setAutoCompletion(true);
        //        box->setModel(const_cast<QAbstractItemModel*>(model));
        //        box->setModelColumn(index.column());
        box->installEventFilter(const_cast<LineEditValidatorDelegate*>(this));
        return box;
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        QLineEdit* box = qobject_cast<QLineEdit*>(editor);
        const QAbstractItemModel* model = index.model();
        if (!box || !model) {
            QItemDelegate::setEditorData(editor, index);
        }
        box->setText(index.data().toString());
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        if (!index.isValid()) {
            return;
        }
        QLineEdit* box = qobject_cast<QLineEdit*>(editor);
        if (!box) {
            return QItemDelegate::setModelData(editor, model, index);
        }
        model->setData(index, box->text(), Qt::DisplayRole);
        model->setData(index, box->text(), Qt::EditRole);
    }
};
class TableView : public QTableView {
    Q_OBJECT
public:
    explicit TableView(QWidget* parent = 0)
        : QTableView(parent)
    {
        //        this->setItemDelegate(new AbstractItemDelegate(this));
        this->setItemDelegateForColumn(0, new LineEditValidatorDelegate(this));
    }

    //private:
    //    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
    //    {
    //        // Just creates a plain line edit.
    //        QLineEdit* editor = new QLineEdit(parent);
    //        return editor;
    //    }

    //    void setEditorData(QWidget* editor, const QModelIndex& index) const
    //    {
    //        // Fetch current data from model.
    //        QString value = index.model()->data(index, Qt::EditRole).toString();

    //        // Set line edit text to current data.
    //        QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    //        lineEdit->setText(value);

    //        // Deselect text.
    //        lineEdit->deselect();

    //        // Move the cursor to the beginning.
    //        lineEdit->setCursorPosition(0);
    //    }

    //    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    //    {
    //        // Set the model data with the text in line edit.
    //        QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    //        QString value = lineEdit->text();
    //        model->setData(index, value, Qt::EditRole);
    //    }
};

#endif // TABLEVIEW_H
