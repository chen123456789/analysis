#ifndef ChannelTimeShiftHistogram_hh
#define ChannelTimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;
class TLine;

class ChannelTimeShiftHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  ChannelTimeShiftHistogram(PostAnalysisCanvas*, int ch);
  virtual ~ChannelTimeShiftHistogram();
  virtual void draw(TCanvas*);
private:
  TLine* m_line;
};

#endif
