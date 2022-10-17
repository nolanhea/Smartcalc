#ifndef SMARTCALC_CONTROLLER_H
#define SMARTCALC_CONTROLLER_H
#include "smartcalc_model.h"
namespace s21 {
class SmartcalcController {
 private:
  SmartcalcModel* model;

 public:
  SmartcalcController(SmartcalcModel* m) : model(m) {}
  void setModel(std::string expression, double xvalue = NAN) {
    model->setModel(expression, xvalue);
  }
  void resetModel() { model->resetModel(); }
  double getResult() { return model->getResult(); }
  bool getValid() { return model->getValid(); }
};
}  // namespace s21
#endif  // SMARTCALC_CONTROLLER_H
