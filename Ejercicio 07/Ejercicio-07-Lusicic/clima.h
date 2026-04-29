#ifndef CLIMA_H
#define CLIMA_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class Clima : public QObject
{
    Q_OBJECT
public:
    explicit Clima(QObject *parent = nullptr);
    void consultar(const QString &ciudad, const QString &apiKey, const QNetworkProxy &proxy, bool online);

signals:
    void climaListo(double temperatura, const QString &descripcion, const QString &horaLocal);
    void errorClima(const QString &mensaje);

private slots:
    void onFinished();

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply = nullptr;
    void activarOffline();
};

#endif // CLIMA_H
