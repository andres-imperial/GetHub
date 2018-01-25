#ifndef REPO_H
#define REPO_H

#include <QObject>

class Repo
{

public:
    Repo(){m_name = m_url = m_api = "";}
    Repo(QObject *parent);
    Repo(const QString &name, const QString &url, const QString &api);
    Repo(Repo const&);

    QString name() const;
    void setName(const QString &name);

    QString api() const;
    void setApi(const QString &api);

    QString url() const;
    void setUrl(const QString &url);

    void chop(const uint index);

private:
    QString m_name;
    QString m_url;
    QString m_api;
};

#endif // REPO_H
