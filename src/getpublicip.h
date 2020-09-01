#ifndef GETPUBLICIP_H
#define GETPUBLICIP_H

#include <QtNetwork>

class GetPublicIP : public QObject
{
  Q_OBJECT
public:
  /*!
   * \brief Consructor
   * \param parent
   */
  GetPublicIP(QWidget* parent = nullptr);
signals:

  void gotPublicIP(QString ip);

protected slots:

  /*!
   * \brief Pocess a download
   * \param reply
   */
  void downloadFinished(QNetworkReply* reply);

  /*!
   * \brief Handle ssl errors
   * \param sslErrors
   */
  void sslErrors(const QList<QSslError>& sslErrors);

protected:
  /*!
   * \brief Method for downloading a given URL
   * \param url
   * \return
   */
  QNetworkReply* downloadUrl(QUrl url);

  /*!
   * \brief Check if the reply was http redirected
   * \param reply
   * \return
   */
  bool isHttpRedirect(QNetworkReply* reply);

  /*!
   * \brief Server where the isntallers are
   * \return
   */
  static QString getPublicIpServerUrl();

  /*!
   * \brief Network manager
   */
  QNetworkAccessManager* manager_ = nullptr;
};

#endif  // GETPUBLICIP_H
