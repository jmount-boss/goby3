# Goby Release Notes (Major version 3)

## Version 3.1.3

### General

- Updates to configuration reader (called by ProtobufConfigurator) to better support creation of command line tools (of the style 'tool <action> <subaction> [--options]'). New `goby::middleware::ToolHelper` class for tools to use.
	+ Support for environmental variables
	+ Support for custom positional parameters to replace default `<cfg_file> <app_name>`
	+ Support for custom short (e.g., '-l' for '--library') command line options.
- New `goby` command line tool that unifies existing tools and apps into a single point of entry and adds new tools (most notable are `goby zeromq publish` and `goby zeromq subscribe`, allowing command line publish/subscribe to `gobyd`):
  - `goby log`: Manage goby log files
	  + `convert`: Convert .goby log files to other formats [goby_log_tool]
  - `goby launch`: Launch goby *.launch files [goby_launch]
  - `goby zeromq`: Interact with ZeroMQ Goby pub/sub [goby_zeromq_tool]
	  - `terminate`: Terminate Goby applications [goby_terminate]
 	  - `publish`: Publish a message (on interprocess)
      - `subscribe`: Subscribe to messages (on interprocess)
      - `playback`: Playback .goby log files [goby_playback]
      - `daemon`: Publish/subscribe broker for interprocess and, optionally, intervehicle portal [gobyd]
      - `logger`: Binary logger of interprocess messages [goby_logger]
      - `coroner`: Monitoring of process health [goby_coroner]
      - `intervehicle_portal`: Standalone intervehicle portal [goby_intervehicle_portal]
      - `gps`: GPSD client that publishes data into Goby pub/sub [goby_gps]
      - `frontseat_interface`: Interface to vehicle Frontseat system (control system) [goby_frontseat_interface]
      - `geov`: Interface to Google Earth via GEOV (https://gobysoft.org/geov/) [goby_geov_interface]
      - `liaison`: Web-based UI for control/monitoring of Goby pub/sub [goby_liaison]
      - `opencpn`: Interface to the OpenCPN GUI [goby_opencpn_interface]
      - `moos_gateway`: Gateway to the MOOS middleware [goby_moos_gateway]
  - `goby protobuf`: Tools for the Google Protocol Buffers (protobuf) marshalling scheme
	  + `show`: Display definition for message




****************

## Version 3.1.2

### Bugs

- Added IPv6 support to most components


****************

## Version 3.1.1

### Bugs

- Added missing "inline" on intermodule function
- Added missing index on UDP threads, and use_indexed_groups on UDP/PTY threads.

### External Dependencies

Updated nlohmann JSON to 3.11.2

****************
Version 3.1.0

### Middleware

-  Update Subscription DCCL message to fill full uint32 (with varint) now that Group using uint32 group sizes. Also switch dccl_id to default id codec. Add intervehicle API version for future changes to Subscription message. Reworked Subscription message to use new DCCL ID 3 to distinguish from old Subscription message (DCCL ID 2) and moved dccl_id and group fields to front of message to detect future changes **Important: this necessary change makes Goby 3.0 and Goby 3.1 NOT wire compatible** for intervehicle publish/subscribe.

### Acomms

- Support for Iridium 9503 (RockBlock) and Rockblock SBD HTTP shore-side API in Iridium drivers. New `goby_rockblock_simulator` to simulate a system with N Rockblock modems and a shore-side server.

### General

- Add optional log rotation on a fixed time to glog files


### Bugs

- Remove warnings about boost::placeholder deprecation in boost::bind
- Create common functions for responding to goby_terminate and goby_coroner and add these to gobyd/goby_intervehicle_portal and TimerThread to get full responses for goby_coroner
- Improve Protobuf log plugin: recursively add field extensions to ensure we pick them all up at log writing time. 

****************
Version 3.0.17

### MOOS

- Support for Iver Remote Helm version 5 in Iver frontseat driver.
- Config to assign Iver modes to Frontseat states in Iver frontseat driver.

### goby_log_tool

- Add ability for goby_log_tool HDF5 output to chunk the HDF5 file to bound (and greatly reduce) memory requirements on larger files and to optional compress the chunked files. Use `--hdf5_chunk_length` to enable chunking, and (optionally) also `--hdf_compression_level` to enable compression.
- Added regexes to configuration to include or exclude groups and/or types from the output file.

### goby_gps

- Compute usat/nsat in goby_gps SkyView message (previously these were unpopulated fields).

### ModemDriver

- Add config for partial frame padding in mm_driver (and change default behavior to not pad frames)

### Bugs

- Catch exceptions in ModemDriverThread caused by bad DCCL messages
- Update Benthos driver to use "@P1Prompt" instead of "@Prompt". This will likely preclude older versions of the Benthos modem, but will support newer ones (exact Benthos version for this change is unknown).
- Fix computation of buffer size in DynamicBuffer when several unacked messages are in the buffer
- Remove deprecation warnings for newer ZMQ and Boost (for Ubuntu 22.04 and newer).

****************
Version 3.0.16

### Build

Support for Mac OS X builds
Support for DCCL 4.1 (forthcoming)

### MOOS

Blackout changed to double from int
iFrontSeat publishes NAV_YAW

****************
Version 3.0.15

### Acomms

- New ModemDriverBase report(...) method to provide information about link availability, link quality (if known), and any other data the driver wants to provide.

### Middleware

- Call new ModemDriverBase::report(...) regularly from intervehicle driver threads and publish to goby::middleware::intervehicle::modem_report.
- Improve names of glog groups for intervehicle acomms components.
- Add new goby_intervehicle_portal application that acts just like the intervehicle part of gobyd (so that the intervehicle drivers can be run in a separate app from the interprocess manager, etc. that gobyd provides). Users can use gobyd just as before, if desired.

### Bugs

- Fix lack of subscriptions when removing and creating a second InterProcessTransporter on the same thread (used by ModemDrivers).

### Build

- Fix build with MOOS enabled and ZeroMQ disabled

****************
Version 3.0.14

### Bugs

- Fix Liaison Commander crash with messages containing embeded messages that have "oneof" fields
- Fix bug where intermodule subscriptions only go to the first subscriber.


****************
Version 3.0.13

### Bugs

- Add missing codec_version to all Goby DCCL messages (required by DCCL4).
- Fix bug in goby_moos_gateway where subscriptions won't happen after MOOS is connected

****************
Version 3.0.12

### Bugs

- Fix goby_gps race condition where it will occasionally miss the gpsd connect and thus never send the WATCH command (and thus never receive GPS data).
- Switch Thread to use steady_clock to calculate loop() to reduce strange behavior if the system clock jumps (e.g. due to NTP correction on startup).


****************
Version 3.0.11

### Debian packages

- Added support for Ubuntu Jammy (22.04)
- Dropped support for end-of-life Debian Stretch (9)
- Changed from Clang to GCC for building Debian packages to workaround ABI incompatibility problem with goby_liaison on Debian 10 armhf (and possibly other armhf builds).

### Bugs

- Add support for Proj.6+ API while maintaining support for Proj.4 old API.

****************
Version 3.0.10

### FrontSeat

- Iver driver publish node status on $OSI update as well as on $C

### Bugs

- Fix health reporting bug from MultiThreadApplication that prevented reporting of health status less than OK
- Fix Goby log tool to always pick up extensions.
- Support updates in released DCCL4

****************
Version 3.0.9

### Logging

- Add --format=JSON output to goby_log_tool and implemented it for Protobuf/DCCL, and JSON formats
- Add flag --write_hdf5_zero_length_dim to goby_log_tool that when set to false does not write zero length dimensions to the HDF5 file (which caused problems with older HDF5 libraries on Windows). Default is true, which means this change does not affect the default behavior.
- Add start/stop/rotate subscription to goby_logger.

### Coroner

- Do not automatically add apps that are not on --expected_name list unless --auto_add_new_apps is set. This changes the default behavior, which had the potential to be confusing (fixes https://github.com/GobySoft/goby3/issues/249).

### Liaison

Various efficiency improvements in Scope (fixes https://github.com/GobySoft/goby3/issues/250):

- Set maximum message size to be handled (messages larger than this are omitted by the Scope) to avoid CPU lockups when huge messages are received
- Improve regex functionality to exclude messages from processing rather than just from visualization. This should improve performance when omitting messages
- Add update frequency to be configurable in the Scope UI.

### Bugs

- Fix race condition with subscription from HealthMonitorThread by ensure it's launched after InterProcessPortal is constructed. This should fix bug where apps where running fine but not responding to the goby_coroner health request.
- Fix bug in goby_logger Protobuf/DCCL extension protos were not being embedded correctly in the .goby log file (no data was lost but the Protobufs were then required when running goby_log_tool). Partially fixes https://github.com/GobySoft/goby3/issues/247



****************
Version 3.0.8

### Liaison
- Support for oneof in Commander fields

### Bugs
- Fix "Host not found" errors in various apps using Boost ASIO when loopback is the only interface configured.

****************
Version 3.0.7

### Middleware
- Change goby::middleware::Group numeric value from uint8 to uint32
- Improve support for goby_coroner - MultiThreadApplication now queries its child threads and receives their health back to put in the overall report
- gobyd now correctly quits (rather than hangs) if a ModemDriver has an uncaught exception

### Liaison
- Support for DCCL4 dynamic conditionals (when using DCCL4) in Liaison Commander forms

### Build
- Support Debian Bullseye

### MOOS
- Support pHelmIvP standby mode in iFrontSeat
- Fix bug parsing MOOS log file when pLogger block is involved ("log=" value)
- Add wildcard trigger option to TranslatorBase

****************
Version 3.0.6

### Util

- Fix bug in geodesy where Zone 60 (174-180 degrees E) was report at Zone 0 (does not exist) and thus caused an exception.

### Middleware

- Add scheme to output of HDF5 file as field '_scheme_' to allow distinguishing between DCCL and PROTOBUF messages.


****************
Version 3.0.5

### Middleware

- Added IO threads (TCP, Serial, PTY) for the Consistent Overhead Byte Stuffing (COBS) packet framing algorithm, which is a easy to implement scheme for delimiting binary packets on a stream protocol.
- Changed file_log to be a singular field, and added -z flags (which can be repeated, e.g. -zz or -zzz) for quickly changing the file log verbosity on the command line.

### ZeroMQ

- Add symlink for latest log in goby_logger.


****************


****************
Version 3.0.4

### Middleware

- Add publications from the IntervehiclePortal (gobyd) for various Goby-Acomms signals (driver / AMAC)

### ZeroMQ

- Fix Liaison Scope truncating messages in drop-down when the messages vary in size within the same Group

### Util

- Fix bug in AIS encoding for negative lat/lon (was undefined behavior, but exhibits the problem only on ARM processors)


****************
Version 3.0.3

### Middleware

- Support for update to geodesy conversion datum at runtime using publication to goby::datum_update
- Support shared library plugins for modem drivers in InterVehicleTransporter (and thus gobyd)
- Add parsing of RTE and WPL messages from OpenCPN for receiving data from the map (routes and waypoints).
- Added regex for optionally omitting certain groups and/or nodes from the goby_clang_tool visualization.

****************
Version 3.0.2

### Middleware

- Support for intermodule in goby_clang_tool
- Added extensions to frontseat and health messages

****************
Version 3.0.1

### Bug fixes

- Fix segfault in goby_liaison when thread_id was reused by the OS.



****************
Version 3.0.0

First stable release of Goby3 nested communications middleware.

****************
Version 3.0.0~beta*

### Middleware

Increasingly stable look at the new Goby3 nested communications middleware. See examples at https://github.com/GobySoft/goby3-examples.

****************
Version 3.0.0~alpha*

### Middleware

Early look at the new Goby3 nested communications middleware. See examples at https://github.com/GobySoft/goby3-examples.

Goby Release Notes (Major version 2)

****************
Version 2.1.13

### Bug fixes
- Fix to Dynamic Protobuf unit test for changes in DCCL 3.0.10

See https://github.com/GobySoft/goby/compare/2.1.12a...2.1.13

****************
Version 2.1.12a

### Bug fixes
- Fixed version.h header macros (regression introduced by clang-format)

See https://github.com/GobySoft/goby/compare/2.1.12...2.1.12a

****************
Version 2.1.12

### Code aesthetics
- Consistent code formatting using clang-format style

### Bug fixes
- Corrected moos_translator1 unit test for Ubuntu 18.04

See https://github.com/GobySoft/goby/compare/2.1.11...2.1.12

****************
Version 2.1.11

### MOOS
- Working iFrontSeat driver for OceanServer Iver3 vehicles (listen-mode only)

### Acomms
- Support for generic LBL message for MicroModem driver ($CCPGT)

### Bug fixes
- Resolved segfault with `goby_hdf5` when outputting string fields

See https://github.com/GobySoft/goby/compare/2.1.10...2.1.11

****************
Version 2.1.10

### MOOS
- Started iFrontSeat driver for OceanServer Iver3 vehicles (listen-mode only)

### Bug fixes
- Can build with C++11
- Fixed bug with GobyMOOSApp configuration reader when submessage fields were empty

See https://github.com/GobySoft/goby/compare/2.1.9...2.1.10

****************
Version 2.1.9

Bug fix release.

- Bug fix for logging (glog) in GobyMOOSApp
- Improved GPS surface handling in Bluefin driver for iFrontSeat
- Improved robustness of Iridium driver in the event that echo gets turned back on (ATE1 or modem reboot)

See https://github.com/GobySoft/goby/compare/2.1.8...2.1.9


****************
Version 2.1.8

### Acomms
- Significant reduction in data usage by IridiumDriver by using DCCL to compress the ModemTransmission wrapper message
- goby_ip_gateway minor improvements
  - Configurable queue size
  - Configurable tun number (e.g. tun10)
  - Ability to bypass MAC for modemdrivers that have built in MAC
  
### Common
- goby_hdf5: Default protobuf value written into HDF5 file

### MOOS
- iFrontSeat driver for Liquid Robotics SV2 waveglider

### Compiling
- Improvements for Mac OS X compilation


See https://github.com/GobySoft/goby/compare/2.1.7...2.1.8

****************
Version 2.1.7

- Minor compilation updates for non-Ubuntu systems.

### MOOS
- Bug fix for iFrontSeat to correctly handle large time skews

See https://github.com/GobySoft/goby/compare/2.1.6...2.1.7

****************
Version 2.1.6
### Acomms

 - New driver for the Teledyne Benthos ATM-900 series acoustic modems

### MOOS

 - Updates to GobyMOOSApp to support MOOSTimeWarp for all subclasses and to resubscribe automatically if the MOOSDB connection is lost.


See https://github.com/GobySoft/goby/compare/2.1.5...2.1.6
****************
Version 2.1.5
### MOOS
- Bug fix for "register_time" in GobyMOOSApp to allow use with MOOSTimeWarp #15 
- Bug fix for "inverse_publish" in moos_translators #14 
- Cleanups for Dependencies list.

See https://github.com/GobySoft/goby/compare/2.1.4...2.1.5
>>>>>>> 2.1

****************
Version 2.1.4

### MOOS

- Added function "register_timer" to GobyMOOSApp to allow for low frequency (<AppTick) register events. Ofter it is usual to do something every 10 sec, every 60 sec, etc. This functionality simplifies doing so.

See https://github.com/GobySoft/goby/compare/2.1.3...2.1.4

****************
Version 2.1.3

This release contains minor bug fixes.

See https://github.com/GobySoft/goby/compare/2.1.2a...2.1.3
****************
Version 2.1.2

### Acomms 

- Improved support for Flexible Data Packet (FDP) in Micro-Modem 2. Now supports both mini-packet and full packets (previously only mini-packets were supported), and an optional application acknowledgment is available that can be used to get acks from FDP messages.
- Added ability to update MicroModem NVRAM configuration while driver is running, rather than just at initialization.
- Added option for MACManager to use a fixed reference (in Unix time) rather than the start of the current day).
- Fix to set RTS line low to avoid garbled messages with the MicroModem2 in Linux.
- (experimental). New `goby_ip_gateway` application: Linux `tun` interface to ease sending of IP/UDP datagrams over Goby.

### PB 

- Added new tool (`goby_hdf5`) for converting Google Protocol Buffers (protobuf) logs into an HDF5 file (https://www.hdfgroup.org/HDF5/) suitable for easy data analysis by common scientific tools such as MATLAB, Octave, and Python. This tool is extensible to work on any format log that stores protobuf messages by a plugin interface, where the plugin is responsible for reading and parsing the protobuf log and providing Messages to `goby_hdf5` to process. An example plugin is given in goby/src/test/common/hdf5/test-plugin.cpp. See Github #8.

### Compiling 

- Various improvements to the build system. See Github #12.
  - All compiled objects (binaries, libraries, etc.) are now stored in subdirectories of the ${CMAKE_BINARY_DIR} (defaults to goby/build when using the build.sh script), and symlinks are created from goby/lib, goby/bin, goby/include, and goby/share to the actual built directories. This makes the out-of-source build more proper, and thus all autogenerated code can by removed by simply removing the contents of ${CMAKE_BINARY_DIR} (e.g. rm -r goby/build/*)
  - Added support for CMake's install(EXPORT) and export(...) functions that produces a file (goby-config.cmake) that makes it easier for a project dependent on Goby to find the Goby libraries and include the appropriate headers. 

### Bug fixes

Various bug fixes. See https://github.com/GobySoft/goby/compare/2.1.1...2.1.2

****************
Version 2.1.1

= Acomms = 
- DCCL is now its own project separate from Goby: http://libdccl.org. Removed DCCL code and added dependency on libdccl. Left backwards-compatible API in goby/acomms/dccl to allow seamless transition for previous Goby users. One exception: users of Google Protocol Buffers versions prior to 2.5.0 will need to change all DCCL .proto files to import the DCCL extensions .proto instead of the Goby one. That is:
  From:
        import "goby/acomms/protobuf/dccl_option_extensions.proto";
  To:
        import "dccl/protobuf/option_extensions.proto";
- Updated IridiumDriver to handle the Iridium Short Burst Data (SBD) protocol in addition to the previously available RUDICS protocol. Removed shore-based components of IridiumDriver (now it is only for the mobile side). Removed dependency on the goby_pb library and so IridiumDriver has moved into the goby_acomms library.
- Removed goby_rudics_shore and added new IridiumShoreDriver to handle all shore-based SBD ("DirectIP") and RUDICS protocol transactions.

= MOOS = 
-  Added failure handling in pAcommsHandler for multiple modems - can try to use one of the listen modems as the new primary modem, or restart the modem with a configurable backoff period. 

= PB =
- Protocol Change! Switched order of group and protobuf name in ZMQ identifier string. Thus, version 2.1 goby_pb is not backwards compatible with 2.0.

bzr log --line -n0 -r tag:2.0.6..

586: Toby Schneider 2015-09-21 Release 2.1.1
585: Toby Schneider 2015-09-21 [merge] Various minor fixes
  538.1.67: Toby Schneider 2015-09-20 [merge] Merge from pre-ST10
    538.3.2: Toby Schneider 2015-09-19 Fixed liaison acomms to load DCCL libraries to allow custom codecs
    538.3.1: Toby Schneider 2015-09-19 Process loopback before signal_in_route to accomodate RouteCommand message
  538.1.66: Toby Schneider 2015-09-04 Fixed regression on time update in Goby Bridge
  538.1.65: Toby Schneider 2015-09-04 Added in NetworkAck expire for time update and hardware control messages in goby_bridge
  538.1.64: Toby Schneider 2015-09-04 Updated pAcommsHandler MAC start/stop to apply to all drivers
584: Toby Schneider 2015-08-28 [merge] Merge: primarily driver fixes
  538.1.63: Toby Schneider 2015-08-28 Added manual driver reset command to pAcommsHandler
  538.1.62: Toby Schneider 2015-08-28 Changed unsigned to uint32 for next_frame to ensure overflow matches protobuf type
  538.1.61: Toby Schneider 2015-08-28 Added debugging info explaining clearing acks
  538.1.60: Toby Schneider 2015-08-28 Fixed frame count updates in PB Modem driver for multiple frame messages; Updated other drivers to use same logic in the event they become multi-frame in the future
  538.1.59: Toby Schneider 2015-08-24 Fixed infinite loop in shore Iridium driver if SBD packet could not be decodded correctly
  538.1.58: Toby Schneider 2015-08-24 Added check on correct packet source for Mosh relay
583: Toby Schneider 2015-08-20 [merge] Merge various bug fixes and feature that adds destination-based encryption passphrases for Queue
  538.1.57: Toby Schneider 2015-08-20 Added option to allow queue to set DCCL passphrase on a destination by destination basis
  538.1.56: Toby Schneider 2015-08-20 Fixed missing initialized value for time in Iridium driver
  538.1.55: Toby Schneider 2015-08-20 Added MOOSCommClient connect and disconnect callback handling to moos_gateway_g to allow it to be robust to disconnects from the MOOSDB
  538.1.54: Toby Schneider 2015-08-19 Added number of resets tracker to ModemDriverStatus
  538.1.53: Toby Schneider 2015-08-19 Fixed bug with previous commit
  538.1.52: Toby Schneider 2015-08-19 Added disconnect on multiple RUDICS packet failures
  538.1.51: Toby Schneider 2015-08-19 Fixed segmentation fault when replacing a Liaison message caused by new multiple _time field fix
582: Toby Schneider 2015-08-19 [merge] Merge changes from waveglider testing
  538.1.50: Toby Schneider 2015-08-19 Added default gateway for routing
  538.1.49: Toby Schneider 2015-08-18 Correctly set size of encrypted passthrough messages
  538.1.48: Toby Schneider 2015-08-18 Added handling of edge case bug where Iridium phone gives an 'OK' in response to a dial (shouldn't happen, but just did today)
  538.1.47: Toby Schneider 2015-08-17 Corrected revision parsing for Micro-Modem 1; Removed std::cout spam from serial2tcp_server
581: Toby Schneider 2015-08-13 [merge] Merge TimeUpdate code
  538.1.46: Toby Schneider 2015-08-13 Added rough time update code into goby_bridge
580: Toby Schneider 2015-08-10 [merge] Merge payload status updates in BF frontseat driver
  578.1.2: Toby Schneider 2015-07-31 Removed unnecessary copy
  578.1.1: Toby Schneider 2015-07-28 {2.1.1~rc4} Added ability to post payload status in Bluefin frontseat driver
579: Toby Schneider 2015-07-20 Changed pGobyMOOSAppTemplate to use non-static configuration object since Protobuf seems to have issues with default strings in statically configured Protobuf objects
578: Toby Schneider 2015-07-15 [merge] Merge fix to unit test
  538.1.45: Toby Schneider 2015-07-15 Updated unit test for change to ignore broadcast messages originated from the current modem id
577: Toby Schneider 2015-07-14 [merge] Merge multiple modem failure handling in pAcommsHandler
  538.1.44: Toby Schneider 2015-07-14 Added failure handling in pAcommsHandler for multiple modems - can try to use one of the listen modems as the new primary modem, or restart the modem with a configurable backoff period. To support this, added these features: unbind/unconnect for Goby slots, increasing inddex for DriverBase objects as they are created (to sort them by order of creation)
576: Toby Schneider 2015-07-13 [merge] Merge fix to MOOSCommander time field filler
  538.1.43: Toby Schneider 2015-07-13 Corrected MOOSCommander Liaison plugin to autofill timestamps on messages with multiple _time fields
575: Toby Schneider 2015-07-10 [merge] Merge fix for bug introduced in previous merge
  538.1.42: Toby Schneider 2015-07-10 Fixed bug where routing now fails if decoding the whole message fails (e.g. encrypted when we don't have the crypto key
574: Toby Schneider 2015-07-10 [merge] Merge changes to Goby-Queue to support configurable routing
  538.1.41: Toby Schneider 2015-07-10 1. Switched order of decode/signal_route_in in Queue to enable processing of data (specifically, RouteCommand) before the routing is done. 2. Added handling of RouteCommand to pAcommsHandler, 3. Changed QueueManager config route_broadcast_packets to route_for_additional_modem_id which is more generic (you can specify additional routing for any modem id(s), not just 0 (broadcast)
573: Toby Schneider 2015-07-09 [merge] Merge change for NMEASentence non-standard talker
  538.1.40: Toby Schneider 2015-07-09 Added option to use non-standard (5 character) talker length
572: Toby Schneider 2015-06-18 [merge] {2.1.1~rc3} Merge in improvements for serial2tcp_server and added PB methods for GobyMOOSApp
  538.1.39: Toby Schneider 2015-06-18 Added publish_pb and subscribe_pb methods to GobyMOOSApp that directly use the serialize_for_moos and parse_for_moos functions
  538.1.38: Toby Schneider 2015-06-17 Added configurable run frequency for serial2tcp_server
  538.1.37: Toby Schneider 2015-06-17 Improved efficiency of std::string overload of readline in linebasedcomms
  538.1.36: Toby Schneider 2015-06-16 Renamed serial2tcp_server to goby_serial2tcp_server for consistency
571: Toby Schneider 2015-06-16 [merge] Merge in hardware control support for Micro-Modem driver
  538.1.35: Toby Schneider 2015-06-15 Added support for CCMEC - hardware control of Micro-Modem line and HardwareControlCommand remote DCCL command
  538.1.34: Toby Schneider 2015-06-15 Brought pTranslator create_on_multiplex_publish in line with pAcommsHandler version
  538.1.33: Toby Schneider 2015-06-09 Added logic to deal with packet loss in Mosh relay
  538.1.32: Toby Schneider 2015-06-09 Added code to correctly set tx and rx times for iridium shore driver
570: Toby Schneider 2015-06-02 Added support for binary protobuf encoding into Liaison MOOS Scope
569: Toby Schneider 2015-06-02 Added two new MOOS <-> protobuf translators: TECHNIQUE_PROTOBUF_NATIVE_HEX and TECHNIQUE_PREFIXED_PROTOBUF_NATIVE_HEX which used ascii-encoded hexadecimal representation of the protobuf native encoding
568: Toby Schneider 2015-05-11 [merge] Merge move Iridium ModemDriver into acomms
  538.1.31: Toby Schneider 2015-05-11 Fixed line based connection handler for RUDICS in iridium shore driver
  538.1.30: Toby Schneider 2015-05-11 Moved Iridium driver into Acomms, since it no longer depends on ZMQ stuff in PB
567: Toby Schneider 2015-05-11 [merge] Merge rework of goby_rudics_shore into standalone ModemDriver
  538.1.29: Toby Schneider 2015-05-11 [merge] Merge restructure of shore-side rudics/sbd to fix several bugs and streamline code
    538.2.3: Toby Schneider 2015-05-11 Finished rework of goby_rudics_shore into new ModemDriver.
    538.2.2: Toby Schneider 2015-05-08 Reworked goby_rudics_shore into new ModemDriver: iridium_shore_driver and removed old goby_rudics_shore
    538.2.1: Toby Schneider 2015-05-08 Reworking RUDICS shore for multiple gliders
  538.1.28: Toby Schneider 2015-05-05 Switched modem_id in MTAsyncData to be consistent with other rudics shore messages
  538.1.27: Toby Schneider 2015-05-05 Register ZMQ connect on data receive
  538.1.26: Toby Schneider 2015-05-04 Set disposition flag on MT Iridium SBD to flush buffer with new messages
566: Toby Schneider 2015-04-28 {2.1.1~rc2} Added rx timestamp to linebasedcomms data
565: Toby Schneider 2015-04-27 Added pitch, roll, z-rate optionally to desired course
564: Toby Schneider 2015-04-22 [merge] Merge changes for real Iridium SBD
  538.1.25: Toby Schneider 2015-04-22 Minor modifications to make SBD work with actual Iridium hardware
563: Toby Schneider 2015-04-14 [merge] {2.1.1~rc1} Merge fix in moos_gateway_g
  538.1.24: Toby Schneider 2015-04-14 Fixed moos_gateway_g double-connect bug - must choose either MOOSNode or ProtobufNode connection on Goby side
  538.1.23: Toby Schneider 2015-04-10 [merge] Merge from 2.1
562: Toby Schneider 2015-04-10 [merge] Merge RUDICS handshake and route command messages
  538.1.22: Toby Schneider 2015-04-09 1. Implemented routing command DCCL message to change goby_bridge routes; 2. Added creation symlinks to log file for latest log; 3. Minor fixes to moos liaison commander
  538.1.21: Toby Schneider 2015-04-02 Added handshake to RUDICS disconnect
  538.1.20: Toby Schneider 2015-04-02 [merge] Merge fixes from 2.1 main
  538.1.19: Toby Schneider 2015-03-31 Added transition for NoCarrier during Answer
  538.1.18: Toby Schneider 2015-03-09 Added route command message
561: Toby Schneider 2015-04-10 Minor mods to logging test to track down multithreading logging issues
560: Toby Schneider 2015-04-02 Added output for DCCL location that is being used
559: Toby Schneider 2015-03-30 [merge] Merged fix from 2.0 for libzmq version 3+
  513.1.43: Toby Schneider 2015-03-30 Fixes for libzmq3: tested against libzmq1:2.2.0+dfsg-5 (backwards compatbility and libzmq3:4.0.4+dfsg-2
558: Toby Schneider 2015-03-18 [merge] Merged fix of typo in driver_exception header guard
  513.1.42: Toby Schneider 2015-03-18 1. Fixed bug where HAS_NCURSES definition is in header file; 2. Fixed typo in DriverException header guard
557: Toby Schneider 2015-03-18 Fixed conflict in extension numbers in iridium_driver.proto
556: Toby Schneider 2015-03-18 [merge] Merge bug fixes from 2.0
  513.1.41: Toby Schneider 2015-03-06 Fixes for Clang in OSX Yosemite
  513.1.40: Toby Schneider 2014-11-07 [merge] Merge from ST8c
    513.5.3: Toby Schneider 2014-10-04 Fixed bug in MMDriver where  would clear both a  and then a 
    513.5.2: Toby Schneider 2014-10-03 Added option to disable use of DTR
    513.5.1: Toby Schneider 2014-10-01 Workaround for partial frame bug in MM2
  513.1.39: Toby Schneider 2014-09-24 Added transition for Iridium driver to Answer state to correctly return to ready if the call disconnects in the process of being answered (RING -> ATA -> NO CARRIER)
555: Toby Schneider 2015-03-11 Fixed bug in glog where if you added a stream and then later replaced it with the same stream of a different verbosity, the highest_verbosity would remain at the previous level, causing unnecessary CPU overhead generated streams without any sink
554: Toby Schneider 2015-03-06 [merge] Merged SBD updates
  538.1.17: Toby Schneider 2015-03-07 Took out SBDConfigure to allow re-entry to SBDReady after RUDICS call; added minor debug output
  538.1.16: Toby Schneider 2015-03-06 Added reset after several retries of an AT command to Iridium driver
553: Toby Schneider 2015-03-05 [merge] Merged in SBD initial support
  538.1.15: Toby Schneider 2015-03-04 Finalized initial implementation of Iridium SBD
  538.1.14: Toby Schneider 2015-03-04 Removed debug std::cout
  538.1.13: Toby Schneider 2015-02-27 Added shore side (MO) support for Iridium short-burst data into goby_rudics_shore
  538.1.12: Toby Schneider 2015-02-19 Added in great deal of structure and code for SBD that largely works. Also added in async messages from goby_rudics_shore - this is currently non-functional
552: Toby Schneider 2015-02-17 [merge] Merge changes to NetworkAck
  538.1.11: Toby Schneider 2015-02-17 Added random port to route test as well
  538.1.10: Toby Schneider 2015-02-17 Initialized random seed for ports in tests to reduce conflicts on parallel builds
  538.1.9: Toby Schneider 2015-02-16 Changed UDPDriver test to use random ports to avoid conflicts during parallel builds
  538.1.8: Toby Schneider 2015-02-16 Moved NetworkAck from goby_bridge to goby::acomms::QueueManager so it can be used by any application (e.g. pAcommsHandler). Added EXPIRE messages to NetworkAck as well
  538.1.7: Toby Schneider 2015-02-16 [merge] Merge fixes to pTranslator
551: Toby Schneider 2015-02-16 [merge] Merge change to wire protocol of ZMQ for PB messages - makes it possible to subscribe to a group that contains multiple types of messages
  538.1.6: Toby Schneider 2015-02-05 More work on functionality to moos_gateway_g to decode PB messages and repost as MOOS
  538.1.5: Toby Schneider 2015-02-04 1. (PB Wire Protocol Change\!) Switched order of group and protobuf name in ZMQ identifier string. 2. Update goby_file_transfer to report number of fragments in Response message, 3. Updated DynamicProtobufNode and changed goby_bridge to use that instead of just StaticProtobufNode (to handle external queue push messages), 4. Add functionality to moos_gateway_g to decode PB messages and repost as MOOS TECHNIQUE_PREFIXED_PROTOBUF_TEXT_FORMAT messages
  538.1.4: Toby Schneider 2015-01-14 [merge] Merge from 2.1 main
550: Toby Schneider 2015-02-12 Fixed pTranslator to work with MOOS Time Warp; Fixed TECHNIQUE_FORMAT parser to properly handle nested embedded messages
549: Toby Schneider 2015-01-27 Reverted UseShortRepeated primitives as this does not work on Protobuf 2.3.0
548: Toby Schneider 2015-01-20 Used protobuf TextFormat ShortRepeatedPrimitives for MOOS parsing/serialization
547: Toby Schneider 2015-01-14 [merge] Merge changes from 2.1-dsop
  538.1.3: Toby Schneider 2015-01-12 Added guard to Iridium driver state machine so it won't try to dial when it's on a call already
  538.1.2: Toby Schneider 2014-11-17 [merge] Merge bug fixes from 2.1
  538.1.1: Toby Schneider 2014-11-05 {2.1.0+shark1} Release 2.1.0+shark1
546: Toby Schneider 2015-01-14 Fixed bug where GobyMOOSApp fails to read last line of .moos file if it doesn't contain a final newline character
545: Toby Schneider 2015-01-02 Added template GobyMOOSApp
544: Toby Schneider 2014-12-03 Added plugin infrastructure for pAcommsHandler drivers; also added ability to use custom id codec with pAcommsHandler
543: Toby Schneider 2014-12-02 Reworked GobyMOOSApp so that it can be used (optionally) with AppCastingMOOSApp
542: Toby Schneider 2014-11-17 Fixed bug introduced by use of new frame_start in ModemTransmission: ACKs did not have correct frame number for PBDriver and IridiumDriver
541: Toby Schneider 2014-11-13 Added missing header
540: Toby Schneider 2014-11-06 Fixed very bad bug: Removed HAS_NCURSES macro from header file since this needed to be defined by library user code to work correctly; also removed option to compile without pthread for the Goby logger
539: Toby Schneider 2014-11-06 Fixed typo in docs
538: Toby Schneider 2014-10-22 Minor changes to examples to compile properly
537: Toby Schneider 2014-10-20 Fixed thread-safe logger for custom operator<<() overloads when not using explicit FlexOstream::is() method
536: Toby Schneider 2014-10-17 [merge] 1. Merge changes to Liaison Acomms tab; 2. Updated for namespace and other structural changes in DCCL; 3. Fixed frame ack bug in UDP, PB and Iridium driver (where frames could be inadvertently ack'd
  535.1.4: Toby Schneider 2014-09-30 Added percent bad frames into Liaison Acomms tab stats
  535.1.3: Toby Schneider 2014-09-30 Finished initial graph of stats
  535.1.2: Toby Schneider 2014-09-30 More work on liaison acomms tab - updated MAC and started driver work
  535.1.1: Toby Schneider 2014-09-29 Finished AMAC part of MOOSAcomms tab in Goby Liaison - also minor changes to MACManager to support this
535: Toby Schneider 2014-08-27 [merge] Merge from 2.0
  513.1.38: Toby Schneider 2014-08-25 Added subscription for pAcommsHandler on ACOMMS_MAC_INITIATE_TRANSMISSION for folks to handle MAC externally
  513.1.37: Toby Schneider 2014-08-01 {2.0.6} Increment version

****************
Version 2.0.6

= Acomms = 
- MOSH Client allowing support for the MOSH shell (mosh.mit.edu) over Goby. Requires use of a patched version of MOSH available at https://github.com/tsaubergine/mosh/tree/mosh-goby
- Fixes to goby_file_transfer
- DCCL ModemDriverStatus message reports status of a given driver - used by goby_modemdriver.
- Support for the WHOI Micro-Modem Flexible Data Protocol (FDP)

bzr log --line -n0 -r tag:2.0.5..

546: Toby Schneider 2014-07-31 Support for the Micro-Modem 2 FDP
545: Toby Schneider 2014-06-15 Added ability to reuse database for goby_store_server
544: Toby Schneider 2014-06-10 Replaced couts with glog in goby_mosh_relay
543: Toby Schneider 2014-05-12 {2.0.6~rc2} Increment version
542: Toby Schneider 2014-05-08 [merge] Merge changes including bug fixes and ModemDriverStatus message
  541.1.6: Toby Schneider 2014-05-08 Fixed bug where DriverBase keep adding repeated writes to the raw log file if it was restarted
  541.1.5: Toby Schneider 2014-05-08 Added keep-alive  (NVRAM status query) to detect if the modem has disappeared (e.g. power failure, etc.)
  541.1.4: Toby Schneider 2014-05-08 Added functionality to goby_bridge to queue ModemDriverStatus
  541.1.3: Toby Schneider 2014-05-08 Improved error reporting messages from TCPClient
  541.1.2: Toby Schneider 2014-05-08 Fixed bug in PULL file transfers that terminated transfer before completed if the TransferReponse message was received before the last fragment
  541.1.1: Toby Schneider 2014-05-08 Added functionality to base_convert to preserve zeros at the most significant end, e.g. 002 in base10 becomes 0010 in base2, which then stays 002 when back to base10; we need this because encoded values can end in necessary zeros
541: Toby Schneider 2014-05-06 Added ModemDriverStatus message and related code to fill it out in goby_modemdriver
540: Toby Schneider 2014-05-06 Fixed bugs allowing DriverBase to properly reset itself and changed ModemDriver application to handle exceptions from the DriverBase and derived class, then reset the driver after a delay
539: Toby Schneider 2014-04-08 {2.0.6~rc1} Initially working MOSH relay over Goby UDP driver (at 320 bytes/sec)
538: Toby Schneider 2014-04-08 1. Reworked application_base run() method to take the configuration object as an argument (instead of requiring that the Application class have a static configuration object (this was causing problems properly constructing the protobuf configuration for some compilers). Changed all derived applications to use this new structure. 2. Started framework for mosh relay client
537: Toby Schneider 2014-04-01 {2.0.5} Release 2.0.5

*****************
Version 2.0.5

= Acomms =
- New IridiumDriver that handles the Iridium RUDICS protocol (phone-call style of data transfer)

= MOOS =
- New MOOS application iFrontSeat (and corresponding additions to libgoby_moos) that provide an extensible interface to the low-level control ("frontseat") of an autonomous vehicle
      - Driver for iFrontSeat for the Bluefin 21" and 9" family of vehicles (libgoby_frontseat_bluefin)
      - New drivers can be written outside of Goby and are runtime loaded by iFrontSeat
- libgoby_moos and Goby MOOS applications (pAcommsHandler, iFrontSeat, etc) compile and runs on MOOS 10 (and still supports pre-10 (SVN revision 2376) MOOS)

= PB = 
- New application goby_modemdriver that allows provides a ZeroMQ interface to the following Goby ModemDrivers:
      - goby::acomms::MMDriver (WHOI Micro-Modem)
      - goby::pb::PBDriver (store-and-forward driver using ZeroMQ and goby_store_server [SQLite])
      - goby::acomms::UDPDriver (unicast UDP driver)
      - goby::acomms::IridiumDriver (Iridium)
- goby_bridge updated to use goby_modemdriver instead of having the ModemDriver components directly in the application
- goby_file_transfer that works with goby_bridge to allow arbitrary file transfer over a Goby link.

bzr log --line -n0 -r tag:2.0.4..

534: Toby Schneider 2014-03-25 Remove Xerces from default dependencies install
533: Toby Schneider 2014-03-25 Moved more dependencies into Debian install now that they are available in Wheezy (stable)
532: Toby Schneider 2014-03-25 Switched to default to MOOS10 for MOOS dependency when running DEPENDENCIES script
531: Toby Schneider 2014-03-25 Fixed proper component disabling when missing GMP library
530: Toby Schneider 2014-03-25 Changed header on all files to reflect change in license to GPL/LGPL2
529: Toby Schneider 2014-03-25 Changed license to GPL2 as per MIT open source request in COPYING
528: Toby Schneider 2014-03-19 Removed unused typedef in goby_bridge
527: Toby Schneider 2014-03-19 Added include for abc_frontseat_driver example
526: Toby Schneider 2014-03-19 Fixed double reconnect attempt in Bluefin driver for iFrontSeat
525: Toby Schneider 2014-03-17 Patch removing boost foreach which doesn't play nice with new g++ and protobuf 2.3.0
524: Toby Schneider 2014-03-17 [merge] Merge from DASH ST8a
  520.1.6: Toby Schneider 2014-03-03 1. workaround third-party ACK bug in Micro-Modem firmware; 2. Added name to publish of iFrontSeat status
  520.1.5: Toby Schneider 2014-02-23 Added short wait between Iridium redial attempts
  520.1.4: Toby Schneider 2014-02-23 Added HangingUp and PostDisconnected states to Iridium state machine to clean up series of actions in both of these end-of-call situations
  520.1.3: Toby Schneider 2014-02-22 Added split on commas for MOOS strings in MOOSLiaisonScope
  520.1.2: mituser 2014-02-22 [merge] Merge changes to Iridium Driver for CEER debug message and configurable start up time
    520.2.2: Toby Schneider 2014-02-07 Need to kick Offline before handling Disconnect (because of +CEER)
    520.2.1: Toby Schneider 2014-02-07 Added +CEER extended error message check for Iridium driver
  520.1.1: Toby Schneider 2014-02-22 {2.1.0~beta8} In iFrontSeat Bluefin driver: Changed BPRCB to BPRCE on advice of Patrick Larocque; changed command of 0 speed as 0 rpm to also send 0 pitch and 0 rudder
523: Toby Schneider 2014-03-11 Added check for newer versions of protobuf to add intentional enum aliases to avoid error in protobuf 2.5.0 and newer
522: Toby Schneider 2014-03-11 Removed newest() call from Application
521: Toby Schneider 2014-03-11 Removed newest() call from Protobuf pub sub wrapper since the underlying node doesn't have the call anymore
520: Toby Schneider 2014-02-03 Reduced verbosity of store server send for iridium driver
519: Toby Schneider 2014-02-03 {2.1.0~beta7} Increment version
518: Toby Schneider 2014-02-03 Demoted certain reliability ZMQ messages to a lower debug level to reduce debug file spam
517: Toby Schneider 2014-01-10 [merge] Merge BFSMC support from 87mcmahon
  510.1.1: Jim McMahon 2013-12-13 Added start mission confirm bluefin command
516: Toby Schneider 2013-12-18 Fixed conversion error in NAV_YAW in the iFrontSeat legacy translators.
515: Toby Schneider 2013-12-11 {2.1.0~beta6} Increment version
514: Toby Schneider 2013-11-21 [merge] Merge changes made on DASH-ST6 deep cruise
  513.1.5: Toby Schneider 2013-11-15 Small tweak to Liaison geodesy tab
  513.1.4: Toby Schneider 2013-11-15 More features for new MOOS Geodesy liaison tab
  513.1.3: Toby Schneider 2013-11-14 Updates on frontseat driver documentation
  513.1.2: Toby Schneider 2013-11-13 Minor changes to user manual
  513.1.1: Toby Schneider 2013-11-13 1. Changed tes::DynamicMOOSVars to goby::moos::DynamicMOOSVars; 2. More documentation on iFrontSeat
513: Toby Schneider 2013-11-12 [merge] Merge from DASH-ST6 leg 1 - added example frontseat simulator, and updated related documentation
  512.1.1: Toby Schneider 2013-11-09 Added ABC example frontseat simulator and corresponding iFrontSeat driver as an example on how to write drivers
512: Toby Schneider 2013-10-31 {2.1.0~beta5} increment version
511: Toby Schneider 2013-10-31 [merge] Ported BFCTL handoff of control from lamss DSOP branch
  509.2.1: Toby Schneider 2013-10-31 Ported BFCTL handoff of control from DSOP branch
510: Toby Schneider 2013-10-30 [merge] Merge from shore node
  509.1.1: Toby Schneider 2013-10-30 Various fixes to python PPP/Goby RUDCS forward script
509: Toby Schneider 2013-10-30 Reduced verbosity of ZMQ messages to reduce debug file sizes
508: Toby Schneider 2013-10-29 Added some nice transfer success/failed messages to goby_file_transfer
507: Toby Schneider 2013-10-29 Finalized goby_file_transfer - added Response for each transfer indicating success or failure (with error messages in case of failure)
506: Toby Schneider 2013-10-23 {2.1.0~beta4} Increment version
505: Toby Schneider 2013-10-23 Fixed detection of start of Rudics call
504: Toby Schneider 2013-10-23 Added missing include
503: Toby Schneider 2013-10-23 Changed default PPP server to localhost
502: Toby Schneider 2013-10-23 Minor change to debugging log output
501: Toby Schneider 2013-10-23 Added python script for multiplexing PPP and Goby on Rudics shore side
500: Toby Schneider 2013-10-23 Added unique identifier to start of Goby RUDICS call to distinguish it from other protocols (e.g. PPP)
499: Toby Schneider 2013-10-23 Moved decoding after routing so if decoding fails (e.g. on encrypted message), the routing still takes place
498: Toby Schneider 2013-10-19 Changed enable_testing_asio to only disable running of test if set to OFF (still builds test); added enable_testing_zeromq to work around running test bug in Ubuntu Saucy build machines
497: Toby Schneider 2013-10-18 [merge] Merge changes from DASH-ST6 shallow test
  496.1.5: Toby Schneider 2013-10-17 Added unit test for salinity (from lamss repos)
  496.1.4: Toby Schneider 2013-10-17 Fixed bug causing segmentation fault when the Micro-Modem has a frame count mismatch
  496.1.3: Toby Schneider 2013-10-16 Disallowing sending of uninitialized protobuf message prevents protobuf fatal error from killing application
  496.1.2: Toby Schneider 2013-10-16 Modified headers on seawater calculations
  496.1.1: Toby Schneider 2013-10-16 Ported iFrontSeat from lamss project to reside here
496: Toby Schneider 2013-09-27 {2.1.0~beta3} Incremented version
495: Toby Schneider 2013-09-27 Added option to write raw modem log to separate file
494: Toby Schneider 2013-09-27 Resized MACUpdate message to account for new always_initiate field in ModemTransmission message
493: Toby Schneider 2013-09-27 1. Added option to MAC slot to always initiate allowing hybrid centralized/decentralized MAC setups; 2. Added option to set MMDriver silent by changing source ID (so no ACKs are generated for messages sent to it); 3. Added ability to attach one or more drivers to pAcommsHandler that are listeners (receive only)
492: Toby Schneider 2013-09-26 Increased MAC update to 64 byte message, and increased bound on MACUpdate message
491: Toby Schneider 2013-09-25 {2.1.0~beta2} Incremented version
490: Toby Schneider 2013-09-24 1. Fixed bug where Iridium DATA messages sent by the side that didn't originate the first call were not getting ACK'd; 2. In goby_bridge: made sending of CACSTs and RangingReply messages optional, and made NetworkAck generation only enabled for given source Modem Ids;
489: Toby Schneider 2013-09-24 Fixed bug in Queue when using pass through messages (to allow encrypted messages to be routed without unencryptied) in conjunction with non-encrypted message(s) that caused the non-encrpyted message(s) to be lost.
488: Toby Schneider 2013-09-23 Removed unneeded load_dccl_message from goby_bridge; fixed bug where two messages with the same ID could be queued without the second being validated (and thus catching that there were two messages with the same ID)
487: Toby Schneider 2013-09-19 Fixed minor compile failure for lucid build
486: Toby Schneider 2013-09-19 Added in goby_file_transfer for raw point-to-point transfer of files over Goby. Currently only supported using goby_bridge.
485: Toby Schneider 2013-09-16 [merge] {2.1.0~beta1} Merge from 2.0-moos10; Goby2 should now properly build against the original MOOS and MOOS10
  283.1.17: Toby Schneider 2013-09-16 Modifications so that project still builds against old MOOS (pre v10)
  283.1.16: Toby Schneider 2013-09-16 Removed MOOSGeodesy in favor of new shell with same API that makes calls to libproj (PROJ.4)
  283.1.15: Toby Schneider 2013-09-16 [merge] Merge from 2.0
  283.1.14: Toby Schneider 2013-09-12 Fix header for MOOS 10
  283.1.13: Toby Schneider 2013-09-12 [merge] Merged 2.0.3 from mainline
  283.1.12: Toby Schneider 2013-03-27 [merge] Merge from 2.0
  283.1.11: Toby Schneider 2013-02-27 [merge] merge from 2.0
  283.1.10: Toby Schneider 2013-02-27 [merge] merge from 2.0
  283.1.9: Toby Schneider 2013-02-27 [merge] Merge from 2.0
  283.1.8: Toby Schneider 2013-02-27 [merge] merge from 2.0
  283.1.7: Toby Schneider 2013-02-27 [merge] Merge bug fix from 2.0
  283.1.6: Toby Schneider 2013-02-27 Compiles against the new MOOS
  283.1.5: Toby Schneider 2013-02-27 [merge] Merged from 2.0
  283.1.4: Toby Schneider 2012-03-18 [merge] merge from 2.0
  283.1.3: Toby Schneider 2012-03-15 [merge] merge from 2.0
484: Toby Schneider 2013-09-06 Moved glog output for Iridium driver when talking to goby_rudics_shore to ZMQ group to unclutter modemdriver group for AT commands
483: Toby Schneider 2013-09-06 Changes to IridiumDriver: Reset hangup timeout at the start of a call; do not make a call if not data to send
482: Toby Schneider 2013-09-06 Changed native_handle() to native() to allow IridiumDriver to compile on older versions of boost::asio
481: Toby Schneider 2013-09-06 Added DTR toggle for serial connection hangups; Changed boost::remove_if to std::remove_if
480: Toby Schneider 2013-09-05 Shore side mobile-originated calls working with ATDuck simulator
479: Toby Schneider 2013-09-04 Moved Iridium driver into PB library to allow us to use ZMQ and PB node functionality
478: Toby Schneider 2013-09-04 Working unit test for mobile-terminated rudics call
477: Toby Schneider 2013-09-04 Started Iridium RUDICS shore side server; changed to omit more problem characters from RUDICS protocol
476: Toby Schneider 2013-09-03 Progress on Iridium driver: changes to DriverBase - if modem is inactive, do not block waiting for it to become active, TCP reconnect configurable, startup() and shutdown() should block in driver until complete (or fail throwing exception); changes to IridiumDriver - increased timeout on retry for long events such as dial, many minor changes to state machine
475: Toby Schneider 2013-08-26 [merge] Switched to boost::statechart for finite state machine representation of Hayes DTE (for Iridium Driver)
  474.1.3: Toby Schneider 2013-08-26 Added code to prevent integer overflows on huge time differences (related to using MOOS timewarp)
  474.1.2: Toby Schneider 2013-08-26 Passes IridiumDriver1 test with ATduck emulator (with new FSM code)
  474.1.1: Toby Schneider 2013-08-26 In progress work to FSM model
474: Toby Schneider 2013-08-23 Initially basic working connection and data passing with Iridium Driver using ATduck emulator
473: Toby Schneider 2013-08-23 Created shared driver_tester code for various modem driver tests to share
472: Toby Schneider 2013-08-13 Added subdirectory for iridium driver test
471: Toby Schneider 2013-08-13 Added arbitrary base conversion for base256 <--> base255 conversions
470: Toby Schneider 2013-08-12 Fixed bug when group is the null string
469: Toby Schneider 2013-08-12 [merge] Merge work on standalone goby_modemdriver binary
  468.1.3: Toby Schneider 2013-08-12 Tentatively fully working standalone goby_modemdriver that exposes the C++ API via ZMQ publish/subscribe messages. Update goby_bridge to use new goby_modemdriver and removed ModemDriver module from goby_bridge.
  468.1.2: Toby Schneider 2013-08-12 Working string group into Application structure
  468.1.1: Toby Schneider 2013-08-12 Working string group into Application structure
468: Toby Schneider 2013-08-09 Folder for standalone modem driver
467: Toby Schneider 2013-08-09 Framework for Iridium driver
466: Toby Schneider 2013-08-09 {2.0.4} Updated moos_goby_liaison script to use exec to avoid problems when killing the shell script doesn't kill the goby_liaison process

*****************
Version 2.0.4

- Licensing terms slightly changed to make them completely compatible with third-party libraries: Liaison is GPL2 only (due to Wt) and goby_moos is GPL3 (because of MOOS). All other licenses remain the same (GPL3+ for binaries, LGPL3+ for libraries)
- Fixes to make Liaison compatible with Wt 3.3.0 (which works properly with Firefox 20+; older Wt versions do not)

bzr log --line -n0 -r tag:2.0.3..

466: Toby Schneider 2013-08-09 {2.0.4} Updated moos_goby_liaison script to use exec to avoid problems when killing the shell script doesn't kill the goby_liaison process
465: Toby Schneider 2013-07-09 Changed licensing somewhat: Liaison and related libraries is GPL2 now (because of Wt) and goby_moos is GPL3 (because of MOOS). All other licenses remain the same (GPL3+ for binaries, LGPL3+ for libraries)
464: Toby Schneider 2013-07-08 Fixed location of queue warning to place in correct glog group
463: Toby Schneider 2013-06-28 Removed tooltip from commander for the time being as it seems to freeze up on Firefox
462: Toby Schneider 2013-06-28 Added missing header required for older Wt (3.1.8)
461: Toby Schneider 2013-06-18 Updated liaison for libwt 3.3.0
460: Toby Schneider 2013-06-18 Fixed bug in dccl test 1 that was causing garbage rejection test to not run
459: Toby Schneider 2013-06-13 Updated arithmetic coder to properly pick closest symbol (not floor)
458: Toby Schneider 2013-05-10 {2.0.3} Updated release notes

*****************
Version 2.0.3

- Addition to DCCL and goby_bridge to allow forwarding of encrypted messages without unencrypting.
- Added Bluefin Comms Driver (for using the SonarDyne Avtrak acoustic modem via the Bluefin NMEA interface: See Bluefin Payload Interface Guid v1.8 dated 04-04-13 and newer.
- For the MOOS translators: TECHNIQUE_FORMAT returns the default field instead of the empty string for unset fields.

bzr log --line -n0 -r tag:2.0.2..

457: Toby Schneider 2013-05-10 {2.0.3} Incremented version number and release date
456: Toby Schneider 2013-04-30 TECHNIQUE_FORMAT returns the default field value instead of the empty string on fields that are not set
455: Toby Schneider 2013-04-22 Finished initial implementation of Bluefin modem driver for Sonardynae and tested on bench with two modems
454: Toby Schneider 2013-04-12 Start of Bluefin Comms driver for MOOS
453: Toby Schneider 2013-04-10 Cleaned up another case where linebasedcomms connection could be 'active' even if the connection failed
452: Toby Schneider 2013-03-27 Fixed bug in mimic of Micro-Modem ping where ping reports from all vehicles in range were reported
451: Toby Schneider 2013-03-27 Added mimic of Micro-Modem ping command ($CCMPC) into MOOS UFld Driver
450: Toby Schneider 2013-03-18 Fixed bug where segmentation fault would occur on invalid or encrpyted messages (if the wrong crypto key is loaded) messages. Now the behavior is either a DCCLException or a nonsense decoded message
449: Toby Schneider 2013-03-18 {2.0.3~beta1} Release 2.0.3~beta1
448: Toby Schneider 2013-03-14 1. Possible to decode DCCL header without decrypting body; 2. Possible to pass messages through route without decrypting
447: Toby Schneider 2013-02-28 {2.0.2} Incremented version to 2.0.2


*****************
Version 2.0.2

- First stable release of Goby version 2!
- A handful of minor breaking changes from the 2.0.1 release candidates to support a standalone DCCLv3+
  - Reworked Queue messages to not use the .proto files. So no more (goby.field).queue and (goby.msg).queue. Use the goby::acomms::protobuf::QueuedMessageEntry protobuf class instead to configure queues. This makes more sense since different situations may cause differing queueing parameters.
  - Changed (goby.field).dccl to (dccl.field) and (goby.msg).dccl to (dccl.msg)
- Goby Liaison loads all modules using shared libraries instead of having the MOOS components compiled in. This makes it truly generic and extensible.

bzr log --line -n0 -r tag:2.0.1~rc4..

445: Toby Schneider 2013-02-28 Tweaks and corrections to developer documentation
444: Toby Schneider 2013-02-28 Added moos_goby_liaison script to launch goby_liaison with the MOOS plugins (commander and scope)
443: Toby Schneider 2013-02-28 Try/catch around translator entries
442: Toby Schneider 2013-02-27 Need boost::signals linkage if using Wt
441: Toby Schneider 2013-02-27 missing link against Boost for the MOOS goby_liaison library; DCCL bitset operator< more fixes
440: Toby Schneider 2013-02-27 Fixed operator< that didn't return false on equality
439: Toby Schneider 2013-02-27 Removed references to filesystem3 in favor of correct new functions so that Goby compiles with newer versions of boost
438: Toby Schneider 2013-02-27 Found and fixed non-thread safe use of glog in linebasedcomms
437: Toby Schneider 2013-02-27 Added configuration time check that queue fields used in roles exist in the current message
436: Toby Schneider 2013-02-27 Removed unused hook framework
435: Toby Schneider 2013-02-22 Moved nmea_time2ptime into the common header
434: Toby Schneider 2013-02-22 Added CMAKE_DL_LIBS to cause linking against libdl if necessary
433: Toby Schneider 2013-02-12 Modified FromProtoCustomMessage to copy the custom class on encode since dynamic_cast do not work on dynamically generated google::protobuf::Messages. Augmented test_dccl9 to include testing for this problem
432: Toby Schneider 2013-02-11 Work on CCL example, but pAcommsHandler seems to have difficulty encoding MDAT State
431: Toby Schneider 2013-02-11 Added dual status MOOS example configuration and continued work on User Manual
430: Toby Schneider 2013-02-08 Updated images
429: Toby Schneider 2013-02-08 [merge] More doc changes
  427.1.1: Toby Schneider 2013-02-08 Added images for the user manual
428: Toby Schneider 2013-02-08 Added images
427: Toby Schneider 2013-02-07 Lots of documentation updates for the user manual
426: Toby Schneider 2013-02-07 Autogenerate user doc configuration snippets; improved version number information
425: Toby Schneider 2013-01-30 Reverted new load_plugin_libraries configuration option; instead added GOBY_LIAISON_PLUGINS env variable so that plugins are loaded *before* configuration structure is even determined, allowing for configuration extensions in the plugins
424: Toby Schneider 2013-01-29 Bumped soversion
423: Toby Schneider 2013-01-28 Removed unneeded dependency on goby_moos in Liaison
422: Toby Schneider 2013-01-28 Added new files for dlfcn plugins for Liaison
421: Toby Schneider 2013-01-28 Mostly split out Liaison into modules; a bit more work to do to make it completely dl'loading
420: Toby Schneider 2013-01-26 Removed unneeded dependencies on 0MQ
419: Toby Schneider 2013-01-25 Incremented copyright year to 2013 in source code files
418: Toby Schneider 2013-01-11 Added cmake option 'enable_testing_asio' to allow disable building of Boost ASIO related tests on Ubuntu quantal and raring PPAs that have an old kernel + new glibc - see Boost ASIO bug #7012
417: Toby Schneider 2013-01-11 changed cmake variable 'workaround_2_4_kernel' to 'disable_epoll' to make it more clear what it does
416: Toby Schneider 2012-12-12 Added 'add' and 'subtract' MOOS algorithms
415: Toby Schneider 2012-12-05 Fixed bug where queue would cause a segmentation fault if the messagejust pushed was popped (due to full queue with newest_first = false
414: Toby Schneider 2012-11-20 Fixed translator test for changes in MOOS
413: Toby Schneider 2012-11-20 Changed logging verbosity of entire configuration to DEBUG2 so it doesn't spam the output in less verbose settings
412: Toby Schneider 2012-11-20 [merge] Check for duplicate translator entries
  411.1.1: Toby Schneider 2012-11-20 typo in QueueManager and fixed initialization in pAcommsHandler so messages are all loaded *before* Queues are configured
411: Toby Schneider 2012-11-08 [merge] Merge from 2.0-dsop
  321.2.20: Toby Schneider 2012-11-08 Strip white spaces from end of init strings by default in GobyMOOSApp
410: Toby Schneider 2012-11-02 Always store bit map for Arithmetic coder
409: Toby Schneider 2012-10-28 updated examples to reflect Queue changes
408: Toby Schneider 2012-10-28 Removed extra (unconfigured) add_queue call from pAcommsHandler that was erasing the Queue configuration
407: Toby Schneider 2012-10-28 Reworked Queue to not use DCCL hooks
406: Toby Schneider 2012-10-28 moved all meta work in Queue to single function
405: Toby Schneider 2012-10-22 Finished splitting arithmetic coding into a separate library (that can be dynamically loaded at runtime) and fixed compilation of goby_dccllite
404: Toby Schneider 2012-10-19 Fixed liaison commander to use dccl codec '_time' to detect timestamp field instead of now defunct queue.is_time
403: Toby Schneider 2012-10-19 Removed Queue is_dest is_src and is_time from Proto files in favor of configured 'Roles'
402: Toby Schneider 2012-10-19 [merge] Fixed header in User Manual for Quantal changes; removed ccl header from dccl.cpp
  401.1.1: Toby Schneider 2012-10-19 split 2.0 for merge
401: Toby Schneider 2012-10-19 Fixed examples for changes to DCCL option extensions
400: Toby Schneider 2012-10-18 Added missing new files
399: Toby Schneider 2012-10-18 Split CCL compatibility into its own shared library that can be loaded at runtime to enable CCL compatible headers
398: Toby Schneider 2012-10-18 Removed Queue options from .proto files in favor of runtime configuration
397: Toby Schneider 2012-10-18 split out dccl option extensions from goby for future use in separate 3.0 projects
396: Toby Schneider 2012-10-18 [merge] Merge various fixes from 2.0-dsop
  321.2.19: Toby Schneider 2012-10-07 {2.0.1~rc1+DSOP20121007+1} Version 2.0.1~rc1+DSOP20121007+1
  321.2.18: Toby Schneider 2012-10-05 Fixed crash bug in Liaison due to non-thread safe use of Google Protocol Buffer error reporting facility
  321.2.17: Toby Schneider 2012-10-05 Added bounds check for DCCLDefaultNumericCodec
  321.2.16: Toby Schneider 2012-10-05 Fixed bug where modem ID to name conversions were not loaded if debug logging was turned off
395: Toby Schneider 2012-10-03 [merge] Merge from 2.0-dsop
  321.2.15: Toby Schneider 2012-10-03 Replace / with _ in logging file for goby moos apps
394: Toby Schneider 2012-09-25 Fixed some out of date links
393: Toby Schneider 2012-09-25 Made various updates to links on front page of 2.0 doc
392: Toby Schneider 2012-09-24 Added mkdir for bin during cmake configure
391: Toby Schneider 2012-09-24 Moved boost requirements so that building dccllite doesn't require finding unneeded boost *.so library components
390: Toby Schneider 2012-09-24 [merge] Another merge from 2.0-dsop fixing signals2 stuff
  321.2.14: Toby Schneider 2012-09-24 missed a few more signals references
  321.2.13: Toby Schneider 2012-09-24 Missed some of the signal -> signals2::signal changes
389: Toby Schneider 2012-09-24 [merge] merge from 2.0-dsop
  321.2.12: Toby Schneider 2012-09-24 removed unneeded boost signals library in favor of header-only signals2
388: Chris Murphy 2012-09-19 Adding stdexcept as a dependency, since std::runtime_error is used.
387: Toby Schneider 2012-08-24 [merge] {2.0.1~rc4} Merge correcting fix for build system bug fix from 2.0-dsop, release 2.0.1~rc4
  321.2.11: Toby Schneider 2012-08-24 Actually fixed dynamic protobuf test


*****************
Version 2.0.1~rc3
Version 2.0.1~rc4

- Build system fixes

bzr log --line -n0 -r tag:2.0.1~rc2..

387: Toby Schneider 2012-08-24 [merge] {2.0.1~rc4} Merge correcting fix for build system bug fix from 2.0-dsop, release 2.0.1~rc4
  321.2.11: Toby Schneider 2012-08-24 Actually fixed dynamic protobuf test
386: Toby Schneider 2012-08-24 [merge] {2.0.1~rc3} Merge build system bug fix from 2.0-dsop, release 2.0.1~rc3
  321.2.10: Toby Schneider 2012-08-24 1. Fixed race condition in building dynamic protobuf test; 2. local CCCYC has only the requested number of frames
  321.2.9: Toby Schneider 2012-08-23 {2.0.1~rc1+DSOP20120823+2} Release for mbat: 2.0.1~rc1+DSOP20120823+2
  321.2.8: Toby Schneider 2012-08-23 [merge] Merge from 2.0 until before arithmetic encoding changes
  321.2.7: Toby Schneider 2012-08-23 {2.0.1~rc1+DSOP20120823+1} Release for mbat: 2.0.1~rc1+DSOP20120823+1
  321.2.6: Toby Schneider 2012-08-08 Backported new arithmetic extensions from 2.0 main
385: Toby Schneider 2012-08-23 Added adaptive arithmetic coding
384: Toby Schneider 2012-08-23 {2.0.1~rc2} Release 2.0.1~rc2

*****************
Version 2.0.1~rc2

- Release candidate 2: Added a arithmetic encoder to finish thesis work. 
- Various bug fixes (see below)

bzr log --line -n0 -r tag:2.0.1~rc1..

384: Toby Schneider 2012-08-23 {2.0.1~rc2} Release 2.0.1~rc2
383: Toby Schneider 2012-08-07 made goby::moos::val_from_string case insensitive
382: Toby Schneider 2012-08-06 Fixed edge case in arithmetic encoding (used 1111 to represent 1111.... instead of 10000
381: Toby Schneider 2012-08-04 Sets CCCYC in MMDriver to correct number of frames, if known (local cycle)
380: Toby Schneider 2012-08-03 Fixed bug with enumerations in TECHNIQUE_COMMA_SEPARATED_KEY_EQUALS_VALUE_PAIRS introduced after automatic lower casing of input; enabled dccl10 run by default
379: Toby Schneider 2012-08-02 1. Dynamic Protobuf manager includes pwd by default, 2. PB Modem driver gives warning on invalid transmission types
378: Toby Schneider 2012-08-02 Cleanup on arithmetic coder - reduced computational complexity and thus time on decode
377: Toby Schneider 2012-08-01 Arithmetic encoder implemented and tested using test_dccl10 - hopefully good to go
376: Toby Schneider 2012-08-01 Basically working arithmetic coder (still some details to iron out)
375: Toby Schneider 2012-07-30 more work on (broken) arithmetic encoder/decoder
374: Toby Schneider 2012-07-27 (currently broken) arithmetic encoder done
373: Toby Schneider 2012-07-27 Added missing file for arithmetic coder
372: Toby Schneider 2012-07-27 minor change to nightly script
371: Toby Schneider 2012-07-27 added new unit test for arithmetic encoding
370: Toby Schneider 2012-07-26 Added shell for arithmetic codec
369: Toby Schneider 2012-07-26 Reverted mmdriver1 test to before Tiger12 cruise where I messed it up testing the modems. Will use mmdriver2 test for such modem tests in the future.
368: Toby Schneider 2012-07-26 Fixed minor bug in mmdriver2 test that caused the summary results to be incomplete
367: Toby Schneider 2012-07-25 [merge] merge bug fix from 2.0-dsop
  321.2.5: Toby Schneider 2012-07-25 Fixed bug causing improper destruction of poll_mutex upon reload of goby_liaison webpage
366: Toby Schneider 2012-07-17 Fixed problems with std::abs(uint64) on lucid
365: Toby Schneider 2012-07-17 1. Took out 0xFF padding workaround for MM2 (no longer needed), 2; split up the test_mmdriver2 results based on the transmission
364: Toby Schneider 2012-07-11 [merge] merge from 2.0-dsop
  321.2.4: Toby Schneider 2012-07-10 Various fixes to allow Goby to fully operate with MOOSTimeWarp
363: Toby Schneider 2012-07-05 [merge] Merge from DSOP branch
  321.2.3: Toby Schneider 2012-07-05 Added 'poll' functionality to pb driver. Changed option extensions for various drivers to use same extension numbers for DriverConfig and ModemTransmission messages (similar to what Google does for various options messages)
  321.2.2: Toby Schneider 2012-06-25 {2.0.1~rc1+DSOP20120625+1} Release 2.0.1~rc1+DSOP20120625+1
362: Toby Schneider 2012-06-27 Added new unit test for testing the Micro-Modem (more than testing the Goby code)
361: Toby Schneider 2012-06-27 Prefixed all unit tests with 'goby_'
360: Toby Schneider 2012-06-27 Added 'draft' to title of user manual
359: Toby Schneider 2012-06-25 [merge] Merge from 2.0-dsop
  321.2.1: Toby Schneider 2012-06-25 [merge] Merge from DSOP Tiger12 cruise
    321.1.30: Toby Schneider 2012-06-15 Added ability for goby_bridge to forward micromodem ping response to topside
    321.1.29: Toby Schneider 2012-06-15 moved clear inbox for PB driver back to each send (so retries don't pile up)
    321.1.28: Toby Schneider 2012-06-15 added changes to make CACST into a DCCL message
    321.1.27: Toby Schneider 2012-06-15 Added option to reset ZMQ socket after timeout in PB modemdriver for more robust performance on abysmal connections
358: Toby Schneider 2012-06-25 [merge] merge from Tiger12 cruise
  357.1.1: Toby Schneider 2012-06-15 [merge] merge from 2.0-dsop and work on User Manual
    321.1.26: Toby Schneider 2012-06-13 1. Cleaned up UDPDriver output, 2. Changed fill for MMDriver from 0 to 255, 3. Fixed bug where PBDriver would only return first row in SELECT
    321.1.25: Toby Schneider 2012-06-13 Adding time on round trip for pb_driver
    321.1.24: Toby Schneider 2012-06-13 Improved glog output from ZMQ service
    321.1.23: Toby Schneider 2012-06-12 1. added link-level ack for NetworkAck message, but disallowed generation of NetworkAcks from these; 2. Changes to mm_driver test to work on DSOP june test
    321.1.22: Toby Schneider 2012-06-11 zero pad out to end of message for MicroModem 2 bug (should be temporary)
    321.1.21: Toby Schneider 2012-06-11 workarounds for various bugs in Micro-Modem 2 (svn -r 8876)
357: Toby Schneider 2012-06-06 [merge] merge from 2.0-dsop
  321.1.20: Toby Schneider 2012-06-06 1. Fixed uncommon bug (discovered on Beagleboard running Angstrom 2011-03-maintenance using Boost 1.45) where pushing an empty string to NMEASentence does not result in increasing the vector size
  321.1.19: Toby Schneider 2012-06-01 removed unneeded linkage to Wt for goby_store_server
  321.1.18: Toby Schneider 2012-06-01 {2.0.1~rc1+DSOP20120601+1} Release Release 1~rc1+DSOP20120601+1
  321.1.17: Toby Schneider 2012-06-01 added minimal blackout time from a queue's last sent message to when it tries to resend (if no ACK was received)
  321.1.16: Toby Schneider 2012-06-01 Fixed bug where route did not properly handle messages at destination if destination was an intermediate hop
  321.1.15: Toby Schneider 2012-05-31 {2.0.1~rc1+DSOP20120531+1, 1~rc1+DSOP20120531+1} 1. Added version (-V) to goby applications; 2. Release 1~rc1+DSOP20120531+1
  321.1.14: Toby Schneider 2012-05-31 [merge] Merge from 2.0 for Liaison fixes
  321.1.13: Toby Schneider 2012-05-25 [merge] merge memory leak fix and other minor fixes from 2.0
  321.1.12: Toby Schneider 2012-05-16 [merge] merge from 2.0 main branch for Release Candidate 1
356: Toby Schneider 2012-05-30 1. Extensions in LiaisonCommander are displayed with full name (same as TextFormat does); 2. fixed seg fault in Liaison; 3. MOOSTranslator inverse publish publishes trigger *last* so that all other content is received first
355: Toby Schneider 2012-05-25 removed debugging std::couts that slipped through the cracks
354: Toby Schneider 2012-05-22 fixed examples in lieu of changes to ModemTransmission::type
353: Toby Schneider 2012-05-18 updated goby nightly
352: Toby Schneider 2012-05-18 [merge] merge fix for memory leak in Logger from 1.1
  60.6.16: Toby Schneider 2012-05-18 fixed memory leak in goby_logger
  60.6.15: Toby Schneider 2012-02-14 renamed BUILD to build.sh to deal with name conflict on case-insensitive fileystems
  60.6.14: Toby Schneider 2012-02-14 version 1.1.2
  60.6.13: Toby Schneider 2012-02-14 added missing <string> header
  60.6.12: Toby Schneider 2011-12-06 [merge] merge from MBAT cruise
    60.7.1: Toby Schneider 2011-11-29 added new algorithms for MBAT cruise
  60.6.11: Toby Schneider 2011-09-22 fixes for oneiric
  60.6.10: Toby Schneider 2011-09-22 added LOOPBACK_AS_SENT (loopbacks all queued messages only as they are sent) and PROMISCUOUS (decode all messages for a queue regardless of destination) manipulators
  60.6.9: Toby Schneider 2011-09-20 changed references to gobysoft.com/doc to gobysoft.com/doc/1.0 and a few typos
  60.6.8: Toby Schneider 2011-08-31 reverted bug where cmake did not recognize .. properly
  60.6.7: Chris Murphy 2011-06-27 Adding getter for size field of DCCLMessage.
  60.6.6: Chris Murphy 2011-06-26 Making destructor public.
  60.6.5: Chris Murphy 2011-06-26 Making ModemDriverBase destructor virtual.
  60.6.4: Chris Murphy 2011-06-20 Adding a microsecond time function to goby time utilities.
  60.6.3: Toby Schneider 2011-06-16 release 1.1.1
  60.6.2: Toby Schneider 2011-06-16 incremented .so version since we broke ABI compatibility from 1.0 -> 1.1
  60.6.1: Toby Schneider 2011-06-10 fixed bug in DEPENDENCIES for lucid which does not add deb-src when using add-apt-repostories
351: Toby Schneider 2012-05-18 changed default max queue to 100 to avoid accidental memory leaks (infinitely growing buffers)
350: Toby Schneider 2012-05-16 [merge] Merge from 2.0-dsop
  321.1.11: Toby Schneider 2012-04-24 added start of unit test for NMEASentence
349: Toby Schneider 2012-05-16 Updated minimum Boost version to 1.40
348: Toby Schneider 2012-05-15 fixed bug with pAcommsHandler fails to add 'prefix' to subscribed variables
347: Toby Schneider 2012-05-10 added % to udpdriver test to avoid unintentional overflow
346: Toby Schneider 2012-05-07 {2.0.1~rc1} Release 2.0.1~rc1

*****************
Version 2.0.1~rc1

- Release candidate 1: all version 2 functionality complete; documentation still needs to be finished.
- Wire format of DCCL message changed to be little-endian for both body and header (previously header was big-endian and body was little-endian). This release is wire-incompatible with previous betas.
- Added set of CCL messages implemented as subset of DCCL.
- "dccllite" option added for compiling DCCL alone with a minimum of other dependencies within Goby and external.
- Basic router and two drivers added to support "traditional" networking (TCP / UDP) over slow links (predominantly Iridium)

bzr log --line -n0 -r 285..

345: Toby Schneider 2012-05-07 {2.0.1~rc1} added new acomms chapter
344: Toby Schneider 2012-05-04 1. Work on Goby2 user manual: MOOS and Acomms components; 2. Changed TransmissionType enumeration in DriverBase to have DRIVER_SPECIFIC value and removed all MICROMODEM_* values and put them in mm_driver.proto extensions; 3. Added prefix for pAcommsHandler MOOS variables
343: Toby Schneider 2012-05-03 added rounding to legacy CCL encoding for vehicle speed to avoid incorrect values due to truncation
342: Toby Schneider 2012-05-03 Changed DCCLFieldCodecBase::any_size and any_size_repeated to use WireType instead of FieldType to make it the same as encode
341: Toby Schneider 2012-05-03 changed CCL Error message to use bytes codec to avoid default DCCL string 1 byte overhead
340: Toby Schneider 2012-05-01 changed ccl_compatible flag to a selectable enumeration of various identifier codec options (with varint still being the default)
339: Toby Schneider 2012-05-01 added CCL message: MDAT_COMMAND
338: Toby Schneider 2012-05-01 completed implementation of base set of CCL messages: MDAT_EMPTY MDAT_REDIRECT MDAT_BATHY MDAT_CTD MDAT_STATE
337: Toby Schneider 2012-05-01 moved all #include <boost/filesystem.hpp> to .cpp files to avoid bug where including that header brings in a library dependency on boost_system
336: Toby Schneider 2012-04-30 Corrected fix for 64-bit architecture in CCL Lat/Long routine
335: Toby Schneider 2012-04-30 Renamed DCCLDefaultArithmeticFieldCodec to DCCLDefaultNumericFieldCodec to avoid confusion with arithmetic coding (which DCCLDefaultNumericFieldCodec does not do)
334: Toby Schneider 2012-04-30 Nearly finished with CCL State message - cleaned up traversal of message Body and Head for custom embedded messages
333: Toby Schneider 2012-04-30 Started added Legacy CCL as subset of DCCL
332: Toby Schneider 2012-04-27 1. Changed confusing wire format of DCCL where header was big-endian and body was little-endian. Entire message is sent as little-endian now, so header starts at the least significant byte and works upwards. This will break compatibility with prior beta releases of Goby2 but that's why we're still in beta. 2. Started adding CCL compatibility option
331: Toby Schneider 2012-04-26 Much cleaning up in Liaison scope: pause correctly still reads mail in another thread to avoid huge bandwidth and CPU hit when resuming. Made ZeroMQService poll() thread-safe. Added MOOS translator with type name prefix
330: Toby Schneider 2012-04-25 added missing header in goby_bridge
329: Toby Schneider 2012-04-25 Added ability to compile goby-logger without thread-safety (boost thread) and DynamicProtobufManager without ability to read files from disk (boost filesystem)
328: Chris Murphy 2012-04-25 Removing a duplicate include from flex_ostream.h
327: Chris Murphy 2012-04-25 Adding a 'dccllite' option, which builds a minimal DCCL shared library.  The goal is to have it use the minimum required code to support DCCL functionality, and only require linking against protobuf, dccl, and maybe dl or m or the like.  Still tracking down a boost::system dependency, which sneaks in somewhere.
326: Chris Murphy 2012-04-25 Adding an include line for dlfcn, since dccl.cpp calls dlsym.
325: Toby Schneider 2012-04-24 [merge] merge from 2.0-dsop
  321.1.10: Toby Schneider 2012-04-24 in DCCL, replaced boost::dynamic_bitset and get_more_bits signal with custom class based on std::deque<bool>
  321.1.9: Toby Schneider 2012-04-23 removed unused header from dccl.h
324: Toby Schneider 2012-04-20 [merge] merge from 2.0-dsop
  321.1.8: Toby Schneider 2012-04-20 reworked some functions of Dynamic Protobuf Manager so that compiled in descriptors are the same as those returned from Dynamic Protobuf Manager. Also added network level ACKs for goby_bridge
  321.1.7: Toby Schneider 2012-04-20 no need to try to route messages to yourself
  321.1.6: Toby Schneider 2012-04-20 moved runtime compilation of Protobuf messages from individual apps to dynamic_protobuf_manager to reduce duplication of code
323: Toby Schneider 2012-04-19 [merge] small bug fix from 2.0-dsop
  321.1.5: Toby Schneider 2012-04-19 fixed CMakeLists bug which caused pAcommsHandler build to be disabled
322: Toby Schneider 2012-04-19 [merge] merge from 2.0-dsop
  321.1.4: Toby Schneider 2012-04-18 moved goby_store_server to acomms from util
  321.1.3: Toby Schneider 2012-04-12 fixed bug where pure virtual function was called on destruction of SerialClient
  321.1.2: Toby Schneider 2012-04-12 Added second test for UDP driver for DSOP trial (split between two computers)
  321.1.1: Toby Schneider 2012-04-11 Wrote goby bridge for bridging various subnets together
321: Toby Schneider 2012-04-11 initial working unit test for goby-route
320: Toby Schneider 2012-04-10 Fixed overflow bug in Liaison Commander and started work on goby-route
319: Toby Schneider 2012-04-09 added UDP Driver to pAcommsHandler
318: Toby Schneider 2012-04-09 Added working UDP driver and test
317: Toby Schneider 2012-04-09 PBDriver1 test passes with new GobyStoreServer for two data packets (mimic MM rates 0 and 2)
316: Toby Schneider 2012-04-09 Added preliminary work on PB modem driver for DSOP May trial
315: Toby Schneider 2012-04-04 workaround for bug on Mac OS X 10.7.2 where MOOS translator would not compile because of private copy-constructor on RepeatedPtrField in Protobuf 2.3.0
314: Toby Schneider 2012-04-03 [merge] back-ported protobuf node from 3.0
  195.1.16: Toby Schneider 2012-04-03 put new header on goby-3 files that needed it
  195.1.15: Toby Schneider 2012-04-03 [merge] merged from 2.0
  195.1.14: Toby Schneider 2012-04-03 minor improvements to alog_to_goby_db
313: Toby Schneider 2012-04-03 added storage server for SQL storage of Protobuf messages
312: Toby Schneider 2012-03-18 downgraded DIE to WARN on transitional cfg being set in pAcommsHandler
311: Toby Schneider 2012-03-15 updated amac test to reflect changes to startup/restart in MACManager
310: Toby Schneider 2012-03-15 split doxygen html and pdf generation and turned off pdf generation by default because of problems with pdf generation on certain versions of Doxygen
309: Toby Schneider 2012-03-15 added stop/start to MACUpdate message
308: Toby Schneider 2012-03-14 1. MICROMODEM_MINI_DATA properly does not send if no data is given by Queue, 2. MOOS Format serializer uses std::boolalpha for all boolean values
307: Toby Schneider 2012-03-13 Added start of repeated typed codec and bug fix to default numeric codec involved rounding of doubles
306: Toby Schneider 2012-03-13 removed ~ backup files
305: Toby Schneider 2012-03-13 Added ability to load DCCL Codecs at runtime via shared libraries in pAcommsHandler
304: Toby Schneider 2012-03-09 Added install targets for examples
303: Toby Schneider 2012-03-09 [merge] 1. Changed license terms to LGPL for libraries (GPL still for binaries); 2. fixed bugs in examples
  302.1.1: Toby Schneider 2012-03-09 stripped old Copyright header notice
302: Toby Schneider 2012-03-09 clarifications about running unit tests in INSTALL.txt
301: Toby Schneider 2012-03-08 [merge] fixed merge from bug discovered on Mac OS X lion
  297.1.1: Toby Schneider 2012-03-08 [merge] fixed out of range access to vector using operator[] in moos translator
300: Toby Schneider 2012-03-08 additional correction to FindCryptopp.cmake
299: Toby Schneider 2012-03-07 improved Doxygen output for Developer's doc to include autogenerated protobuf code
298: Toby Schneider 2012-03-06 fixes to Crypto++ find and headers to attempt to resolve Fedora build problems
297: Toby Schneider 2012-03-05 fixed integer overflows on 32 bit system for far future times
296: Toby Schneider 2012-03-05 Added polling for ufld driver; made NaN default output in MOOS translator for unspecified repeated fields
295: Toby Schneider 2012-03-05 1. updated MACManager to properly use goby_time for its slot timer, 2. updated Liaison commander to be able to take its time from an external source (e.g. for MOOSTimeWarp)
294: Toby Schneider 2012-03-05 fixed bugs in time functions that caused overflows in large date, e.g. year 2391
293: Toby Schneider 2012-03-05 tweak to doc CMakeLists for precise
292: Toby Schneider 2012-03-05 [merge] merge from 2.S998 branch
  283.1.2: Toby Schneider 2012-03-05 small modifications to uFld driver
291: Toby Schneider 2012-03-03 small fix to ufield driver to all compiling
290: Toby Schneider 2012-03-03 [merge] merged in UFld driver for MOOS
  283.1.1: Toby Schneider 2012-02-20 added driver for mikerb's uField MOOS toolbox: successfully sends and receives message. Need to add ACK
289: Toby Schneider 2012-03-02 version.h now includes compile date as well as release date
288: Toby Schneider 2012-03-02 added missing boost-regex dependency to DEPENDECIES file
287: Toby Schneider 2012-02-29 fixes ACK bug on multi-frame messages
286: Toby Schneider 2012-02-23 backwards compatibility for Goby1 TesMoosApp verbosity settings and made Liaison modal dialogs configurable in size
285: Toby Schneider 2012-02-22 {2.0.0-beta4} release 2.0.0-beta4



*****************
Version 2.0.0-beta4

- MOOS Functionality (pAcommsHandler) complete, including backwards compatibility with version 1.
- New MOOS Application pTranslator that can be used for general purpose string<-->Google conversions
- goby_liaison backported from 3.0
  - liaison_commander written to take the place of iCommander
  - liaison_scope complete for MOOS scoping
- moos_gateway_g backported from 3.0 to interface goby_liaison with a MOOS community

bzr log --line -n0 -r 224..

285: Toby Schneider 2012-02-22 release 2.0.0-beta4
284: Toby Schneider 2012-02-17 1. Added basic Fedora and OpenSuse to DEPENDENCIES file; 2. fixed bug whe...
283: Toby Schneider 2012-02-14 updated DEPENDENCIES to be a script again
282: Toby Schneider 2012-02-13 fixed bug with MMDriver not clearing transmit message (and thereby sending the same message) when XST is not set or not available
281: Toby Schneider 2012-02-13 1. Examples now compile correctly; 2. Fixed bug with Liaison that was causing repeated fields not to expand on reload. 3. Added analyze_dccl_xml (as shell script) for backwards compatibility
280: toby 2012-02-13 workaround for broken maverick compiler
279: Toby Schneider 2012-02-13 Commented out building of zero_mq_test1 which fails in pbuilder on launchpad for unknown reasons: we're not using EPGM anyway in Goby 2
278: Toby Schneider 2012-02-09 added compiled-in options for Liaision docroot so most of the time users won't have to specify it
277: Toby Schneider 2012-02-09 added basic rounding to x,y <--> lat,lon algorithms to avoid precision based differences
276: Toby Schneider 2012-02-09 fixed unnecessary warning from mm_driver about expected frames never being received, added index level specification for MOOS translator
275: Toby Schneider 2012-02-08 fixed direct double equality comparison 
274: Toby Schneider 2012-02-07 More substantial cleanups in glog entries
273: Toby Schneider 2012-02-07 fixed compile problem with newer versions of boost filesystem
272: Toby Schneider 2012-02-06 a whole bunch of minor fixes and improvements discovered when integrating pAcommsHandler into missions-dsop (LAMSS)
271: Toby Schneider 2012-02-06 added manipulators back in to acomms (all in Queue) and a test (queue6) for them
270: Toby Schneider 2012-02-02 1. MAC Cycle update message configured for sending with DCCL and integrated into pAcommsHandler; 2. moved time from util->common; 3. tweaked ncurses closing so errors are properly shown after ncurses exits
269: Toby Schneider 2012-02-01 removed unneeded code from Transitional DCCL
268: Toby Schneider 2012-02-01 changed all explicit library linking to use CMake variable names to facilitate compilation when libraries are installed to non-standard places
267: Toby Schneider 2012-02-01 tweaks to CMakeLists for correct library finding when not on standard library search path
266: Toby Schneider 2012-02-01 Liaison needs ZeroMQ enabled - doing so
265: Toby Schneider 2012-02-01 fixed bug in Translator1 test where incomplete message was trying to be serialized
264: Toby Schneider 2012-02-01 Fix to FindWt
263: Toby Schneider 2012-02-01 streamlined optional package dependency resolution
262: Toby Schneider 2012-01-30 a few more disables to build correctly without 0MQ or Xerces
261: Toby Schneider 2012-01-30 added flag and modification to disable v1 transitional support and still be able to build MOOS
260: Toby Schneider 2012-01-30 fixed Dynamic protobuf test for dynamic libraries not ending in .so
259: Toby Schneider 2012-01-30 minor build system bug fix
258: Toby Schneider 2012-01-30 do not build zeromq tests if no ZeroMQ installed
257: Toby Schneider 2012-01-30 renamed BUILD to build.sh to deal with name conflict on case-insensitive fileystems
256: Toby Schneider 2012-01-30 Wt resources link autogenerated by CMake; logger GUI separated from VERBOSITY settings; removed analyze_dccl_xml in favor of dccl_xml_to_dccl_proto + analyze_dccl
255: Toby Schneider 2012-01-27 properly renamed namespaces for logger and configuration_reader
254: Toby Schneider 2012-01-27 missing <string> include in version and added a missing default parameter for the MOOS Translator
253: Toby Schneider 2012-01-25 renamed TesMoosApp GobyMOOSApp
252: Toby Schneider 2012-01-23 Added ability to access submessage fields from MOOS translator TECHNIQUE_FORMAT
251: Toby Schneider 2012-01-23 fixed MOOS<->protobuf translator for repeated fields
250: Toby Schneider 2012-01-23 fixed bug where Queued messages would get acknowledged (and popped) incorrectly
249: Toby Schneider 2012-01-19 Can cycle throw incoming messages in Liaison
248: Toby Schneider 2012-01-18 gave Liaison Commander and Scope their own ZeroMQ service and added field information/validation for commander
247: Toby Schneider 2012-01-16 improvements for Liaison scope
246: Toby Schneider 2012-01-11 modified pTranslator and wrote a preliminarily working pAcommsHandler using this translation functionality
245: Toby Schneider 2012-01-05 liaison commander now complete enough to send messages
244: Toby Schneider 2012-01-02 wrote much of the background for Liaison Commander and fixed some small memory leaks in Liaison
243: Toby Schneider 2011-12-30 working publish *from* liaison
242: Toby Schneider 2011-12-30 liaison scope properly pauses when in a different menu item
241: Toby Schneider 2011-12-30 [merge] merges from 3.0 for liaison
  195.1.13: Toby Schneider 2011-12-30 moved pubsub wrapper to common
  240.1.2: Toby Schneider 2011-12-30 renamed core namespace to common
  240.1.1: Toby Schneider 2011-12-30 [merge] merge from 3.0 for liaison
    195.1.12: Toby Schneider 2011-12-30 moved liaison and many zero mq components to common
    195.1.11: Toby Schneider 2011-12-30 [merge] merged changes from 2.0
    195.1.10: Toby Schneider 2011-11-17 [merge] merged from 2.0
    195.1.9: Toby Schneider 2011-11-17 [merge] renamed goby_core to goby_pb to suggest optional nature of the library
    195.1.8: Toby Schneider 2011-11-16 [merge] merged from 2.0, rejecting removed messages we need
    195.1.7: Toby Schneider 2011-11-16 [merge] merge from 2.0
    195.1.6: Toby Schneider 2011-11-16 fixed bugs from previous merge
    195.1.5: Toby Schneider 2011-11-16 [merge] merge from 2.0
    195.1.4: Toby Schneider 2011-11-16 [merge] merged from 2.0
    195.1.3: Toby Schneider 2011-09-23 fixed #include to wrong places
    195.1.2: Toby Schneider 2011-09-23 empty folder
    195.1.1: Toby Schneider 2011-09-23 [merge] merged into 2.0 except removes from 2.0 that we want to have in 3.0 (all core stuff)
240: Toby Schneider 2011-12-22 removed protobuf_generate_cpp line from pAcommsHandler CMakelists.txt
239: Toby Schneider 2011-12-21 mostly finished MOOS <--> Protobuf translator and reworked dynamic protobuf class to be more powerful
238: Toby Schneider 2011-12-20 incorporated publish, moos_var elements into DCCLv2->v1 backwards compatibility
237: Toby Schneider 2011-12-20 more work on Goby v1->v2 backwards compatibility
236: Toby Schneider 2011-12-19 added pTranslator MOOS translation application and added algorithms for key=value parser
235: Toby Schneider 2011-12-16 continued work on the MOOS translator: incorporated algorithms from pAcommsHandler v1
234: Toby Schneider 2011-12-06 more work on MOOS <--> Protobuf translator
233: Toby Schneider 2011-11-17 started working on MOOS string to Protobuf translator
232: Toby Schneider 2011-11-17 replaced use of core_header.proto with test specific header.proto
231: Toby Schneider 2011-11-16 fixed missed test on changing path
230: Toby Schneider 2011-11-16 merged libtransitional into moos, moved protobuf files out of common into their respective libraries where possible
229: Toby Schneider 2011-11-16 removed unneeded core protobuf messages (punted to 3.0)
228: Toby Schneider 2011-11-16 fixed file name bug in tests from renamed proto files
227: Toby Schneider 2011-11-16 renamed goby_protobuf to goby_common to avoid confusion with 3.0 library goby_pb
226: Toby Schneider 2011-11-16 renamed proto files to reflect goby module
225: Toby Schneider 2011-11-10 Dynamic Protobuf Manager now looks in the compiled-in pool; also added test for it
224: Toby Schneider 2011-11-10 added missing library depends for libgoby_moos.so

*****************
Version 2.0.0-beta3

bzr log --line -n0 -r 200..

223: Toby Schneider 2011-11-10 pAcommsHandler compiles, but is almost certainly broken. Release 2.0.0-beta3
222: Toby Schneider 2011-11-10 changed ceil(log(VALUES)/log(2)) to new ceil_log2 integer function to avoid floating point rounding problems caused by inlining under GCC 4.6.1
221: Toby Schneider 2011-11-09 removed some unnecessary dependencies from suggested list
220: Toby Schneider 2011-11-02 removed zmq from DEPENDENCIES
219: Toby Schneider 2011-10-18 updated out-of-date example for signal-slot in developer doc; also started pAcommsHandler migration from 1.0
218: Toby Schneider 2011-09-29 minor workaround for doxygen bug 660501 in versions of doxygen > 1.7.4
217: Toby Schneider 2011-09-29 went back to autogenerated Doxygen HTML header/footer so as not to break compatibility with newer versions of Doxygen
216: Toby Schneider 2011-09-23 Fixed more problems with boost::filesystem version 3 on Ubuntu oneiric
215: Toby Schneider 2011-09-23 fixed #include with incorrect path
214: Toby Schneider 2011-09-23 removed unneeded empty folder
213: Toby Schneider 2011-09-23 renamed moos_util to moos and moved files up a directory
212: Toby Schneider 2011-09-23 removed old entries from CMakeLists
211: Toby Schneider 2011-09-23 removed empty directories
210: Toby Schneider 2011-09-23 cleaned up moos tools and tests and fixed minor compilation problems
209: Toby Schneider 2011-09-22 updates to support ubuntu 11.10 beta
208: Toby Schneider 2011-09-21 removed core examples and fixed others
207: Toby Schneider 2011-09-21 added V2 to dev doc
206: Toby Schneider 2011-09-21 switched FlexOstream::is to use enumeration instead of function pointers which were mysteriously failing when -Bsymbolic-functions was linked (required by Ubuntu packaging)
205: Toby Schneider 2011-09-21 always link rpath into tests
204: Toby Schneider 2011-09-21 changed from using operator== for double comparison to custom function that compares a certain precision
203: Toby Schneider 2011-09-21 switched default codec name to null string from _default as a workaround for protobuf bug in 32-bit ubuntu 10.04; also fixed 32 bit long bug in MACManager
202: Toby Schneider 2011-09-21 tests now are set with full path
201: Toby Schneider 2011-09-20 another unsigned/signed comparison warning squashed
200: Toby Schneider 2011-09-20 another missing header

*****************
Version 2.0.0-beta1

bzr log --line -n0 

199: Toby Schneider 2011-09-20 changed version number for 2.0.0-beta1
198: Toby Schneider 2011-09-20 fixed incorrect headers
197: Toby Schneider 2011-09-20 updated nightly testing script to build tests
196: Toby Schneider 2011-09-20 removed core which will sadly be delayed to release 3.0
195: Toby Schneider 2011-09-20 cleaned up exception handling for bad decode
194: Toby Schneider 2011-09-20 turned off encryption for Chat example (doesn't work properly with iModemSim bugs) and make a single frame of the null string count as a dataless packet for MMDriver
193: Toby Schneider 2011-09-20 we no longer set NVRAM values in MMDriver before they are acknowledged (with ) to make sure our view of the MMDriver state is accurate
192: Toby Schneider 2011-09-20 [merge] merged in changes post CAPTURE2011 cruise from lp:~tes/goby/2.0-rework-modem-messaging
  191.1.18: Toby Schneider 2011-09-20 added static casts to remove compiler warning about unsigned / signed comparisons
  191.1.17: Toby Schneider 2011-09-20 finished update of documentation before 2.0 beta
  191.1.16: Toby Schneider 2011-09-14 updated documentation - mostly preliminarily done except rest of WHOI Micro-Modem NMEA mapping
  191.1.15: Toby Schneider 2011-09-14 fixed logic error where all nodes should be receivers - not just the one directed to at MMDriver level
  191.1.14: Toby Schneider 2011-09-14 updated MMDriver to provide XST - also aggregated CACST from both CYC and RXD for rate 0 transmissions
  191.1.13: Toby Schneider 2011-09-14 more work on bringing dev docu up to speed
  191.1.12: Toby Schneider 2011-09-14 significant updates to 2.0 developer doc
  191.1.11: Toby Schneider 2011-08-25 minor API improvements to DCCLCodec and start on Doxygen
  191.1.10: Toby Schneider 2011-08-24 QueueManager is no longer a singleton (no need) and all acomms examples brought up to speed
  191.1.9: Toby Schneider 2011-08-24 added 1 second wait between tests for MicroModem to recover
  191.1.8: Toby Schneider 2011-08-24 more work on DCCL API so that embedded messages are not copied more than necessary + fixed unit tests to reflect this change
  191.1.7: Toby Schneider 2011-08-22 reworked DCCL to avoid making many copies of embedded messages
  191.1.6: Toby Schneider 2011-08-22 corrected order of mmdriver unit test tests
  191.1.5: Toby Schneider 2011-08-22 modified Queue to work with new modem driver changes and fixed all unit tests
  191.1.4: Toby Schneider 2011-08-16 added unit test for time utilities; finished unit test for MMDriver to do 3 types of ranging, rate 0 data, rate 2 data and made appropriate fixes to MMDriver discovered while running that test
  191.1.3: Toby Schneider 2011-08-16 added unit test for `as` function family; added enum functionality to `as`, starting testing reworked MMDriver by adding a test for MPC
  191.1.2: Toby Schneider 2011-08-15 removed old tools directories
  191.1.1: Toby Schneider 2011-08-15 significantly reworked the modem messages and the MicroModem driver - everything is probably broken now
191: Toby Schneider 2011-08-15 moved core apps to apps parallel tree and moved dynamic protobuf manager into goby_util (from goby_protobuf)
190: Toby Schneider 2011-08-11 moved apps to a parallel tree from libraries
189: Toby Schneider 2011-08-11 [merge] moved tests out of library build into separate parallel tree
  188.1.1: Toby Schneider 2011-08-11 merged all acomms libraries into a single libgoby_acomms.so
188: Toby Schneider 2011-08-11 merged all util libraries into a single libgoby_util.so
187: Toby Schneider 2011-08-11 renamed acomms and util protobuf messages to have the module prefix
186: Toby Schneider 2011-08-11 make install no longer installs autogenerated Protobuf .cc files
185: Toby Schneider 2011-08-10 added boost signals back in since signals2 is not available before boost 1.39
184: Toby Schneider 2011-08-10 removed old MOOS modules and started writing a better DEPENDENCIES file. Also made Crypto++ optional
183: Toby Schneider 2011-08-10 renamed string.h as.h
182: Toby Schneider 2011-08-10 cleaned up locations of Exception classes and made them all capitalized words: e.g. ModemDriverException instead of driver_exception
181: Toby Schneider 2011-08-10 [merge] merged changes from 2.0-rework_dccl_option_extensions
  173.1.5: Toby Schneider 2011-08-09 [merge] merged from CAPTURE cruise
    180.1.12: Toby Schneider 2011-08-07 added NARROWBAND LBL as a slot type in MAC
    180.1.11: Toby Schneider 2011-08-07 implemented preliminary narrowband lbl ranging and updated whoi ranging example
    180.1.10: Toby Schneider 2011-08-07 added back in J. Walls check to set CCCLK at top of second and start of narrowband lbl in proto files
    180.1.9: Toby Schneider 2011-08-04 added missing virtual Destructors and a few other pedantic warnings
    180.1.8: Toby Schneider 2011-08-03 Removed UNKNOWN clock mode in favor of NO_SYNC_TO_PPS_AND_CCCLK_BAD so it takes 2 bits
    180.1.7: Toby Schneider 2011-08-02 added option to enable tests (default OFF)
    180.1.6: Toby Schneider 2011-08-01 Added ability to define custom ID codec for specialized cases where a different technique of encoding the DCCL ID is required
    180.1.5: Toby Schneider 2011-08-01 fixed bug in Mini Packet support about network byte order
    180.1.4: Toby Schneider 2011-08-01 fixed examples and ABC Driver for new ModemDataInit transmit initialization
    180.1.3: Toby Schneider 2011-08-01 started perls driver for their custom mini packet header + continued reworking Goby to have generic handling of MicroModem minipacket
    180.1.2: Toby Schneider 2011-08-01 [merge] merged up to LP
      180.2.1: Toby Schneider 2011-08-01 initial work to pull out PERLS specific implementation details into a separate driver and leave goby MMDriver support for Mini Packet generic
    180.1.1: Toby Schneider 2011-08-01 [merge] merged in 2.0-perls
      177.1.1: Toby Schneider 2011-07-28 [merge] merged in goby-perls, fixing most erroneous removals from lp:goby/1.0 revisions 137+
        60.5.13: jeff 2011-07-28 added some intelligence in setting the modem clock -- only set during
        60.5.12: jeff 2011-07-20 JMW: added unknown clock mode enum and update clk_mode_ from xst messages from
        60.5.11: jeff 2011-07-20 JMW: fixed a few bugs for mini packet encoding after testing in 2-node setup
        60.5.10: jeff 2011-07-20 JMW: encoding some meta data into mini packets -- useful for owtt and
        60.5.9: jeff 2011-06-07 JMW: piped in mini packet support and added mode to ranging message definition
  173.1.4: Toby Schneider 2011-08-08 [merge] merged changes from lp:goby/2.0 and made them work with the new Google assigned option extensions
  173.1.3: Toby Schneider 2011-07-29 reworked goby to use a single option extension assigned by Google for the project
  173.1.2: Toby Schneider 2011-07-22 reworked Queue to use a single option extension and modified tests and examples to work. Also removed old DCCL examples
  173.1.1: Toby Schneider 2011-07-22 initial work on making dccl option extensions an embedded message instead of all primitive fields
180: Toby Schneider 2011-08-01 improved dccl1 test to test order of repeated hex
179: Toby Schneider 2011-07-29 fixed bug where MACManager would start cycle on add_slot even if startup had not been called
178: Toby Schneider 2011-07-28 bool, numeric, and derivatives (enum) will not waste a value on presence if the field is `required`. DCCLCodec checks that a message is initialized before encoded (all required fields set)
177: Toby Schneider 2011-07-28 fixed bug using new slots in middle if slots are added before TDMA type is known
176: Toby Schneider 2011-07-27 removed old references to goby::acomms::NaN
175: Toby Schneider 2011-07-27 1. AMAC fixed tdma can start in the middle; 2. added indexing for DBO
174: Chris Murphy 2011-07-27 Silence two warnings (unused NaN, long long instead of int64_t)
173: Toby Schneider 2011-07-21 improved Doxygen comments for QueueManager
172: Toby Schneider 2011-07-21 1. fixed libqueue to work with new non-CCL DCCL header 2. removed most references to CCL (will add them in as a CCL <--> DCCL translator) 3. restored and augmented on_demand to work with the new libqueue 4. Added a unit test (queue5) and an example (queue_encode_on_demand) for the new on_demand queuing
171: Toby Schneider 2011-07-21 cleaned up end of example ALOG file for moos alog_to_goby_db example
170: Toby Schneider 2011-07-12 added two more tests for DCCL and a few functions for examining the DCCL ID (dccl.id)
169: Chris Murphy 2011-07-07 Public virtual destructor for modemdriverbase ported from goby1.
168: Chris Murphy 2011-07-07 Adding 'microtime' function, which hopefully is the future of goby timekeeping?
167: Chris Murphy 2011-07-07 Adjusting where cmake looks for modules to be referenced off of the *CURRENT* source directory, which lets goby work well as a sub-project.
166: Chris Murphy 2011-07-07 Fixing a bug in FindZeroMQ cmake module -- copy/paste error from XERCES original file.
165: Toby Schneider 2011-07-05 added example for MOOS tool alog_to_goby_db
164: Toby Schneider 2011-07-01 updated Doxygen for DCCL v2
163: Toby Schneider 2011-06-30 removed some relics in core from goby1 testing and added/improved API documentation for DCCLCodec
162: toby 2011-06-29 moved buggy gcc workaround for maverick from goby root to goby-core
161: Toby Schneider 2011-06-28 moved Dynamic ProtobufManager from goby-core to goby-protobuf so that goby-acomms does not depend on goby-core; changed DCCLv2 header to minimum 1 byte
160: toby 2011-06-23 workaround for broken gcc-4.4 compiler in Ubuntu
159: Toby Schneider 2011-06-22 more missing headers for older Wt
158: Toby Schneider 2011-06-22 work around for broken boost::optional support in Wt < 3.1.4
157: Toby Schneider 2011-06-22 missing header for older (pre 3.1.8) versions of Wt
156: Toby Schneider 2011-06-21 more work on parser for MOOS alogs
155: Toby Schneider 2011-06-21 added alog to SQLite tool
154: Toby Schneider 2011-06-20 removed another non-existent header
153: Toby Schneider 2011-06-20 removed reference to example C++ file that no longer exists
152: Toby Schneider 2011-06-20 more tweaks to Goby Liaison MOOS scope
151: Toby Schneider 2011-06-16 finished initial work on MOOS Liaison scope
150: Toby Schneider 2011-06-14 much more work on Liaison scope - additions to moos_node and zeromq service for unsubscribe
149: Toby Schneider 2011-06-10 fixed thread memory collision caused by static object
148: toby 2011-06-10 progress on goby-liaison and scope for it
147: toby 2011-06-09 goby_liaison can launch a hello world Wt application
146: toby 2011-06-08 [merge] finished plugins for dbo database
  145.1.1: toby 2011-06-08 added plugins for DBO
145: toby 2011-06-08 initial thread safety for new core classes
144: Toby Schneider 2011-06-07 [merge] merged from testing zeromq restructure
  143.1.1: Toby Schneider 2011-06-07 work on zeromq node structure
143: Toby Schneider 2011-06-07 added goby-liaison folder
142: toby 2011-06-07 [merge] merged changes from release 1.1.0
  60.5.8: toby 2011-06-07 typo in mm_driver warning
141: toby 2011-06-07 [merge] merge from 1.0
  60.5.7: toby 2011-06-06 version 1.1.0
  60.5.6: toby 2011-06-06 improved robustness for matching Micro-Modem CADRQ to Goby frame so that only frames we expect to be ack'd will be popped. removed setting CMOOSMsg Community to originating modem id so that we can properly identify whether the local pAcommsHandler published IN_HEX_*_B or not
  60.5.5: toby 2011-06-02 fixed bug for third-party 'pokes' of IN_*_HEX_*B messages in pAcommsHandler
  60.5.4: Henrik Schmidt 2011-06-02 fix so that pAcommsHandler does not reject messages sent by another community's pAcommsHandler
  60.5.3: toby 2011-06-02 [merge] merged changes from bf9 cruise
    60.4.17: toby 2011-05-30 fixed bug in WHOI Micro-Modem driver where multi-frame messages initiated via a third-party were not being properly acked if the number of frames sent < total number of frames / packet
    60.4.16: toby 2011-05-30 added feature to flush queues from topside
  60.5.2: Toby Schneider 2011-05-26 improved handling of CAERR,NMEA to avoid segmentation fault if multiple CAERR,NMEA posted for a given outgoing message
  60.5.1: Toby Schneider 2011-05-24 moved XML related headers to .cpp files
140: toby 2011-06-02 another non-existent header
139: toby 2011-06-02 fixed header with non-existent include
138: toby 2011-05-30 [merge] merged fixes from 1.0
  60.4.15: toby 2011-05-30 fixed regression on multi-frame message ack
  60.4.14: toby 2011-05-30 buffer publish data when there's no MOOS connection for later sending
  60.4.13: toby 2011-05-30 qsize posts on startup
  60.4.12: mituser 2011-05-20 build issues resolved for gcc-33
137: toby 2011-05-30 added graphviz for latex manual
136: toby 2011-05-30 added test for multi-frame ack
135: Toby Schneider 2011-05-27 reworked default Message codec to eliminate copy-paste code and thus make way for orthogonal traversal
134: Toby Schneider 2011-05-27 new tools for old DCCL
133: Toby Schneider 2011-05-27 [merge] merged 2.0-transitional
  132.1.8: Toby Schneider 2011-05-27 working pAcommsHandler and iCommander
  132.1.7: Toby Schneider 2011-05-26 cleaned out much of old (now transitional) DCCL encode / decode. Fixed iCommander to work with libtransitional, added queueing to libtransitional
  132.1.6: Toby Schneider 2011-05-22 [merge] merged changes from 1.0 related to building on gcc-3.3
    60.4.11: Stephanie Petillo 2011-05-20 fixed build for moos
    60.4.10: toby 2011-05-20 updated manual for release 1.1 and a few changes to support pViewMessage in TesMoosApp; also changed <all> in DCCL to not try to add already added message_var
    60.4.9: toby 2011-05-18 recommitted removal of unneeded dependencies on boost::regex
    60.4.8: toby 2011-05-18 fixed regression in flex_ncurses
    60.4.7: toby 2011-05-18 moved to single BUILD script instead of CONFIGURE/INSTALL and added GOBY_CMAKE_FLAGS and GOBY_MAKE_FLAGS environmental variable options
    60.4.6: Toby Schneider 2011-05-17 workaround for ASIO problem with epoll in 2.4.27 kernel and more fixes for gcc-3.3
    60.4.5: Toby Schneider 2011-05-17 added option to turn off building of certain MOOS apps
    60.4.4: toby 2011-05-17 fixed queue_xml for changes to support gcc-3.3
    60.4.3: Toby Schneider 2011-05-17 tentative work on supporting gcc-3.3
    60.4.2: toby 2011-05-17 removed unneeded dep on boost regex
    60.4.1: toby 2011-05-17 removed unfinished goby-core components - all development of goby-core is happening in lp:goby/2.0
  132.1.5: Toby Schneider 2011-05-16 transitional dccl feature complete; working out a few bugs
  132.1.4: Toby Schneider 2011-05-15 progress on getting DCCL1 backwards compatibility working
  132.1.3: Toby Schneider 2011-05-14 transitional DCCL XML -> protobuf schema finished
  132.1.2: Toby Schneider 2011-05-13 restored xml headers for transitional package
  132.1.1: Toby Schneider 2011-05-13 [merge] moved old DCCL to libtransitional
    60.2.65: Toby Schneider 2011-05-13 moved libraries for transitional pAcommsHandler
    60.2.64: Toby Schneider 2011-05-11 updated AUTHORS file to point to launchpad team
132: toby 2011-05-13 working Chat example with DCCL2
131: toby 2011-05-12 1. getting examples to work with new DCCL / Queue; 2. fixed goby::core::Application for when PubSub and/or Database not being used
130: Toby Schneider 2011-05-11 added make clean target for generated html dev doc
129: toby 2011-05-11 tweaks to test scripts
128: toby 2011-05-11 working DCCL in libqueue - fixed addressing issues, wrote two new tests to verify this. Found and fixed (longstanding?) bug - reported as 781376 for series 1.0
127: Toby Schneider 2011-05-11 updated AUTHORS file to point to launchpad team
126: Toby Schneider 2011-05-11 [merge] merged changes from series 1.0
  60.2.63: toby 2011-05-09 switched DEPENDENCIES for ubuntu to apt-get mbuild-dep
  60.2.62: Toby Schneider 2011-05-02 updated doc locations
  60.2.61: Toby Schneider 2011-05-02 updated main.dox Documentation page removing items available now in the wiki
  60.2.60: Toby Schneider 2011-04-29 release 1.0.3
  60.2.59: Toby Schneider 2011-04-29 Updated release notes for 1.0.3
125: toby 2011-05-11 preliminary working hooks system for DCCL callbacks for arbitrary google::protobuf::FieldOption extensions
124: toby 2011-05-10 pulled out boost::any from 'leaf' field codecs
123: toby 2011-05-10 renamed functions for libdccl that deal with boost::any to prefix 'any_' to allow for non-prefixed names to be used with concrete types
122: toby 2011-05-10 rename DCCLFieldCodec to DCCLFieldCodecBase
121: Toby Schneider 2011-05-09 add pre-encode and post-decode to DCCL, adding the concept of 'wire_type' (type off the wire) and 'field_type' (end user) and started working on hooks for allowing libqueue to fetch source and destination addressing
120: Toby Schneider 2011-05-09 reworked structure of ZeroMQNode and Application - ApplicationBase renamed to Application, MinimalApplicationBase renamed to ApplicationBase, ZeroMQNode made into a singleton instead of using virtual inheritance, fixed goby_database in light of these changes, fixed all examples, split out database and pubsub components of Application into two new classes: PubSubNode and DatabaseClient, added configuration *.proto for the new 'pubsub_node'
119: toby 2011-05-06 removed preview from eps images - causes trouble with epstopdf
118: toby 2011-05-06 correcting missing symbols in libdbo and version numbers
117: toby 2011-05-06 removed unneeded graphicspath command for Doxygen pdf latex
116: toby 2011-05-06 changed paths in user_manual to relative so that packages including tilde work with latex
115: toby 2011-05-06 removed old ManipulatorManager code
114: toby 2011-05-06 removed reference to xml proto file
113: Toby Schneider 2011-05-05 modified goby_db_to_alog to work with wt 3.1.2
112: toby 2011-05-05 [merge] merged vm changes
  111.2.1: toby schneider 2011-05-05 [merge] updated dependencies for zeromq
    111.1.1: toby schneider 2011-05-05 updated dependencies for zeromq
111: toby 2011-05-04 missed a file from last commit
110: toby 2011-05-04 Reworked ZeroMQNode to handle REQ/REP as well as publish subscribe and added an arbitrary number of Goby user sockets. Added two tests for the new ZeroMQNode
109: Toby Schneider 2011-05-03 added test for libqueue and got it basically working with the new DCCL2
108: toby 2011-05-02 tweaked documentation build
107: Toby Schneider 2011-05-02 moved doc source from share/doc to src/doc and set up CMake to build it
106: toby 2011-04-28 [merge] merged bug fixes from 1.0 series
  60.2.58: toby 2011-04-28 fix for Bug 772640 pertaining to finding MOOS with CMake
  60.2.57: toby 2011-04-21 [merge] merged changes from trunk to WHOI Micro-Modem Driver mm_driver
  60.2.56: toby 2011-04-06 turned off unneeded MOOS test from building
  60.2.55: toby 2011-04-06 Added basic configure / build testing using CTest/CDash
  60.2.54: Chris Murphy 2011-04-05 Switching int with unsigned int on an incrementer variable to silence a comparison warning.
  60.2.53: toby 2011-04-04 added example for WHOI REMUS LBL ranging, added link to new wiki to doc page, other minor front page documentation edits
  60.2.52: toby 2011-03-29 release 1.0.2b - working install on MAC OSX (with MacPorts)
  60.2.51: toby 2011-03-21 typo in basic_ccl example with serial port paths
  60.2.50: toby 2011-03-20 fixed bug in doxygen where includes were being missed
  60.2.49: toby 2011-03-17 updated DEPENDENCIES with list of MacPorts required
  60.2.48: toby 2011-03-17 moved find_package for Crytopp from DCCL to acomms because modemdriver needs it
105: toby 2011-04-28 made INSTALL run CONFIGURE if no CMakeCache.txt exists
104: toby 2011-04-26 reworking libqueue to work properly with the new DCCL
103: toby 2011-04-21 fixed files missing change from glogger() to goby::glog
102: toby 2011-04-21 more edits on user_manual for release 2.0
101: toby 2011-04-21 missed file from last commit
100: toby 2011-04-21 updating user manual with changes in store for Release 2
99: Toby Schneider 2011-04-20 fixed typo
98: toby 2011-04-20 modified Goby Logger to take advantage of short-circuiting behavior of && to avoid computation when logging verbosity is low; wrote version of hex_encode / hex_decode that doesn't rely on Crypto++; also wrote two tests - logger and hex codec.
97: Toby Schneider 2011-04-19 added test configuration for moos_gateway
96: Toby Schneider 2011-04-19 integrated MOOS functionality into Goby database and wrote a tool (goby_db_to_alog) to convert the resulting database to a traditional MOOS alog file
95: toby 2011-04-19 continuing to work out ZeroMQ C++ interface for goby - working preliminary moos gateway as well
94: Toby Schneider 2011-04-15 [merge] created workaround for issue with zeromq multi-messaging over PGM and started MOOS Gateway application
  90.2.1: toby 2011-04-11 [merge] playing with 0mq
93: toby 2011-04-15 continued work on database and goby-core PGM with 0mq
92: toby 2011-04-13 [merge] split ApplicationBase into MinimalApplicationBase -> ProtobufApplicationBase -> ApplicationBase and add Database
  86.1.13: toby 2011-04-08 added repeated option to DCCLCodec; added DCCLCommon object to manager logging and other shared tasks
  86.1.12: toby 2011-04-06 added script for testing
  86.1.11: toby 2011-04-06 Set up experimental CTest / CDash configuration
  86.1.10: toby 2011-04-06 Missed adds from previous commit
  86.1.9: toby 2011-04-06 Mostly working support for custom codecs for arbitrary protobuf message types
  86.1.8: toby 2011-04-05 properly working repeated types
  86.1.7: toby 2011-03-31 started work on getting repeats to work
  86.1.6: toby 2011-03-30 mostly working DCCL2 for non-repeated types
  86.1.5: toby 2011-03-30 working variable & fixed size encoding of basic types
  86.1.4: toby 2011-03-29 added variable size option to codecs, fleshed out DCCL info call, created some new bugs
  86.1.3: toby 2011-03-29 more progress on DCCL2 - reworked DCCLFieldCodec so derived classes of non-CPPTYPE_MESSAGE can more easily access protobuf extensions without knowledge of protobuf
  86.1.2: toby 2011-03-25 more work on new DCCL2
  86.1.1: toby 2011-03-25 gutted original DCCL and working on protobuf DCCL2
91: toby 2011-04-11 [merge] working core examples with 0mq and PGM (without gobyd)
  90.1.1: toby 2011-04-11 playing around with 0mq
90: toby 2011-04-06 removed regression with Polling MAC on WHOI Micro-Modem
89: toby 2011-04-06 streamlined DRQ process and added more logic sanity checks
88: toby 2011-03-22 added REMUS LBL ranging example
87: toby 2011-03-17 [merge] merged MAC OS X building changes from 1.0 series
  60.2.47: toby 2011-03-16 updated release notes for 1.0.2
  60.2.46: toby 2011-03-16 incremented version #
  60.2.45: toby 2011-03-16 Made minor changes to CMake build system to allow building with MacPorts on OS X
  60.2.44: toby 2011-03-10 fixed typo in user_manual.pdf (goby/cfg -> goby/share/cfg)
86: toby 2011-03-16 improved package finding for examples, removed some goby/util/string.h code where a Boost solution exists
85: toby 2011-03-10 [merge] minor fixes to CMake build
  60.2.43: toby 2011-03-09 version explicitly defined
84: toby 2011-03-09 cleaned up CMakeLists.txt slightly
83: toby 2011-03-08 [merge] merged changes needed for .deb packaging 1.0
  60.2.42: toby 2011-03-08 patched from downstream debian-changes-1.0.1-1gobydev1~maverick1
  60.2.41: toby 2011-03-07 missing SOVERSION on liblogger
  60.2.40: toby 2011-03-07 updated ubuntu package names
  60.2.39: toby 2011-03-07 changed example ccl_and_dccl to use *relative* path for xml files
  60.2.38: toby 2011-03-07 minor typo in README
  60.2.37: toby 2011-03-07 fixes to User Manual with change to example paths
  60.2.36: toby 2011-03-07 [merge] merged changes from trunk to make .deb packaging feasible
  60.2.35: toby 2011-03-04 [merge] merged changes from trunk for release 1.0.0
82: toby 2011-03-07 moved examples, documentation, xml to goby/share
81: toby 2011-03-04 Fixed a few bugs in chat example, finalized installing documentation
80: toby 2011-03-04 added user manual PDF
79: toby 2011-03-04 Updated doxy2pdf to include release version and date
78: toby 2011-03-04 finished release V1.0 version of the User Manual, including updates to the Goby-Core examples
77: toby 2011-03-03 updated pREMUSCodec and iCommander to use new MOOS configuration. Fixed bug that was causing seg fault in libqueue due to bug in GCC with string constructors
76: toby 2011-03-02 Added some basic MOOS files and included them in the User Manual
75: toby 2011-03-02 continued work on User Manual
74: toby 2011-03-02 documented moos_protobuf_helpers
73: toby 2011-03-02 Work on User Manual in advance of release 1.0
72: toby 2011-02-28 substantial updates to documentation in advance of v1.0, updates to linebasedcomms to support more flexible TCP networking (addressing with TCP Server) and groundwork for later goby-core work with TCP networking, wrote a 'fake' driver ABCDriver and corresponding modem simulator (abc_modem_simulator) in order to demonstrate basic modem driver functionality
71: toby 2011-02-18 moved LAMSS specific xml messages to missions-lamss repository
70: toby 2011-02-17 updated build for moos script to add libcore
69: toby 2011-02-17 moved all .proto files to common goby_protobuf library
68: toby 2011-02-17 [merge] merged pre-release v1.0
  60.2.34: toby 2011-02-17 fixed bug with improper destination addressing and acknowledge on frames > 0
  60.2.33: toby 2011-02-17 added release notes and fixed small parsing error with command-line configuration
  60.2.32: toby 2011-02-17 [merge] merged changes from trunk
  60.2.31: toby 2011-02-17 fixed bug when values destined for the DCCL header are overwritten by defaults if these values are a different type to be later converted by an Algorithm
  60.2.30: toby 2011-02-15 changed doxygen header on doc pages to reflect series number
  60.2.29: toby 2011-02-15 [merge] fixed a number of bugs with new configuration and streamlined new internal protobuf message passing
  60.2.28: toby 2011-02-14 working on new configuration for MOOS apps that gives command line support and example .moos block
  60.2.27: toby 2011-02-10 finished major work cleaning out MOOS modules and dependencies and reworking configuration for pAcommsHandler
  60.2.26: toby 2011-02-09 [merge] changed manipulator handling to separate ManipulatorManager class
    60.3.1: toby 2011-02-09 fixed lowercase enumerations, manipulators, and tcp_share
  60.2.25: toby 2011-02-09 missed file from previous commit
  60.2.24: toby 2011-02-09 work on new protobuf configuration for pAcommsHandler
  60.2.23: toby 2011-02-03 partial progress towards protobuf configuration in pAcommsHandler and goby
  60.2.22: toby 2011-02-02 reworking driver base to use protobuf configuration object and bringing documentation into line
  60.2.21: toby 2011-01-22 corrected entry in CMakeLists
  60.2.20: toby 2011-01-22 replaced libamac Slot and libqueue QueueKey and QueueConfig with Google Protocol Buffers equivalents
  60.2.19: toby 2011-01-19 replaced ModemMessage with a variety of protocol buffers messages
67: toby 2011-02-17 [merge] added XML files for DCCL
  66.2.1: toby schneider 2011-02-17 [merge] updated DEPENDENCIES for Debian 6
    66.1.1: toby schneider 2011-02-17 updated DEPENDENCIES for Debian 6
66: toby 2011-02-17 fixed error in DEPENDENCIES script with Ubuntu PPA for Wt
65: toby 2011-02-15 minor edit to Installing Goby doc
64: toby 2011-02-14 fixed type in main doxygen page
63: toby 2011-02-14 updated main document page
62: toby 2011-01-29 minor updates to main page documentation (including installation instructions)
61: toby 2011-01-24 [merge] merge changes from SWAMSI11 cruise - added MOOS applications
  60.2.18: mituser 2011-01-17 modem driver subclass (e.g. MMDriver) now handles raw callbacks
  60.2.17: mituser 2011-01-17 fixed missing <CR><NL> at the end of the Hydroid Gateway GPS request
  60.2.16: toby 2011-01-17 added GPS request for Hydroid Gateway buoy
  60.2.15: toby 2011-01-13 updated functionality for Hydroid buoy
  60.2.14: toby 2011-01-12 no xml files is not an error
  60.2.13: mituser 2011-01-12 fixed unsigned for int
  60.2.12: mituser 2011-01-12 minor fix to output properly missing LBL beacon values
  60.2.11: toby 2011-01-12 added compatibility for REMUS LBL
  60.2.10: toby 2011-01-12 started work on .proto files for acomms p.2 
  60.2.9: toby 2011-01-12 started work on .proto files for acomms
  60.2.8: mituser 2011-01-11 more robust Wt finding
  60.2.7: toby 2011-01-11 updated defaults for the CMakeLists
  60.2.6: toby 2011-01-11 split out CONFIGURE from INSTALL
  60.2.5: mituser 2011-01-11 improved robustness of FindMOOS
  60.2.4: toby 2011-01-11 fixed FindMOOS typo
  60.2.3: toby 2011-01-11 added MOOS applications from moos-ivp-local
  60.2.2: toby 2011-01-11 updated .bzrignore to ignore build related files for documentation
  60.2.1: toby 2011-01-11 [merge] improved CMake error reporting when missing packages. Finished ex2
    60.1.1: toby 2011-01-02 finished example 2: gps driver
60: toby 2010-12-13 added support to ApplicationBase for Header .proto
59: toby 2010-11-23 fixed png size
58: toby 2010-11-23 updated UML images
57: toby 2010-11-03 made NCurses optional: working on building for Windows
56: toby 2010-11-03 started second example for core and added more MOOS mimics
55: toby 2010-10-14 [merge] merged changes from flight to San Diego: mostly documentation (Doxygen and otherwise). See submerge changes for details
  54.1.2: toby 2010-10-14 wrote some examples, a second whitepaper, and the start of a user manual
  54.1.1: toby 2010-10-12 doxygen documentation for goby::core and some work on making the logger optionally thread safe
54: toby 2010-10-04 seamless configuration / cli initially finished for goby core
53: toby 2010-09-29 fixed minor errors from previous commit
52: toby 2010-09-29 added configuration file / command line reader for gobyd and application_base
51: toby 2010-09-29 proto files (*.pb.cc and *.pb.h) are generated to goby/include/... to simplify compiling
50: toby 2010-09-24 significant progress on core: working cmoosapp mimic, full database support for all google protobuf types (included embedded messages and repeated message)
49: toby 2010-09-22 worked out several critical thread safety bugs in core
48: toby 2010-09-20 initial work on a mimic for CMOOSApp for easy transition for MOOS users
47: toby 2010-09-18 basic working core (gobyd)
46: toby 2010-09-15 [merge] significant initial work on goby core
  45.1.5: toby 2010-09-15 working connection between gobyd and goby base application
  45.1.4: toby 2010-09-14 initial testing work on gobyd and goby app
  45.1.3: toby 2010-08-31 significant testing work for gobyd integrating google protobuf with Wt::Dbo and boost::interprocess
  45.1.2: toby 2010-08-16 new shells for exception classes and tests for goby core
  45.1.1: toby 2010-08-12 significant cleanup of callback structure
45: toby 2010-08-10 [merge] merged changes from GLINT10 cruise
  44.1.3: toby 2010-08-10 minor adjustments to MAC log output and line-based-comms methods
  44.1.2: toby 2010-08-01 checking of DCCL message algorithm validity
  44.1.1: toby 2010-07-29 changed linebasedcomms to a single user system for simplicity
44: toby 2010-07-27 [merge] merged work on replacement for serial client with abstracted networking "serial-like" client (liblinebasedcomms)
  43.1.3: toby 2010-07-26 many small changes to improve logging facilities
  43.1.2: toby 2010-07-19 replaced libserial with liblinebasedcomms which allows emulated serial type line based connections over serial, tcp (as client), tcp (as server) and (in the future) UDP
  43.1.1: toby 2010-07-19 added preliminary work for tcp_server option
43: toby 2010-07-15 [merge] merged lp:~tes/goby/time_fix with namespace cleanup and time source consistency
  41.1.4: toby 2010-07-15 fixed some Doxygen comments to reflect new namespaces
  41.1.3: toby 2010-07-14 reduced excessive number of namespaces and changed namespaces to reflect file structure: goby::acomms and goby::util. Also changed some class names to resolve the resulting ambiguity.
  41.1.2: toby 2010-07-14 put all of goby into goby:: namespace and renamed some libraries
  41.1.1: toby 2010-07-13 calls to time should uniformly use boost date_time (and util/gtime.h helper functions)
42: Chris Murphy 2010-07-14 Minor COPYING tweak to test chrismurf commit.
41: toby 2010-07-13 [merge] merged lp:~chrismurf/goby/nmea_rewrite
  40.1.3: Chris Murphy 2010-07-13 Removing super typedef and references.
  40.1.2: Chris Murphy 2010-07-13 Going back to Toby's original nomenclature for message(), it was better.
  40.1.1: Chris Murphy 2010-07-13 Initial commit of new NMEASentence class.
40: toby 2010-07-12 fixed bug taking base 10 log of negative number & changed double size_ to unsigned size_ in modem_message
39: toby 2010-07-11 mainly minor bug and memory fixes
38: toby 2010-06-15 added support for arrays and delta-difference encoded arrays
37: toby 2010-06-11 added support for the Hydroid Gateway Buoy from Andrew Bouchard
36: toby 2010-05-13 white paper
35: toby 2010-05-12 fixed missing include
34: toby 2010-04-29 changed website references to gobysoft.com
33: toby 2010-04-01 working on html css
32: toby 2010-03-30 fixed bad eps conversion
31: toby schneider 2010-03-21 minor doc update
30: toby schneider 2010-03-21 updated doc to reflect recent changes
29: toby schneider 2010-03-20 changed the header to 6 bytes to include time, src_id, dest_id, larger dccl_id and added many features related to that
28: toby schneider 2010-03-17 more work on crypto
27: toby 2010-03-15 added crypto
26: toby schneider 2010-03-09 cleaned up encode/decode interface to use dccl::MessageVal in place of basic types
25: toby schneider 2010-03-04 cleaning up DCCL and adding delta encoding
24: toby 2010-02-28 updated URLs in documentation
23: toby 2010-02-25 reworked priority queuing
22: toby 2010-02-04 fixed wrong path in several examples
21: toby schneider 2010-02-04 reworked UML diagrams
20: toby 2010-01-26 tweaked one of the UML models
19: toby schneider 2010-01-25 added more UML models to the documentation
18: toby schneider 2010-01-25 libamac documentation
17: toby schneider 2010-01-21 added DCCL ids used by LAMSS
16: toby schneider 2010-01-21 more documentation
15: toby schneider 2010-01-20 added simple examples for each of the goby-acomms libraries
14: toby schneider 2010-01-17 [merge] mostly documentation work and added hex
  13.1.2: toby schneider 2010-01-16 added MAC
  13.1.1: toby schneider 2010-01-15 work on documents
13: toby 2010-01-06 fixed bug with modem configuration
12: toby schneider 2010-01-03 added <hex/> tag for DCCL
11: toby 2009-12-21 documentation!
10: toby schneider 2009-12-19 documentation work
9: toby schneider 2009-12-17 worked on making driver a base class so that different types of modems can have their own driver class (like MMDriver for the WHOI Micro-Modem). also did a good deal of doxygen
8: toby schneider 2009-12-16 added some namespaces and made consistent header files
7: toby schneider 2009-12-11 [merge] reworked structure to be more in line with linux conventions
  6.1.1: toby schneider 2009-12-10 working on cleaning up structure
6: toby schneider 2009-12-09 minor interface changes to deal with MAC
5: toby 2009-12-05 [merge] initial working version of libqueue, incorporating much of the heart of pAcommsHandler
  4.1.1: toby schneider 2009-12-03 consistent header comments, algorithms can take in MessageVal and thus have different input and output, included documentation for libdccl, cleaned up MessageVal
4: toby schneider 2009-12-03 added more information on installing and made install/dependencies files executable
3: toby schneider 2009-12-03 incorporated utility libraries and fixed compile problems with libmmdriver
2: toby schneider 2009-12-02 completed transition to CMake
1: toby schneider 2009-12-01 initial commit
