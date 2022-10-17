#include "view.h"

#include <QFile>
#include <QTextStream>
#include <QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <cmath>
#include <cstdlib>

#include "./ui_view.h"
using s21::SmartcalcController;
using s21::SmartcalcModel;
View::View(SmartcalcController *c, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::View), controller(c) {
  ui->setupUi(this);
}

View::~View() { delete ui; }

void View::on_calculate_clicked() {
  controller->resetModel();
  controller->setModel(ui->expression->text().toStdString(),
                       ui->xvalue->text().toDouble());
  double result = controller->getResult();

  if (!controller->getValid()) {
    ui->result->setText("INVALID EXPRESSION");
  } else {
    ui->result->setText(QString::number(result));
  }
}

void View::on_plot_clicked() {
  QLineSeries *series = new QLineSeries();
  QString qdomain = ui->domain->text();

  double domain = qdomain.toDouble();
  if (domain < 0.0000001 && domain > -0.0000001) {
    delete series;
    return;
  }
  double di = 0;
  if (ui->domain->text().toStdString().size() == 0 ||
      ui->EditYRange->text().toStdString().size() == 0) {
    delete series;
    return;
  }
  di = (domain * 2 / 1000);
  for (double i = -domain; i <= domain; i = i + di) {
    QString source = ui->expression->text();
    QString value_x = ui->xvalue->text();
    controller->resetModel();
    controller->setModel(source.toStdString(), i);
    double result = controller->getResult();
    if (result > ui->EditYRange->text().toDouble() ||
        result < -(ui->EditYRange->text().toDouble)()) {
      continue;
    }
    if (controller->getValid()) {
      series->append(i, result);
    }
  }
  QChart *chart = new QChart();
  chart->legend()->hide();
  chart->addSeries(series);
  chart->createDefaultAxes();
  chart->setTitle("Your function");
  ui->chartView->setChart(chart);
}

void View::on_creditCalculate_clicked() {
  ui->monthlyPayment->clear();
  double creditAmount = (ui->editCreditAmount->text()).toDouble();
  double taxRate = (ui->editInterestRate->text()).toDouble();
  int term = (ui->editTerm->text()).toInt();
  if (ui->radioDifferentiated->isChecked()) {
    double basePayment = creditAmount / term;
    double remainingLoan = creditAmount;
    double totalPayment = 0;
    for (int i = 1; i <= term; i++) {
      double currentPayment = basePayment + remainingLoan * taxRate / 1200;
      totalPayment += currentPayment;
      remainingLoan -= basePayment;
      std::string output = "Payment for month #" + std::to_string(i) +
                           " is: " + std::to_string(currentPayment);
      ui->monthlyPayment->append(QString::fromStdString(output));
    }
    ui->labelTotalCreditOverpay->setText(
        QString::fromStdString(std::to_string(totalPayment - creditAmount)));
    ui->labelTotalPayment->setText(
        QString::fromStdString(std::to_string(totalPayment)));
  } else {
    double annuityPayment =
        creditAmount * taxRate /
        (1200 * (1 - std::pow((1 + taxRate / 1200), term * -1)));
    ui->labelTotalPayment->setText(
        QString::fromStdString(std::to_string(annuityPayment * term)));
    ui->labelTotalCreditOverpay->setText(QString::fromStdString(
        std::to_string(annuityPayment * term - creditAmount)));
    std::string output =
        "Mounthly payment is: " + std::to_string(annuityPayment);
    ui->monthlyPayment->append(QString::fromStdString(output));
  }
}

void View::on_depositCalculate_clicked() {
  double depositAmount = (ui->editDepositAmount->text()).toDouble();
  double depositTaxRate = (ui->editDepositTax->text()).toDouble();
  double depositInterestRate = (ui->editDepositInterest->text()).toDouble();
  int periodicyOfPayments = (ui->editDepositPeriodicy->text()).toInt();
  int depositTerm = (ui->editDepositTerm->text()).toInt();
  double I = depositInterestRate * periodicyOfPayments / (100 * 365);
  double depositAtEnd;
  if (I > 0.0000001) {
    depositAtEnd = (-depositAmount) *
                   (1 - pow((I + 1), (depositTerm / periodicyOfPayments))) /
                   (I);
  } else {
    depositAtEnd = (depositAmount) * (depositTerm / periodicyOfPayments);
  }
  double accuredInterest =
      depositAtEnd - depositAmount * (depositTerm / periodicyOfPayments);
  double taxAmount = accuredInterest * depositTaxRate / (100);
  ui->labelAccuredInterest->setText(
      QString::fromStdString(std::to_string(accuredInterest)));
  ui->labelTaxAmount->setText(
      QString::fromStdString(std::to_string(taxAmount)));
  ui->labelDepositEOT->setText(
      QString::fromStdString(std::to_string(depositAtEnd - taxAmount)));
}
