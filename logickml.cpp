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

    writeDictionaryFile(outputFileName, placemarkers, kmlFileNameWithoutExtension);

    int count = placemarkers.count();
    emit parsingCompleted(count);

    return;
}


void LogicKml::writeDictionaryFile(const QString &outputFileName, const QDomNodeList &placemarkers, QString &kmlFileNameWithoutExtension)
{
    QFile txtFile(outputFileName);
    if (!txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo TXT";
        return;
    }

    QTextStream txtStream(&txtFile);


    txtStream << "__" << kmlFileNameWithoutExtension << " = { ";
    int count = 0;




    if (kmlFileNameWithoutExtension == "uncover_events") {
        //uncover_events.kml
        //1:{'lat': 0.058750776, 'lon': 0.008640828, 'crs': 20, 'type': "incio_circuito_1",'dist_to_next': 0, 'current_dist': 0},

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
                txtStream << " }}\n\n";
            } else {
                txtStream << "},\n                  ";
            }
            count++;
        }
    }
    else if (kmlFileNameWithoutExtension == "roadType") {
        //roadType.kml
        //{ 1: { 'lat': 0.0172808243863708, 'lon': 0.0677908626541539, 'event': "recto", 'value_can': 1, 'type': "A", 'dist_to_next': 11000, 'current_dist': 0},
        for (int i = 0; i < placemarkers.count(); ++i) {
            QDomElement placemarker = placemarkers.at(i).toElement();
            QString name = placemarker.firstChildElement("name").text();
            QString coordinates = placemarker.firstChildElement("Point")
                                      .firstChildElement("coordinates").text();
            QString lat = coordinates.split(',').at(1);
            QString lon = coordinates.split(',').at(0);
            QString event = name;
            event = event.replace('\n', "");
            event = event.replace('\'', "");
            event = event.replace('[', "");
            event = event.replace(']', "");
            txtStream << i << ": {'lat': " << lat << ", "
                      << "'lon': " << lon << ", "
                      << "'event': '" << event << "', "
                      << "'value_can': " << name.split("_")[1] << ", "
                      << "'current_dist': 0}";

            if (i == placemarkers.count() - 1) {
                txtStream << " }}\n\n";
            } else {
                txtStream << "},\n                  ";
            }
            count++;
        }
    }
    else if (kmlFileNameWithoutExtension == "recom_speeds") {
        //recom_speeds.kml
        //# 1: {'lat': 0.058750776, 'lon': 0.008640828, 'crs': 20, 'type': "incio_circuito_1",'dist_to_next': 0, 'current_dist': 0},
        for (int i = 0; i < placemarkers.count(); ++i) {
            QDomElement placemarker = placemarkers.at(i).toElement();
            QString name = placemarker.firstChildElement("name").text();
            QString coordinates = placemarker.firstChildElement("Point")
                                      .firstChildElement("coordinates").text();
            QString lat = coordinates.split(',').at(1);
            QString lon = coordinates.split(',').at(0);

            int distance = 0;
            QString recom_speed = name.split("_")[2];
            int value_can = 0;

            if (name.contains("15")) {
                value_can = 1;
            } else if (name.contains("17")) {
                value_can = 2;
            } else if (name.contains("20")) {
                value_can = 3;
            } else if (name.contains("33")) {
                value_can = 4;
            } else if (name.contains("50")) {
                value_can = 5;
            }

            txtStream << i << ": {'lat': " << lat << ", "
                      << "'lon': " << lon << ", "
                      << "'crs': " << recom_speed << ", "
                      << "'type': '" << name << "', "
                      << "'value_can': " << value_can << ", "
                      << "'dist_to_next': " << distance << ", "
                      << "'current_dist': " << 0;

            if (i == placemarkers.count() - 1) {
                txtStream << " }}\n\n";
            } else {
                txtStream << "},\n                  ";
            }
            count++;
        }
    }
    else if (kmlFileNameWithoutExtension == "next_speed_limits") {
        //next_speed_limits.kml
        //# 1: {'lat': 0.058750776, 'lon': 0.008640828, 'crs': 20, 'type': "incio_circuito_1",'dist_to_next': 0, 'current_dist': 0},
        for (int i = 0; i < placemarkers.count(); ++i) {
            QDomElement placemarker = placemarkers.at(i).toElement();
            QString name = placemarker.firstChildElement("name").text();

            QString coordinates = placemarker.firstChildElement("Point")
                                      .firstChildElement("coordinates").text();
            QString lat = coordinates.split(',').at(1);
            QString lon = coordinates.split(',').at(0);

            int distance = 0;
            QString nsl = name.split("_")[2];
            QString type = name.split("_")[1];
            int value_raw = nsl.toInt();

            int value_can = 0;
            if (value_raw == 5) {
                value_can = 1;
            } else if (value_raw == 7) {
                value_can = 2;
            } else if (value_raw == 10) {
                value_can = 3;
            } else if (value_raw == 20) {
                value_can = 4;
            } else if (value_raw == 30) {
                value_can = 5;
            } else if (value_raw == 40) {
                value_can = 6;
            } else if (value_raw == 50) {
                value_can = 7;
            } else if (value_raw == 60) {
                value_can = 8;
            } else if (value_raw == 70) {
                value_can = 9;
            } else if (value_raw == 80) {
                value_can = 10;
            } else if (value_raw == 90) {
                value_can = 11;
            } else if (value_raw == 100) {
                value_can = 12;
            } else if (value_raw == 110) {
                value_can = 13;
            } else if (value_raw == 120) {
                value_can = 14;
            } else if (value_raw == 130) {
                value_can = 15;
            }

            txtStream << i << ": {'lat': " << lat << ", "
                      << "'lon': " << lon << ", "
                      << "'event': '" << name << "', "
                      << "'nsl': " << nsl << ", "
                      << "'value_can': " << value_can << ", "
                      << "'type': '" << type << "', "
                      << "'dist_to_next': " << distance << ", "
                      << "'current_dist': " << 0;

            if (i == placemarkers.count() - 1) {
                txtStream << " }}\n\n";
            } else {
                txtStream << "},\n                  ";
            }
            count++;
        }
    }
    else if (kmlFileNameWithoutExtension == "3d_groups") {
        //3d_groups.kml
        //# 1: {'lat': 0.058750776, 'lon': 0.008640828, 'crs': 20, 'type': "incio_circuito_1",'dist_to_next': 0, 'current_dist': 0},
        for (int i = 0; i < placemarkers.length(); ++i) {
            QDomElement placemarker = placemarkers.at(i).toElement();
            QString name = placemarker.firstChildElement("name").text();
            QString coordinates = placemarker.firstChildElement("Point")
                                      .firstChildElement("coordinates").text();
            QString lat = coordinates.split(',').at(1); // lat
            QString lon = coordinates.split(',').at(0); // lon
            QString event = name;

            // Realiza el procesamiento de la cadena "event"
            event = event.replace("\n", "");
            event = event.replace("'", "");
            event = event.replace("[", "");
            event = event.replace("]", "");

            // Escribe los datos en el archivo usando QTextStream
            txtStream << i << ": {'lat': " << lat << ", ";
            txtStream << "'lon': " << lon << ", ";
            txtStream << "'event': '" << event << "', ";
            txtStream << "'value_can': " << name.split("_")[1].toInt() << ", ";
            txtStream << "'current_dist': " << 0;

            if (i == placemarkers.length() - 1) {
                txtStream << "}}\n\n";
            } else {
                txtStream << "},\n                  ";
            }
            count++;
        }
    }
    else if (kmlFileNameWithoutExtension == "filter_events") {
        //filter_events.kml
        //#{ 1: { 'lat': 0.0172808243863708, 'lon': 0.0677908626541539, 'event': "recto", 'value_can': 1, 'type': "A", 'dist_to_next': 11000, 'current_dist': 0},
        for (int i = 0; i < placemarkers.length(); ++i) {
            QDomElement placemarker = placemarkers.at(i).toElement();
            QString name = placemarker.firstChildElement("name").text();
            count++;

            QString coordinates = placemarker.firstChildElement("Point")
                                      .firstChildElement("coordinates").text();
            QString lat = coordinates.split(",").at(1); // lat
            QString lon = coordinates.split(",").at(0); // lon

            int distance = 0;
            QString state = name.split("_")[3];

            int can_value = 0;
            if (state == "on") {
                can_value = 1;
            }

            txtStream << i << ": {'lat': " << lat << ", ";
            txtStream << "'lon': " << lon << ", ";
            txtStream << "'can': " << can_value << ", ";
            txtStream << "'state': '" << state << "', ";
            txtStream << "'current_dist': '" << 0 << "'";

            if (i == placemarkers.length() - 1) {
                txtStream << "}}\n\n";
            } else {
                txtStream << "},\n                  ";
            }
        }
    }
    else if (kmlFileNameWithoutExtension == "id_crosswalk") {
        //id_crosswalk.kml
        //# { 1: { 'lat': 0.0172808243863708, 'lon': 0.0677908626541539, 'event': "recto", 'value_can': 1, 'type': "A", 'dist_to_next': 11000, 'current_dist': 0},
        for (int i = 0; i < placemarkers.length(); ++i) {
            QDomElement placemarker = placemarkers.at(i).toElement();
            QString name = placemarker.firstChildElement("name").text();

            QString coordinates = placemarker.firstChildElement("Point")
                                      .firstChildElement("coordinates").text();
            QString lat = coordinates.split(",").at(1); // lat
            QString lon = coordinates.split(",").at(0); // lon

            int distance = 0;
            QString event = name;
            int value_can = 0;

            if (name.contains("Crosswalk")) {
                QStringList params_name = name.split("_");
                if (params_name.length() > 1) {
                    int id_ref = params_name[1].toInt();
                    txtStream << i << ": {'lat': " << lat << ", ";
                    txtStream << "'lon': " << lon << ", ";
                    txtStream << "'event': '" << event << "', ";
                    txtStream << "'value_can': " << value_can << ", ";
                    txtStream << "'type': '" << "A" << "', ";
                    txtStream << "'dist_to_next': " << distance << ", ";
                    txtStream << "'current_dist': " << 0 << ", ";
                    txtStream << "'id_ref': " << id_ref << " ";
                } else {
                    qDebug() << "Faltan valores en Crosswalk (" << name << ")";
                }
            } else {
                qDebug() << "Error en el manejo de eventos (" << name << ")";
            }

            if (i == placemarkers.length() - 1) {
                txtStream << "}}\n\n";
            } else {
                txtStream << "},\n                  ";
            }
        }

    }
    txtFile.close();
    qDebug() << "Puntos obtenidos: " << count;
}
