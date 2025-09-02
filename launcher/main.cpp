// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2022 Sefa Eyeoglu
 *  (voir licences dans le fichier original)
 */

#include "Application.h"

#include <QApplication>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QFile>
#include <QIcon>
#include <QDir>

// #define BREAK_INFINITE_LOOP
// #define BREAK_EXCEPTION
// #define BREAK_RETURN

#ifdef BREAK_INFINITE_LOOP
#include <chrono>
#include <thread>
#endif

static void applyYumeBranding(QApplication& app)
{
    // Identité
    QCoreApplication::setOrganizationName("Yume");
    QCoreApplication::setOrganizationDomain("yume.gg");
    QCoreApplication::setApplicationName("Yume Launcher");
    QApplication::setApplicationDisplayName("Yume Launcher");

    // 1) Icône (ressource -> fallback disque)
    QIcon appIcon(":/yume/icons/yume.svg");
    if (appIcon.isNull()) {
        const QString diskIcon = QCoreApplication::applicationDirPath() + "/yume/icons/yume.svg";
        if (QFile::exists(diskIcon))
            appIcon = QIcon(diskIcon);
    }
    if (!appIcon.isNull())
        QApplication::setWindowIcon(appIcon);

    // 2) Thème (ressource -> fallback disque)
    bool themed = false;
    {
        QFile f(":/yume/style.qss");
        if (f.open(QIODevice::ReadOnly)) {
            app.setStyleSheet(QString::fromUtf8(f.readAll()));
            themed = true;
        }
    }
    if (!themed) {
        const QString diskQss = QCoreApplication::applicationDirPath() + "/yume/style.qss";
        QFile f(diskQss);
        if (f.open(QIODevice::ReadOnly)) {
            app.setStyleSheet(QString::fromUtf8(f.readAll()));
        }
    }
}

int main(int argc, char* argv[])
{
#ifdef BREAK_INFINITE_LOOP
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
#endif
#ifdef BREAK_EXCEPTION
    throw 42;
#endif
#ifdef BREAK_RETURN
    return 42;
#endif

#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    // initialize Qt
    Application app(argc, argv);

    switch (app.status()) {
        case Application::StartingUp:
        case Application::Initialized: {
            // Ressources Prism existantes
            Q_INIT_RESOURCE(multimc);
            Q_INIT_RESOURCE(backgrounds);
            Q_INIT_RESOURCE(documents);
            Q_INIT_RESOURCE(prismlauncher);

            Q_INIT_RESOURCE(pe_dark);
            Q_INIT_RESOURCE(pe_light);
            Q_INIT_RESOURCE(pe_blue);
            Q_INIT_RESOURCE(pe_colored);
            Q_INIT_RESOURCE(breeze_dark);
            Q_INIT_RESOURCE(breeze_light);
            Q_INIT_RESOURCE(OSX);
            Q_INIT_RESOURCE(iOS);
            Q_INIT_RESOURCE(flat);
            Q_INIT_RESOURCE(flat_white);

            Q_INIT_RESOURCE(shaders);

            // ⚠️ IMPORTANT : PAS DE Q_INIT_RESOURCE(yume) ici.
            // On utilise le fallback fichiers copiés à côté de l'exe.

            applyYumeBranding(app);
            return app.exec();
        }
        case Application::Failed:
            return 1;
        case Application::Succeeded:
            return 0;
        default:
            return -1;
    }
}
