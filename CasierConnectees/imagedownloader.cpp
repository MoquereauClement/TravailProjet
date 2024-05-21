#include "imagedownloader.h"

ImageDownloader::ImageDownloader(QObject *parent)
    : QObject{parent}
{
    connect(&manager, &QNetworkAccessManager::finished, this, &ImageDownloader::onDownloadFinished);
}

void ImageDownloader::downloadImage(const QUrl &imageUrl, QToolButton *toolButton)
{
    QNetworkRequest request(imageUrl);
    QNetworkReply *reply = manager.get(request);
    // Associer le reply au bouton dans la map
    replyButtonMap.insert(reply, toolButton);
}

void ImageDownloader::onDownloadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        QToolButton *toolButton = replyButtonMap.value(reply); // Récupérer le bouton associé
        if (toolButton) {
            emit imageDownloaded(pixmap, toolButton);
        } else {
            qDebug() << "Failed to find associated tool button";
        }
    } else {
        qDebug() << "Failed to download image:" << reply->errorString();
    }
    reply->deleteLater();
    // Supprimer l'entrée de la map après que le reply est traité
    replyButtonMap.remove(reply);
}
