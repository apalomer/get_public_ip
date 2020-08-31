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
	  std::cout << "Download of " << url.toEncoded().constData() << " failed: " << qPrintable(reply->errorString()) << std::endl;
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
    std::cout << "Public IP: " << data.toStdString() << std::endl;
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
