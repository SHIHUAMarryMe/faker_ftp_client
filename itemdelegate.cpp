
#include <QPainter>
#include <QImage>
#include <QDebug>

#include "itemdef.h"
#include "itemdelegate.h"


ItemDelegate::ItemDelegate(QObject * const parent)
    :QStyledItemDelegate{parent}
{
}


ItemDelegate::~ItemDelegate()
{
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();

        QVariant variant{ index.data(Qt::UserRole+1) };
        ItemData data{ variant.value<ItemData>() };

        QStyleOptionViewItem viewOption(option);

        QRectF rect{};
        rect.setX(viewOption.rect.x());
        rect.setY(viewOption.rect.y());
        rect.setWidth(viewOption.rect.width()-1);
        rect.setHeight(viewOption.rect.height()-1);

        QRectF icon_rect{ rect.x() + 29.5, rect.y() + 29.5, 32.0, 32.0 };
        painter->drawImage(icon_rect, QImage{data.icon_path_str_});


        painter->restore();
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)option;
    (void)index;

    return QSize{90, 90};
}
