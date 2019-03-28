/*! \file d7ap_fs_data.c
 *

 *  \copyright (C) Copyright 2019 University of Antwerp and others (http://mosaic-lopow.github.io/dash7-ap-open-source-stack/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  \author glenn.ergeerts@uantwerpen.be
 *
 */

#include "d7ap_fs.h"
#include "platform_defs.h"
#include "MODULE_D7AP_defs.h"

#ifdef MODULE_D7AP_USE_DEFAULT_SYSTEMFILES

// The cog section below does not generate code but defines some global variables and functions which are used in subsequent cog sections below,
// which do the actual code generation

/*[[[cog
import cog
from d7a.system_files.system_files import SystemFiles
from d7a.system_files.access_profile import AccessProfileFile
from d7a.system_files.dll_config import DllConfigFile
from d7a.system_files.firmware_version import FirmwareVersionFile
from d7a.system_files.system_file_ids import SystemFileIds
from d7a.system_files.not_implemented import NotImplementedFile
from d7a.system_files.security_key import SecurityKeyFile
from d7a.system_files.uid import UidFile
from d7a.system_files.engineering_mode import EngineeringModeFile
from d7a.fs.file_permissions import FilePermissions
from d7a.fs.file_properties import FileProperties
from d7a.fs.file_properties import ActionCondition, StorageClass, FileProperties
from d7a.fs.file_header import FileHeader
from d7a.dll.access_profile import AccessProfile, CsmaCaMode, SubBand
from d7a.dll.sub_profile import SubProfile
from d7a.phy.channel_header import ChannelHeader, ChannelBand, ChannelCoding, ChannelClass
from d7a.types.ct import CT

default_channel_header = ChannelHeader(
  channel_class=ChannelClass.LO_RATE,
  channel_coding=ChannelCoding.FEC_PN9,
  channel_band=ChannelBand.BAND_868
)

default_channel_index = 0

# AP used by GW doing continuous FG scan
ap_cont_fg_scan = AccessProfile(
  channel_header=default_channel_header,
  sub_profiles=[SubProfile(subband_bitmap=0x01, scan_automation_period=CT.compress(0))] * 4,
  sub_bands=[SubBand(eirp=14, channel_index_start=default_channel_index, channel_index_end=default_channel_index)] * 8
)

# AP used for scanning for BG request every second
ap_bg_scan = AccessProfile(
  channel_header=default_channel_header,
  sub_profiles=[SubProfile(subband_bitmap=0x01, scan_automation_period=CT.compress(1024))] * 4,
  sub_bands=[SubBand(eirp=14, channel_index_start=default_channel_index, channel_index_end=default_channel_index)] * 8
)

# AP used for push only, no scanning
ap_no_scan = AccessProfile(
  channel_header=default_channel_header,
  sub_profiles=[SubProfile(subband_bitmap=0x00, scan_automation_period=CT.compress(0))] * 4,
  sub_bands=[SubBand(eirp=14, channel_index_start=default_channel_index, channel_index_end=default_channel_index)] * 8
)

system_files = [
  UidFile(),
  NotImplementedFile(SystemFileIds.FACTORY_SETTINGS.value, 1),
  FirmwareVersionFile(),
  NotImplementedFile(SystemFileIds.DEVICE_CAPACITY.value, 19),
  NotImplementedFile(SystemFileIds.DEVICE_STATUS.value, 9),
  EngineeringModeFile(),
  NotImplementedFile(SystemFileIds.VID.value, 3),
  NotImplementedFile(SystemFileIds.RFU_07.value, 0),
  NotImplementedFile(SystemFileIds.PHY_CONFIG.value, 9),
  NotImplementedFile(SystemFileIds.PHY_STATUS.value, 24),  # TODO assuming 3 channels for now
  DllConfigFile(active_access_class=0x21),
  NotImplementedFile(SystemFileIds.DLL_STATUS.value, 12),
  NotImplementedFile(SystemFileIds.NWL_ROUTING.value, 1),  # TODO variable routing table
  NotImplementedFile(SystemFileIds.NWL_SECURITY.value, 5),
  SecurityKeyFile(),
  NotImplementedFile(SystemFileIds.NWL_SSR.value, 4),  # TODO 0 recorded devices
  NotImplementedFile(SystemFileIds.NWL_STATUS.value, 20),
  NotImplementedFile(SystemFileIds.TRL_STATUS.value, 1),  # TODO 0 TRL records
  NotImplementedFile(SystemFileIds.SEL_CONFIG.value, 6),
  NotImplementedFile(SystemFileIds.FOF_STATUS.value, 10),
  NotImplementedFile(SystemFileIds.RFU_14.value, 0),
  NotImplementedFile(SystemFileIds.RFU_15.value, 0),
  NotImplementedFile(SystemFileIds.RFU_16.value, 0),
  NotImplementedFile(SystemFileIds.LOCATION_DATA.value, 1),  # TODO 0 recorded locations
  NotImplementedFile(SystemFileIds.D7AALP_RFU_18.value, 0),
  NotImplementedFile(SystemFileIds.D7AALP_RFU_19.value, 0),
  NotImplementedFile(SystemFileIds.D7AALP_RFU_1A.value, 0),
  NotImplementedFile(SystemFileIds.D7AALP_RFU_1B.value, 0),
  NotImplementedFile(SystemFileIds.D7AALP_RFU_1C.value, 0),
  NotImplementedFile(SystemFileIds.D7AALP_RFU_1D.value, 0),
  NotImplementedFile(SystemFileIds.D7AALP_RFU_1E.value, 0),
  NotImplementedFile(SystemFileIds.D7AALP_RFU_1F.value, 0),
  AccessProfileFile(0, ap_cont_fg_scan),
  AccessProfileFile(1, ap_bg_scan),
  AccessProfileFile(2, ap_no_scan),
  AccessProfileFile(3, ap_no_scan),
  AccessProfileFile(4, ap_no_scan),
  AccessProfileFile(5, ap_no_scan),
  AccessProfileFile(6, ap_no_scan),
  AccessProfileFile(7, ap_no_scan),
  AccessProfileFile(8, ap_no_scan),
  AccessProfileFile(9, ap_no_scan),
  AccessProfileFile(10, ap_no_scan),
  AccessProfileFile(11, ap_no_scan),
  AccessProfileFile(12, ap_no_scan),
  AccessProfileFile(13, ap_no_scan),
  AccessProfileFile(14, ap_no_scan)
]

sys_file_permission_default = FilePermissions(encrypted=False, executeable=False, user_readable=True, user_writeable=False, user_executeable=False,
                   guest_readable=True, guest_writeable=False, guest_executeable=False)
sys_file_permission_non_readable = FilePermissions(encrypted=False, executeable=False, user_readable=False, user_writeable=False, user_executeable=False,
                   guest_readable=False, guest_writeable=False, guest_executeable=False)
sys_file_prop_default = FileProperties(act_enabled=False, act_condition=ActionCondition.WRITE, storage_class=StorageClass.PERMANENT)

def output_file(file):
  file_type = SystemFileIds(file.id)
  cog.outl("\t// {} - {}".format(file_type.name, file_type.value))
  file_array_elements = "\t"
  for byte in bytearray(file):
    file_array_elements += "{}, ".format(hex(byte))

  cog.outl(file_array_elements)

def output_fileheader(file):
  file_type = SystemFileIds(system_file.id)
  cog.outl("\t// {} - {}".format(file_type.name, file_type.value))
  file_header = FileHeader(permissions=file_permissions, properties=sys_file_prop_default, alp_command_file_id=0xFF, interface_file_id=0xFF, file_size=system_file.length, allocated_size=system_file.length)
  file_header_array_elements = "\t"
  for byte in bytearray(file_header):
    file_header_array_elements += "{}, ".format(hex(byte))

  cog.outl(file_header_array_elements)

def output_system_file_offsets():
  current_offset = 0
  for system_file in system_files:
    file_type = SystemFileIds(system_file.id)
    cog.outl("\t{}, // {} - {} (length {}))".format(hex(current_offset), file_type.name, file_type.value, system_file.length))
    current_offset += system_file.length
]]]*/
//[[[end]]] (checksum: d41d8cd98f00b204e9800998ecf8427e)

#ifdef PLATFORM_FS_SYSTEMFILES_IN_SEPARATE_LINKER_SECTION
  #define LINKER_SECTION_FS_SYSTEM_FILE __attribute__((section(".d7ap_fs_systemfiles")))
#else
  #define LINKER_SECTION_FS_SYSTEM_FILE
#endif
fs_systemfiles_t fs_systemfiles __attribute__((used)) LINKER_SECTION_FS_SYSTEM_FILE = {
  .magic_number = D7AP_FS_MAGIC_NUMBER,
  .header_data = {
      /*[[[cog
      file_permissions = sys_file_permission_default
      for system_file in system_files:
        output_fileheader(system_file)
      ]]]*/
      // UID - 0
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0, 0x0, 0x8, 
      // FACTORY_SETTINGS - 1
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 
      // FIRMWARE_VERSION - 2
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0xf, 0x0, 0x0, 0x0, 0xf, 
      // DEVICE_CAPACITY - 3
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x13, 0x0, 0x0, 0x0, 0x13, 
      // DEVICE_STATUS - 4
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x9, 
      // ENGINEERING_MODE - 5
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x9, 
      // VID - 6
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x3, 
      // RFU_07 - 7
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // PHY_CONFIG - 8
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x9, 
      // PHY_STATUS - 9
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x18, 0x0, 0x0, 0x0, 0x18, 
      // DLL_CONFIG - 10
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x3, 
      // DLL_STATUS - 11
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0xc, 0x0, 0x0, 0x0, 0xc, 
      // NWL_ROUTING - 12
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 
      // NWL_SECURITY - 13
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0, 0x0, 0x5, 
      // NWL_SECURITY_KEY - 14
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0, 0x10, 
      // NWL_SSR - 15
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x4, 
      // NWL_STATUS - 16
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x14, 0x0, 0x0, 0x0, 0x14, 
      // TRL_STATUS - 17
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 
      // SEL_CONFIG - 18
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 0x6, 
      // FOF_STATUS - 19
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0xa, 0x0, 0x0, 0x0, 0xa, 
      // RFU_14 - 20
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // RFU_15 - 21
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // RFU_16 - 22
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // LOCATION_DATA - 23
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 
      // D7AALP_RFU_18 - 24
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // D7AALP_RFU_19 - 25
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // D7AALP_RFU_1A - 26
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // D7AALP_RFU_1B - 27
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // D7AALP_RFU_1C - 28
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // D7AALP_RFU_1D - 29
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // D7AALP_RFU_1E - 30
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // D7AALP_RFU_1F - 31
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // ACCESS_PROFILE_0 - 32
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_1 - 33
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_2 - 34
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_3 - 35
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_4 - 36
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_5 - 37
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_6 - 38
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_7 - 39
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_8 - 40
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_9 - 41
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_10 - 42
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_11 - 43
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_12 - 44
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_13 - 45
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      // ACCESS_PROFILE_14 - 46
      0x24, 0x23, 0xff, 0xff, 0x0, 0x0, 0x0, 0x41, 0x0, 0x0, 0x0, 0x41, 
      //[[[end]]] (checksum: 5b12650d4774ea114679f5e2cd12b882)
  },
  .file_data = {
      /*[[[cog
      for system_file in system_files:
        output_file(system_file)
      ]]]*/
      // UID - 0
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // FACTORY_SETTINGS - 1
      0x00,
      // FIRMWARE_VERSION - 2
      0x0, 0x0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
      // DEVICE_CAPACITY - 3
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // DEVICE_STATUS - 4
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // ENGINEERING_MODE - 5
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // VID - 6
      0x0, 0x0, 0x0, 
      // RFU_07 - 7

      // PHY_CONFIG - 8
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // PHY_STATUS - 9
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // DLL_CONFIG - 10
      0x21, 0xff, 0xff, 
      // DLL_STATUS - 11
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // NWL_ROUTING - 12
      0x0, 
      // NWL_SECURITY - 13
      0x0, 0x0, 0x0, 0x0, 0x0, 
      // NWL_SECURITY_KEY - 14
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // NWL_SSR - 15
      0x0, 0x0, 0x0, 0x0, 
      // NWL_STATUS - 16
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // TRL_STATUS - 17
      0x0, 
      // SEL_CONFIG - 18
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // FOF_STATUS - 19
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
      // RFU_14 - 20

      // RFU_15 - 21

      // RFU_16 - 22

      // LOCATION_DATA - 23
      0x0, 
      // D7AALP_RFU_18 - 24

      // D7AALP_RFU_19 - 25

      // D7AALP_RFU_1A - 26

      // D7AALP_RFU_1B - 27

      // D7AALP_RFU_1C - 28

      // D7AALP_RFU_1D - 29

      // D7AALP_RFU_1E - 30

      // D7AALP_RFU_1F - 31

      // ACCESS_PROFILE_0 - 32
      0x32, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_1 - 33
      0x32, 0x1, 0x70, 0x1, 0x70, 0x1, 0x70, 0x1, 0x70, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_2 - 34
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_3 - 35
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_4 - 36
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_5 - 37
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_6 - 38
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_7 - 39
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_8 - 40
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_9 - 41
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_10 - 42
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_11 - 43
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_12 - 44
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_13 - 45
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      // ACCESS_PROFILE_14 - 46
      0x32, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 0x0, 0x0, 0x0, 0x0, 0xe, 0x56, 0xff, 
      //[[[end]]] (checksum: 39ae54c08fc3c0d6f08a50e94cf09cd8)
  }
};


// Store the offsets of the start of each system file in the data section, for fast lookup
// This is stored only in RAM since it doesn't take much space
__attribute__((used)) uint16_t fs_systemfiles_file_offsets[] = {
  /*[[[cog
  output_system_file_offsets()
  ]]]*/
  0x0, // UID - 0 (length 8))
  0x8, // FACTORY_SETTINGS - 1 (length 1))
  0x9, // FIRMWARE_VERSION - 2 (length 15))
  0x18, // DEVICE_CAPACITY - 3 (length 19))
  0x2b, // DEVICE_STATUS - 4 (length 9))
  0x34, // ENGINEERING_MODE - 5 (length 9))
  0x3d, // VID - 6 (length 3))
  0x40, // RFU_07 - 7 (length 0))
  0x40, // PHY_CONFIG - 8 (length 9))
  0x49, // PHY_STATUS - 9 (length 24))
  0x61, // DLL_CONFIG - 10 (length 3))
  0x64, // DLL_STATUS - 11 (length 12))
  0x70, // NWL_ROUTING - 12 (length 1))
  0x71, // NWL_SECURITY - 13 (length 5))
  0x76, // NWL_SECURITY_KEY - 14 (length 16))
  0x86, // NWL_SSR - 15 (length 4))
  0x8a, // NWL_STATUS - 16 (length 20))
  0x9e, // TRL_STATUS - 17 (length 1))
  0x9f, // SEL_CONFIG - 18 (length 6))
  0xa5, // FOF_STATUS - 19 (length 10))
  0xaf, // RFU_14 - 20 (length 0))
  0xaf, // RFU_15 - 21 (length 0))
  0xaf, // RFU_16 - 22 (length 0))
  0xaf, // LOCATION_DATA - 23 (length 1))
  0xb0, // D7AALP_RFU_18 - 24 (length 0))
  0xb0, // D7AALP_RFU_19 - 25 (length 0))
  0xb0, // D7AALP_RFU_1A - 26 (length 0))
  0xb0, // D7AALP_RFU_1B - 27 (length 0))
  0xb0, // D7AALP_RFU_1C - 28 (length 0))
  0xb0, // D7AALP_RFU_1D - 29 (length 0))
  0xb0, // D7AALP_RFU_1E - 30 (length 0))
  0xb0, // D7AALP_RFU_1F - 31 (length 0))
  0xb0, // ACCESS_PROFILE_0 - 32 (length 65))
  0xf1, // ACCESS_PROFILE_1 - 33 (length 65))
  0x132, // ACCESS_PROFILE_2 - 34 (length 65))
  0x173, // ACCESS_PROFILE_3 - 35 (length 65))
  0x1b4, // ACCESS_PROFILE_4 - 36 (length 65))
  0x1f5, // ACCESS_PROFILE_5 - 37 (length 65))
  0x236, // ACCESS_PROFILE_6 - 38 (length 65))
  0x277, // ACCESS_PROFILE_7 - 39 (length 65))
  0x2b8, // ACCESS_PROFILE_8 - 40 (length 65))
  0x2f9, // ACCESS_PROFILE_9 - 41 (length 65))
  0x33a, // ACCESS_PROFILE_10 - 42 (length 65))
  0x37b, // ACCESS_PROFILE_11 - 43 (length 65))
  0x3bc, // ACCESS_PROFILE_12 - 44 (length 65))
  0x3fd, // ACCESS_PROFILE_13 - 45 (length 65))
  0x43e, // ACCESS_PROFILE_14 - 46 (length 65))
  //[[[end]]] (checksum: 0adb2f883d72be16ca76884aa87acee0)
};


#endif

// The userfiles are only stored in RAM for now
fs_file_header_t fs_userfiles_header_data[FRAMEWORK_FS_USER_FILE_COUNT];
uint8_t fs_userfiles_file_data[FRAMEWORK_FS_USER_FILESYSTEM_SIZE];

