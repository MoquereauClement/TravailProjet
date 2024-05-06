#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QtNetwork>
#include <QPixmap>
#include <QToolButton>
#include <QMap>

class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloader(QObject *parent = nullptr);
    void downloadImage(const QUrl &imageUrl, QToolButton *toolButton);
signals:
    void imageDownloaded(const QPixmap &pixmap, QToolButton *toolButton);
private slots:
    void onDownloadFinished(QNetworkReply *reply);
private:
    QNetworkAccessManager manager;
    QMap<QNetworkReply*, QToolButton*> replyButtonMap; // Map pour associer les réponses aux boutons
};

#endif // IMAGEDOWNLOADER_H
