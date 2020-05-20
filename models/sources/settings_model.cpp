#include "./models/headers/settings_model.h"

Settings_model::Settings_model(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::settings_name] = "settings_name";
    m_roles[(int)RolesNames::settings_number] = "settings_number";

    m_settings.push_back(std::make_tuple("Change nickname", m_settings.size()));
    m_settings.push_back(std::make_tuple("Change password", m_settings.size()));
    m_settings.push_back(std::make_tuple("Change avatar", m_settings.size()));
}

int Settings_model::rowCount(const QModelIndex& ) const
{
    return m_settings.size();
}

QVariant Settings_model::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_settings.size()) return QVariant();

    switch (role) {

    case (int)RolesNames::settings_name: {
        return std::get<0>(m_settings[row]);
    }

    case (int)RolesNames::settings_number: {
        return std::get<1>(m_settings[row]);
    }

    }

    return QVariant();
}

QHash<int, QByteArray> Settings_model::roleNames() const
{
    return m_roles;
}

