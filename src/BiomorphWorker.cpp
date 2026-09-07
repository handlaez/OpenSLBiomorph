#include "BiomorphWorker.hpp"
#include "BiomorphGenerator.hpp"

BiomorphWorker::BiomorphWorker(QObject* parent) : QObject(parent)
{
}

void BiomorphWorker::initialize()
{
    try
    {
        generator_ = new BiomorphGenerator;
    }
    catch (const std::exception& e)
    {
        emit errorOccurred(QString::fromStdString(e.what()));
    }
}

BiomorphWorker::~BiomorphWorker()
{
    delete generator_;
}

void BiomorphWorker::generatePreview(BiomorphParameters params)
{
    try
    {
        emit generationStarted();

        BiomorphImage image = generator_->generate(params);

        emit previewReady(std::move(image));
        emit generationFinished();
    }
    catch (const std::exception& e)
    {
        emit errorOccurred(QString::fromStdString(e.what()));
        emit generationFinished();
    }
}

void BiomorphWorker::generateFull(BiomorphParameters params)
{
    try
    {
        emit generationStarted();

        BiomorphImage image = generator_->generate(params);

        emit fullImageReady(std::move(image));
        emit generationFinished();
    }
    catch (const std::exception& e)
    {
        emit errorOccurred(QString::fromStdString(e.what()));
        emit generationFinished();
    }
}