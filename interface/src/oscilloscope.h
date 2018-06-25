#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include "oscilloscopethread.h"

#include <QWidget>
#include <QTimer>


namespace Ui {
class OscilloScope;
}

class OscilloScope : public QWidget {
  Q_OBJECT

public:
//    JOINT_HANDLE joint;

public:
  explicit OscilloScope(QWidget *parent = 0);
  ~OscilloScope();
  QwtPlotCurve *curveTgPOS = NULL;
  QwtPlotCurve *curveTgSPD = NULL;
  QwtPlotCurve *curveTgCUR = NULL;
  QwtPlotCurve *curveRlPOS = NULL;
  QwtPlotCurve *curveRlSPD = NULL;
  QwtPlotCurve *curveRlCUR = NULL;


public slots:
  void OscilloScopeInitialize(int ID);

signals:

private slots:
  void on_tgPOSPushButton_clicked();
  void on_tgSPDPushButton_clicked();
  void on_tgCURPushButton_clicked();
  void on_rlPOSPushButton_clicked();
  void on_rlSPDPushButton_clicked();
  void on_rlCURPushButton_clicked();
  void on_scopeEnablePushButton_clicked();
#if 0
  void on_offset_POSLineEdit_editingFinished();
  void on_offset_SPDLineEdit_editingFinished();
  void on_offset_CURLineEdit_editingFinished();
  void on_prComboBox_currentIndexChanged(int index);
  void on_srComboBox_currentIndexChanged(int index);
  void on_crComboBox_currentIndexChanged(int index);
  void on_ScanFrequencyComboBox_currentIndexChanged(int index);
#endif
  void updatePlot();

  void on_yLeft_clicked();

private:
  void SetValue(double value);
  Ui::OscilloScope *uiOscilloScope;
  QString tgPOSPushButtonOn;
  QString tgPOSPushButtonOff;
  QString tgSPDPushButtonOn;
  QString tgSPDPushButtonOff;
  QString tgCURPushButtonOn;
  QString tgCURPushButtonOff;
  QString rlPOSPushButtonOn;
  QString rlPOSPushButtonOff;
  QString rlSPDPushButtonOn;
  QString rlSPDPushButtonOff;
  QString rlCURPushButtonOn;
  QString rlCURPushButtonOff;
  QString scopeEnablePushButtonOn;
  QString scopeEnablePushButtonOff;
  OscilloScopeThread* osthread = NULL;
  QTimer * timerOscilloScope  = NULL;
  QwtPlotGrid * grid = NULL;
  QwtPlotCurve * curveGrid = NULL;
  QwtPlotMagnifier * plotMag = NULL;
  QwtPlotMagnifier * plotSPDMag = NULL;
  QwtPlotMagnifier * plotPOSMag = NULL;
};

#endif // OSCILLOSCOPE_H
