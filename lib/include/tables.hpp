#ifndef TABLES_HPP_URXDMEZB
#define TABLES_HPP_URXDMEZB

#include "Database/Query.hpp"

#include <QMap>
#include <QString>
#include <QDate>

#define TABLES_MAP(XX) \
    XX(  0, AssignedEmployees,     CREATE_ASSIGNED_EMPLOYEES_TABLE )      \
    XX(  1, Accidents,             CREATE_ACCIDENTS_TABLE )               \
    XX(  2, Accounting,            CREATE_ACCOUNTING_TABLE )              \
    XX(  3, AccountingType,        CREATE_ACCOUNTING_TYPE_TABLE )         \
    XX(  4, Contracts,             CREATE_CONTRACTS_TABLE )               \
    XX(  5, DutySchedule,          CREATE_DUTY_SCHEDULE_TABLE )           \
    XX(  6, EmployeesAndCustomers, CREATE_EMPLOYEES_AND_CUSTOMERS_TABLE ) \
    XX(  7, RoleCommands,          CREATE_ROLE_COMMANDS_TABLE )           \
    XX(  8, Role,                  CREATE_ROLES_TABLE )                   \
    XX(  9, Wapons,                CREATE_WAPONS_TABLE )                  \
    XX( 10, objectType,            CREATE_OBJECT_TYPE_TABLE )             \

namespace Database {

#define XX(num, name, query) name = num,
    enum class Tables : int {
        TABLES_MAP(XX)
        TablesCount
    };
#undef XX

} /* Database  */

#endif /* end of include guard: TABLES_HPP_URXDMEZB */
