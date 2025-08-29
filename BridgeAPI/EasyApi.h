#ifndef EASYAPI_H
#define EASYAPI_H
#pragma once
#include <QObject>
#include <QString>
#include <QVariantMap>

class EasyApi : public QObject {
    Q_OBJECT
public:
    explicit EasyApi(QObject* parent=nullptr);

    Q_INVOKABLE static double cap();
    Q_INVOKABLE static double hp();
    Q_INVOKABLE static double mp();
    Q_INVOKABLE static double lvl();
    Q_INVOKABLE static int x();
    Q_INVOKABLE static int y();
    Q_INVOKABLE static int z();

    Q_INVOKABLE static void say(const QString& msg);
    Q_INVOKABLE static void close(const QString& container_name);
    Q_INVOKABLE static void openAll(int index);
    Q_INVOKABLE static void useWith(int item_id, int x, int y, int z);
    Q_INVOKABLE static QVariantList getSpectatorsInRangeEx(int radius);
    Q_INVOKABLE static QVariantMap getTopThing(int x, int y, int z);
    Q_INVOKABLE static QVariant isTargeting();

    Q_INVOKABLE static void sleep(int ms);

    Q_INVOKABLE static void label(const QString& name);
    static std::function<void(const QString&)> onLabel;




};


#endif
