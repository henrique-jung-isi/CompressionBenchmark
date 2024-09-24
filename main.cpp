#include <QByteArray>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QFile>

int main(int argc, char *argv[])
{
    const auto addTag = true;

    QFile batch("./batch.json");
    QFile file;
    QFile bench("./benchmark.md");
    bench.open(QFile::WriteOnly);
    QTextStream benchStream(&bench);

    QElapsedTimer timer;
    if (batch.open(QFile::ReadOnly)) {
        const auto json = batch.readAll();
        batch.close();
        benchStream << "# qCompress  \n";
        benchStream << "|Type|Elapsed Time (ms)|ByteArray size (MB)|File Size (MB)|Ratio|\n";
        benchStream << "|:---|:---|:---|:---|:---|\n";

        for (auto x = -1; x < 10; x++) {
            timer.restart();
            qDebug() << "compressing";
            const auto compressed = qCompress(json, x);
            const auto time = timer.elapsed();
            const auto size = (qreal) compressed.size() / 1024 / 1024;
            benchStream << "|Compression " << x << "|" << time << "|" << size << "|";
            qDebug() << "Compression" << x << "done.";
            file.setFileName(QString("./compressed%1.zip").arg(x));
            if (file.exists()) {
                qDebug() << "removing" << file.remove();
            }
            if (file.open(QFile::WriteOnly)) {
                QDataStream stream(&file);
                if (addTag) {
                    QString tag("Compressed Polygons\n");
                    stream << tag;
                }
                stream << compressed;
                file.close();
                benchStream << (qreal) file.size() / 1024 / 1024 << "|";
                benchStream << size / time << "|\n";
                benchStream.flush();
            }
        }
    }

    benchStream << "\n\n# qUncompress  \n";
    benchStream << "|Type|Elapsed Time (ms)|ByteArray size (MB)|\n";
    benchStream << "|:---|:---|:---|\n";

    for (auto x = -1; x < 10; x++) {
        file.setFileName(QString("./compressed%1.zip").arg(x));
        if (file.open(QFile::ReadOnly)) {
            QDataStream stream(&file);
            qDebug() << file.fileName();
            QString tag;
            QByteArray compressed;

            if (addTag) {
                stream >> tag;
                qDebug() << "extracted" << tag;
            }
            stream >> compressed;

            file.close();

            timer.restart();
            const auto uncompressed = qUncompress(compressed);
            const auto time = timer.elapsed();
            benchStream << "|Uncompression " << x << "|" << time << "|"
                        << (qreal) uncompressed.size() / 1024 / 1024 << "|\n";
            benchStream.flush();
        }
    }
    bench.close();

    return 0;
}
