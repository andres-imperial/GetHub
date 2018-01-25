#include "repo.h"


Repo::Repo(const QString &name, const QString &url, const QString &api)
    :  m_name(name), m_url(url), m_api(api)
{
}

Repo::Repo(Repo const& other)
{
    this->m_name = other.m_name;
    this->m_url = other.m_url;
    this->m_api = other.m_api;
}

QString Repo::name() const
{
    return m_name;
}


void Repo::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
    }
}

QString Repo::url() const
{
    return m_url;
}

void Repo::setUrl(const QString &url)
{
    if (url != m_url) {
        m_url = url;
    }
}

QString Repo::api() const
{
    return m_api;
}

void Repo::setApi(const QString &api)
{
    if (api != m_api) {
        m_api = api;
    }
}

void Repo::chop(const uint index)
{
    m_api.chop(index);
}
