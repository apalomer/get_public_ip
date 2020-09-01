#include "getpublicip.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  GetPublicIP* gpi = new GetPublicIP;
  QObject::connect(gpi, &GetPublicIP::gotPublicIP, [](QString ip) {
    if (ip.isEmpty())
      QMessageBox::critical(nullptr, "Error", "Impossible to retrieve the public IP.");
    else
      QMessageBox::information(nullptr, "Public IP", "Public IP: " + ip);
  });
  return app.exec();
}
