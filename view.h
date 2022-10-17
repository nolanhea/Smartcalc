#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QtCharts/QChartView>

#include "smartcalc_controller.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE
using s21::SmartcalcController;
using s21::SmartcalcModel;
class View : public QMainWindow {
  Q_OBJECT

 public:
  View(SmartcalcController* c, QWidget* parent = nullptr);
  ~View();

 private slots:

  void on_calculate_clicked();

  void on_plot_clicked();

  void on_creditCalculate_clicked();

  void on_depositCalculate_clicked();

 private:
  SmartcalcController* controller;
  Ui::View* ui;
};
#endif  // VIEW_H
