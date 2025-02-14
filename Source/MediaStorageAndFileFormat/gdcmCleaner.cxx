/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library

  Copyright (c) 2006-2011 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmCleaner.h"
#include "gdcmAnonymizeEvent.h"
#include "gdcmAttribute.h"
#include "gdcmCSAHeader.h"
#include "gdcmDataSetHelper.h"
#include "gdcmDicts.h"
#include "gdcmEvent.h"
#include "gdcmGlobal.h"

#include "gdcmext/csa.h"
#include "gdcmext/mec_mr3.h"

namespace gdcm {

static const PrivateTag part15_table_E_1_1[] = {
    PrivateTag(0x0019, 0x0C, "SIEMENS MR HEADER"),
    PrivateTag(0x0019, 0x0D, "SIEMENS MR HEADER"),
    PrivateTag(0x0019, 0x0E, "SIEMENS MR HEADER"),
    PrivateTag(0x0019, 0x23, "GEMS_ACQU_01"),
    PrivateTag(0x0019, 0x24, "GEMS_ACQU_01"),
    PrivateTag(0x0019, 0x27, "GEMS_ACQU_01"),
    PrivateTag(0x0019, 0x27, "SIEMENS MR HEADER"),
    PrivateTag(0x0019, 0x9E, "GEMS_ACQU_01"),
    PrivateTag(0x0025, 0x01, "Philips ST80i"),
    PrivateTag(0x0025, 0x07, "GEMS_SERS_01"),
    PrivateTag(0x0043, 0x27, "GEMS_PARM_01"),
    PrivateTag(0x0043, 0x39, "GEMS_PARM_01"),
    PrivateTag(0x0043, 0x6F, "GEMS_PARM_01"),
    PrivateTag(0x0045, 0x01, "GEMS_HELIOS_01"),
    PrivateTag(0x0045, 0x02, "GEMS_HELIOS_01"),
    PrivateTag(0x0099, 0x01, "NQHeader"),
    PrivateTag(0x0099, 0x02, "NQHeader"),
    PrivateTag(0x0099, 0x04, "NQHeader"),
    PrivateTag(0x0099, 0x05, "NQHeader"),
    PrivateTag(0x0099, 0x10, "NQHeader"),
    PrivateTag(0x0099, 0x20, "NQHeader"),
    PrivateTag(0x0099, 0x21, "NQHeader"),
    PrivateTag(0x00E1, 0x21, "ELSCINT1"),
    PrivateTag(0x00E1, 0x50, "ELSCINT1"),
    PrivateTag(0x0119, 0x00, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x01, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x02, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x03, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x04, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x05, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x06, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x07, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x08, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x09, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x10, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x11, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x12, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x13, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0119, 0x21, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x00, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x02, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x03, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x04, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x05, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x06, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x07, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x08, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x09, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x10, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x11, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x12, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x20, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x21, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x22, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x29, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0129, 0x30, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0139, 0x01, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0149, 0x01, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0149, 0x02, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0149, 0x03, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x0199, 0x01, "NQLeft"),
    PrivateTag(0x0199, 0x02, "NQLeft"),
    PrivateTag(0x0199, 0x03, "NQLeft"),
    PrivateTag(0x0199, 0x04, "NQLeft"),
    PrivateTag(0x0199, 0x05, "NQLeft"),
    PrivateTag(0x0199, 0x06, "NQLeft"),
    PrivateTag(0x0199, 0x07, "NQLeft"),
    PrivateTag(0x0199, 0x08, "NQLeft"),
    PrivateTag(0x0199, 0x09, "NQLeft"),
    PrivateTag(0x0199, 0x0A, "NQLeft"),
    PrivateTag(0x0199, 0x0B, "NQLeft"),
    PrivateTag(0x0199, 0x0C, "NQLeft"),
    PrivateTag(0x0199, 0x0D, "NQLeft"),
    PrivateTag(0x0199, 0x0E, "NQLeft"),
    PrivateTag(0x0199, 0x0F, "NQLeft"),
    PrivateTag(0x0199, 0x10, "NQLeft"),
    PrivateTag(0x0199, 0x11, "NQLeft"),
    PrivateTag(0x0199, 0x12, "NQLeft"),
    PrivateTag(0x0199, 0x13, "NQLeft"),
    PrivateTag(0x0199, 0x14, "NQLeft"),
    PrivateTag(0x0199, 0x15, "NQLeft"),
    PrivateTag(0x0199, 0x16, "NQLeft"),
    PrivateTag(0x01E1, 0x26, "ELSCINT1"),
    PrivateTag(0x01F1, 0x01, "ELSCINT1"),
    PrivateTag(0x01F1, 0x07, "ELSCINT1"),
    PrivateTag(0x01F1, 0x26, "ELSCINT1"),
    PrivateTag(0x01F1, 0x27, "ELSCINT1"),
    PrivateTag(0x0299, 0x01, "NQRight"),
    PrivateTag(0x0299, 0x02, "NQRight"),
    PrivateTag(0x0299, 0x03, "NQRight"),
    PrivateTag(0x0299, 0x04, "NQRight"),
    PrivateTag(0x0299, 0x05, "NQRight"),
    PrivateTag(0x0299, 0x06, "NQRight"),
    PrivateTag(0x0299, 0x07, "NQRight"),
    PrivateTag(0x0299, 0x08, "NQRight"),
    PrivateTag(0x0299, 0x09, "NQRight"),
    PrivateTag(0x0299, 0x0A, "NQRight"),
    PrivateTag(0x0299, 0x0B, "NQRight"),
    PrivateTag(0x0299, 0x0C, "NQRight"),
    PrivateTag(0x0299, 0x0D, "NQRight"),
    PrivateTag(0x0299, 0x0E, "NQRight"),
    PrivateTag(0x0299, 0x0F, "NQRight"),
    PrivateTag(0x0299, 0x10, "NQRight"),
    PrivateTag(0x0299, 0x11, "NQRight"),
    PrivateTag(0x0299, 0x12, "NQRight"),
    PrivateTag(0x0299, 0x13, "NQRight"),
    PrivateTag(0x0299, 0x14, "NQRight"),
    PrivateTag(0x0299, 0x15, "NQRight"),
    PrivateTag(0x0299, 0x16, "NQRight"),
    PrivateTag(0x0903, 0x10, "GEIIS PACS"),
    PrivateTag(0x0903, 0x11, "GEIIS PACS"),
    PrivateTag(0x0903, 0x12, "GEIIS PACS"),
    PrivateTag(0x2001, 0x00, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x01, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x01, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x01, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x01, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x02, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x02, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x02, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x02, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x03, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x03, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x03, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x03, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x04, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x04, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x04, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x04, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x05, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x05, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x05, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x05, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x06, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x06, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x06, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x06, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x07, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x07, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x07, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x07, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x08, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x08, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x08, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x08, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x09, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x09, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x09, "Philips Imaging DD 129"),
    PrivateTag(0x2001, 0x0A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x0A, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x0A, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x0B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x0B, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x0B, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x0C, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x0C, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x0D, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x0D, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x0E, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x0E, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x0E, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x0F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x0F, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x0F, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x10, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x10, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x11, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x11, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x12, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x12, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x12, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x13, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x13, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x14, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x14, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x15, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x15, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x16, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x16, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x17, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x17, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x17, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x18, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x18, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x18, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x19, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x19, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x19, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x1A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x1A, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x1A, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x1B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x1B, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x1B, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x1C, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x1C, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x1C, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x1D, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x1D, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x1D, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x1E, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x1E, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x1E, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x1F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x1F, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x1F, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x20, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x21, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x21, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x21, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x22, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x22, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x22, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x23, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x23, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x23, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x24, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x24, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x24, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x25, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x25, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x25, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x26, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x26, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x26, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x27, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x27, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x27, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x28, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x28, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x28, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x29, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x29, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x2A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x2A, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x2A, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x2B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x2B, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x2B, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x2C, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x2C, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x2C, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x2D, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x2D, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x2D, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x2E, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x2E, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x2E, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x2F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x2F, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x2F, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x30, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x30, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x30, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x31, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x31, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x31, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x32, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x32, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x32, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x33, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x33, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x34, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x34, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x35, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x35, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x36, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x36, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x36, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x37, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x37, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x37, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x38, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x38, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x38, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x39, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x39, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x39, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x3A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x3A, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x3A, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x3B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x3B, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x3B, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x3C, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x3C, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x3C, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x3D, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x3D, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x3D, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x3E, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x3E, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x3E, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x3F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x3F, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x40, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x40, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x40, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x41, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x41, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x42, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x42, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x43, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x44, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x44, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x45, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x45, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x46, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x46, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x47, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x47, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x48, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x49, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x49, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x4A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x4A, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x4B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x4B, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x4C, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x4C, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0x4D, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x4E, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x4F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x50, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x50, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x51, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x52, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x53, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x53, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x54, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x55, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x56, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x57, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x57, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x58, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x58, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x59, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x5A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x5A, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x5C, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x5D, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x5D, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x5E, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x5E, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x5F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x5F, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x60, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x61, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x62, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x63, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x63, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x64, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x64, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x65, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x65, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x66, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x66, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x67, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x67, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x68, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x68, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x69, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x6A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x6B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x6B, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x6D, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x6E, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x6F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x71, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x71, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x72, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x72, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x73, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x73, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x74, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x74, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x75, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x75, "Philips Imaging DD 002"),
    PrivateTag(0x2001, 0x76, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x77, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x79, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x7A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x7B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x7C, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x7D, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x7E, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x7F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x80, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x81, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x82, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x83, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x84, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x85, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x86, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x87, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x88, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x89, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x8A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x8B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x8C, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x90, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x91, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x92, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x93, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x94, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x9A, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x9B, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x9D, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0x9F, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xA1, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xA1, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xA2, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xA2, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xA3, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xA3, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xA4, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xA4, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xA5, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xA5, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xA6, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xA8, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xA9, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xAA, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xAB, "Philips Imaging DD 097"),
    PrivateTag(0x2001, 0xC0, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xC1, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xC2, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xC3, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xC5, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xC6, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xC7, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xCA, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xCB, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD0, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD1, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD2, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD3, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD4, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD5, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD6, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD7, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD8, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xD9, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xDA, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xDB, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xDC, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xDD, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xDE, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xDF, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xE9, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xF1, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xF2, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xF3, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xF4, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xF5, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xF6, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xF7, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xF9, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xFB, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xFC, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xFD, "Philips Imaging DD 001"),
    PrivateTag(0x2001, 0xFF, "Philips Imaging DD 001"),
    PrivateTag(0x2005, 0x0D, "Philips MR Imaging DD 001"),
    PrivateTag(0x2005, 0x0E, "Philips MR Imaging DD 001"),
    PrivateTag(0x7053, 0x00, "Philips PET Private Group"),
    PrivateTag(0x7053, 0x09, "Philips PET Private Group"),
    PrivateTag(0x7E01, 0x01, "HOLOGIC, Inc."),
    PrivateTag(0x7E01, 0x02, "HOLOGIC, Inc."),
    PrivateTag(0x7E01, 0x10, "HOLOGIC, Inc."),
    PrivateTag(0x7E01, 0x11, "HOLOGIC, Inc."),
    PrivateTag(0x7E01, 0x12, "HOLOGIC, Inc."),
    PrivateTag(0x7FD1, 0x01, "SIEMENS SYNGO ULTRA-SOUND TOYON DATA STREAMING"),
    PrivateTag(0x7FD1, 0x01, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x7FD1, 0x09, "SIEMENS SYNGO ULTRA-SOUND TOYON DATA STREAMING"),
    PrivateTag(0x7FD1, 0x09, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x7FD1, 0x10, "SIEMENS SYNGO ULTRA-SOUND TOYON DATA STREAMING"),
    PrivateTag(0x7FD1, 0x10, "SIEMENS Ultrasound SC2000"),
    PrivateTag(0x7FD1, 0x11, "SIEMENS SYNGO ULTRA-SOUND TOYON DATA STREAMING"),
    PrivateTag(0x7FD1, 0x11, "SIEMENS Ultrasound SC2000"),
};

static inline bool in_part15(const PrivateTag &pt) {
  static const size_t len =
      sizeof(part15_table_E_1_1) / sizeof *part15_table_E_1_1;
  for (size_t i = 0; i < len; ++i) {
    if (part15_table_E_1_1[i] == pt) {
      return true;
    }
  }
  return false;
}

typedef std::set<DataElement> DataElementSet;
typedef DataElementSet::const_iterator ConstIterator;

struct Cleaner::impl {
  std::set<DPath> preserve_dpaths;
  std::set<DPath> empty_dpaths;
  std::set<DPath> remove_dpaths;
  std::set<DPath> scrub_dpaths;
  std::set<Tag> empty_tags;
  std::set<PrivateTag> empty_privatetags;
  std::set<Tag> remove_tags;
  std::set<PrivateTag> remove_privatetags;
  std::set<Tag> scrub_tags;
  std::set<PrivateTag> scrub_privatetags;
  std::set<VR> empty_vrs;
  std::set<VR> remove_vrs;
  bool AllMissingPrivateCreator;
  bool AllGroupLength;
  bool AllIllegal;
  impl()
      : AllMissingPrivateCreator(true),
        AllGroupLength(true),
        AllIllegal(true) {}

  enum ACTION { NONE, EMPTY, REMOVE, SCRUB };
  enum ACTION ComputeAction(File const &file, DataSet &ds,
                            const DataElement &de, VR const &ref_dict_vr,
                            const std::string &tag_path);

  bool ProcessDataSet(Subject &s, File &file, DataSet &ds,
                      const std::string &tag_path);

  template <typename T>
  bool CheckVRBeforeInsert(std::set<VR> &empty_or_remove_vrs, T const &t,
                           std::set<T> &set) {
    if (empty_or_remove_vrs.empty()) {
      set.insert(t);
      return true;
    } else {
      // Let's check if VR of tag is already contained in VR
      static const Global &g = GlobalInstance;
      static const Dicts &dicts = g.GetDicts();
      const DictEntry &entry = dicts.GetDictEntry(t);
      const VR &refvr = entry.GetVR();
      if (empty_or_remove_vrs.find(refvr) != empty_or_remove_vrs.end()) {
        gdcmWarningMacro(
            "Tag: " << t << " is also cleanup with VR cleaning. skipping");
      } else if (refvr == VR::INVALID) {
        gdcmWarningMacro("inserting unknown tag "
                         << t << ". no check on VR is done");
        set.insert(t);
      } else {
        set.insert(t);
      }
      return true;
    }
  }
  bool Empty(Tag const &t) {
    if (t.IsPublic() && !t.IsGroupLength()) {
      return CheckVRBeforeInsert(empty_vrs, t, empty_tags);
    }
    return false;
  }
  bool Empty(PrivateTag const &pt) {
    const char *owner = pt.GetOwner();
    if (pt.IsPrivate() && *owner) {
      if (in_part15(pt)) {
        gdcmErrorMacro("Cannot add Part 15 attribute for now");
        return false;
      }
      return CheckVRBeforeInsert(empty_vrs, pt, empty_privatetags);
    }
    return false;
  }
  bool Empty(DPath const &dpath) {
    empty_dpaths.insert(dpath);
    return true;
  }
  bool Empty(VR const &vr) {
    if (vr == VR::PN) {
      empty_vrs.insert(vr);
      return true;
    }
    return false;
  }

  bool Remove(Tag const &t) {
    if (t.IsPublic() && !t.IsGroupLength()) {
      return CheckVRBeforeInsert(remove_vrs, t, remove_tags);
    }
    return false;
  }
  bool Remove(PrivateTag const &pt) {
    const char *owner = pt.GetOwner();
    if (pt.IsPrivate() && *owner) {
      if (in_part15(pt)) {
        gdcmErrorMacro("Cannot add Part 15 attribute for now");
        return false;
      }
      return CheckVRBeforeInsert(remove_vrs, pt, remove_privatetags);
    }
    return false;
  }
  bool Remove(DPath const &dpath) {
    remove_dpaths.insert(dpath);
    return true;
  }
  bool Remove(VR const &vr) {
    if (vr == VR::PN) {
      remove_vrs.insert(vr);
      return true;
    }
    return false;
  }

  bool Scrub(Tag const &t) { return false; }
  bool Scrub(PrivateTag const &pt) {
    static const PrivateTag &csa1 = CSAHeader::GetCSAImageHeaderInfoTag();
    static const PrivateTag &csa2 = CSAHeader::GetCSASeriesHeaderInfoTag();
    const PrivateTag mec_mr3(0x700d, 0x08, "TOSHIBA_MEC_MR3");
    const PrivateTag pmtf1(0x0029, 0x90, "PMTF INFORMATION DATA");
    const PrivateTag pmtf2(0x0029, 0x90, "TOSHIBA_MEC_MR3");
    if (pt == csa1 || pt == csa2 || pt == mec_mr3 || pt == pmtf1 || pt == pmtf2) {
      scrub_privatetags.insert(pt);
      return true;
    }
    return false;
  }
  bool Scrub(DPath const &dpath) {
    scrub_dpaths.insert(dpath);
    return true;
  }

  bool Scrub(VR const &vr) { return false; }

  bool Preserve(DPath const &dpath) {
    preserve_dpaths.insert(dpath);
    return true;
  }

  void RemoveAllMissingPrivateCreator(bool remove) {
    AllMissingPrivateCreator = remove;
  }
  bool RemoveMissingPrivateCreator(Tag const &t) { return false; }
  void RemoveAllGroupLength(bool remove) { AllGroupLength = remove; }
  void RemoveAllIllegal(bool remove) { AllIllegal = remove; }
};

static VR ComputeDictVR(File &file, DataSet &ds, DataElement const &de) {
  VR dict_vr = de.GetVR();
  const Tag &tag = de.GetTag();
  bool compute_dict_vr = true;
  if (tag.IsPublic() || tag.IsGroupLength() || tag.IsPrivateCreator()) {
  } else {
    const PrivateTag pt = ds.GetPrivateTag(tag);
    const char *owner = pt.GetOwner();
    assert(owner);
    compute_dict_vr = *owner != 0;
  }
  if (compute_dict_vr) dict_vr = DataSetHelper::ComputeVR(file, ds, tag);

  if (de.GetVR() == VR::SQ) {
    assert(dict_vr != VR::UN);
    if (!dict_vr.Compatible(de.GetVR())) {
      gdcmErrorMacro("Impossible. Dict states VR is: "
                     << dict_vr << " which is impossible for SQ");
      dict_vr = VR::SQ;
    }
  }
  if (dict_vr != VR::SQ) {
    if (de.GetVL().IsUndefined()) {
      Tag pixelData(0x7fe0, 0x0010);
      assert(dict_vr == VR::OB);
      if (tag != pixelData) {
        gdcmErrorMacro("Impossible happen: " << de);
        return VR::SQ;
      }
    }
  }
  return dict_vr;
}

static inline std::string tostring(uint16_t const val, int const width = 4) {
  std::ostringstream oss;
  oss.setf(std::ios::right);
  oss << std::hex << std::setw(width) << std::setfill('0') << val;
  return oss.str();
}

static std::vector<std::string> tag2strings(DataSet const &ds, Tag const &tag) {
  std::vector<std::string> ret;
  if (tag.IsPublic() || tag.IsPrivateCreator() || tag.IsGroupLength()) {
    ret.push_back(tostring(tag.GetGroup()));
    ret.push_back(tostring(tag.GetElement()));
  } else {
    const PrivateTag pt = ds.GetPrivateTag(tag);
    ret.push_back(tostring(pt.GetGroup()));
    ret.push_back(tostring(pt.GetElement(), 2));
    ret.push_back(pt.GetOwner());
  }
  return ret;
}

template <typename T>
static void print_contents(std::ostream &oss, const std::vector<T> &v,
                           const char *const separator = ",") {
  if (!v.empty()) {
    std::copy(v.begin(), --v.end(), std::ostream_iterator<T>(oss, separator));
    oss << v.back();
  }
}

static bool isAllZero(const char *buffer, size_t len) {
  while (len-- > 0) {
    if (buffer[len] != 0) return false;
  }
  return true;
}

static bool CleanCSA(DataSet &ds, const DataElement &de) {
  const ByteValue *bv = de.GetByteValue();
  // fast path:
  if (!bv) return true;
  static const char vs01[] = "VS01";
  // bogus big-endian conversion
  if (bv->GetLength() >= 4 && memcmp(bv->GetPointer(), vs01, 4) == 0) {
    // technically there is digital trash, but since it is written in byte-swap
    // mode, it cannot be detected easily.
    return true;
  }
  static const char pds_com[] = "<pds><com>";
  // PET_REPLAY_PARAM case:
  if (bv->GetLength() >= 10 && memcmp(bv->GetPointer(), pds_com, 10) == 0) {
    return true;
  }
  static const char end[] = "END!      ";
  if (bv->GetLength() >= 10 &&
      memcmp(bv->GetPointer() + bv->GetLength() - 10, end, 10) == 0) {
    return true;
  }
  const bool zero = isAllZero(bv->GetPointer(), bv->GetLength());
  if (zero) return true;

  DataElement clean(de.GetTag());
  clean.SetVR(de.GetVR());
  std::vector<char> v;
  v.resize(bv->GetLength());
  if (csa_memcpy(&v[0], bv->GetPointer(), bv->GetLength())) {
    clean.SetByteValue(&v[0], v.size());
    ds.Replace(clean);
    return true;
  }
  gdcmErrorMacro("Failure to call CleanCSA");
  return false;
}

static bool CleanMEC_MR3(DataSet &ds, const DataElement &de) {
  const ByteValue *bv = de.GetByteValue();
  // fast path:
  if (!bv) return true;

  DataElement clean(de.GetTag());
  clean.SetVR(de.GetVR());
  std::vector<char> v;
  v.resize(bv->GetLength());
  if (mec_mr3_memcpy(&v[0], bv->GetPointer(), bv->GetLength())) {
    clean.SetByteValue(&v[0], v.size());
    ds.Replace(clean);
    return true;
  }
  gdcmErrorMacro("Failure to call CleanMEC_MR3");
  return false;
}

static bool CleanPMTF(DataSet &ds, const DataElement &de) {
  const ByteValue *bv = de.GetByteValue();
  // fast path:
  if (!bv) return true;
  const char *input = bv->GetPointer();
  const size_t len = bv->GetLength();

  bool success = true;
  try {
    std::istringstream is;
    {
      std::vector<char> copy(input, input + len);
      std::reverse(copy.begin(), copy.end());
      std::string dup(&copy[0], copy.size());
      is.str(dup);
    }

    // FIXME gdcm::Cleaner will by default change defined length SQ into undef
    // length...there is a risk of incompatibily with vendor
    gdcm::Cleaner cleaner;
    gdcm::File &file = cleaner.GetFile();
    gdcm::FileMetaInformation &fmi = file.GetHeader();
    fmi.SetDataSetTransferSyntax(gdcm::TransferSyntax::ExplicitVRLittleEndian);
    gdcm::DataSet &revds = file.GetDataSet();
    revds.Read<gdcm::ExplicitDataElement, gdcm::SwapperNoOp>(is);

    gdcm::VR vr = VR::PN;
    cleaner.Empty(vr);
    if (!cleaner.Clean()) {
      success = false;
    } else {
      std::ostringstream os;
      revds.Write<gdcm::ExplicitDataElement, gdcm::SwapperNoOp>(os);
      const std::string str = os.str();
      std::vector<char> v(str.c_str(), str.c_str() + str.size());
      std::reverse(v.begin(), v.end());

      DataElement clean(de.GetTag());
      clean.SetVR(de.GetVR());
      clean.SetByteValue(&v[0], v.size());
      ds.Replace(clean);
    }
  } catch (...) {
    success = false;
  }

  if (success) {
    return true;
  }
  gdcmErrorMacro("Failure to call CleanPMTF");
  return false;
}

static DPath ConstructDPath(std::string const &tag_path, const DataSet &ds,
                            const Tag &tag) {
  DPath dpath;
  std::ostringstream oss;
  oss << tag_path;
  const std::vector<std::string> tag_strings = tag2strings(ds, tag);
  print_contents(oss, tag_strings);
  dpath.ConstructFromString(oss.str().c_str());
  return dpath;
}

static bool IsDPathInSet(std::set<DPath> const &aset, DPath const dpath) {
  bool found = false;
  for (std::set<DPath>::const_iterator it = aset.begin();
       found == false && it != aset.end(); ++it) {
    found = it->Match(dpath);
  }

  return found;
}

Cleaner::impl::ACTION Cleaner::impl::ComputeAction(
    File const &file, DataSet &ds, const DataElement &de, VR const &ref_dict_vr,
    const std::string &tag_path) {
  const Tag &tag = de.GetTag();
  // Group Length & Illegal cannot be preserved so it is safe to do them now:
  if (tag.IsGroupLength()) {
    if (AllGroupLength) return Cleaner::impl::REMOVE;
  } else if (tag.IsIllegal()) {
    if (AllIllegal) return Cleaner::impl::REMOVE;
  }

  if (tag.IsPublic()) {
    const DPath dpath = ConstructDPath(tag_path, ds, tag);
    // Preserve
    if (IsDPathInSet(preserve_dpaths, dpath)) return Cleaner::impl::NONE;
    // Scrub
    if (scrub_tags.find(tag) != scrub_tags.end() ||
        IsDPathInSet(scrub_dpaths, dpath)) {
      return Cleaner::impl::SCRUB;
    }
    // Empty
    if (empty_tags.find(tag) != empty_tags.end() ||
        IsDPathInSet(empty_dpaths, dpath)) {
      assert(!tag.IsGroupLength());
      assert(!tag.IsPrivateCreator());
      assert(ds.FindDataElement(tag));
      return Cleaner::impl::EMPTY;
    }
    // Remove
    if (remove_tags.find(tag) != remove_tags.end() ||
        IsDPathInSet(remove_dpaths, dpath)) {
      return Cleaner::impl::REMOVE;
    }
  }

  if (tag.IsPrivate() && !tag.IsPrivateCreator() && !tag.IsGroupLength()) {
    const PrivateTag pt = ds.GetPrivateTag(tag);
    const char *owner = pt.GetOwner();
    assert(owner);
    if (*owner == 0 && AllMissingPrivateCreator) {
      return Cleaner::impl::REMOVE;
    }
    // At this point we have a private creator, it makes sense to check for
    // preserve: Preserve
    const DPath dpath = ConstructDPath(tag_path, ds, tag);
    if (IsDPathInSet(preserve_dpaths, dpath)) return Cleaner::impl::NONE;
    // Scrub
    if (scrub_privatetags.find(pt) != scrub_privatetags.end() ||
        IsDPathInSet(scrub_dpaths, dpath)) {
      return Cleaner::impl::SCRUB;
    }
    // Empty
    if (empty_privatetags.find(pt) != empty_privatetags.end() ||
        IsDPathInSet(empty_dpaths, dpath)) {
      return Cleaner::impl::EMPTY;
    }
    // Remove
    if (remove_privatetags.find(pt) != remove_privatetags.end() ||
        IsDPathInSet(remove_dpaths, dpath)) {
      return Cleaner::impl::REMOVE;
    }
  }

  // VR cleanup
  if (!empty_vrs.empty() || !remove_vrs.empty()) {
    VR vr = de.GetVR();
    assert(ref_dict_vr != VR::INVALID);
    // be careful with vr handling since we must always prefer the one from the
    // dict in case of attribute written as 'OB' but dict states 'PN':
    if (ref_dict_vr != VR::UN /*&& ref_dict_vr != VR::INVALID*/) {
      // we want to clean VR==PN; but this is a problem for implicit transfer
      // syntax, so let's be nice to the user and prefer dict_vr. however for
      // explicit, do not assume value in dict can take over the read VR
      if (vr == VR::UN || vr == VR::INVALID) {
        vr = ref_dict_vr;
      }
      if (vr != ref_dict_vr) {
        // assert(vr == VR::OB || vr == VR::OW);
        vr = ref_dict_vr;
      }
    }
    // Empty
    if (empty_vrs.find(vr) != empty_vrs.end()) {
      return Cleaner::impl::EMPTY;
    }
    // Remove
    if (remove_vrs.find(vr) != remove_vrs.end()) {
      return Cleaner::impl::REMOVE;
    }
  }

  // default action:
  return Cleaner::impl::NONE;
}

bool Cleaner::impl::ProcessDataSet(Subject &subject, File &file, DataSet &ds,
                                   const std::string &tag_path) {
  subject.InvokeEvent(IterationEvent());
  ConstIterator it = ds.GetDES().begin();

  for (; it != ds.GetDES().end(); /*++it*/) {
    const DataElement &de = *it;
    ++it;  // 'Remove/Empty' may invalidate iterator
    const Tag &tag = de.GetTag();
    AnonymizeEvent ae;
    ae.SetTag(tag);

    VR dict_vr = ComputeDictVR(file, ds, de);
    Cleaner::impl::ACTION action =
        Cleaner::impl::ComputeAction(file, ds, de, dict_vr, tag_path);

    if (action == Cleaner::impl::NONE) {
      // nothing to do, but recurse in nested-dataset:
      if (dict_vr == VR::SQ) {
        SmartPointer<SequenceOfItems> sqi = de.GetValueAsSQ();
        if (sqi) {
          SequenceOfItems::SizeType s = sqi->GetNumberOfItems();
          for (SequenceOfItems::SizeType i = 1; i <= s; ++i) {
            Item &item = sqi->GetItem(i);

            DataSet &nestedds = item.GetNestedDataSet();
            const std::vector<std::string> tag_strings = tag2strings(ds, tag);

            std::ostringstream os;
            os << tag_path;  // already padded with trailing '/'
            print_contents(os, tag_strings);
            os << '/';
            os << '*';  // no need for item numbering
            os << '/';

            if (!ProcessDataSet(subject, file, nestedds, os.str())) {
              gdcmErrorMacro("Error processing Item #" << i);
              return false;
            }
            // Simple memcmp to avoid recomputation of Item Length: make them
            // undefined length. TODO would be nice to only do this when
            // strictly needed.
            item.SetVLToUndefined();
          }
          // Simple mechanism to avoid recomputation of Sequence Length: make
          // them undefined length
          DataElement dup(de.GetTag());
          dup.SetVR(VR::SQ);
          dup.SetValue(*sqi);
          dup.SetVLToUndefined();
          ds.Replace(dup);
        } else {
          // SmartPointer<SequenceOfItems> sqi = de.GetValueAsSQ();
          if (!de.IsEmpty()) {
            gdcmWarningMacro(
                "Please report. Dictionary states this should be a SQ. But we "
                "failed to load it as such. Passing-through as-is"
                << de);
          }
        }
      }
    } else if (action == Cleaner::impl::EMPTY) {
      DataElement clean(de.GetTag());
      clean.SetVR(de.GetVR());
      ds.Replace(clean);
      subject.InvokeEvent(ae);
    } else if (action == Cleaner::impl::REMOVE) {
      ds.Remove(tag);
      subject.InvokeEvent(ae);
    } else if (action == Cleaner::impl::SCRUB) {
      const PrivateTag pt = ds.GetPrivateTag(tag);

      static const PrivateTag &csa1 = CSAHeader::GetCSAImageHeaderInfoTag();
      static const PrivateTag &csa2 = CSAHeader::GetCSASeriesHeaderInfoTag();
      const PrivateTag mec_mr3(0x700d, 0x08, "TOSHIBA_MEC_MR3");
      const PrivateTag pmtf1(0x0029, 0x90, "PMTF INFORMATION DATA");
      const PrivateTag pmtf2(0x0029, 0x90, "TOSHIBA_MEC_MR3");

      if (pt == csa1) {
        const bool ret = CleanCSA(ds, de);
        if (!ret) return false;
      } else if (pt == csa2) {
        const bool ret = CleanCSA(ds, de);
        if (!ret) return false;
      } else if (pt == mec_mr3) {
        const bool ret = CleanMEC_MR3(ds, de);
        if (!ret) return false;
      } else if (pt == pmtf1) {
        const bool ret = CleanPMTF(ds, de);
        if (!ret) return false;
      } else if (pt == pmtf2) {
        const bool ret = CleanPMTF(ds, de);
        if (!ret) return false;
      } else {
        gdcmErrorMacro(" not implemented");
        return false;
      }
      subject.InvokeEvent(ae);
    } else {
      gdcmErrorMacro("Missing handling of action: " << action);
      return false;
    }
  }
  return true;
}

Cleaner::Cleaner() : F(new File), pimpl(new impl) {}

Cleaner::~Cleaner() { delete pimpl; }

bool Cleaner::Empty(Tag const &t) { return pimpl->Empty(t); }
bool Cleaner::Empty(PrivateTag const &pt) { return pimpl->Empty(pt); }
bool Cleaner::Empty(DPath const &dpath) { return pimpl->Empty(dpath); }
bool Cleaner::Empty(VR const &vr) { return pimpl->Empty(vr); }

bool Cleaner::Remove(Tag const &t) { return pimpl->Remove(t); }
bool Cleaner::Remove(PrivateTag const &pt) { return pimpl->Remove(pt); }
bool Cleaner::Remove(DPath const &dpath) { return pimpl->Remove(dpath); }
bool Cleaner::Remove(VR const &vr) { return pimpl->Remove(vr); }

bool Cleaner::Scrub(Tag const &t) { return pimpl->Scrub(t); }
bool Cleaner::Scrub(PrivateTag const &pt) { return pimpl->Scrub(pt); }
bool Cleaner::Scrub(DPath const &dpath) { return pimpl->Scrub(dpath); }
bool Cleaner::Scrub(VR const &vr) { return pimpl->Scrub(vr); }

bool Cleaner::Preserve(DPath const &dpath) { return pimpl->Preserve(dpath); }

void Cleaner::RemoveAllMissingPrivateCreator(bool remove) {
  pimpl->RemoveAllMissingPrivateCreator(remove);
}
bool Cleaner::RemoveMissingPrivateCreator(Tag const &t) {
  return pimpl->RemoveMissingPrivateCreator(t);
}
void Cleaner::RemoveAllGroupLength(bool remove) {
  pimpl->RemoveAllGroupLength(remove);
}
void Cleaner::RemoveAllIllegal(bool remove) { pimpl->RemoveAllIllegal(remove); }

bool Cleaner::Clean() {
  DataSet &ds = F->GetDataSet();
  this->InvokeEvent(StartEvent());
  const bool ret = pimpl->ProcessDataSet(*this, *F, ds, "/");
  this->InvokeEvent(EndEvent());
  return ret;
}

}  // end namespace gdcm
