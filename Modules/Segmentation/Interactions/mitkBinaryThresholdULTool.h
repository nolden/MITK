/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkBinaryThresholdULTool_h_Included
#define mitkBinaryThresholdULTool_h_Included

#include "mitkAutoSegmentationTool.h"
#include "mitkCommon.h"
#include "mitkDataNode.h"
#include <MitkSegmentationExports.h>

#include <itkBinaryThresholdImageFilter.h>
#include <itkImage.h>

namespace us
{
  class ModuleResource;
}

namespace mitk
{
  /**
  \brief Calculates the segmented volumes for binary images.

  \ingroup ToolManagerEtAl
  \sa mitk::Tool
  \sa QmitkInteractiveSegmentation

  Last contributor: $Author$
  */
  class MITKSEGMENTATION_EXPORT BinaryThresholdULTool : public AutoSegmentationTool
  {
  public:
    Message3<double, double, bool> IntervalBordersChanged;
    Message2<mitk::ScalarType, mitk::ScalarType> ThresholdingValuesChanged;

    mitkClassMacro(BinaryThresholdULTool, AutoSegmentationTool);
    itkFactorylessNewMacro(Self) itkCloneMacro(Self)

      const char **GetXPM() const override;
    us::ModuleResource GetIconResource() const override;
    const char *GetName() const override;

    void Activated() override;
    void Deactivated() override;

    virtual void SetThresholdValues(double lower, double upper);
    virtual void AcceptCurrentThresholdValue();
    virtual void CancelThresholding();

  protected:
    BinaryThresholdULTool(); // purposely hidden
    ~BinaryThresholdULTool() override;

    void SetupPreviewNode();

    void CreateNewSegmentationFromThreshold(DataNode *node);

    void OnRoiDataChanged();
    void UpdatePreview();

    DataNode::Pointer m_ThresholdFeedbackNode;
    DataNode::Pointer m_OriginalImageNode;
    DataNode::Pointer m_NodeForThresholding;

    mitk::ScalarType m_SensibleMinimumThresholdValue;
    mitk::ScalarType m_SensibleMaximumThresholdValue;
    mitk::ScalarType m_CurrentLowerThresholdValue;
    mitk::ScalarType m_CurrentUpperThresholdValue;

    bool m_IsOldBinary = false;

    typedef itk::Image<int, 3> ImageType;
    typedef itk::Image<Tool::DefaultSegmentationDataType, 3> SegmentationType; // this is sure for new segmentations
    typedef itk::BinaryThresholdImageFilter<ImageType, SegmentationType> ThresholdFilterType;
    ThresholdFilterType::Pointer m_ThresholdFilter;
  };

} // namespace

#endif
