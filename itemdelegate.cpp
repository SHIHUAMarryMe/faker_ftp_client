
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

        QStyleOptionViewItem view_option(option);

        QRectF rect{};
        rect.setX(view_option.rect.x());
        rect.setY(view_option.rect.y());
        rect.setWidth(view_option.rect.width()-1);
        rect.setHeight(view_option.rect.height()-1);

        QRectF icon_rect{ rect.x() + 29.5, rect.y() + 29.5, 32.0, 32.0 };
        painter->drawImage(icon_rect, QImage{data.icon_path_str_});

        QRectF text_rect{ rect.x(), icon_rect.y()+60.0, rect.width(), 20.0 };
        painter->drawText(text_rect, data.name_);

        painter->restore();
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)option;
    (void)index;

    return QSize{160, 90};
}
