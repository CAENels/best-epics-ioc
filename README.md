[![Build Status](https://travis-ci.org/CAENels/best-epics-ioc.svg?branch=master)](https://travis-ci.org/CAENels/best-epics-ioc)

## Changelog

### 1.0.32

 - Added PIDconf in config.ini

### 1.0.31

 - Fixed ROCs in BEST:Init

### 1.0.30

 - Modified BPMselector to use config.ini (set and init)
 - Removed TetrAMM number check on BPM selector
 - Added TetrAMM check in TetrAMMx:Range/HVvoltage/HVenable
 - Added BEST:SFP_A and BEST:SFP_B
 - Fixed BEST:Init

### 1.0.29

- compatible with libbest_1.8.12
- Fixed set/get PID param functions
- Fixed BPM:Selector and BPM:Selector_RBV -> X, Y, I0
- Fixed PID frequency in BEST:Init
- Removed unused PID:Config and PID:Config_RBV records
- Fixed buildnumber

### 1.0.28

- Added best-lib functions check (before writing config.ini)
- Removed PID :RoiX, :RoiY, :RoiIntMin, :RoiIntMax, :Roc
- Init moved from startup to writeBest
- Added :TetrAMM0HVvoltage :TetrAMM1HVvoltage
- Added :TetrAMM0HVenable :TetrAMM1HVenable
- Added best-lib dependence in Debian-xx/control

### 1.0.27

- Fixed best_rbv_PidSetpoint with setSetpoint2
- Fixed Init record
- Used ostringstream in UpdateFrequency (setWindAvg)
- Added PID:SetpointX_RBV, PID:SetpointY_RBV, PID:SetpointI0_RBV
- Changed to Bo BPMx:ENArocx and BPMx:ENAbeamOff
- Changed to Bi BPMx:ENArocx_RBV and BPMx:ENAbeamOff_RBV
- Modified best_NumberTetrAMM with getTetrammsNumber()

### 1.0.26

- Moved scaling to best-lib (set/getROClimits and set/getBPMoffset)
- Included <stream> to write in confi.ini with scientific notation
- Added ROCs enable to init and to config.ini
- compatible with libbest_1.8-10

### 1.0.25

- Added :Init (Initialize device with parameters in config.ini)
- Added Binary Input Bi and String Input Stringin record types

### 1.0.24

- Added :PID:FreqX      (Write PID X update frequency to FPGA and CONFIG_FILE)
- Added :PID:FreqY      (Write PID Y update frequency to FPGA and CONFIG_FILE)
- Added :PID:FreqI0     (Write PID I0 update frequency to FPGA and CONFIG_FILE)
- Added :PID:FreqX_RBV  (Read PID X update frequency from FPGA)
- Added :PID:FreqY_RBV  (Read PID Y update frequency from FPGA)
- Added :PID:FreqI0_RBV (Read PID I0 update frequency from FPGA)
- compatible with libbest_1.8-9

### 1.0.23

- Added :BPM0:OffsetX      (Write BPM Offset to FPGA and CONFIG_FILE)
- Added :BPM0:OffsetY      (Write BPM Offset to FPGA and CONFIG_FILE)
- Added :BPM0:OffsetX_RBV  (Read BPM Offset from FPGA)
- Added :BPM0:OffsetY_RBV  (Read BPM Offset from FPGA)
- compatible with libbest_1.8-8

### 1.0.22

- Added :BPM0:RocX      (Write ROC to FPGA)
- Added :BPM0:RocY      (Write ROC to FPGA)
- Added :BPM0:BeamOffTh (Write ROC to FPGA)
- Added :BPM1:RocX      (Write ROC to FPGA)
- Added :BPM1:RocY      (Write ROC to FPGA)
- Added :BPM1:BeamOffTh (Write ROC to FPGA)
- Added :BPM0:RocX_RBV  (Read ROC from FPGA)
- Added :BPM0:RocY_RBV  (Read ROC from FPGA)
- Added :BPM0:BeamOffTh_RBV (Read ROC from FPGA)
- Added :BPM1:RocX_RBV  (Read ROC from FPGA)
- Added :BPM1:RocY_RBV  (Read ROC from FPGA)
- Added :BPM1:BeamOffTh_RBV (Read ROC from FPGA)
- compatible with libbest_1.8-7

### 1.0.21

- Changed :BPM0:ENArocX  (Write enable ROC configuration to FPGA only)
- Changed :BPM0:ENArocY  (Write enable ROC configuration to FPGA only)
- Changed :BPM0:ENArocI0 to :BPM0:ENAbeamOff (Write enable ROC configuration to FPGA only)
- Changed :BPM1:ENArocX  (Write enable ROC configuration to FPGA only)
- Changed :BPM1:ENArocY  (Write enable ROC configuration to FPGA only)
- Changed :BPM1:ENArocI0 to :BPM1:ENAbeamOff (Write enable ROC configuration to FPGA only)
- Changed :BPM0:ENArocI0_RBV to :BPM0:ENAbeamOff_RBV (Read enable ROC configuration from FPGA)
- Changed :BPM1:ENArocI0_RBV to :BPM1:ENAbeamOff_RBV (Read enable ROC configuration from FPGA)
- compatible with libbest_1.8-6
- Roc & Roi values and enable won't be saved in CONFIG_FILE and thus not loaded at startup

### 1.0.20

- Added :BPM0:ENArocX  (Write enable ROC configuration to FPGA and CONFIG_FILE)
- Added :BPM0:ENArocY  (Write enable ROC configuration to FPGA and CONFIG_FILE)
- Added :BPM0:ENArocI0 (Write enable ROC configuration to FPGA and CONFIG_FILE)
- Added :BPM1:ENArocX  (Write enable ROC configuration to FPGA and CONFIG_FILE)
- Added :BPM1:ENArocY  (Write enable ROC configuration to FPGA and CONFIG_FILE)
- Added :BPM1:ENArocI0 (Write enable ROC configuration to FPGA and CONFIG_FILE)
- Added :BPM0:ENArocX_RBV  (Read enable ROC configuration from FPGA)
- Added :BPM0:ENArocY_RBV  (Read enable ROC configuration from FPGA)
- Added :BPM0:ENArocI0_RBV (Read enable ROC configuration from FPGA)
- Added :BPM1:ENArocX_RBV  (Read enable ROC configuration from FPGA)
- Added :BPM1:ENArocY_RBV  (Read enable ROC configuration from FPGA)
- Added :BPM1:ENArocI0_RBV (Read enable ROC configuration from FPGA)
- compatible with libbest_1.8-6

### 1.0.19

- Added BPM0:Crossbar (Write TetrAMM crossbar configuration to FPGA and CONFIG_FILE)
- Added BPM1:Crossbar (Write TetrAMM crossbar configuration to FPGA and CONFIG_FILE)
- Added BPM0:Crossbar_RBV (Read TetrAMM crossbar configuration from FPGA)
- Added BPM1:Crossbar_RBV (Read TetrAMM crossbar configuration from FPGA)
- compatible with libbest_1.8-6

### 1.0.18

- Added :BPM:Selector (Write BPM selector configuration to FPGA)
- Added :BPM:Selector_RBV (Read BPM selector configuration from FPGA)
- compatible with libbest_1.8-5

### 1.0.17

- Added :PID:INconfig (Write input PID configuration to FPGA)
- Added :PID:INconfig_RBV (Read input PID configuration from FPGA)

### 1.0.16

- Added :PID:Config_RBV (Read PID conf from FPGA)
- Added :PID:OUTconfig (Write PID output configuration to FPGA and CONFIG_FILE)
- Added :PID:OUTconfig_RBV (Read PID output configuration from FPGA)

### 1.0.15

- Added :BPM0:Orient (write BPM0 orientation to FPGA and CONFIG_FILE)
- Added :BPM0:Orient_RBV (read BPM0 orientation from FPGA)
- Added :BPM1:Orient (write BPM1 orientation to FPGA and CONFIG_FILE)
- Added :BPM1:Orient_RBV (read BPM1 orientation from FPGA)

### 1.0.14

- BPM0:ScaleX changed to BPM0:ScaleX_RBV (since it was a ai)
- BPM0:ScaleX changed to ao db record (write to FPGA and to CONFIG_FILE)
- BPM0:ScaleY changed to BPM0:ScaleY_RBV (since it was a ai)
- BPM0:ScaleY changed to ao db record (write to FPGA and to CONFIG_FILE)
- BPM1:ScaleX changed to BPM1:ScaleX_RBV (since it was a ai)
- BPM1:ScaleX changed to ao db record (write to FPGA and to CONFIG_FILE)
- BPM1:ScaleY changed to BPM1:ScaleY_RBV (since it was a ai)
- BPM1:ScaleY changed to ao db record (write to FPGA and to CONFIG_FILE)
- Added write in CONFIG_FILE the PID parameters and added library to write INI file

### 1.0.13

- updated Readme with all EPICS db list and the relative libbest functions
- added TODO list in Readme


## TODO

- add read/write ROC limits
- add read/write ROI limits (not essential)
- add enable/disable ROC check
- add read/write BPM select
- add read/write BPM Crossbar
- add read/write single BPM offsets
- change PID setpoint to read from FPGA and not from CONFIG_FILE
- remove getROCandROI() from CONFIG_FILE

# List of Records

## EPICS COMMAND LIST TABLE

| PID Configuration                    | EPICS commands                                  | FPGA | CONFIG_FILE |
| ------------------------------------ | ----------------------------------------------- | ---- | ----------- |
| enable/disable feedback              | :PID:Enable                                     | W    | ...         |
| feedback status                      | :PID:Status                                     | W    | ...         |
| reset controller                     | :PID:Reset                                      | W    | ...         |
| Set input PID configuration          | :PID:INconfig                                   | W    | ...         |
| Get input PID configuration          | :PID:INconfig_RBV                               | R    | ...         |
| Set DAC output channel configuration | :PID:OUTconfig                                  | W    | W           |
| Get DAC output channel configuration | :PID:OUTconfig_RBV                              | R    | ...         |
| Set PID setpoint                     | :PID:SetpointX                                  | W    | ...         |
| ...                                  | :PID:SetpointY                                  | W    | ...         |
| ...                                  | :PID:SetpointI0                                 | W    | ...         |
| Get PID setpoint                     | :PID:SetpointX_RBV                              | R    | ...         |
| ...                                  | :PID:SetpointY_RBV                              | R    | ...         |
| ...                                  | :PID:SetpointI0_RBV                             | R    | ...         |
| Set PID offset                       | :PID:OffsetX                                    | W    | W           |
| ...                                  | :PID:OffsetY                                    | W    | W           |
| ...                                  | :PID:OffsetI0                                   | W    | W           |
| Get PID offset                       | :PID:OffsetX_RBV                                | R    | ...         |
| ...                                  | :PID:OffsetY_RBV                                | R    | ...         |
| ...                                  | :PID:OffsetI0_RBV                               | R    | ...         |
| Set PID parameters                   | :PID:KpX, KiX, KdX, eminX, ImaxX, OminX         | W    | W           |
| ...                                  | OmaxX, OgainX                                   | W    | W           |
| ...                                  | :PID:KpY, KiY, KdY, eminY, ImaxY, OminY         | W    | W           |
| ...                                  | OmaxY, OgainY                                   | W    | W           |
| ...                                  | :PID:KpI0, KiI0, KdI0, eminI0, ImaxI0           | W    | W           |
| ...                                  | OminI0, OmaxI0, OgainI0                         | W    | W           |
| Get PID parameters                   | :PID:KpX_RBV, KiX_RBV, KdX_RBV, eminX_RBV       | R    | ...         |
| ...                                  | ImaxX_RBV, OminX_RBV, OmaxX_RBV, OgainX_RBV     | R    | ...         |
| ...                                  | :PID:KpY_RBV, KiY_RBV, KdY_RBV, eminY_RBV       | R    | ...         |
| ...                                  | ImaxY_RBV, OminY_RBV, OmaxY_RBV, OgainY_RBV     | R    | ...         |
| ...                                  | :PID:KpI0_RBV, KiI0_RBV, KdI0_RBV, eminI0_RBV   | R    | ...         |
| ...                                  | ImaxI0_RBV, OminI0_RBV, OmaxI0_RBV, OgainI0_RBV | R    | ...         |
| Set PID frequency                    | :PID:FreqX                                      | W    | W           |
| ...                                  | :PID:FreqY                                      | W    | W           |
| ...                                  | :PID:FreqI0                                     | W    | W           |
| Get PID frequency                    | :PID:FreqX_RBV                                  | R    | ...         |
| ...                                  | :PID:FreqY_RBV                                  | R    | ...         |
| ...                                  | :PID:FreqI0_RBV                                 | R    | ...         |


| TetrAMM functions            | EPICS commands                              | FPGA | CONFIG_FILE |
| ---------------------------- | ------------------------------------------- | ---- | ----------- |
| Set TetrAMM range            | :TetrAMM0:Range, :TetrAMM1:Range            | W    | ...         |
| Read TetrAMM currents        | :TetrAMM0:Ch1, :TetrAMM0:Ch2, :TetrAMM0:Ch3 | R    | ...         |
| ...                          | :TetrAMM0:Ch4, :TetrAMM1:Ch1, :TetrAMM1:Ch2 | ...  | ...         |
| ...                          | :TetrAMM1:Ch3, :TetrAMM1:Ch4                | ...  | ...         |
| Read Number of BPM connected | :NumberTetrAMM                              | R    | ...         |
| Set TetrAMM HV voltage       | :TetrAMM0:HVvoltage, :TetrAMM1:HVvoltage    | W    | ...         |
| Enable/disable TetrAMM HV    | :TetrAMM0:HVenable, :TetrAMM1:HVenable      | W    | ...         |


| PreDAC functions          | EPICS commands                           | FPGA | CONFIG |
| ------------------------- | ---------------------------------------- | ---- | ------ |
| Set DAC output MUX        | :PreDAC0:OutMux                          | W    | ...    |
| Get DAC output MUX        | :PreDAC0:OutMux_RBV                      | R    | ...    |
| Set PreDAC output voltage | :PreDAC0:OutCh1, :PreDAC0:OutCh2         | W    | ...    |
| ...                       | :PreDAC0:OutCh3, :PreDAC0:OutCh4         | ...  | ...    |
| Get PreDAC output voltage | :PreDAC0:OutCh1_RBV, :PreDAC0:OutCh2_RBV | R    | ...    |
| ...                       | :PreDAC0:OutCh3_RBV, :PreDAC0:OutCh4_RBV | ...  | ...    |


| Central Unit            | EPICS commands                                 | FPGA | CONFIG_FILE |
| ----------------------- | ---------------------------------------------- | ---- | ----------- |
| Read Positions          | :BPM0:PosX, :BPM0:PosY, :BPM1:PosX, :BPM1:PosY | R    | ...         |
| Read Intensity          | :BPM0:Int,  :BPM1:Int                          | R    | ...         |
| Login                   | :Login:UserPass                                | W    | ...         |
| Check login level       | :Login:Level                                   | R    | ...         |
| Load config in firmware | :BEST:Init                                     | W    | ...         |


| BPM Configuration              | EPICS commands                         | FPGA | CONFIG_FILE |
| ------------------------------ | -------------------------------------- | ---- | ----------- |
| Set BPM Selector configuration | :BPM:Selector                          | W    | ...         |
| Get BPM Selector configuration | :BPM:Selector_RBV                      | R    | ...         |
| Set BPM Orientation            | :BPM0:Orient, :BPM1:Orient             | W    | W           |
| Get BPM Orientation            | :BPM0:Orient_RBV, :BPM1:Orient_RBV     | R    | ...         |
| Set BPM Crossbar               | :BPM0:Crossbar, :BPM1:Crossbar         | W    | W           |
| Get BPM Crossbar               | :BPM0:Crossbar_RBV, :BPM1:Crossbar_RBV | R    | ...         |
| Set BPM Scaling                | :BPM0:ScaleX, :BPM1:ScaleX             | W    | W           |
| ...                            | :BPM0:ScaleY, :BPM1:ScaleY             | W    | W           |
| Get BPM Scaling                | :BPM0:ScaleX_RBV, :BPM1:ScaleX_RBV     | R    | ...         |
| ...                            | :BPM0:ScaleY_RBV, :BPM1:ScaleY_RBV     | R    | ...         |
| Set BPM Offset                 | :BPM0:OffsetX,     :BPM1:OffsetX       | W    | W           |
| ...                            | :BPM0:OffsetY,     :BPM1:OffsetY       | ...  | ...         |
| Get BPM Offset                 | :BPM0:OffsetX_RBV, :BPM1:OffsetX_RBV   | W    | ...         |
| ...                            | :BPM0:OffsetY_RBV, :BPM1:OffsetY_RBV   | ...  | ...         |


| ROC and ROI Configuration                   | EPICS commands                             | FPGA | CONFIG_FILE |
| ------------------------------------------- | ------------------------------------------ | ---- | ----------- |
| Set RocX and RocY                           | :BPM0:RocX, :BPM0:RocY                     | W    | W           |
| ...                                         | :BPM1:RocX, :BPM1:RocY                     | ...  | ...         |
| Get RocX and RocY                           | :BPM0:RocX_RBV, :BPM0:RocY_RBV             | R    | ...         |
| ...                                         | :BPM1:RocX_RBV, :BPM1:RocY_RBV             | ...  | ...         |
| Set Beam Off Threshold                      | :BPM0:BeamOffTh, :BPM1:BeamOffTh           | W    | W           |
| Get Beam Off Threshold                      | :BPM0:BeamOffTh_RBV, :BPM1:BeamOffTh_RBV   | R    | ...         |
| Enable/Disable Roc Check                    | :BPM0:ENArocX, :BPM0:ENArocY               | W    | W           |
| ...                                         | :BPM1:ENArocX, :BPM1:ENArocY               | ...  | ...         |
| Enable/Disable Beam Off Check               | :BPM0:ENAbeamOff, :BPM1:ENAbeamOff         | W    | W           |
| Enable/Disable Roc Check readback           | :BPM0:ENArocX_RBV, :BPM0:ENArocY_RBV       | R    | ...         |
| ...                                         | :BPM1:ENArocX_RBV, :BPM1:ENArocY_RBV       | ...  | ...         |
| Enable/Disable Beam Off Check readback      | :BPM0:ENAbeamOff_RBV, :BPM1:ENAbeamOff_RBV | R    | ...         |
| Set Roi Positions (backward compatibility)  | :PID:RoiX, :PID:RoiY                       | ...  | R           |
| Set Roi Intensity (backward compatibility)  | :PID:RoiIntMin, :PID:RoiIntMax             | ...  | R           |
| Set Roc Percentage (backward compatibility) | :PID:Roc                                   | ...  | R           |
