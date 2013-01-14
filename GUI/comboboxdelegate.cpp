#include "comboboxdelegate.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
 : QItemDelegate(parent)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
 const QStyleOptionViewItem &/* option */,
 const QModelIndex &/* index */) const
{
   QComboBox *editor = new QComboBox(parent);
   editor->addItem("None");

   return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
   QString value = index.model()->data(index, Qt::EditRole).toString();

   QComboBox *cBox = static_cast<QComboBox*>(editor);
   cBox->setCurrentIndex(cBox->findText(value));
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
   QComboBox *cBox = static_cast<QComboBox*>(editor);
   QString value = cBox->currentText();

   model->setData(index, value, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
 const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
   editor->setGeometry(option.rect);
}
