#include "H2DProjectionPlot.hh"

#include "ProjectionControlWidget.hh"
#include "RootQtWidget.hh"

#include <QVBoxLayout>
#include <QSpinBox>

#include <TPad.h>
#include <TH2D.h>

H2DProjectionPlot::H2DProjectionPlot() :
  QObject(),
  H2DPlot(),
  m_projectionHistX(0),
  m_projectionHistY(0),
  m_controlWidget(new ProjectionControlWidget(this)),
  m_projectionWidget(new RootQtWidget),
  m_type(NoProjection),
  m_pad(0),
  m_prevPad(0)
{
  m_projectionWidget->hide();

  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_controlWidget);
  layout->addWidget(m_projectionWidget);
  QWidget* widget = new QWidget;
  widget->setLayout(layout);
  setSecondaryWidget(widget);
  
  m_controlWidget->connectCheckBoxes();
}

H2DProjectionPlot::~H2DProjectionPlot()
{
  if (m_projectionHistX)
    delete m_projectionHistX;
  if (m_projectionHistY)
    delete m_projectionHistY;
}

void H2DProjectionPlot::setProjectionType(ProjectionType type)
{
  m_projectionWidget->setVisible(type != NoProjection);
  m_type = type;
}

void H2DProjectionPlot::positionChanged(double posX, double posY)
{
  int numberOfBins = m_controlWidget->spinBox()->value();
  RootQtWidget* widget = projectionWidget();
  if (widget->isVisible()) {
    int bin = histogram()->FindBin(posX,posY);
    int binx, biny, binz;
    histogram()->GetBinXYZ(bin, binx, biny, binz);
    TH1D* proj = 0;
    switch(m_type) {
    case (ProjectionOnX): 
      m_projectionHistX = histogram()->ProjectionX(qPrintable(title() + "_px"), biny - numberOfBins/2, biny + numberOfBins/2);
      proj = m_projectionHistX;
      break;
    case (ProjectionOnY) :
      m_projectionHistY = histogram()->ProjectionY(qPrintable(title() + "_py"), binx - numberOfBins/2, binx + numberOfBins/2);
      proj = m_projectionHistY;
      break;
    default:
      Q_ASSERT(false);
      break;
    }
    m_prevPad = gPad;
    TCanvas* can = widget->GetCanvas();
    can->cd();
    can->Clear();
    proj->Draw();
    can->Modified();
    can->Update();
    m_pad = gPad;
    m_prevPad->cd();
    
    m_controlWidget->activateLogCheckBoxes();
  }
}

void H2DProjectionPlot::setLogX(int b)
{
  m_pad->cd();
  m_pad->SetLogx(b);
  m_pad->Modified();
  m_pad->Update();
  m_prevPad->cd();
}

void H2DProjectionPlot::setLogY(int b)
{
  m_pad->cd();
  m_pad->SetLogy(b);
  m_pad->Modified();
  m_pad->Update();
  m_prevPad->cd();
}

void H2DProjectionPlot::setLogZ(int b)
{
  m_pad->cd();
  m_pad->SetLogz(b);
  m_pad->Modified();
  m_pad->Update();
  m_prevPad->cd();
}