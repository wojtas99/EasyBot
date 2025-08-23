#ifndef EASYAPI_H
#define EASYAPI_H
#pragma once
#include <QObject>
#include <QString>

class EasyApi : public QObject {
    Q_OBJECT
public:
    explicit EasyApi(QObject* parent=nullptr);

    Q_INVOKABLE double cap() const;
    Q_INVOKABLE double hp() const;
    Q_INVOKABLE double max_hp() const;
    Q_INVOKABLE double mp() const;
    Q_INVOKABLE double max_mp() const;
    Q_INVOKABLE double lvl() const;
    Q_INVOKABLE double lvl_pc() const;
    Q_INVOKABLE int x() const;
    Q_INVOKABLE int y() const;
    Q_INVOKABLE int z() const;

    Q_INVOKABLE void say(const QString& msg);
    Q_INVOKABLE void close(const QString& container_name);

    Q_INVOKABLE void sleep(int ms);

    Q_INVOKABLE void label(const QString& name);
    static std::function<void(const QString&)> onLabel;




};


#endif
