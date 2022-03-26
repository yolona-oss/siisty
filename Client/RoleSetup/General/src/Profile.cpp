#include "RoleSetup/General/Profile.hpp"
#include "ui_Profile.h"
#include <qnamespace.h>

#include "Database/Database.hpp"

Profile::Profile(bool HideWapon, QWidget *parent) :
    QWidget(parent),
        ui(new Ui::Profile)
{
    ui->setupUi(this);
    if (HideWapon) {
        ui->waponValue->hide();
        ui->waponLabel->hide();
    }

    QPixmap pi = QPixmap(":icons/noProfileImage");
    ui->personaImage->setPixmap(pi.scaled(218, 218,
                                          Qt::AspectRatioMode::KeepAspectRatio,
                                          Qt::FastTransformation));
    Q_EMIT loadPersonInfo();
}

Profile::~Profile()
{
    delete ui;
}

void
Profile::personInfoLoaded(QJsonObject data)
{
    ui->entryDate->setDate(
        QDateTime::fromSecsSinceEpoch(data["entryDate"].toInteger()).date());
    ui->personaName->setText(data["name"].toString());
    if (data["image"].isString()) {
        if (data["image"].toString() != QString()) {
            ui->personaImage->setPixmap(
                QPixmapFromQString(data["image"].toString())
                    .scaled(218, 218, Qt::AspectRatioMode::KeepAspectRatio,
                            Qt::FastTransformation));
        }
    }
    ui->roleValue->setText(QString::number(data["role"].toInt()));
    ui->loginValue->setText(data["login"].toString());
    if (!ui->waponValue->isHidden()) {
        ui->waponValue->setText(QString::number(data["wapon"].toInt()));
    }
}
