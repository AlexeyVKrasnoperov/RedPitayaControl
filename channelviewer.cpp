#include "channelviewer.h"
#include "qwt_plot_curve.h"
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <QMenu>
#include <QContextMenuEvent>
#include <QSettings>
#include <algorithm>
#include <QFileDialog>
#include <utils.h>

const int curveColorsSize = 8;
Qt::GlobalColor curveColors[curveColorsSize] = { Qt::black, Qt::red, Qt::green, Qt::blue,
                                   Qt::cyan, Qt::magenta, Qt::yellow, Qt::white };


ChannelViewer::ChannelViewer(QWidget *parent):QwtPlot(parent)
{
    //
    plotCurves.resize(2);
    //
    yAxisMVRange = 10;
    //
    showGrid   = true;
    autoScaleY = true;    
    timeScaleX = true;
    mvScaleY   = false;
    zeroBaseLine = false;
    //
    nPoints = 0;
    samplingPeriod = 1;
    //
    ReadSettings();
    setAxisTitle(yLeft,(mvScaleY) ? "Amplitude, mV":"Amplitude, adc counts");
    setAxisTitle(xBottom,(timeScaleX) ? "Time, us" : "N ADC Samples");
    setMinimumSize(500,500);
    //
    grid = new QwtPlotGrid;
    grid->enableX(showGrid);
    grid->enableY(showGrid);
    grid->enableXMin(false);
    grid->enableYMin(false);
    grid->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->attach(this);
    //
    zoom = new QwtPlotZoomer(this->canvas());
    zoom->setRubberBandPen(QPen(Qt::red));
    //
    zoom->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
    zoom->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
    zoom->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );
    //
    setMVScaleY(mvScaleY);
    setTimeScaleX(timeScaleX);
    //
}
//
ChannelViewer::~ChannelViewer(void)
{
    WriteSettings();
    ClearPlot();
    delete zoom;
    delete grid;
}

//
void ChannelViewer::WriteSettings(void)
{
    QSettings settings;
    settings.beginGroup("ChannelViewer");
    settings.setValue("showGrid",showGrid);
    settings.setValue("autoScaleY",autoScaleY);
    settings.setValue("timeScaleX",timeScaleX);
    settings.setValue("mvScaleY",mvScaleY);
    settings.setValue("zeroBaseLine",zeroBaseLine);
    settings.endGroup();
}

void ChannelViewer::ReadSettings(void)
{
    QSettings settings;
    settings.beginGroup("ChannelViewer");
    showGrid = settings.value("showGrid",showGrid).toBool();
    autoScaleY = settings.value("autoScaleY",autoScaleY).toBool();
    timeScaleX = settings.value("timeScaleX",timeScaleX).toBool();
    mvScaleY = settings.value("mvScaleY",mvScaleY).toBool();
    zeroBaseLine = settings.value("zeroBaseLine",zeroBaseLine).toBool();
    settings.endGroup();
}
//
void ChannelViewer::setChannelData(int channel, const QVector<double> & xData, const QVector<double> & yData)
{
    if( (channel < 1) || (channel > 2) )
        return;
    QwtPlotCurve *curve = plotCurves[channel-1];
    //
    if( curve == 0 )
    {
        curve = new QwtPlotCurve(QString("Ch %1").arg(channel));
        QPen pen(curveColors[channel%curveColorsSize]);
        pen.setWidthF(1.5);
        curve->setPen(pen);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve->attach(this);
        plotCurves[channel-1] = curve;
    }
    //
    curve->setSamples(xData,yData);
}

//
void ChannelViewer::Update(void)
{
    ClearPlot();
    grid->enableX(showGrid);
    grid->enableY(showGrid);
    replot();
}

void ChannelViewer::ClearPlot(void)
{
    for(size_t i = 0; i < plotCurves.size(); i++)
    {
        if( plotCurves[i] != 0 )
        {
            delete plotCurves[i];
            plotCurves[i] = 0;
        }
    }
}

void ChannelViewer::resetZoom()
{
    setMVScaleY(mvScaleY);
    setTimeScaleX(timeScaleX);
    Update();
}

void ChannelViewer::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu menu;
    //
    QAction *a = 0;
    //
    //if( zoom->zoomRectIndex() != 0 )
    {
        a = new QAction(tr("Reset Zoom"),&menu);
        a->setCheckable(false);
        connect(a,SIGNAL(triggered()),this,SLOT(resetZoom()));
        menu.addAction(a);
        //
        a = new QAction(&menu);
        a->setSeparator(true);
        menu.addAction(a);
    }
    //
    a = new QAction(&menu);
    a->setSeparator(true);
    menu.addAction(a);
    //
    a = new QAction(tr("Show grid"),&menu);
    a->setCheckable(true);
    a->setChecked(showGrid);
    connect(a,SIGNAL(triggered(bool)),this,SLOT(setShowGrid(bool)));
    menu.addAction(a);
    //
    a = new QAction(&menu);
    a->setSeparator(true);
    menu.addAction(a);
    //
    a = new QAction(tr("Time scale on X axis"),&menu);
    a->setCheckable(true);
    a->setChecked(timeScaleX);
    connect(a,SIGNAL(triggered(bool)),this,SLOT(setTimeScaleX(bool)));
    menu.addAction(a);
    //
    a = new QAction(&menu);
    a->setSeparator(true);
    menu.addAction(a);
    //
    a = new QAction(tr("mV scale on Y axis"),&menu);
    a->setCheckable(true);
    a->setChecked(mvScaleY);
    connect(a,SIGNAL(triggered(bool)),this,SLOT(setMVScaleY(bool)));
    menu.addAction(a);
    //
    a = new QAction(tr("Auto scale on Y axis"),&menu);
    a->setCheckable(true);
    a->setChecked(autoScaleY);
    connect(a,SIGNAL(triggered(bool)),this,SLOT(setAutoScaleY(bool)));
    menu.addAction(a);
    //
    if( ! mvScaleY )
    {
        a = new QAction(tr("Positive/negative Y scale"),&menu);
        a->setCheckable(true);
        a->setChecked(zeroBaseLine);
        connect(a,SIGNAL(triggered(bool)),this,SLOT(setZeroBaseLine(bool)));
        menu.addAction(a);
    }
    //
    menu.exec(event->globalPos());
}

void ChannelViewer::setMVScaleY(bool set)
{
    mvScaleY = set;
    setAxisTitle(yLeft,(mvScaleY) ? "Amplitude, mV":"Amplitude, adc counts");
    setAutoScaleY(autoScaleY);
}

void ChannelViewer::setAutoScaleY(bool set)
{
    autoScaleY = set;
    //
    if( autoScaleY )
        setAxisAutoScale(yLeft,true);
    else
    {
        if( mvScaleY )
            setAxisScale(yLeft,-yAxisMVRange,yAxisMVRange);
        else
        {
            if( zeroBaseLine )
                setAxisScale(yLeft,-2047,2047);
            else
                setAxisScale(yLeft,0,4095);
        }
    }
    Update();
    zoom->setZoomBase();
}
void ChannelViewer::setTimeScaleX(bool set)
{
//    timeScaleX = set;
//    if( nPoints > 0 )
//        setAxisScale(QwtPlot::xBottom,0,(timeScaleX? event.getSamplingPeriod() : 1)*nPoints);
//    setAxisTitle(xBottom,(timeScaleX) ? "Time, us" : "N ADC Samples");
    Update();
    zoom->setZoomBase();
}
void ChannelViewer::setZeroBaseLine(bool set)
{
    zeroBaseLine = set;
    setAutoScaleY(autoScaleY);
    setTimeScaleX(timeScaleX);
    Update();
}
void ChannelViewer::setShowGrid(bool set)
{
    showGrid = set;
    grid->enableX(showGrid);
    grid->enableY(showGrid);
    Update();
}
//
void ChannelViewer::showChannelCurve(int channel, bool on)
{
    if( (channel < 1) || (channel > 2) )
        return;
    QwtPlotCurve *curve = plotCurves[channel-1];
    if( curve == 0 )
        return;
    if( on )
        curve->attach(this);
    else
        curve->detach();
}
