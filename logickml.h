#ifndef LOGICKML_H
#define LOGICKML_H

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QUrl>
#include <QFileInfo>

class LogicKml : public QObject
{
    Q_OBJECT
public:
    LogicKml(QObject *parent = nullptr);

//    QDomDocument doc;
//    QDomNodeList placemarkers;
//    QString outputFileName;

public slots:
    void parseKMLFile(const QUrl &kmlFileName);

private:
    void writeDictionaryFile(const QString &outputFileName, const QDomNodeList &placemarkers, QString &kmlFileNameWithoutExtension);

    void writeUncoverEvents(const QDomNodeList &placemarkers, QTextStream &txtStream, int &count);
    void writeRoadType(const QDomNodeList &placemarkers, QTextStream &txtStream, int &count);
    void writeRecomSpeeds(const QDomNodeList &placemarkers, QTextStream &txtStream, int &count);
    void writeNextSpeedLimits(const QDomNodeList &placemarkers, QTextStream &txtStream, int &count);
    void write3DGroups(const QDomNodeList &placemarkers, QTextStream &txtStream, int &count);
    void writeFilterEvents(const QDomNodeList &placemarkers, QTextStream &txtStream, int &count);
    void writeIDCrosswalk(const QDomNodeList &placemarkers, QTextStream &txtStream, int &count);


signals:
    void parsingCompleted(int pointCount);

};

#endif // LOGICKML_H
