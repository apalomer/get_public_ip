#include "getpublicip.h"

#include <QCloseEvent>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegExp>
#include <QUrl>

#include <iostream>

GetPublicIP::GetPublicIP(QWidget* parent) : QObject(parent)
{
  // Network manager and connections
  manager_ = new QNetworkAccessManager(this);
  connect(manager_, &QNetworkAccessManager::finished, this, &GetPublicIP::downloadFinished);
  downloadUrl(getPublicIpServerUrl());
}

void GetPublicIP::sslErrors(const QList<QSslError>& sslErrors)
{
#if QT_CONFIG(ssl)
  for (const QSslError& error : sslErrors)
    std::cout << "SSL error: " << qPrintable(error.errorString()) << std::endl;
#else
  Q_UNUSED(sslErrors);
#endif
}

void GetPublicIP::downloadFinished(QNetworkReply* reply)
{
  QUrl url = reply->url();
  if (reply->error())
  {
    std::cout << "Download of " << url.toEncoded().constData() << " failed: " << qPrintable(reply->errorString())
              << std::endl;
    return;
  }

  if (isHttpRedirect(reply))
  {
    std::cout << "Request was redirected." << std::endl;
    return;
  }

  std::cout << "Download of " << url.toEncoded().constData() << " succeeded" << std::endl;

  // Check versions
  if (url == getPublicIpServerUrl())
  {
    QString data = QString::fromStdString(reply->readAll().toStdString());
    QRegExp re("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
               "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
               "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
               "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    QString ip;
    if (re.indexIn(data) >= 0)
    {
      ip = re.cap(0);
      std::cout << "Public IP: " << ip.toStdString() << std::endl;
    }
    else
      std::cout << "Did not get an IP: " << data.toStdString() << std::endl;
    emit gotPublicIP(ip);
  }
}

bool GetPublicIP::isHttpRedirect(QNetworkReply* reply)
{
  int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  return statusCode == 301 || statusCode == 302 || statusCode == 303 || statusCode == 305 || statusCode == 307 ||
         statusCode == 308;
}

QString GetPublicIP::getPublicIpServerUrl()
{
  return "http://ipv4bot.whatismyipaddress.com/";
}

QNetworkReply* GetPublicIP::downloadUrl(QUrl url)
{
  QNetworkRequest request(url);
  QNetworkReply* reply = manager_->get(request);
#if QT_CONFIG(ssl)
  connect(reply, &QNetworkReply::sslErrors, this, &GetPublicIP::sslErrors);
#endif
  return reply;
}
