#include "BiomorphWorker.hpp"
#include "BiomorphGenerator.hpp"

BiomorphWorker::BiomorphWorker(QObject* parent) : QObject(parent)
{
    m_generator = new BiomorphGenerator;
}

BiomorphWorker::~BiomorphWorker()
{
    delete m_generator;
}

void BiomorphWorker::generatePreview(BiomorphParameters params)
{
    try
    {
        emit generationStarted();

        BiomorphImage image = m_generator->generate(params);

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

        BiomorphImage image = m_generator->generate(params);

        emit fullImageReady(std::move(image));
        emit generationFinished();
    }
    catch (const std::exception& e)
    {
        emit errorOccurred(QString::fromStdString(e.what()));
        emit generationFinished();
    }
}