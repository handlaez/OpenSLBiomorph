#include "BiomorphController.hpp"
#include "BiomorphWorker.hpp"

#include <QStandardPaths>
#include <QThread>

#include <stb_image_write.h>


BiomorphController::BiomorphController(QObject* parent) : QObject(parent)
{
    thread_ = new QThread(this);
    worker_ = new BiomorphWorker();
    worker_->moveToThread(thread_);

    connect(thread_, &QThread::finished, worker_, &QObject::deleteLater);

    connect(worker_, &BiomorphWorker::generationStarted, this, &BiomorphController::onGenerationStarted);
    connect(worker_, &BiomorphWorker::generationFinished, this, &BiomorphController::onGenerationFinished);
    connect(worker_, &BiomorphWorker::errorOccurred, this, &BiomorphController::onError);
    connect(worker_, &BiomorphWorker::previewReady, this, &BiomorphController::onPreviewReady);
    connect(worker_, &BiomorphWorker::fullImageReady, this, &BiomorphController::onFullImageReady);

    thread_->start();

    QMetaObject::invokeMethod(worker_, "initialize", Qt::QueuedConnection);
}

BiomorphController::~BiomorphController()
{
    thread_->quit();
    thread_->wait();
}

BiomorphParameters BiomorphController::makeParameters() const
{
    BiomorphParameters params;

    params.xmin = xmin_;
    params.xmax = xmax_;
    params.ymin = ymin_;
    params.ymax = ymax_;

    params.alpha = alpha_;
    params.beta = beta_;

    params.gc1x = gc1x_;
    params.gc1y = gc1y_;

    params.gc2x = gc2x_;
    params.gc2y = gc2y_;

    params.K = K_;

    params.R = R_;
    params.freq = freq_;

    params.useSwitch = useSwitch_ ? 1 : 0;

    return params;
}

void BiomorphController::generatePreview()
{
    if (generating_)
        return;

    BiomorphParameters params = makeParameters();

    params.width = 512;
    params.height = 512;

    QMetaObject::invokeMethod(worker_, "generatePreview", Qt::QueuedConnection, Q_ARG(BiomorphParameters, params));
}

void BiomorphController::exportImage(const QUrl& url)
{
    if (generating_)
        return;

    if (!url.isLocalFile())
    {
        onError("Export path is not a local file.");
        return;
    }

    exportPath_ = url.toLocalFile();

    BiomorphParameters params = makeParameters();

    params.width = 4000;
    params.height = 4000;

    QMetaObject::invokeMethod(worker_, "generateFull", Qt::QueuedConnection, Q_ARG(BiomorphParameters, params));
}

void BiomorphController::onFullImageReady(BiomorphImage image)
{
    const QByteArray path = QFile::encodeName(exportPath_);

    const int result = stbi_write_png(
        path.constData(),
        image.width,
        image.height,
        4,
        image.pixels.data(),
        image.stride());

    if (!result)
    {
        onError("Failed to write PNG.");
        return;
    }

    setStatus("Image exported.");
}

bool BiomorphController::generating() const
{
    return generating_;
}

QString BiomorphController::status() const
{
    return status_;
}

void BiomorphController::setGenerating(bool value)
{
    if (generating_ == value)
        return;

    generating_ = value;
    emit generatingChanged();
}

void BiomorphController::setStatus(const QString& value)
{
    if (status_ == value)
        return;

    status_ = value;
    emit statusChanged();
}

void BiomorphController::onGenerationStarted()
{
    setGenerating(true);
    setStatus("Generating...");
}

void BiomorphController::onGenerationFinished()
{
    setGenerating(false);

    if (status_ == "Generating...")
        setStatus("Ready");
}

void BiomorphController::onError(const QString& message)
{
    setGenerating(false);
    setStatus(message);
}

void BiomorphController::onPreviewReady(BiomorphImage image)
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/biomorph_preview.png";
    const QByteArray encodedPath = QFile::encodeName(path);

    const int result = stbi_write_png(
        encodedPath.constData(),
        image.width,
        image.height,
        4,
        image.pixels.data(),
        image.stride());

    if (!result)
    {
        onError("Failed to write preview PNG.");
        return;
    }

    emit previewReady(QUrl::fromLocalFile(path));
}

double BiomorphController::xmin() const
{
    return xmin_;
}

void BiomorphController::setXmin(double value)
{
    if (qFuzzyCompare(xmin_, value))
        return;

    xmin_ = value;
    emit xminChanged();
}

double BiomorphController::xmax() const
{
    return xmax_;
}

void BiomorphController::setXmax(double value)
{
    if (qFuzzyCompare(xmax_, value))
        return;

    xmax_ = value;
    emit xmaxChanged();
}

double BiomorphController::ymin() const
{
    return ymin_;
}

void BiomorphController::setYmin(double value)
{
    if (qFuzzyCompare(ymin_, value))
        return;

    ymin_ = value;
    emit yminChanged();
}

double BiomorphController::ymax() const
{
    return ymax_;
}

void BiomorphController::setYmax(double value)
{
    if (qFuzzyCompare(ymax_, value))
        return;

    ymax_ = value;
    emit ymaxChanged();
}

double BiomorphController::alpha() const
{
    return alpha_;
}

void BiomorphController::setAlpha(double value)
{
    if (qFuzzyCompare(alpha_, value))
        return;

    alpha_ = value;
    emit alphaChanged();
}

double BiomorphController::beta() const
{
    return beta_;
}

void BiomorphController::setBeta(double value)
{
    if (qFuzzyCompare(beta_, value))
        return;

    beta_ = value;
    emit betaChanged();
}

double BiomorphController::gc1x() const
{
    return gc1x_;
}

void BiomorphController::setGc1x(double value)
{
    if (qFuzzyCompare(gc1x_, value))
        return;

    gc1x_ = value;
    emit gc1xChanged();
}

double BiomorphController::gc1y() const
{
    return gc1y_;
}

void BiomorphController::setGc1y(double value)
{
    if (qFuzzyCompare(gc1y_, value))
        return;

    gc1y_ = value;
    emit gc1yChanged();
}

double BiomorphController::gc2x() const
{
    return gc2x_;
}

void BiomorphController::setGc2x(double value)
{
    if (qFuzzyCompare(gc2x_, value))
        return;

    gc2x_ = value;
    emit gc2xChanged();
}

double BiomorphController::gc2y() const
{
    return gc2y_;
}

void BiomorphController::setGc2y(double value)
{
    if (qFuzzyCompare(gc2y_, value))
        return;

    gc2y_ = value;
    emit gc2yChanged();
}

int BiomorphController::K() const
{
    return K_;
}

void BiomorphController::setK(int value)
{
    if (K_ == value)
        return;

    K_ = value;
    emit KChanged();
}

double BiomorphController::R() const
{
    return R_;
}

void BiomorphController::setR(double value)
{
    if (qFuzzyCompare(R_, value))
        return;

    R_ = value;
    emit RChanged();
}

double BiomorphController::freq() const
{
    return freq_;
}

void BiomorphController::setFreq(double value)
{
    if (qFuzzyCompare(freq_, value))
        return;

    freq_ = value;
    emit freqChanged();
}

bool BiomorphController::useSwitch() const
{
    return useSwitch_;
}

void BiomorphController::setUseSwitch(bool value)
{
    if (useSwitch_ == value)
        return;

    useSwitch_ = value;
    emit useSwitchChanged();
}