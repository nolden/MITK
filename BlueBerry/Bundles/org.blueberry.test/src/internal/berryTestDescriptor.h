/*===================================================================

BlueBerry Platform

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/


#ifndef BERRYTESTDESCRIPTOR_H_
#define BERRYTESTDESCRIPTOR_H_

#include "berryITestDescriptor.h"

#include <berryIConfigurationElement.h>

namespace berry {

class TestDescriptor : public ITestDescriptor
{
public:

  berryObjectMacro(TestDescriptor)

  TestDescriptor(IConfigurationElement::Pointer elem);

  CppUnit::Test* CreateTest() override;
  QString GetId() const override;
  QString GetContributor() const override;
  QString GetDescription() const override;

  bool IsUITest() const override;

private:

  IConfigurationElement::Pointer configElem;
};

}

#endif /* BERRYTESTDESCRIPTOR_H_ */
