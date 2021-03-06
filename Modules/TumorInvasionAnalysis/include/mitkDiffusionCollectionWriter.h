/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef MITK_DIFFUSIONCOLLECTION_WRITER_H
#define MITK_DIFFUSIONCOLLECTION_WRITER_H

#include "mitkCommon.h"
#include "mitkDataCollection.h"
#include "mitkDiffusionCollectionReader.h"
#include <MitkTumorInvasionAnalysisExports.h>

namespace mitk {
  class MITKTUMORINVASIONANALYSIS_EXPORT DiffusionCollectionWriter
  {
  public:

    typedef float TensorScalar;
    /**
    * @brief ExportCollectionToFolder
    *
    * Creates an XML file and stores all data in the same folder as the xml file (creating sub-folders for sub-collections)
    *
    *
    * Naming Conventions (Neccessary for proper saving of these images):
    * DWI - Diffusion Weighted Images
    * DTI - Diffusion Tensor Images
    * FIB - Fiber Bundles
    *
    * @param dataCollection
    * @param xmlFile
    * @param filter - (optional) only items with names contained in this list are written, if list is empty all items are written
    * @return
    */
    static bool ExportCollectionToFolder(DataCollection* dataCollection, std::string xmlFile , std::vector<std::string> filter);

    static bool ExportCollectionToFolder(DataCollection* dataCollection, std::string xmlFile);

    /**
    * @brief SaveCollection - Stores data collection at original location
    *
    * Writes the collection back to the files given in the original XML file.
    * New data items are stored into the default location, which is relative to the XML file.
    *
    * If a XML file is provided the files are stored as stated above with the differences that a new XML file is generated and new files are saved
    * relative to the newly generated xml.
    *
    * @param dataCollection
    * @param filter
    * @param xmlFile
    * @return
    */

    static bool SaveCollection(DataCollection* dataCollection, std::vector<std::string> filter, std::string xmlFile  = "");

    static bool FolderToXml(std::string folder, std::string collectionType, std::string xmlFile, std::vector<std::string> filter, std::vector<std::string> seriesNames);

    // GTV last entry in filter list, this item will be made to TARGET
    static bool SingleFolderToXml(std::string folder, std::string xmlFile, std::vector<std::string> filter, std::vector<std::string> seriesNames, bool longDate = true, int skipUntil = 0, float months = 0);

  protected:

  private:

    static size_t GetIndexForinXMonths(DiffusionCollectionReader::FileListType fileList, float months, size_t curIndex, std::vector<std::string> filter);
  };
} // namespace mitk

#endif /* MITK_COLLECTION_WRITER_H */
