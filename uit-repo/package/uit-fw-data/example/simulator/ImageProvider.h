#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QQuickImageProvider>
#include <QImage>

class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    static ImageProvider *current();

    ImageProvider();
    ~ImageProvider();


    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    QImage m_img;

signals:
    void refreshImage();

public slots:
    bool setImage(const QString &path);
    bool setImage(const uchar *pData, int width, int height, QImage::Format format);

private:
    static ImageProvider *g_sInstance;
};

#endif // IMAGEPROVIDER_H
