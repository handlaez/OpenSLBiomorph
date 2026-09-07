#ifndef BIOMORPH_WORKER_HPP
#define BIOMORPH_WORKER_HPP

#include "BiomorphParameters.hpp"
#include "BiomorphImage.hpp"

#include <QObject>

class BiomorphGenerator;

class BiomorphWorker : public QObject
{
    Q_OBJECT

public:
    explicit BiomorphWorker(QObject* parent = nullptr);
    ~BiomorphWorker();

public slots:
    void generatePreview(BiomorphParameters params);
    void generateFull(BiomorphParameters params);

signals:
    void previewReady(BiomorphImage image);
    void fullImageReady(BiomorphImage image);

    void generationStarted();
    void generationFinished();

    void errorOccurred(QString message);

private:
    BiomorphGenerator* m_generator = nullptr;
};

#endif // !BIOMORPH_WORKER_HPP
