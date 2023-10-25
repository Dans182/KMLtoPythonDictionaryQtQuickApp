#include "logickml.h"

LogicKml::LogicKml(QObject *parent)
{

}

void LogicKml::parseKMLFile(const QUrl &kmlFileName)
{
    qDebug() << "Ruta completa del archivo KML: " << kmlFileName; // Línea de depuración

    QFile kmlFile(kmlFileName.toLocalFile());

    if (!kmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo KML: " << kmlFile.errorString();
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&kmlFile)) {
        qDebug() << "No se pudo analizar el archivo KML";
        kmlFile.close();
        return;
    }
    kmlFile.close();

    // Obtener la lista de marcadores
    QDomNodeList placemarkers = doc.elementsByTagName("Placemark");

    // Obtén la ruta del archivo KML sin el prefijo "file://"
    QString kmlFilePath = kmlFileName.toLocalFile();

    // Obtiene el nombre del archivo KML sin la extensión
    QFileInfo fileInfo(kmlFilePath);
    QString kmlFileNameWithoutExtension = fileInfo.baseName();

    // Crea el nombre de archivo de salida con la extensión ".txt"
    QString outputFileName = kmlFileNameWithoutExtension + ".txt";

//    // Abrir el archivo TXT y escribir el encabezado
//    QString outputFileName = "uncover3D_dictionary.txt";

    writeDictionaryFile(outputFileName, placemarkers);

    int count = placemarkers.count();
    emit parsingCompleted(count);

    return;
}


void LogicKml::writeDictionaryFile(const QString &outputFileName, const QDomNodeList &placemarkers)
{
    QFile txtFile(outputFileName);
    if (!txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo TXT";
        return;
    }

    QTextStream txtStream(&txtFile);
    txtStream << "__uncover3D_info = { ";
    int count = 0;

    // Resto del código para escribir en el archivo de salida

    for (int i = 0; i < placemarkers.count(); ++i) {
        QDomElement placemarker = placemarkers.at(i).toElement();
        QString name = placemarker.firstChildElement("name").text();
        QString coordinates = placemarker.firstChildElement("Point")
                                  .firstChildElement("coordinates").text();
        QString lat = coordinates.split(',').at(1);
        QString lon = coordinates.split(',').at(0);
        QString type = name.split("_").at(1);

        txtStream << i << ": {'lat': " << lat << ", "
                  << "'lon': " << lon << ", "
                  << "'event': '" << name << "', "
                  << "'value_can': " << type << ", "
                  << "'dist_to_next': 0, "
                  << "'current_dist': 0}";

        if (i == placemarkers.count() - 1) {
            txtStream << " }\n\n";
        } else {
            txtStream << ",\n                  ";
        }

        count++;
    }

    txtFile.close();
    qDebug() << "Puntos obtenidos: " << count;
}
