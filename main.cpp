#include <QApplication>

#include "view.h"
using s21::SmartcalcController;
using s21::SmartcalcModel;
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  SmartcalcModel model;
  SmartcalcController controller(&model);
  View w(&controller);

  w.show();
  return a.exec();
}
