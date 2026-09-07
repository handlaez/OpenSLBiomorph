#ifndef BIOMORPH_CONTROLLER_HPP
#define BIOMORPH_CONTROLLER_HPP

#include <QObject>
#include <QUrl>
#include <QFile>

#include "BiomorphParameters.hpp"
#include "BiomorphImage.hpp"

class BiomorphWorker;
class QThread;

class BiomorphController : public QObject
{
    Q_OBJECT

        Q_PROPERTY(bool generating
            READ generating
            NOTIFY generatingChanged)

        Q_PROPERTY(QString status
            READ status
            NOTIFY statusChanged)

        // Coordinate range
        Q_PROPERTY(double xmin
            READ xmin
            WRITE setXmin
            NOTIFY xminChanged)

        Q_PROPERTY(double xmax
            READ xmax
            WRITE setXmax
            NOTIFY xmaxChanged)

        Q_PROPERTY(double ymin
            READ ymin
            WRITE setYmin
            NOTIFY yminChanged)

        Q_PROPERTY(double ymax
            READ ymax
            WRITE setYmax
            NOTIFY ymaxChanged)

        // General parameters
        Q_PROPERTY(double alpha
            READ alpha
            WRITE setAlpha
            NOTIFY alphaChanged)

        Q_PROPERTY(double beta
            READ beta
            WRITE setBeta
            NOTIFY betaChanged)

        // Complex constants
        Q_PROPERTY(double gc1x
            READ gc1x
            WRITE setGc1x
            NOTIFY gc1xChanged)

        Q_PROPERTY(double gc1y
            READ gc1y
            WRITE setGc1y
            NOTIFY gc1yChanged)

        Q_PROPERTY(double gc2x
            READ gc2x
            WRITE setGc2x
            NOTIFY gc2xChanged)

        Q_PROPERTY(double gc2y
            READ gc2y
            WRITE setGc2y
            NOTIFY gc2yChanged)

        // Algorithm parameters
        Q_PROPERTY(int K
            READ K
            WRITE setK
            NOTIFY KChanged)

        Q_PROPERTY(double R
            READ R
            WRITE setR
            NOTIFY RChanged)

        Q_PROPERTY(double freq
            READ freq
            WRITE setFreq
            NOTIFY freqChanged)

        Q_PROPERTY(bool useSwitch
            READ useSwitch
            WRITE setUseSwitch
            NOTIFY useSwitchChanged)

public:
    explicit BiomorphController(QObject* parent = nullptr);
    ~BiomorphController();

    bool generating() const;
    QString status() const;

    double xmin() const;
    void setXmin(double value);

    double xmax() const;
    void setXmax(double value);

    double ymin() const;
    void setYmin(double value);

    double ymax() const;
    void setYmax(double value);

    double alpha() const;
    void setAlpha(double value);

    double beta() const;
    void setBeta(double value);

    double gc1x() const;
    void setGc1x(double value);

    double gc1y() const;
    void setGc1y(double value);

    double gc2x() const;
    void setGc2x(double value);

    double gc2y() const;
    void setGc2y(double value);

    int K() const;
    void setK(int value);

    double R() const;
    void setR(double value);

    double freq() const;
    void setFreq(double value);

    bool useSwitch() const;
    void setUseSwitch(bool value);

public slots:
    void generatePreview();
    void exportImage(const QUrl& url);

signals:
    void generatingChanged();
    void statusChanged();

    void xminChanged();
    void xmaxChanged();
    void yminChanged();
    void ymaxChanged();

    void alphaChanged();
    void betaChanged();

    void gc1xChanged();
    void gc1yChanged();
    void gc2xChanged();
    void gc2yChanged();

    void KChanged();
    void RChanged();
    void freqChanged();
    void useSwitchChanged();

    void previewReady(const QUrl& url);

private slots:
    void onGenerationStarted();
    void onGenerationFinished();
    void onError(const QString& message);

    void onPreviewReady(BiomorphImage image);
    void onFullImageReady(BiomorphImage image);

private:
    BiomorphParameters makeParameters() const;

    void setGenerating(bool value);
    void setStatus(const QString& value);

    BiomorphWorker* worker_ = nullptr;
    QThread* thread_ = nullptr;

    bool generating_ = false;
    QString status_ = "Ready";

    double xmin_ = -1.0;
    double xmax_ = 1.0;
    double ymin_ = -1.0;
    double ymax_ = 1.0;

    double alpha_ = 1.0;
    double beta_ = 0.0;

    double gc1x_ = 0.7;
    double gc1y_ = -0.35;

    double gc2x_ = 0.0;
    double gc2y_ = 1.5;

    int K_ = 15;

    double R_ = 10.0;
    double freq_ = 0.2;

    bool useSwitch_ = true;

    QString exportPath_;
};

#endif // !BIOMORPH_CONTROLLER_HPP
