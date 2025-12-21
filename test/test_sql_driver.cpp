/**
 * @file test_sql_driver.cpp
 * @brief 测试SQLite驱动是否可用
 */

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "可用的SQL驱动:";
    QStringList drivers = QSqlDatabase::drivers();
    for (const QString& driver : drivers) {
        qDebug() << "  -" << driver;
    }

    bool hasSQLite = drivers.contains("QSQLITE");
    qDebug() << "\nQSQLITE驱动可用:" << (hasSQLite ? "是" : "否");

    return hasSQLite ? 0 : 1;
}
