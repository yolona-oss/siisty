#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QComboBox>
#include <QThread>
#include <QLabel>
#include <QStatusBar>
#include <QListWidget>
#include <QFileInfo>
#include <QScopedPointer>
#include <QMainWindow>
#include <QDockWidget>
#include <QVBoxLayout>

#include <initializer_list>

#include "Database/Database.hpp"
#include "Database/Role.hpp"
#include "General/PagesManager.hpp"
#include "General/ClickableLabel.hpp"
#include "Pages/WelcomePage.hpp"
#include "Pages/ControlPannel.hpp"
#include "Pages/ControlPannel/ServerInfoPage.hpp"
#include "Pages/ControlPannel/ConnectionsPage.hpp"
#include "Pages/Users.hpp"
#include "Pages/Users/RegisterUser.hpp"
#include "Pages/Users/UsersList.hpp"
#include "Widgets/Progresses.hpp"

#include "General/Matrix.hpp"
#include "General/logger.hpp"
#include "SslServer/iiServer.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

struct Settings {
    bool useSsl = false;

    QString certPath = "./cert.pem";
    QString caPath = "./ca.pem";
    QString keyPath = "./key.pem";

    QHostAddress serveAddress = QHostAddress::AnyIPv4;
    quint16 servePort = 2142;

    QString databasePath = "./database.db";

    int logginLeve = LoggingLevel::Trace;
    QString logFile = "./log";

    QString defultPage = "Main";

    int maxThreads = QThread::idealThreadCount();
    int maxPendingConnections = 100;
};

class Controller : public QMainWindow
{
    Q_OBJECT

    public:
        Controller(Settings settings, QWidget *parent = nullptr);

        virtual ~Controller();

        /* static int newProgressUid(); */

    Q_SIGNALS:
        void resized(QResizeEvent*);
        void setProgress(int, int, QString, int);
        void send_to_log(QString, int);

    public Q_SLOTS:
        void changeServeAddress(QHostAddress&, quint16);
        void changeLoggingLeve(int level);

    private Q_SLOTS:
        void setupPages();
        void setupLogger();
        void setupDatabase();
        void setupServer();

        void on_listeningStateChanged(QHostAddress, quint16, bool);
        void on_actionQuit_triggered();
        void on_actionToggle_server_triggered();

        void on_databaseError(Database::CmdError);
            // command exec error
        void on_databaseInited();
        void on_databaseInitializationFailed(QSqlError e);

        void on_pathNodeClicked();
            // draw path
        void changePage(QString);

        void changeIndicatorState(QHostAddress, quint16, bool);
        void logMessage(QString, int = LoggingLevel::Trace);

        void on_clearLogClicked();

        virtual void resizeEvent(QResizeEvent*);

    private:
        enum class NavPages {
            Main = 0,
            ControlPannel,
            Users,
            COUNT,
        };

    private:
        Settings _settings;

        QThread _databaseThread;
        QThread _loggingThread;
        QThread _serverThread;
        iiServer _server;
        logger   _log;
        Database::Driver _database;

        pSetProgress * _pBars;
        
        int im1 = 200;
        int im2 = 100;
        int im3 = 200;
        int im4 = 50;

        int i1 = -1;
        int i2 = -1;
        int i3 = -1;
        int i4 = -1;

        QTimer _timer1;
        QTimer _timer2;
        QTimer _timer3;
        QTimer _timer4;

        QWidget * _listenIndicator;
        Ui::Controller * ui;
};
#endif // SERVERMANAGER_H