#ifndef CHANNELVIEWER_H
#define CHANNELVIEWER_H

#include "typedef.h"
#include <vector>
#include "qwt_plot.h"
#include <qwt_plot_grid.h>
using namespace std;

class QwtPlotCurve;
class QwtPlot;
class QContextMenuEvent;
class QwtPlotGrid;
class QwtPlotZoomer;

class ChannelViewer: public QwtPlot
{
    Q_OBJECT
protected:
    vector<QwtPlotCurve*> plotCurves;
    virtual void contextMenuEvent(QContextMenuEvent * event);
    //
    bool timeScaleX;
    bool mvScaleY;
    bool autoScaleY;    
    bool zeroBaseLine;
    bool showGrid;
    //
    QwtPlotGrid *grid;
    QwtPlotZoomer *zoom;
    //
    void WriteSettings(void);
    void ReadSettings(void);
    //
    double samplingPeriod;
    //
    double yAxisMVRange;
    //
    uint32_t nPoints;
public:
    ChannelViewer(QWidget *parent);
    ~ChannelViewer(void);
    void Update();
    void ClearPlot(void);
    void setChannelData(int channel, const QVector<double> & xData, const QVector<double> & yData );
    void showChannelCurve(int channel, bool on);
public slots:
    void setAutoScaleY(bool set);
    void setTimeScaleX(bool set);
    void setMVScaleY(bool set);
    void setZeroBaseLine(bool set);
    void setShowGrid(bool set);
    void resetZoom(void);
};

#endif // CHANNELVIEWER_H
