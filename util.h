#ifndef UTIL_H
#define UTIL_H

#include <QStatusBar>
#include <QString>


class Util
{
private:
    Util();

public:

    template<typename T> static bool isAnyEquals(const T& value, std::initializer_list<T> args) {
        return std::any_of(args.begin(), args.end(), [&value](const T& arg) {
            return value == arg;
        });
    }

    static QStatusBar* findStatusBar();

    static QByteArray loadFile(const QString &path);

    static int showMessage(const QString &message);
};

#endif // UTIL_H
