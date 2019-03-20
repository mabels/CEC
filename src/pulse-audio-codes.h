typedef enum cec_adapter_messagecode
  {
    MSGCODE_NOTHING = 0,
    MSGCODE_PING,
    MSGCODE_TIMEOUT_ERROR,
    MSGCODE_HIGH_ERROR,
    MSGCODE_LOW_ERROR,
    MSGCODE_FRAME_START,
    MSGCODE_FRAME_DATA,
    MSGCODE_RECEIVE_FAILED,
    MSGCODE_COMMAND_ACCEPTED,
    MSGCODE_COMMAND_REJECTED,
    MSGCODE_SET_ACK_MASK,
    MSGCODE_TRANSMIT,
    MSGCODE_TRANSMIT_EOM,
    MSGCODE_TRANSMIT_IDLETIME,
    MSGCODE_TRANSMIT_ACK_POLARITY,
    MSGCODE_TRANSMIT_LINE_TIMEOUT,
    MSGCODE_TRANSMIT_SUCCEEDED,
    MSGCODE_TRANSMIT_FAILED_LINE,
    MSGCODE_TRANSMIT_FAILED_ACK,
    MSGCODE_TRANSMIT_FAILED_TIMEOUT_DATA,
    MSGCODE_TRANSMIT_FAILED_TIMEOUT_LINE,
    MSGCODE_FIRMWARE_VERSION,
    MSGCODE_START_BOOTLOADER,
    MSGCODE_GET_BUILDDATE,
    MSGCODE_SET_CONTROLLED,
    MSGCODE_GET_AUTO_ENABLED,
    MSGCODE_SET_AUTO_ENABLED,
    MSGCODE_GET_DEFAULT_LOGICAL_ADDRESS,
    MSGCODE_SET_DEFAULT_LOGICAL_ADDRESS,
    MSGCODE_GET_LOGICAL_ADDRESS_MASK,
    MSGCODE_SET_LOGICAL_ADDRESS_MASK,
    MSGCODE_GET_PHYSICAL_ADDRESS,
    MSGCODE_SET_PHYSICAL_ADDRESS,
    MSGCODE_GET_DEVICE_TYPE,
    MSGCODE_SET_DEVICE_TYPE,
    MSGCODE_GET_HDMI_VERSION,
    MSGCODE_SET_HDMI_VERSION,
    MSGCODE_GET_OSD_NAME,
    MSGCODE_SET_OSD_NAME,
    MSGCODE_WRITE_EEPROM,
    MSGCODE_GET_ADAPTER_TYPE,
    MSGCODE_SET_ACTIVE_SOURCE,

    MSGCODE_FRAME_EOM = 0x80,
    MSGCODE_FRAME_ACK = 0x40,
  } cec_adapter_messagecode;