#ifndef ITEMDEF_H
#define ITEMDEF_H

#include <QObject>
#include <QMetaType>

struct ItemData
{
    QString name_;
    QString icon_path_str_;
};

Q_DECLARE_METATYPE(ItemData)

#endif // ITEMDEF_H
