#pragma once
#include "GenTL.h"

namespace GenTL
{
	/* This enumeration defines commands to retrieve information with the GenICam::TL::Client::DevGetInfo function on a device handle. */
	enum DEVICE_INFO_CMD_CUSTOM_LIST
	{
		//DEVICE_INFO_CUSTOM_ID = 1000				/* Starting value for GenTL Producer custom IDs. */
		DEVICE_INFO_ENDIANNESS_MECHANISM = 1001,	 /* INT64	Endianess Mechanism Device should use. */
		DEVICE_INFO_GEV_IP_ADDRESS = 1002,			 /* UINT32	IP address of the device. */
		DEVICE_INFO_GEV_SUBNET_MASK = 1003,			 /* UINT32	Subnet mask of the device. */
		DEVICE_INFO_GEV_MAC_ADDRESS = 1004,			 /* INT64	MAC address of the device. */
		DEVICE_INFO_GEV_DEFAULT_GATEWAY = 1005,		 /* UINT32	Default Gateway of the device. */
		DEVICE_INFO_GEV_CONFIG_DHCP = 1006,			 /* BOOL	Is DHCP ip configuration on. */
		DEVICE_INFO_GEV_CONFIG_PERSISTENT_IP = 1007, /* BOOL	IS Persistent Ip configuration on. */
		DEVICE_INFO_GEV_CONFIG_LLA = 1008,			 /* BOOL	IS LLA configuration on. */
		DEVICE_INFO_IS_CONNECTED = 1009				 /* BOOL	Is a device connected and can this GenTL module communicate with it. */
	};

	/* This enumeration defines special start flags for the acquisition engine. The function used is GenICam::TL::Client::DSStartAcquisition. */
	enum ACQ_START_FLAGS_CUSTOM_LIST
	{
		ACQ_START_FLAGS_GVSP_LEGACY_ID = 1001 /* Start GVSP 2.0 stream forcing legacy mode if possible. */
	};

	/* This enumeration defines commands to retrieve information with the GenICam::TL::Client::GetInfo function on a stream handle. */
	enum STREAM_INFO_CMD_CUSTOM_LIST
	{
		//STREAM_INFO_CUSTOM_ID = 1000				/* Starting value for GenTL Producer custom IDs. */
		STREAM_INFO_BUFFER_HANDLING_MODE = 1001,		    /* INT64	Buffer Handling Mode. */
		STREAM_INFO_MISSED_IMAGE_COUNT = 1002,			    /* UINT64	Number of missed images beginning from stream start. */
		STREAM_INFO_MISSED_PACKET_COUNT = 1003,			    /* UINT64	Number of missed packets beginning from stream start. */
		STREAM_INFO_TOTAL_MISSED_IMAGE_COUNT = 1004,	    /* UINT64   Number of missed images, not resetting on each stream start. */
		STREAM_INFO_TOTAL_INCOMPLETE_IMAGE_COUNT = 1005,    /* UINT64   Number of incomplete images, not resetting on each stream start. */
        STREAM_INFO_PACKET_RESEND_ENABLE = 1006,            /* BOOL     Is packet resend enabled for this stream. */
        STREAM_INFO_IMAGE_BUFFER_EXPIRY_MS = 1007,          /* UINT32   Image buffer will be completed when timeout expires. */
		STREAM_INFO_AUTO_PACKET_SIZE_ENABLE = 1008,		 /* BOOL     Enable/Disable auto packet size negotiation. */
		STREAM_INFO_INTER_PACKET_EXPIRY_MS = 1009,		 /* UINT32   A packet resend request will be fired when timeout expires. */
		STREAM_INFO_MAX_RESEND_REQUESTS = 1010			 /* UINT32   Max number of resend request packets allowed per image */
	};

	/* This enumeration defines commands to retrieve additional information about the interface handle.*/
	enum INTERFACE_INFO_CMD_CUSTOM_LIST
	{
		//INTERFACE_INFO_CUSTOM_ID = 1000				/* Starting value for GenTL Producer custom IDs. */
		INTERFACE_INFO_GEV_MAC_ADDRESS = 1001,		 /* INT64	MAC address of the interface. */
		INTERFACE_INFO_GEV_SUBNET_IP_ADDRESS = 1002, /* INT64	Subnet IP address of the interface. */
		INTERFACE_INFO_GEV_SUBNET_MASK = 1003		 /* INT64	Subnet mask of the interface. */
	};

	/* This enumeration defines commands to retrieve information with the GenICam::TL::Client::GetInfo function on a stream handle. */
	enum PAYLOADTYPE_INFO_CSTOM_IDS
	{
		//STREAM_INFO_CUSTOM_ID = 1000				/* Starting value for GenTL Producer custom IDs. */
		PAYLOAD_TYPE_IMAGE_EXTENDED_CHUNK = 0x4001 /* Buffer Handling Mode. */
	};

	enum GC_ERROR_CUSTOM_LIST
	{
		GC_ERR_MISMATCHED_MSG = GC_ERR_CUSTOM_ID + 1,
		GC_ERR_MISMATCHED_CMD = GC_ERR_CUSTOM_ID + 2,
	};

	//custom functions extending basic gentl implementation
	GC_API IFForceIp(IF_HANDLE hIface, uint64_t mac, uint32_t ip, uint32_t subnet, uint32_t gateway);
} // namespace GenTL
