#ifndef USERSLIST_H
#define USERSLIST_H

#include <QWidget>
#include <QTableWidgetItem>
#include "Database/Database.hpp"
#include "Logger/logger.hpp"

namespace Ui {
  class UsersList;
}

class UsersList : public QWidget {
    Q_OBJECT

    private:
        enum Columns {
            ID = 0,
            NAME,
            ROLE,
            COLUMNS
        };

    public:
     explicit UsersList(const QMap<Database::RoleId, Database::Driver::role_set>& roles,
                        QWidget *parent = nullptr);
     ~UsersList();

    Q_SIGNALS:
        void requestedUsers(Database::DatabaseCmd);

        void logMessage(QString, int = LoggingLevel::Trace);

    public Q_SLOTS:

    private Q_SLOTS:
        void requestUsers();

        void clearTable();

        void on_usersLoaded(QJsonObject);
        void on_usersLoadError(Database::CmdError);
        void on_userLoaded(QJsonObject);
        void on_userLoadError(Database::CmdError);

        void on_userClicked(QTableWidgetItem*);

    private:
        Database::DriverAssistant * _usersWaiter;
        Database::DriverAssistant * _userWaiter;

        const QMap<Database::RoleId, Database::Driver::role_set>& _roles;

        Ui::UsersList *ui;
};

#endif // USERSLIST_H
