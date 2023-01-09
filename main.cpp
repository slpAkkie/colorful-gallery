#include "screens/GalleryWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>

#define QT_CUSTOM_THEME "dark"

int main(int argc, char *argv[])
{
    // Create an app instance
    QApplication app(argc, argv);

    // Load language according to the system one
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages)
    {
        const QString baseName = QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }

#ifdef QT_CUSTOM_THEME
    // Set theme for the application
    QFile styleFile(":/themes/" QT_CUSTOM_THEME ".qss");
    styleFile.open(QFile::ReadOnly);
    app.setStyleSheet(styleFile.readAll());
#endif

    // Create a GalleryWindow and show it
    GalleryWindow galleryWindow;
    galleryWindow.show();

    // Execute the app
    return app.exec();
}
