/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkCorrectorTool2D_h_Included
#define mitkCorrectorTool2D_h_Included

#include "mitkCommon.h"
#include "mitkFeedbackContourTool.h"
#include <MitkSegmentationExports.h>

namespace us
{
  class ModuleResource;
}

namespace mitk
{
  class Image;
  class StateMachineAction;
  class InteractionEvent;

  /**
    \brief Corrector tool for 2D binary segmentations

    \sa FeedbackContourTool
    \sa ExtractImageFilter
    \sa OverwriteSliceImageFilter

    \ingroup Interaction
    \ingroup ToolManagerEtAl

    Lets the user draw a (multi-point) line and intelligently decides what to do. The underlying algorithm
    tests if the line begins and ends inside or outside a segmentation and either adds or subtracts a piece of
    segmentation.

    Algorithm is implemented in CorrectorAlgorithm (so that it could be reimplemented in a more modern fashion some
    time).

    \warning Only to be instantiated by mitk::ToolManager.

    $Author$
  */
  class MITKSEGMENTATION_EXPORT CorrectorTool2D : public FeedbackContourTool
  {
  public:
    mitkClassMacro(CorrectorTool2D, FeedbackContourTool);
    itkFactorylessNewMacro(Self) itkCloneMacro(Self)

      const char **GetXPM() const override;
    us::ModuleResource GetCursorIconResource() const override;
    us::ModuleResource GetIconResource() const override;

    const char *GetName() const override;

  protected:
    CorrectorTool2D(int paintingPixelValue = 1); // purposely hidden
    ~CorrectorTool2D() override;

    void ConnectActionsAndFunctions() override;

    void Activated() override;
    void Deactivated() override;

    virtual void OnMousePressed(StateMachineAction *, InteractionEvent *);
    virtual void OnMouseMoved(StateMachineAction *, InteractionEvent *);
    virtual void OnMouseReleased(StateMachineAction *, InteractionEvent *);
    int m_PaintingPixelValue;

    Image::Pointer m_WorkingSlice;
  };

} // namespace

#endif
