#include "getpublicip.h"

#include <QApplication>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  GetPublicIP* gpi = new GetPublicIP;
  return app.exec();
}
