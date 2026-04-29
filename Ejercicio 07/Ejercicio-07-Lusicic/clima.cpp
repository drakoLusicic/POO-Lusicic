#include "clima.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkProxy>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <limits>

Clima::Clima(QObject *parent)
    : QObject(parent), manager(new QNetworkAccessManager(this))
{
}

void Clima::consultar(const QString &ciudad, const QString &apiKey, const QNetworkProxy &proxy, bool online)
{
    if (!online || apiKey.trimmed().isEmpty() || ciudad.trimmed().isEmpty()) {
        activarOffline();
        return;
    }

    manager->setProxy(proxy);

    QUrl url(QStringLiteral("https://api.openweathermap.org/data/2.5/weather"));
    QUrlQuery query;
    query.addQueryItem("q", ciudad + ",AR");
    query.addQueryItem("units", "metric");
    query.addQueryItem("appid", apiKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-Api-Key", apiKey.toUtf8());
    request.setHeader(QNetworkRequest::UserAgentHeader, "DarkTechClima/1.0");

    if (reply) {
        reply->abort();
        reply->deleteLater();
    }

    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Clima::onFinished);
}

void Clima::onFinished()
{
    if (!reply) {
        activarOffline();
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        reply = nullptr;
        activarOffline();
        return;
    }

    const QByteArray data = reply->readAll();
    reply->deleteLater();
    reply = nullptr;

    const QJsonDocument document = QJsonDocument::fromJson(data);
    if (!document.isObject()) {
        activarOffline();
        return;
    }

    const QJsonObject root = document.object();
    if (!root.contains("main") || !root.contains("weather")) {
        activarOffline();
        return;
    }

    const QJsonObject mainObj = root.value("main").toObject();
    const QJsonArray weatherArray = root.value("weather").toArray();
    if (weatherArray.isEmpty()) {
        activarOffline();
        return;
    }

    const double temperatura = mainObj.value("temp").toDouble(std::numeric_limits<double>::quiet_NaN());
    const QString descripcion = weatherArray.first().toObject().value("description").toString("Clima no disponible");
    if (qIsNaN(temperatura)) {
        activarOffline();
        return;
    }

    const QString horaLocal = QDateTime::currentDateTime().toString("hh:mm:ss dd/MM/yyyy");
    emit climaListo(temperatura, descripcion, horaLocal);
}

void Clima::activarOffline()
{
    const double temperatura = 19.0;
    const QString descripcion = QStringLiteral("Modo offline: datos simulados");
    const QString horaLocal = QDateTime::currentDateTime().toString("hh:mm:ss dd/MM/yyyy");
    emit errorClima(descripcion);
    emit climaListo(temperatura, descripcion, horaLocal);
}
