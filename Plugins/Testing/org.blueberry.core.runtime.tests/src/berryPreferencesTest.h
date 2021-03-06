/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/


#ifndef BERRYPREFERENCESTEST_H_
#define BERRYPREFERENCESTEST_H_

#include <berryTestCase.h>

namespace berry {

class PreferencesTest : public berry::TestCase
{
public:

  PreferencesTest(const std::string& testName);

  static CppUnit::Test* Suite();

  void TestAll();

};

}

#endif /* BERRYPREFERENCESTEST_H_ */
