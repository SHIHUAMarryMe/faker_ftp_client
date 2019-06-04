#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H


#include <QModelIndex>
#include <QStandardItemModel>
#include <QStyledItemDelegate>


class ItemDelegate final : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ItemDelegate(QObject* const parent = nullptr);
    ItemDelegate(const ItemDelegate&) = delete;
    ItemDelegate& operator=(const ItemDelegate&) = delete;

    virtual ~ItemDelegate();

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;


};


#endif // ITEMDELEGATE_H
