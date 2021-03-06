/*

  Linux Driver for BusLogic MultiMaster SCSI Host Adapters

  Copyright 1995 by Leonard N. Zubkoff <lnz@dandelion.com>

  See BusLogic.c for licensing information.

*/


/*
  Define types for some of the structures that interface with the rest
  of the Linux Kernel and SCSI Subsystem.
*/

typedef struct pt_regs Registers_T;
typedef Scsi_Host_Template SCSI_Host_Template_T;
typedef struct Scsi_Host SCSI_Host_T;
typedef struct scsi_disk SCSI_Disk_T;
typedef struct scsi_cmnd SCSI_Command_T;
typedef struct scatterlist SCSI_ScatterList_T;
typedef kdev_t KernelDevice_T;


/*
  Define prototypes for the BusLogic Driver Interface Functions.
*/

const char *BusLogic_DriverInfo(SCSI_Host_T *);
int BusLogic_DetectHostAdapter(SCSI_Host_Template_T *);
int BusLogic_ReleaseHostAdapter(SCSI_Host_T *);
int BusLogic_QueueCommand(SCSI_Command_T *,
			  void (*CompletionRoutine)(SCSI_Command_T *));
int BusLogic_AbortCommand(SCSI_Command_T *);
int BusLogic_ResetCommand(SCSI_Command_T *);
int BusLogic_BIOSDiskParameters(SCSI_Disk_T *, KernelDevice_T, int *);


/*
  Define the BusLogic SCSI Host Template structure.
*/

#define BUSLOGIC							 \
  { NULL,				/* Next			     */  \
    NULL,				/* Usage Count Pointer	     */  \
    NULL,				/* /proc Directory Entry     */	 \
    NULL,				/* /proc Info Function	     */	 \
    "BusLogic",				/* Driver Name		     */  \
    BusLogic_DetectHostAdapter,		/* Detect Host Adapter	     */  \
    BusLogic_ReleaseHostAdapter,	/* Release Host Adapter	     */  \
    BusLogic_DriverInfo,		/* Driver Info Function	     */  \
    NULL,				/* Command Function	     */  \
    BusLogic_QueueCommand,		/* Queue Command Function    */  \
    BusLogic_AbortCommand,		/* Abort Command Function    */  \
    BusLogic_ResetCommand,		/* Reset Command Function    */  \
    NULL,				/* Slave Attach Function     */  \
    BusLogic_BIOSDiskParameters,	/* Disk BIOS Parameters	     */  \
    0,					/* Can Queue		     */  \
    0,					/* This ID		     */  \
    0,					/* Scatter/Gather Table Size */  \
    0,					/* SCSI Commands per LUN     */  \
    0,					/* Present		     */  \
    1,					/* Default Unchecked ISA DMA */  \
    ENABLE_CLUSTERING }			/* Enable Clustering	     */


/*
  BusLogic_DriverVersion protects the private portion of this file.
*/

#ifdef BusLogic_DriverVersion


/*
  Define the maximum number of BusLogic Host Adapters that are supported.
*/

#define BusLogic_MaxHostAdapters		10


/*
  Define the maximum number of I/O Addresses that may be probed.
*/

#define BusLogic_IO_MaxProbeAddresses		16


/*
  Define the maximum number of Target IDs supported by this driver.
*/

#define BusLogic_MaxTargetIDs			16


/*
  Define the number of Incoming and Outgoing Mailboxes used by this driver.
  The maximum possible value is 255, since the MailboxCount parameter to the
  Initialize Extended Mailbox command is limited to a single byte.
*/

#define BusLogic_MailboxCount			64


/*
  Define the number of Command Control Blocks (CCBs) to create during
  initialization for each Host Adapter.  Additional CCBs will be allocated
  if necessary as commands are queued.
*/

#define BusLogic_InitialCCBs			32


/*
  Define the maximum number of Scatter/Gather Segments used by this driver.
  For maximum performance, it is important that this limit be at least as
  large as the maximum single request generated by the routine make_request.
*/

#define BusLogic_ScatterGatherLimit		128


/*
  Define the default number of Concurrent Commands per Logical Unit to allow
  for Target Devices depending on whether or not ISA bounce buffers are
  required.
*/

#define BusLogic_Concurrency			7
#define BusLogic_Concurrency_BB			1


/*
  Define the default amount of time in seconds to wait between a Host Adapter
  Hard Reset which initiates a SCSI Bus Reset and issuing any SCSI commands.
  Some SCSI devices get confused if they receive SCSI commands too soon after
  a SCSI Bus Reset.
*/

#define BusLogic_DefaultBusSettleTime		2


/*
  Define the possible Local Options.
*/

#define BusLogic_InhibitTargetInquiry		1


/*
  Define the possible Global Options.
*/

#define BusLogic_TraceProbe			1
#define BusLogic_TraceHardReset			2
#define BusLogic_TraceConfiguration		4
#define BusLogic_TraceErrors			8


/*
  Define the possible Error Recovery Options.
*/

#define BusLogic_ErrorRecoveryDefault		0
#define BusLogic_ErrorRecoveryHardReset		1
#define BusLogic_ErrorRecoveryBusDeviceReset	2
#define BusLogic_ErrorRecoveryNone		3

static char
  *BusLogic_ErrorRecoveryOptions[] =
    { "Default", "Hard Reset", "Bus Device Reset", "None" },
  *BusLogic_ErrorRecoveryOptions2[] =
    { "D", "H", "B", "N" };


/*
  Define a boolean data type.
*/

#define false 0
#define true  1
typedef unsigned char boolean;


/*
  Define the BusLogic SCSI Host Adapter I/O Register Offsets.
*/

#define BusLogic_IO_PortCount			4	/* I/O Registers */
#define BusLogic_ControlRegister		0	/* WO register */
#define BusLogic_StatusRegister			0	/* RO register */
#define BusLogic_CommandParameterRegister	1	/* WO register */
#define BusLogic_DataInRegister			1	/* RO register */
#define BusLogic_InterruptRegister		2	/* RO register */
#define BusLogic_GeometryRegister		3	/* RO, undocumented */


/*
  Define the bits in the write-only Control Register.
*/

#define BusLogic_ReservedCR			0x0F
#define BusLogic_SCSIBusReset			0x10
#define BusLogic_InterruptReset			0x20
#define BusLogic_SoftReset			0x40
#define BusLogic_HardReset			0x80


/*
  Define the bits in the read-only Status Register.
*/

#define BusLogic_CommandInvalid			0x01
#define BusLogic_ReservedSR			0x02
#define BusLogic_DataInRegisterReady		0x04
#define BusLogic_CommandParameterRegisterBusy	0x08
#define BusLogic_HostAdapterReady		0x10
#define BusLogic_InitializationRequired		0x20
#define BusLogic_DiagnosticFailure		0x40
#define BusLogic_DiagnosticActive		0x80


/*
  Define the bits in the read-only Interrupt Register.
*/

#define BusLogic_IncomingMailboxLoaded		0x01
#define BusLogic_OutgoingMailboxAvailable	0x02
#define BusLogic_CommandComplete		0x04
#define BusLogic_SCSIResetState			0x08
#define BusLogic_ReservedIR			0x70
#define BusLogic_InterruptValid			0x80


/*
  Define the bits in the undocumented read-only Geometry Register.
*/

#define BusLogic_Drive0Geometry			0x03
#define BusLogic_Drive1Geometry			0x0C
#define BusLogic_ReservedGR			0x70
#define BusLogic_ExtendedTranslationEnabled	0x80


/*
  Define the BusLogic SCSI Host Adapter Command Register Operation Codes.
*/

typedef enum
{
  BusLogic_TestCommandCompleteInterrupt =	0x00,	/* documented */
  BusLogic_InitializeMailbox =			0x01,	/* documented */
  BusLogic_StartMailboxCommand =		0x02,	/* documented */
  BusLogic_StartBIOSCommand =			0x03,	/* documented */
  BusLogic_InquireBoardID =			0x04,	/* documented */
  BusLogic_EnableOutgoingMailboxAvailableIRQ =	0x05,	/* documented */
  BusLogic_SetSCSISelectionTimeout =		0x06,	/* documented */
  BusLogic_SetPreemptTimeOnBus =		0x07,	/* documented */
  BusLogic_SetTimeOffBus =			0x08,	/* ISA Bus only */
  BusLogic_SetBusTransferRate =			0x09,	/* ISA Bus only */
  BusLogic_InquireInstalledDevicesID0to7 =	0x0A,	/* documented */
  BusLogic_InquireConfiguration =		0x0B,	/* documented */
  BusLogic_SetTargetMode =			0x0C,	/* now undocumented */
  BusLogic_InquireSetupInformation =		0x0D,	/* documented */
  BusLogic_WriteAdapterLocalRAM =		0x1A,	/* documented */
  BusLogic_ReadAdapterLocalRAM =		0x1B,	/* documented */
  BusLogic_WriteBusMasterChipFIFO =		0x1C,	/* documented */
  BusLogic_ReadBusMasterChipFIFO =		0x1D,	/* documented */
  BusLogic_EchoCommandData =			0x1F,	/* documented */
  BusLogic_HostAdapterDiagnostic =		0x20,	/* documented */
  BusLogic_SetAdapterOptions =			0x21,	/* documented */
  BusLogic_InquireInstalledDevicesID8to15 =	0x23,	/* Wide only */
  BusLogic_InitializeExtendedMailbox =		0x81,	/* documented */
  BusLogic_InquireFirmwareVersion3rdDigit =	0x84,	/* undocumented */
  BusLogic_InquireFirmwareVersionLetter =	0x85,	/* undocumented */
  BusLogic_InquireBoardModelNumber	 =	0x8B,	/* undocumented */
  BusLogic_InquireSynchronousPeriod =		0x8C,	/* undocumented */
  BusLogic_InquireExtendedSetupInformation =	0x8D,	/* documented */
  BusLogic_EnableStrictRoundRobinMode =		0x8F,	/* documented */
  BusLogic_ModifyIOAddress =			0x95,	/* PCI only */
  BusLogic_EnableWideModeCCB =			0x96	/* Wide only */
}
BusLogic_OperationCode_T;


/*
  Define the Inquire Board ID reply structure.
*/

typedef struct BusLogic_BoardID
{
  unsigned char BoardType;
  unsigned char CustomFeatures;
  unsigned char FirmwareVersion1stDigit;
  unsigned char FirmwareVersion2ndDigit;
}
BusLogic_BoardID_T;


/*
  Define the Inquire Installed Devices ID 0 to 7 and Inquire Installed
  Devices ID 8 to 15 reply type.  For each Target ID, a byte is returned
  where bit 0 set indicates that Logical Unit 0 exists, bit 1 set indicates
  that Logical Unit 1 exists, and so on.
*/

typedef unsigned char BusLogic_InstalledDevices8_T[8];

typedef unsigned char BusLogic_InstalledDevices_T[BusLogic_MaxTargetIDs];


/*
  Define the Inquire Configuration reply structure.
*/

typedef struct BusLogic_Configuration
{
  unsigned char :5;				/* Byte 0: DMA Channel */
  boolean DMA_Channel5:1;
  boolean DMA_Channel6:1;
  boolean DMA_Channel7:1;
  boolean IRQ_Channel9:1;			/* Byte 1: IRQ Channel */
  boolean IRQ_Channel10:1;
  boolean IRQ_Channel11:1;
  boolean IRQ_Channel12:1;
  unsigned char :1;
  boolean IRQ_Channel14:1;
  boolean IRQ_Channel15:1;
  unsigned char :1;
  unsigned char HostAdapterID:4;		/* Byte 2: Host Adapter ID */
  unsigned char :4;
}
BusLogic_Configuration_T;


/*
  Define the Inquire Setup Information reply structure.
*/

typedef struct BusLogic_SynchronousValue
{
  unsigned char Offset:4;
  unsigned char TransferPeriod:3;
  boolean Synchronous:1;
}
BusLogic_SynchronousValue_T;

typedef BusLogic_SynchronousValue_T
  BusLogic_SynchronousValues8_T[8];

typedef BusLogic_SynchronousValue_T
  BusLogic_SynchronousValues_T[BusLogic_MaxTargetIDs];

typedef struct BusLogic_SetupInformation
{
  boolean SynchronousInitiationEnabled:1;	/* Byte 0 */
  boolean ParityCheckEnabled:1;
  unsigned char :6;
  unsigned char BusTransferRate;		/* Byte 1 */
  unsigned char PreemptTimeOnBus;		/* Byte 2 */
  unsigned char TimeOffBus;			/* Byte 3 */
  unsigned char MailboxCount;			/* Byte 4 */
  unsigned char MailboxAddress[3];		/* Bytes 5-7 */
  BusLogic_SynchronousValues8_T SynchronousValuesID0to7; /* Bytes 8-15 */
  unsigned char DisconnectPermittedID0to7;	/* Byte 16 */
  unsigned char Signature;			/* Byte 17 */
  unsigned char CharacterD;			/* Byte 18 */
  unsigned char BusLetter;			/* Byte 19 */
  unsigned char :8;				/* Byte 20 */
  unsigned char :8;				/* Byte 21 */
  BusLogic_SynchronousValues8_T SynchronousValuesID8to15; /* Bytes 22-29 */
  unsigned char DisconnectPermittedID8to15;	/* Byte 30 */
}
BusLogic_SetupInformation_T;


/*
  Define the Initialize Extended Mailbox request structure.
*/

typedef struct BusLogic_ExtendedMailboxRequest
{
  unsigned char MailboxCount;
  void *BaseMailboxAddress __attribute__ ((packed));
}
BusLogic_ExtendedMailboxRequest_T;


/*
  Define the Inquire Firmware Version 3rd Digit reply type.
*/

typedef unsigned char BusLogic_FirmwareVersion3rdDigit_T;


/*
  Define the Inquire Firmware Version Letter reply type.
*/

typedef unsigned char BusLogic_FirmwareVersionLetter_T;


/*
  Define the Inquire Board Model Number reply type.
*/

typedef unsigned char BusLogic_BoardModelNumber_T[5];


/*
  Define the Inquire Synchronous Period reply type.  For each Target ID, a byte
  is returned which represents the Synchronous Transfer Period in units of 10
  nanoseconds.
*/

typedef unsigned char BusLogic_SynchronousPeriod_T[BusLogic_MaxTargetIDs];


/*
  Define the Inquire Extended Setup Information reply structure.
*/

typedef struct BusLogic_ExtendedSetupInformation
{
  unsigned char BusType;				/* Byte 0 */
  unsigned char BIOS_Address;				/* Byte 1 */
  unsigned short ScatterGatherLimit;			/* Bytes 2-3 */
  unsigned char MailboxCount;				/* Byte 4 */
  void *BaseMailboxAddress __attribute__ ((packed));	/* Bytes 5-8 */
  struct { unsigned char :6;				/* Byte 9 */
	   boolean LevelSensitiveInterrupts:1;
	   unsigned char :1; } Misc;
  unsigned char FirmwareRevision[3];			/* Bytes 10-12 */
  boolean HostWideSCSI:1;				/* Byte 13 Bit 0 */
  boolean HostDifferentialSCSI:1;			/* Byte 13 Bit 1 */
  unsigned char :6;
}
BusLogic_ExtendedSetupInformation_T;


/*
  Define the Enable Strict Round Robin Mode request type.
*/

#define BusLogic_AggressiveRoundRobinMode	0x00
#define BusLogic_StrictRoundRobinMode		0x01

typedef unsigned char BusLogic_RoundRobinModeRequest_T;


/*
  Define the Modify I/O Address request type.  On PCI Host Adapters, the
  Modify I/O Address command allows modification of the ISA compatible I/O
  Address that the Host Adapter responds to; it does not affect the PCI
  compliant I/O Address assigned at system initialization.
*/

#define BusLogic_ModifyIO_330			0x00
#define BusLogic_ModifyIO_334			0x01
#define BusLogic_ModifyIO_230			0x02
#define BusLogic_ModifyIO_234			0x03
#define BusLogic_ModifyIO_130			0x04
#define BusLogic_ModifyIO_134			0x05
#define BusLogic_ModifyIO_Disable		0x06
#define BusLogic_ModifyIO_Disable2		0x07

typedef unsigned char BusLogic_ModifyIOAddressRequest_T;


/*
  Define the Enable Wide Mode SCSI CCB request type.  Wide Mode CCBs are
  necessary to support more than 8 Logical Units per Target.
*/

#define BusLogic_NormalModeCCB			0x00
#define BusLogic_WideModeCCB			0x01

typedef unsigned char BusLogic_WideModeCCBRequest_T;


/*
  Define the Requested Reply Length type used by the Inquire Setup Information,
  Inquire Board Model Number, Inquire Synchronous Period, and Inquire Extended
  Setup Information commands.
*/

typedef unsigned char BusLogic_RequestedReplyLength_T;


/*
  Define a Lock data structure.  Until a true symmetric multiprocessing kernel
  is available, locking is implemented as saving the processor flags and
  disabling interrupts, and unlocking restores the saved processor flags.
*/

typedef unsigned long BusLogic_Lock_T;


/*
  Define the Outgoing Mailbox Action Codes.
*/

typedef enum
{
  BusLogic_OutgoingMailboxFree =		0,
  BusLogic_MailboxStartCommand =		1,
  BusLogic_MailboxAbortCommand =		2
}
BusLogic_ActionCode_T;


/*
  Define the Incoming Mailbox Completion Codes.
*/

typedef enum
{
  BusLogic_IncomingMailboxFree =		0,
  BusLogic_CommandCompletedWithoutError =	1,
  BusLogic_CommandAbortedAtHostRequest =	2,
  BusLogic_AbortedCommandNotFound =		3,
  BusLogic_CommandCompletedWithError =		4
}
BusLogic_CompletionCode_T;


/*
  Define the Command Control Block (CCB) Opcodes.
*/

typedef enum
{
  BusLogic_InitiatorCCB =			0x00,
  BusLogic_TargetCCB =				0x01,
  BusLogic_InitiatorCCB_ScatterGather =		0x02,
  BusLogic_InitiatorCCB_ResidualDataLength =	0x03,
  BusLogic_InitiatorCCB_ScatterGatherResidual =	0x04,
  BusLogic_SCSIBusDeviceReset =			0x81
}
BusLogic_CCB_Opcode_T;


/*
  Define the CCB Data Direction Codes.
*/

typedef enum
{
  BusLogic_UncheckedDataTransfer =		0x00,
  BusLogic_DataInLengthChecked =		0x01,
  BusLogic_DataOutLengthChecked =		0x02,
  BusLogic_NoDataTransfer =			0x03
}
BusLogic_DataDirection_T;


/*
  Define the Host Adapter Status Codes.
*/

typedef enum
{
  BusLogic_CommandCompletedNormally =		0x00,
  BusLogic_LinkedCommandCompleted =		0x0A,
  BusLogic_LinkedCommandCompletedWithFlag =	0x0B,
  BusLogic_SCSISelectionTimeout =		0x11,
  BusLogic_DataOverUnderRun =			0x12,
  BusLogic_UnexpectedBusFree =			0x13,
  BusLogic_InvalidBusPhaseRequested =		0x14,
  BusLogic_InvalidOutgoingMailboxActionCode =	0x15,
  BusLogic_InvalidCommandOperationCode =	0x16,
  BusLogic_LinkedCCBhasInvalidLUN =		0x17,
  BusLogic_InvalidCommandParameter =		0x1A,
  BusLogic_AutoRequestSenseFailed =		0x1B,
  BusLogic_TaggedQueuingMessageRejected =	0x1C,
  BusLogic_UnsupportedMessageReceived =		0x1D,
  BusLogic_HostAdapterHardwareFailed =		0x20,
  BusLogic_TargetFailedResponseToATN =		0x21,
  BusLogic_HostAdapterAssertedRST =		0x22,
  BusLogic_OtherDeviceAssertedRST =		0x23,
  BusLogic_TargetDeviceReconnectedImproperly =	0x24,
  BusLogic_HostAdapterAssertedBusDeviceReset =	0x25,
  BusLogic_AbortQueueGenerated =		0x26,
  BusLogic_HostAdapterSoftwareError =		0x27,
  BusLogic_HostAdapterHardwareTimeoutError =	0x30,
  BusLogic_SCSIParityErrorDetected =		0x34
}
BusLogic_HostAdapterStatus_T;


/*
  Define the SCSI Target Device Status Codes.
*/

typedef enum
{
  BusLogic_OperationGood =			0x00,
  BusLogic_CheckCondition =			0x02,
  BusLogic_DeviceBusy =				0x08
}
BusLogic_TargetDeviceStatus_T;


/*
  Define the Queue Tag Codes.
*/

typedef enum
{
  BusLogic_SimpleQueueTag =			0x00,
  BusLogic_HeadOfQueueTag =			0x01,
  BusLogic_OrderedQueueTag =			0x02,
  BusLogic_ReservedQT =				0x03
}
BusLogic_QueueTag_T;


/*
  Define the SCSI Command Descriptor Block (CDB).
*/

#define BusLogic_CDB_MaxLength			12

typedef unsigned char SCSI_CDB_T[BusLogic_CDB_MaxLength];


/*
  Define the SCSI Sense Data.
*/

#define BusLogic_SenseDataMaxLength		255

typedef unsigned char SCSI_SenseData_T[BusLogic_SenseDataMaxLength];


/*
  Define the Scatter/Gather Segment structure required by the Host Adapter
  Firmware Interface.
*/

typedef struct BusLogic_ScatterGatherSegment
{
  unsigned long SegmentByteCount;
  void *SegmentDataPointer;
}
BusLogic_ScatterGatherSegment_T;


/*
  Define the 32 Bit Mode Command Control Block (CCB) structure.  The first 40
  bytes are defined by the Host Adapter Firmware Interface.  The remaining
  components are defined by the Linux BusLogic Driver.  Wide Mode CCBs differ
  from standard 32 Bit Mode CCBs only in having the TagEnable and QueueTag
  fields moved from byte 17 to byte 1, and the Logical Unit field in byte 17
  expanded to 6 bits; unfortunately, using a union of structs containing
  enumeration type bitfields to provide both definitions leads to packing
  problems, so the following definition is used which requires setting
  TagEnable to Logical Unit bit 5 in Wide Mode CCBs.
*/

typedef struct BusLogic_CCB
{
  /*
    BusLogic Host Adapter Firmware Portion.
  */
  BusLogic_CCB_Opcode_T Opcode:8;			/* Byte 0 */
  unsigned char :3;					/* Byte 1 Bits 0-2 */
  BusLogic_DataDirection_T DataDirection:2;		/* Byte 1 Bits 3-4 */
  boolean WideModeTagEnable:1;				/* Byte 1 Bit 5 */
  BusLogic_QueueTag_T WideModeQueueTag:2;		/* Byte 1 Bits 6-7 */
  unsigned char CDB_Length;				/* Byte 2 */
  unsigned char SenseDataLength;			/* Byte 3 */
  unsigned long DataLength;				/* Bytes 4-7 */
  void *DataPointer;					/* Bytes 8-11 */
  unsigned char :8;					/* Byte 12 */
  unsigned char :8;					/* Byte 13 */
  BusLogic_HostAdapterStatus_T HostAdapterStatus:8;	/* Byte 14 */
  BusLogic_TargetDeviceStatus_T TargetDeviceStatus:8;	/* Byte 15 */
  unsigned char TargetID;				/* Byte 16 */
  unsigned char LogicalUnit:5;				/* Byte 17 Bits 0-4 */
  boolean TagEnable:1;					/* Byte 17 Bit 5 */
  BusLogic_QueueTag_T QueueTag:2;			/* Byte 17 Bits 6-7 */
  SCSI_CDB_T CDB;					/* Bytes 18-29 */
  unsigned char :8;					/* Byte 30 */
  unsigned char :8;					/* Byte 31 */
  unsigned long :32;					/* Bytes 32-35 */
  SCSI_SenseData_T *SenseDataPointer;			/* Bytes 36-39 */
  /*
    BusLogic Linux Driver Portion.
  */
  struct BusLogic_HostAdapter *HostAdapter;
  SCSI_Command_T *Command;
  enum { BusLogic_CCB_Free =	    0,
	 BusLogic_CCB_Active =	    1,
	 BusLogic_CCB_Completed =   2,
	 BusLogic_CCB_Reset =	    3 } Status;
  BusLogic_CompletionCode_T MailboxCompletionCode;
  unsigned int SerialNumber;
  struct BusLogic_CCB *Next;
  struct BusLogic_CCB *NextAll;
  BusLogic_ScatterGatherSegment_T
    ScatterGatherList[BusLogic_ScatterGatherLimit];
}
BusLogic_CCB_T;


/*
  Define the 32 Bit Mode Outgoing Mailbox structure.
*/

typedef struct BusLogic_OutgoingMailbox
{
  BusLogic_CCB_T *CCB;
  unsigned long :24;
  BusLogic_ActionCode_T ActionCode:8;
}
BusLogic_OutgoingMailbox_T;


/*
  Define the 32 Bit Mode Incoming Mailbox structure.
*/

typedef struct BusLogic_IncomingMailbox
{
  BusLogic_CCB_T *CCB;
  BusLogic_HostAdapterStatus_T HostAdapterStatus:8;
  BusLogic_TargetDeviceStatus_T TargetDeviceStatus:8;
  unsigned char :8;
  BusLogic_CompletionCode_T CompletionCode:8;
}
BusLogic_IncomingMailbox_T;


/*
  Define the possible Bus Types.
*/

typedef enum
{
  BusLogic_Unknown_Bus =			0,
  BusLogic_ISA_Bus =				1,
  BusLogic_MCA_Bus =				2,
  BusLogic_EISA_Bus =				3,
  BusLogic_VESA_Bus =				4,
  BusLogic_PCI_Bus =				5
}
BusLogic_BusType_T;

static char
  *BusLogic_BusNames[] =
    { "Unknown", "ISA", "MCA", "EISA", "VESA", "PCI" };


/*
  Define the Linux BusLogic Driver Command Line Entry structure.
*/

typedef struct BusLogic_CommandLineEntry
{
  unsigned short IO_Address;
  unsigned short Concurrency;
  unsigned short BusSettleTime;
  unsigned short LocalOptions;
  unsigned short TaggedQueuingPermitted;
  unsigned short TaggedQueuingPermittedMask;
  unsigned char ErrorRecoveryOption[BusLogic_MaxTargetIDs];
}
BusLogic_CommandLineEntry_T;


/*
  Define the Linux BusLogic Driver Host Adapter structure.
*/

typedef struct BusLogic_HostAdapter
{
  SCSI_Host_T *SCSI_Host;
  unsigned char HostNumber;
  unsigned char ModelName[9];
  unsigned char FirmwareVersion[6];
  unsigned char BoardName[18];
  unsigned char InterruptLabel[62];
  unsigned short IO_Address;
  unsigned char IRQ_Channel;
  unsigned char DMA_Channel;
  unsigned char SCSI_ID;
  BusLogic_BusType_T BusType:3;
  boolean IRQ_ChannelAcquired:1;
  boolean DMA_ChannelAcquired:1;
  boolean SynchronousInitiation:1;
  boolean ParityChecking:1;
  boolean ExtendedTranslation:1;
  boolean LevelSensitiveInterrupts:1;
  boolean HostWideSCSI:1;
  boolean HostDifferentialSCSI:1;
  boolean HostAdapterResetPending:1;
  boolean BounceBuffersRequired:1;
  volatile boolean HostAdapterCommandCompleted:1;
  unsigned short HostAdapterScatterGatherLimit;
  unsigned short DriverScatterGatherLimit;
  unsigned short MaxTargetIDs;
  unsigned short MaxLogicalUnits;
  unsigned short Concurrency;
  unsigned short BusSettleTime;
  unsigned short LocalOptions;
  unsigned short DisconnectPermitted;
  unsigned short TaggedQueuingPermitted;
  unsigned long BIOS_Address;
  BusLogic_InstalledDevices_T InstalledDevices;
  BusLogic_SynchronousValues_T SynchronousValues;
  BusLogic_SynchronousPeriod_T SynchronousPeriod;
  BusLogic_Lock_T Lock;
  struct BusLogic_HostAdapter *Next;
  BusLogic_CommandLineEntry_T *CommandLineEntry;
  BusLogic_CCB_T *All_CCBs;
  BusLogic_CCB_T *Free_CCBs;
  unsigned char ErrorRecoveryOption[BusLogic_MaxTargetIDs];
  unsigned char CommandSuccessfulFlag[BusLogic_MaxTargetIDs];
  unsigned long ReadWriteOperationCount[BusLogic_MaxTargetIDs];
  unsigned char QueuedOperationCount[BusLogic_MaxTargetIDs];
  unsigned long LastSequencePoint[BusLogic_MaxTargetIDs];
  BusLogic_OutgoingMailbox_T *FirstOutgoingMailbox;
  BusLogic_OutgoingMailbox_T *LastOutgoingMailbox;
  BusLogic_OutgoingMailbox_T *NextOutgoingMailbox;
  BusLogic_IncomingMailbox_T *FirstIncomingMailbox;
  BusLogic_IncomingMailbox_T *LastIncomingMailbox;
  BusLogic_IncomingMailbox_T *NextIncomingMailbox;
  BusLogic_OutgoingMailbox_T OutgoingMailboxes[BusLogic_MailboxCount];
  BusLogic_IncomingMailbox_T IncomingMailboxes[BusLogic_MailboxCount];
}
BusLogic_HostAdapter_T;


/*
  Define a symbolic structure for the BIOS Disk Parameters.
*/

typedef struct BIOS_DiskParameters
{
  int Heads;
  int Sectors;
  int Cylinders;
}
BIOS_DiskParameters_T;


/*
  BusLogic_LockHostAdapter acquires exclusive access to Host Adapter.
*/

static inline
void BusLogic_LockHostAdapter(BusLogic_HostAdapter_T *HostAdapter)
{
  save_flags(HostAdapter->Lock);
  cli();
}


/*
  BusLogic_UnlockHostAdapter releases exclusive access to Host Adapter.
*/

static inline
void BusLogic_UnlockHostAdapter(BusLogic_HostAdapter_T *HostAdapter)
{
  restore_flags(HostAdapter->Lock);
}


/*
  BusLogic_LockHostAdapterID acquires exclusive access to Host Adapter,
  but is only called when interrupts are disabled.
*/

static inline
void BusLogic_LockHostAdapterID(BusLogic_HostAdapter_T *HostAdapter)
{
}


/*
  BusLogic_UnlockHostAdapterID releases exclusive access to Host Adapter,
  but is only called when interrupts are disabled.
*/

static inline
void BusLogic_UnlockHostAdapterID(BusLogic_HostAdapter_T *HostAdapter)
{
}


/*
  Define functions to provide an abstraction for reading and writing the
  Host Adapter I/O Registers.
*/

static inline
void BusLogic_WriteControlRegister(BusLogic_HostAdapter_T *HostAdapter,
				   unsigned char Value)
{
  outb(Value, HostAdapter->IO_Address + BusLogic_ControlRegister);
}

static inline
unsigned char BusLogic_ReadStatusRegister(BusLogic_HostAdapter_T *HostAdapter)
{
  return inb(HostAdapter->IO_Address + BusLogic_StatusRegister);
}

static inline
void BusLogic_WriteCommandParameterRegister(BusLogic_HostAdapter_T *HostAdapter,
					    unsigned char Value)
{
  outb(Value, HostAdapter->IO_Address + BusLogic_CommandParameterRegister);
}

static inline
unsigned char BusLogic_ReadDataInRegister(BusLogic_HostAdapter_T *HostAdapter)
{
  return inb(HostAdapter->IO_Address + BusLogic_DataInRegister);
}

static inline
unsigned char BusLogic_ReadInterruptRegister(BusLogic_HostAdapter_T
					     *HostAdapter)
{
  return inb(HostAdapter->IO_Address + BusLogic_InterruptRegister);
}

static inline
unsigned char BusLogic_ReadGeometryRegister(BusLogic_HostAdapter_T *HostAdapter)
{
  return inb(HostAdapter->IO_Address + BusLogic_GeometryRegister);
}


/*
  BusLogic_StartMailboxScan issues a Start Mailbox Scan command, which
  notifies the Host Adapter that an entry has been made in an Outgoing
  Mailbox.
*/

static inline
void BusLogic_StartMailboxScan(BusLogic_HostAdapter_T *HostAdapter)
{
  BusLogic_WriteCommandParameterRegister(HostAdapter,
					 BusLogic_StartMailboxCommand);
}


/*
  BusLogic_Delay waits for Seconds to elapse.
*/

static inline void BusLogic_Delay(int Seconds)
{
  unsigned long TimeoutJiffies = jiffies + Seconds * HZ;
  unsigned long ProcessorFlags;
  save_flags(ProcessorFlags);
  sti();
  while (jiffies < TimeoutJiffies) ;
  restore_flags(ProcessorFlags);
}


/*
  Define prototypes for the forward referenced BusLogic Driver
  Internal Functions.
*/

static void BusLogic_InterruptHandler(int, Registers_T *);
static int BusLogic_ResetHostAdapter(BusLogic_HostAdapter_T *,
				     SCSI_Command_T *);


#endif /* BusLogic_DriverVersion */
