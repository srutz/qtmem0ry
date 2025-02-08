
#include "downloadhandler.h"
#include "util.h"
#include "datafetcher.h"

#include <QThread>
#include <QApplication>


const QString images[] = {
    "https://images.pexels.com/photos/47547/squirrel-animal-cute-rodents-47547.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1661179/pexels-photo-1661179.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/792381/pexels-photo-792381.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/34231/antler-antler-carrier-fallow-deer-hirsch.jpg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/62289/yemen-chameleon-chamaeleo-calyptratus-chameleon-reptile-62289.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/67552/giraffe-tall-mammal-africa-67552.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/325045/pexels-photo-325045.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/162140/duckling-birds-yellow-fluffy-162140.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/219906/pexels-photo-219906.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/17811/pexels-photo.jpg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1108099/pexels-photo-1108099.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/97533/pexels-photo-97533.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/64219/dolphin-marine-mammals-water-sea-64219.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/76957/tree-frog-frog-red-eyed-amphibian-76957.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/70080/elephant-africa-african-elephant-kenya-70080.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/53581/bald-eagles-bald-eagle-bird-of-prey-adler-53581.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/73825/osprey-adler-bird-of-prey-raptor-73825.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/86405/penguin-funny-blue-water-86405.jpeg?auto=compress&cs=tinysrgb&h=350",

    /*
    "https://images.pexels.com/photos/635499/pexels-photo-635499.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/326012/pexels-photo-326012.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/162203/panthera-tigris-altaica-tiger-siberian-amurtiger-162203.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/236599/pexels-photo-236599.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/572861/pexels-photo-572861.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/847393/pexels-photo-847393.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/39857/leopard-leopard-spots-animal-wild-39857.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/2295744/pexels-photo-2295744.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1076758/pexels-photo-1076758.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/45853/grey-crowned-crane-bird-crane-animal-45853.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/460775/pexels-photo-460775.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/50577/hedgehog-animal-baby-cute-50577.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/144234/bull-landscape-nature-mammal-144234.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/3608263/pexels-photo-3608263.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1851164/pexels-photo-1851164.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1319515/pexels-photo-1319515.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1207875/pexels-photo-1207875.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1499596/pexels-photo-1499596.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/302304/pexels-photo-302304.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/106685/pexels-photo-106685.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/34770/haan-crows-mohawk.jpg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/45246/green-tree-python-python-tree-python-green-45246.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/4588065/pexels-photo-4588065.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1335971/pexels-photo-1335971.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/53125/elephant-tusk-ivory-animal-53125.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1829979/pexels-photo-1829979.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/75973/pexels-photo-75973.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/598966/pexels-photo-598966.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/33045/lion-wild-africa-african.jpg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/225869/pexels-photo-225869.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/406014/pexels-photo-406014.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/69932/tabby-cat-close-up-portrait-69932.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/36347/cow-pasture-animal-almabtrieb.jpg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/1059823/pexels-photo-1059823.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/104827/cat-pet-animal-domestic-104827.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/56733/pexels-photo-56733.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/802112/pexels-photo-802112.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/234054/pexels-photo-234054.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/991831/pexels-photo-991831.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/38280/monkey-mandril-africa-baboon-38280.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/3551498/pexels-photo-3551498.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/314865/pexels-photo-314865.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/133394/pexels-photo-133394.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/16040/pexels-photo.jpg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/372166/pexels-photo-372166.jpeg?auto=compress&cs=tinysrgb&h=350",
    "https://images.pexels.com/photos/86596/owl-bird-eyes-eagle-owl-86596.jpeg?auto=compress&cs=tinysrgb&h=350",
    */
};



DownloadHandler::DownloadHandler(int count, QObject *parent) : QObject(parent), m_count(count) {
    qDebug() << "DownloadHandler()";
}

DownloadHandler::~DownloadHandler() {
    qDebug() << "~DownloadHandler()";
}

void DownloadHandler::download() {
    QString resourceFolder = QDir(QDir::homePath()).filePath("qtmemory-resources");
    QDir dir(resourceFolder);
    if (!dir.exists()) {
        if (dir.mkpath(resourceFolder)) {
            qDebug() << "Temporary directory created at:" << resourceFolder;
        } else {
            qDebug() << "Failed to create temporary directory.";
        }
    }
    QString apiKey = getenv("PEXELSAPIKEY");

    for (auto i = 0; i < m_count; i++) {
        auto filename = dir.filePath(QString::asprintf("image_%02d.jpg", i + 1));
        auto file = QFile(filename);
        QByteArray data;
        if (file.exists()) {
            data = Util::loadFile(filename);
        } else {
            qDebug() << "image download starting on thread " << QThread::currentThread()->objectName();
            QString url(images[i]);
            //auto result = Util::downloadUrl(url, { { "Authorization", apiKey }});
            auto result = DataFetcher::downloadSync({ .url = url, .headers = { { "Authorization", apiKey } }});
            if (result.code != 200) {
                qWarning() << "download failed " << url << ", code=" << result.code << ", error=" << result.error.value_or("unknown");
                QApplication::exit(1);
            }
            if (!file.open(QIODevice::WriteOnly)) {
                qWarning() << "Could not open file for writing";
            }
            if (file.write(result.data) == -1) {
                qWarning() << "Failed to write to file " << filename;
            }
            file.close();
            qDebug() << "download completed: " << url;
            data = result.data;
        }
        emit imageCompleted(i, m_count, data, filename);
    }
    qDebug() << "completed " << this << " on thread " << QThread::currentThread()->objectName();
    emit completed();
}


