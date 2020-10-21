/***************************************************************************************
 ***                                                                                 ***
 ***  Copyright (c) 2019, Lucid Vision Labs, Inc.                                    ***
 ***                                                                                 ***
 ***  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     ***
 ***  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       ***
 ***  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    ***
 ***  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         ***
 ***  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ***
 ***  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  ***
 ***  SOFTWARE.                                                                      ***
 ***                                                                                 ***
 ***************************************************************************************/
#pragma once

#include <stdint.h>

//define infinite macro
#define AC_INFINITE  0xFFFFFFFFFFFFFFFFULL  /* Infinite value to be used in various function calls.*/

#if (defined _WIN32 || defined _WIN64)
#ifdef AC_EXPORTS
#define AC_API __declspec(dllexport)
#else
#define AC_API __declspec(dllimport)
#endif // AC_EXPORTS
#else
#define AC_API __attribute__((visibility("default")))
//		#define AC_API
#endif // _WIN32

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef acSystem;
 *
 * Representation of the system object, the entry point into Arena SDK.
 */
typedef void* acSystem;

/**
 * @typedef acDevice;
 *
 * Represents a device, used to configure and stream a device.
 */
typedef void* acDevice;

/**
 * @typedef acBuffer;
 *
 * Represents a buffer, which carries a payload of either an image or a chunk
 * data. A buffer can hold image data and/or chunk data.
 */
typedef void* acBuffer;

/**
 * @typedef acFeatureStream;
 *
 * Represents a feature stream object, allowing node map configurations to be
 * streamed to and from files.
 */
typedef void* acFeatureStream;

/**
 * @typedef acNodeMap;
 *
 * Represents a node map, or feature tree.
 */
typedef void* acNodeMap;

/**
 * @typedef acNode;
 *
 * Represents a node, or feature.
 */
typedef void* acNode;

/**
 * @typedef acCallback;
 *
 * Represents a callback handle, used to deregister callbacks when they are no
 * longer needed.
 */
typedef void* acCallback;

#ifndef GC_TLI_CLIENT_H_
#if defined(__cplusplus)
typedef bool bool8_t;
#else
typedef uint8_t bool8_t;
#endif
#endif

/**
 * @typedef acCallbackFunction
 *
 * The signature of a function needed to register a callback. An example of a
 * function that could work might look something like the following.
 *
 * \code{.c}
 * 	// a function with the appropriate function signature to register as a callback
 * 	// error handling skipped for brevity
 * 	{
 * 		void onCallback(acNode hNode, void* pUserData)
 * 		{
 * 			UserDataStruct* userData = reinterpret_cast<UserDataStruct*>(pUserData);
 * 			
 * 			char pBuf[100];
 * 			size_t len = 100;
 * 			acNodeGetName(hNode, pBuf, &len);
 * 			// ...
 * 		}
 * 	}
 * \endcode
 */
typedef void (*acCallbackFunction)(acNode, void*);

typedef void(*acImageCallbackFunction)(acBuffer, void*);
/**
 * @enum AC_ERROR_LIST
 *
 * This enum represents the different errors that a function might return.
 *
 * @warning 
 *  - Use AC_ERROR integer values in place of AC_ERROR_LIST enum values
 */
enum AC_ERROR_LIST
{
	AC_ERR_SUCCESS = 0,				   /*!< Success, no error */
	AC_ERR_ERROR = -1001,			   /*!< Generic error */
	AC_ERR_NOT_INITIALIZED = -1002,	   /*!< Arena SDK not initialized */
	AC_ERR_NOT_IMPLEMENTED = -1003,	   /*!< Function not implemented */
	AC_ERR_RESOURCE_IN_USE = -1004,	   /*!< Resource already in use */
	AC_ERR_ACCESS_DENIED = -1005,	   /*!< Incorrect access */
	AC_ERR_INVALID_HANDLE = -1006,	   /*!< Null/incorrect handle */
	AC_ERR_INVALID_ID = -1007,		   /*!< Incorrect ID */
	AC_ERR_NO_DATA = -1008,			   /*!< No data available */
	AC_ERR_INVALID_PARAMETER = -1009,  /*!< Null/incorrect parameter */
	AC_ERR_IO = -1010,				   /*!< Input/output error */
	AC_ERR_TIMEOUT = -1011,			   /*!< Timed out */
	AC_ERR_ABORT = -1012,			   /*!< Function aborted */
	AC_ERR_INVALID_BUFFER = -1013,	   /*!< Invalid buffer */
	AC_ERR_NOT_AVAILABLE = -1014,	   /*!< Function not available */
	AC_ERR_INVALID_ADDRESS = -1015,	   /*!< Invalid register address */
	AC_ERR_BUFFER_TOO_SMALL = -1016,   /*!< Buffer too small */
	AC_ERR_INVALID_INDEX = -1017,	   /*!< Invalid index */
	AC_ERR_PARSING_CHUNK_DATA = -1018, /*!< Error parsing chunk data */
	AC_ERR_INVALID_VALUE = -1019,	   /*!< Invalid value */
	AC_ERR_RESOURCE_EXHAUSTED = -1020, /*!< Resource cannot perform more actions */
	AC_ERR_OUT_OF_MEMORY = -1021,	   /*!< Not enough memory */
	AC_ERR_BUSY = -1022,			   /*!< Busy on anothe process */
	AC_ERR_CUSTOM = -10000			   /*!< Start adding custom error LIST here */
};

/**
 * @typedef AC_ERROR;
 *
 * Integer representation of the the error enum (AC_ERROR_LIST).
 */
typedef int32_t AC_ERROR;

/**
 * @enum AC_ACCESS_MODE_LIST
 *
 * This enum represents the different access modes for a device or its features.
 *
 * @warning 
 *  - Use AC_ACCESS_MODE integer values in place of AC_ACCESS_MODE_LIST enum
 *    values
 */
enum AC_ACCESS_MODE_LIST
{
	AC_ACCESS_MODE_NI = 0,		  /*!< Not implemented */
	AC_ACCESS_MODE_NA = 1,		  /*!< Not available */
	AC_ACCESS_MODE_WO = 2,		  /*!< Write only */
	AC_ACCESS_MODE_RO = 3,		  /*!< Read only */
	AC_ACCESS_MODE_RW = 4,		  /*!< Read and write */
	AC_ACCESS_MODE_UNDEFINED = 5, /*!< Undefined */
};

/**
 * @typedef AC_ACCESS_MODE;
 *
 * Integer representation of the access mode enum (AC_ACCESS_MODE_LIST).
 */
typedef int32_t AC_ACCESS_MODE;

/**
 * @enum AC_CACHING_MODE_LIST
 *
 * This enum represents the different caching modes a node might use.
 *
 * @warning 
 *  - Use AC_CACHING_MODE integer values in place of AC_CACHING_MODE_LIST enum
 *    values
 */
enum AC_CACHING_MODE_LIST
{
	AC_CACHING_MODE_NO_CACHE = 0,	   /*!< Do not use cache */
	AC_CACHING_MODE_WRITE_THROUGH = 1, /*!< Write to cache and register */
	AC_CACHING_MODE_WRITE_AROUND = 2,  /*!< Write to register, write to cache on read */
	AC_CACHING_MODE_UNDEFINED = 3,	   /*!< Undefined */
};

/**
 * @typedef AC_CACHING_MODE;
 *
 * Integer representation of the caching mode enum (AC_CACHING_MODE_LIST).
 */
typedef int32_t AC_CACHING_MODE;

/**
 * @enum AC_NAMESPACE_LIST
 *
 * This enum represents whether a node is in the standard or custom namespaces.
 *
 * @warning 
 *  - Use AC_NAMESPACE integer values in place of AC_NAMESPACE_LIST enum values
 */
enum AC_NAMESPACE_LIST
{
	AC_NAMESPACE_CUSTOM = 0,	/*!< Name resides in custom namespace */
	AC_NAMESPACE_STANDARD = 1,	/*!< Name resides in one of the standard namespaces */
	AC_NAMESPACE_UNDEFINED = 2, /*!< Undefined */
};

/**
 * @typedef AC_NAMESPACE;
 *
 * Integer representation of the namespace enum (AC_NAMESPACE_LIST).
 */
typedef int32_t AC_NAMESPACE;

/**
 * @enum AC_INTERFACE_TYPE_LIST
 *
 * This enum represents the interface type of a node.
 *
 * @warning 
 *  - Use AC_INTERFACE_TYPE integer values in place of AC_INTERFACE_LIST enum
 *    values
 */
enum AC_INTERFACE_TYPE_LIST
{
	AC_INTERFACE_TYPE_VALUE = 0,	   /*!< IValue interface */
	AC_INTERFACE_TYPE_BASE = 1,		   /*!< IBase interface */
	AC_INTERFACE_TYPE_INTEGER = 2,	   /*!< IInteger interface */
	AC_INTERFACE_TYPE_BOOLEAN = 3,	   /*!< IBoolean interface */
	AC_INTERFACE_TYPE_COMMAND = 4,	   /*!< ICommand interface */
	AC_INTERFACE_TYPE_FLOAT = 5,	   /*!< IFloat interface */
	AC_INTERFACE_TYPE_STRING = 6,	   /*!< IString interface */
	AC_INTERFACE_TYPE_REGISTER = 7,	   /*!< IRegister interface */
	AC_INTERFACE_TYPE_CATEGORY = 8,	   /*!< ICategory interface */
	AC_INTERFACE_TYPE_ENUMERATION = 9, /*!< IEnumeration interface */
	AC_INTERFACE_TYPE_ENUMENTRY = 10,  /*!< IEnumEntry interface */
	AC_INTERFACE_TYPE_PORT = 11,	   /*!< IPort interface */
};

/**
 * @typedef AC_INTERFACE_TYPE;
 *
 * Integer representation of the interface type enum (AC_INTERFACE_TYPE_LIST).
 */
typedef int32_t AC_INTERFACE_TYPE;

/**
 * @enum AC_VISIBILITY_LIST
 *
 * This enum represents the different visibilities of a node.
 *
 * @warning 
 *  - Use AC_VISIBILITY integer values in place of AC_VISIBILITY_LIST enum
 *    values
 */
enum AC_VISIBILITY_LIST
{
	AC_VISIBILITY_BEGINNER = 0,	  /*!< Always visible */
	AC_VISIBILITY_EXPERT = 1,	  /*!< Visible for experts or gurus */
	AC_VISIBILITY_GURU = 2,		  /*!< Visible for gurus */
	AC_VISIBILITY_INVISIBLE = 3,  /*!< Not visible */
	AC_VISIBILITY_UNDEFINED = 99, /*!< Undefined */
};

/**
 * @typedef AC_VISIBILITY;
 *
 * Integer representation of the visibility enum (AC_VISIBILITY_LIST).
 */
typedef int32_t AC_VISIBILITY;

/**
 * @enum AC_INC_MODE_LIST
 *
 * This enum represents the different increment modes of an integer/float node.
 *
 * @warning 
 *  - Use AC_INC_MODE integer values in place of AC_INC_MODE_LIST enum values
 */
enum AC_INC_MODE_LIST
{
	AC_INC_MODE_NONE = 0,  /*!< The feature has no increment */
	AC_INC_MODE_FIXED = 1, /*!< The feature has a fix increment */
	AC_INC_MODE_LIST = 2   /*!< The feature has a list of valid values */
};

/**
 * @typedef AC_INC_MODE;
 *
 * Integer representation of the increment mode enum (AC_INC_MODE_LIST).
 */
typedef int32_t AC_INC_MODE;

/**
 * @enum AC_REPRESENTATION_LIST
 *
 * This enum represents the different representations of an integer/float node.
 *
 * @warning 
 *  - Use AC_REPRESENTATION integer values in place of AC_REPRESENTATION_LIST
 *    enum values
 */
enum AC_REPRESENTATION_LIST
{
	AC_REPRESENTATION_LINEAR = 0,		/*!< Slider with linear behavior */
	AC_REPRESENTATION_LOGARITHMIC = 1,	/*!< Slider with logarithmic behavior */
	AC_REPRESENTATION_BOOLEAN = 2,		/*!< Check box */
	AC_REPRESENTATION_PURE_NUMBER = 3,	/*!< Decimal number in an edit control */
	AC_REPRESENTATION_HEX_NUMBER = 4,	/*!< Hex number in an edit control */
	AC_REPRESENTATION_IPV4_ADDRESS = 5, /*!< IP address */
	AC_REPRESENTATION_MAC_ADDRESS = 6,	/*!< MAC address */
	AC_REPRESENTATION_UNDEFINED = 7,	/* Undefined */
};

/**
 * @typedef AC_REPRESENTATION;
 *
 * Integer representation of the numeric representation enum
 * (AC_REPRESENTATION_LIST).
 */
typedef int32_t AC_REPRESENTATION;

/**
 * @enum AC_DISPLAY_NOTATION_LIST
 *
 * This enum represents the best way to display a float node.
 *
 * @warning 
 *  - Use AC_DISPLAY_NOTATION integer values in place of
 *    AC_DISPLAY_NOTATION_LIST enum values
 */
enum AC_DISPLAY_NOTATION_LIST
{
	AC_DISPLAY_NOTATION_AUTOMATIC = 0,	/*!< The notation if either scientific or fixed depending on what is shorter */
	AC_DISPLAY_NOTATION_FIXED = 1,		/*!< The notation is fixed, e.g. 123.4 */
	AC_DISPLAY_NOTATION_SCIENTIFIC = 2, /*!< The notation is scientific, e.g. 1.234e2 */
	AC_DISPLAY_NOTATION_UNDEFINED = 3,	/*!< Undefined */
};

/**
 * @typedef AC_DISPLAY_NOTATION;
 *
 * Integer representation of the display notation enum
 * (AC_DISPLAY_NOTATION_LIST).
 */
typedef int32_t AC_DISPLAY_NOTATION;

/**
 * @enum AC_PAYLOAD_TYPE_LIST
 *
 * This enum represents the different payload types of a buffer.
 *
 * @warning 
 *  - Use AC_PAYLOAD_TYPE integer values in place of AC_PAYLOAD_LIST enum
 *    values
 */
enum AC_PAYLOAD_TYPE_LIST
{
	AC_PAYLOAD_TYPE_IMAGE = 0x0001,				   /*!< Image data only */
	AC_PAYLOAD_TYPE_IMAGE_EXTENDED_CHUNK = 0x4001, /*!< Image data extended with chunk data */
	AC_PAYLOAD_TYPE_CHUNKDATA = 0x0004			   /*!< Chunk data only; image data may be present as chunk */
};

/**
 * @typedef AC_PAYLOAD_TYPE;
 *
 * Integer representation of the payload type enum (AC_PAYLOAD_TYPE_LIST).
 */
typedef int32_t AC_PAYLOAD_TYPE;

/**
 * @enum AC_PIXEL_ENDIANNESS_LIST
 *
 * This enum represents the different pixel endiannesses.
 *
 * @warning 
 *  - Use AC_PIXEL_ENDIANNESS integer values in place of
 *    AC_PIXEL_ENDIANNESS_LIST enum values
 */
enum AC_PIXEL_ENDIANNESS_LIST
{
	AC_PIXEL_ENDIANNESS_UNKNOWN = 0, /*!< Unknown pixel endianness */
	AC_PIXEL_ENDIANNESS_LITTLE = 1,	 /*!< Little endian */
	AC_PIXEL_ENDIANNESS_BIG = 2		 /*!< Big endian */
};

/**
 * @typedef AC_PIXEL_ENDIANNESS;
 *
 * Integer representation of the pixel endianness enum
 * (AC_PIXEL_ENDIANNESS_LIST).
 */
typedef int32_t AC_PIXEL_ENDIANNESS;

/**
 * @enum AC_BAYER_ALGORITHM_LIST
 *
 * This enum represents the different algorithms available to interpolate bayer
 * image data.
 *
 * @warning 
 *  - Use AC_BAYER_ALGORITHM integer values in place of AC_BAYER_ALGORITHM_LIST
 *    enum values
 */
enum AC_BAYER_ALGORITHM_LIST
{
	AC_BAYER_ALGORITHM_DIRECTIONAL_INTERPOLATION = 0,	  /*!< Algorithm that averages nearest neighbours (faster) */
	AC_BAYER_ALGORITHM_ADAPTIVE_HOMOGENEITY_DIRECTED = 1, /*!< Adaptive algorithm that uses directional data (slower, more accurate coloring) */
	AC_BAYER_ALGORITHM_UNKNOWN = 2						  /*!< Undefined algorithm */
};

/**
 * @typedef AC_BAYER_ALGORITHM;
 *
 * Integer representation of the bayer algorithm enum
 * (AC_BAYER_ALGORITHM_LIST).
 */
typedef int32_t AC_BAYER_ALGORITHM;

/**
 * @enum AC_START_STREAM_FLAGS_LIST
 *
 * This enum represents the transport layer flags for starting a stream.
 */
enum AC_START_STREAM_FLAGS_LIST
{
	AC_START_STREAM_FLAG_DEFAULT = 1,		/*!< default behaviour for the underlying transport layer */
	AC_START_STREAM_FLAG_GVSP_LEGACY = 1001 /*!< Will try to start Gvsp 2.0 stream with 16bit block id if supported */
};

/**
 * @typedef AC_START_STREAM_FLAGS;
 *
 * Integer representation of the start stream flags
 * (AC_START_STREAM_FLAGS_LIST).
 */
typedef int32_t AC_START_STREAM_FLAGS;


//ArenaC Pixel Format Definitions
#define PFNC_Mono1p 0x01010037 /* Monochrome 1-bit packed */
#define PFNC_Mono2p 0x01020038 /* Monochrome 2-bit packed */
#define PFNC_Mono4p 0x01040039 /* Monochrome 4-bit packed */
#define PFNC_Mono8 0x01080001 /* Monochrome 8-bit */
#define PFNC_Mono8s 0x01080002 /* Monochrome 8-bit signed */
#define PFNC_Mono10 0x01100003 /* Monochrome 10-bit unpacked */
#define PFNC_Mono10p 0x010A0046 /* Monochrome 10-bit packed */
#define PFNC_Mono12 0x01100005 /* Monochrome 12-bit unpacked */
#define PFNC_Mono12p 0x010C0047 /* Monochrome 12-bit packed */
#define PFNC_Mono14 0x01100025 /* Monochrome 14-bit unpacked */
#define PFNC_Mono16 0x01100007 /* Monochrome 16-bit */
#define PFNC_BayerBG8 0x0108000B /* Bayer Blue-Green 8-bit */
#define PFNC_BayerBG10 0x0110000F /* Bayer Blue-Green 10-bit unpacked */
#define PFNC_BayerBG10p 0x010A0052 /* Bayer Blue-Green 10-bit packed */
#define PFNC_BayerBG12 0x01100013 /* Bayer Blue-Green 12-bit unpacked */
#define PFNC_BayerBG12p 0x010C0053 /* Bayer Blue-Green 12-bit packed */
#define PFNC_BayerBG16 0x01100031 /* Bayer Blue-Green 16-bit */
#define PFNC_BayerGB8 0x0108000A /* Bayer Green-Blue 8-bit */
#define PFNC_BayerGB10 0x0110000E /* Bayer Green-Blue 10-bit unpacked */
#define PFNC_BayerGB10p 0x010A0054 /* Bayer Green-Blue 10-bit packed */
#define PFNC_BayerGB12 0x01100012 /* Bayer Green-Blue 12-bit unpacked */
#define PFNC_BayerGB12p 0x010C0055 /* Bayer Green-Blue 12-bit packed */
#define PFNC_BayerGB16 0x01100030 /* Bayer Green-Blue 16-bit */
#define PFNC_BayerGR8 0x01080008 /* Bayer Green-Red 8-bit */
#define PFNC_BayerGR10 0x0110000C /* Bayer Green-Red 10-bit unpacked */
#define PFNC_BayerGR10p 0x010A0056 /* Bayer Green-Red 10-bit packed */
#define PFNC_BayerGR12 0x01100010 /* Bayer Green-Red 12-bit unpacked */
#define PFNC_BayerGR12p 0x010C0057 /* Bayer Green-Red 12-bit packed */
#define PFNC_BayerGR16 0x0110002E /* Bayer Green-Red 16-bit */
#define PFNC_BayerRG8 0x01080009 /* Bayer Red-Green 8-bit */
#define PFNC_BayerRG10 0x0110000D /* Bayer Red-Green 10-bit unpacked */
#define PFNC_BayerRG10p 0x010A0058 /* Bayer Red-Green 10-bit packed */
#define PFNC_BayerRG12 0x01100011 /* Bayer Red-Green 12-bit unpacked */
#define PFNC_BayerRG12p 0x010C0059 /* Bayer Red-Green 12-bit packed */
#define PFNC_BayerRG16 0x0110002F /* Bayer Red-Green 16-bit */
#define PFNC_RGBa8 0x02200016 /* Red-Green-Blue-alpha 8-bit */
#define PFNC_RGBa10 0x0240005F /* Red-Green-Blue-alpha 10-bit unpacked */
#define PFNC_RGBa10p 0x02280060 /* Red-Green-Blue-alpha 10-bit packed */
#define PFNC_RGBa12 0x02400061 /* Red-Green-Blue-alpha 12-bit unpacked */
#define PFNC_RGBa12p 0x02300062 /* Red-Green-Blue-alpha 12-bit packed */
#define PFNC_RGBa14 0x02400063 /* Red-Green-Blue-alpha 14-bit unpacked */
#define PFNC_RGBa16 0x02400064 /* Red-Green-Blue-alpha 16-bit */
#define PFNC_RGB8 0x02180014 /* Red-Green-Blue 8-bit */
#define PFNC_RGB8_Planar 0x02180021 /* Red-Green-Blue 8-bit planar */
#define PFNC_RGB10 0x02300018 /* Red-Green-Blue 10-bit unpacked */
#define PFNC_RGB10_Planar 0x02300022 /* Red-Green-Blue 10-bit unpacked planar */
#define PFNC_RGB10p 0x021E005C /* Red-Green-Blue 10-bit packed */
#define PFNC_RGB10p32 0x0220001D /* Red-Green-Blue 10-bit packed into 32-bit */
#define PFNC_RGB12 0x0230001A /* Red-Green-Blue 12-bit unpacked */
#define PFNC_RGB12_Planar 0x02300023 /* Red-Green-Blue 12-bit unpacked planar */
#define PFNC_RGB12p 0x0224005D /* Red-Green-Blue 12-bit packed   */
#define PFNC_RGB14 0x0230005E /* Red-Green-Blue 14-bit unpacked */
#define PFNC_RGB16 0x02300033 /* Red-Green-Blue 16-bit   */
#define PFNC_RGB16_Planar 0x02300024 /* Red-Green-Blue 16-bit planar */
#define PFNC_RGB565p 0x02100035 /* Red-Green-Blue 5/6/5-bit packed */
#define PFNC_BGRa8 0x02200017 /* Blue-Green-Red-alpha 8-bit */
#define PFNC_BGRa10 0x0240004C /* Blue-Green-Red-alpha 10-bit unpacked */
#define PFNC_BGRa10p 0x0228004D /* Blue-Green-Red-alpha 10-bit packed */
#define PFNC_BGRa12 0x0240004E /* Blue-Green-Red-alpha 12-bit unpacked */
#define PFNC_BGRa12p 0x0230004F /* Blue-Green-Red-alpha 12-bit packed */
#define PFNC_BGRa14 0x02400050 /* Blue-Green-Red-alpha 14-bit unpacked */
#define PFNC_BGRa16 0x02400051 /* Blue-Green-Red-alpha 16-bit */
#define PFNC_BGR8 0x02180015 /* Blue-Green-Red 8-bit */
#define PFNC_BGR10 0x02300019 /* Blue-Green-Red 10-bit unpacked */
#define PFNC_BGR10p 0x021E0048 /* Blue-Green-Red 10-bit packed */
#define PFNC_BGR12 0x0230001B /* Blue-Green-Red 12-bit unpacked */
#define PFNC_BGR12p 0x02240049 /* Blue-Green-Red 12-bit packed */
#define PFNC_BGR14 0x0230004A /* Blue-Green-Red 14-bit unpacked */
#define PFNC_BGR16 0x0230004B /* Blue-Green-Red 16-bit */
#define PFNC_BGR565p 0x02100036 /* Blue-Green-Red 5/6/5-bit packed */
#define PFNC_R8 0x010800C9 /* Red 8-bit */
#define PFNC_R10 0x010A00CA /* Red 10-bit */
#define PFNC_R12 0x010C00CB /* Red 12-bit */
#define PFNC_R16 0x011000CC /* Red 16-bit */
#define PFNC_G8 0x010800CD /* Green 8-bit */
#define PFNC_G10 0x010A00CE /* Green 10-bit */
#define PFNC_G12 0x010C00CF /* Green 12-bit */
#define PFNC_G16 0x011000D0 /* Green 16-bit */
#define PFNC_B8 0x010800D1 /* Blue 8-bit */
#define PFNC_B10 0x010A00D2 /* Blue 10-bit */
#define PFNC_B12 0x010C00D3 /* Blue 12-bit */
#define PFNC_B16 0x011000D4 /* Blue 16-bit */
#define PFNC_Coord3D_ABC8 0x021800B2 /* 3D coordinate A-B-C 8-bit */
#define PFNC_Coord3D_ABC8_Planar 0x021800B3 /* 3D coordinate A-B-C 8-bit planar */
#define PFNC_Coord3D_ABC10p 0x021E00DB /* 3D coordinate A-B-C 10-bit packed */
#define PFNC_Coord3D_ABC10p_Planar 0x021E00DC /* 3D coordinate A-B-C 10-bit packed planar */
#define PFNC_Coord3D_ABC12p 0x022400DE /* 3D coordinate A-B-C 12-bit packed */
#define PFNC_Coord3D_ABC12p_Planar 0x022400DF /* 3D coordinate A-B-C 12-bit packed planar */
#define PFNC_Coord3D_ABC16 0x023000B9 /* 3D coordinate A-B-C 16-bit */
#define PFNC_Coord3D_ABC16_Planar 0x023000BA /* 3D coordinate A-B-C 16-bit planar */
#define PFNC_Coord3D_ABC32f 0x026000C0 /* 3D coordinate A-B-C 32-bit floating point */
#define PFNC_Coord3D_ABC32f_Planar 0x026000C1 /* 3D coordinate A-B-C 32-bit floating point planar */
#define PFNC_Coord3D_AC8 0x021000B4 /* 3D coordinate A-C 8-bit */
#define PFNC_Coord3D_AC8_Planar 0x021000B5 /* 3D coordinate A-C 8-bit planar */
#define PFNC_Coord3D_AC10p 0x021400F0 /* 3D coordinate A-C 10-bit packed */
#define PFNC_Coord3D_AC10p_Planar 0x021400F1 /* 3D coordinate A-C 10-bit packed planar */
#define PFNC_Coord3D_AC12p 0x021800F2 /* 3D coordinate A-C 12-bit packed */
#define PFNC_Coord3D_AC12p_Planar 0x021800F3 /* 3D coordinate A-C 12-bit packed planar */
#define PFNC_Coord3D_AC16 0x022000BB /* 3D coordinate A-C 16-bit */
#define PFNC_Coord3D_AC16_Planar 0x022000BC /* 3D coordinate A-C 16-bit planar */
#define PFNC_Coord3D_AC32f 0x024000C2 /* 3D coordinate A-C 32-bit floating point */
#define PFNC_Coord3D_AC32f_Planar 0x024000C3 /* 3D coordinate A-C 32-bit floating point planar */
#define PFNC_Coord3D_A8 0x010800AF /* 3D coordinate A 8-bit */
#define PFNC_Coord3D_A10p 0x010A00D5 /* 3D coordinate A 10-bit packed */
#define PFNC_Coord3D_A12p 0x010C00D8 /* 3D coordinate A 12-bit packed */
#define PFNC_Coord3D_A16 0x011000B6 /* 3D coordinate A 16-bit */
#define PFNC_Coord3D_A32f 0x012000BD /* 3D coordinate A 32-bit floating point */
#define PFNC_Coord3D_B8 0x010800B0 /* 3D coordinate B 8-bit */
#define PFNC_Coord3D_B10p 0x010A00D6 /* 3D coordinate B 10-bit packed */
#define PFNC_Coord3D_B12p 0x010C00D9 /* 3D coordinate B 12-bit packed */
#define PFNC_Coord3D_B16 0x011000B7 /* 3D coordinate B 16-bit */
#define PFNC_Coord3D_B32f 0x012000BE /* 3D coordinate B 32-bit floating point */
#define PFNC_Coord3D_C8 0x010800B1 /* 3D coordinate C 8-bit */
#define PFNC_Coord3D_C10p 0x010A00D7 /* 3D coordinate C 10-bit packed */
#define PFNC_Coord3D_C12p 0x010C00DA /* 3D coordinate C 12-bit packed */
#define PFNC_Coord3D_C16 0x011000B8 /* 3D coordinate C 16-bit */
#define PFNC_Coord3D_C32f 0x012000BF /* 3D coordinate C 32-bit floating point */
#define PFNC_Confidence1 0x010800C4 /* Confidence 1-bit unpacked */
#define PFNC_Confidence1p 0x010100C5 /* Confidence 1-bit packed */
#define PFNC_Confidence8 0x010800C6 /* Confidence 8-bit */
#define PFNC_Confidence16 0x011000C7 /* Confidence 16-bit */
#define PFNC_Confidence32f 0x012000C8 /* Confidence 32-bit floating point */
#define PFNC_BiColorBGRG8 0x021000A6 /* Bi-color Blue/Green - Red/Green 8-bit */
#define PFNC_BiColorBGRG10 0x022000A9 /* Bi-color Blue/Green - Red/Green 10-bit unpacked */
#define PFNC_BiColorBGRG10p 0x021400AA /* Bi-color Blue/Green - Red/Green 10-bit packed */
#define PFNC_BiColorBGRG12 0x022000AD /* Bi-color Blue/Green - Red/Green 12-bit unpacked */
#define PFNC_BiColorBGRG12p 0x021800AE /* Bi-color Blue/Green - Red/Green 12-bit packed */
#define PFNC_BiColorRGBG8 0x021000A5 /* Bi-color Red/Green - Blue/Green 8-bit */
#define PFNC_BiColorRGBG10 0x022000A7 /* Bi-color Red/Green - Blue/Green 10-bit unpacked */
#define PFNC_BiColorRGBG10p 0x021400A8 /* Bi-color Red/Green - Blue/Green 10-bit packed */
#define PFNC_BiColorRGBG12 0x022000AB /* Bi-color Red/Green - Blue/Green 12-bit unpacked */
#define PFNC_BiColorRGBG12p 0x021800AC /* Bi-color Red/Green - Blue/Green 12-bit packed */
#define PFNC_SCF1WBWG8 0x01080067 /* Sparse Color Filter #1 White-Blue-White-Green 8-bit */
#define PFNC_SCF1WBWG10 0x01100068 /* Sparse Color Filter #1 White-Blue-White-Green 10-bit unpacked */
#define PFNC_SCF1WBWG10p 0x010A0069 /* Sparse Color Filter #1 White-Blue-White-Green 10-bit packed */
#define PFNC_SCF1WBWG12 0x0110006A /* Sparse Color Filter #1 White-Blue-White-Green 12-bit unpacked */
#define PFNC_SCF1WBWG12p 0x010C006B /* Sparse Color Filter #1 White-Blue-White-Green 12-bit packed */
#define PFNC_SCF1WBWG14 0x0110006C /* Sparse Color Filter #1 White-Blue-White-Green 14-bit unpacked */
#define PFNC_SCF1WBWG16 0x0110006D /* Sparse Color Filter #1 White-Blue-White-Green 16-bit unpacked */
#define PFNC_SCF1WGWB8 0x0108006E /* Sparse Color Filter #1 White-Green-White-Blue 8-bit */
#define PFNC_SCF1WGWB10 0x0110006F /* Sparse Color Filter #1 White-Green-White-Blue 10-bit unpacked */
#define PFNC_SCF1WGWB10p 0x010A0070 /* Sparse Color Filter #1 White-Green-White-Blue 10-bit packed */
#define PFNC_SCF1WGWB12 0x01100071 /* Sparse Color Filter #1 White-Green-White-Blue 12-bit unpacked */
#define PFNC_SCF1WGWB12p 0x010C0072 /* Sparse Color Filter #1 White-Green-White-Blue 12-bit packed */
#define PFNC_SCF1WGWB14 0x01100073 /* Sparse Color Filter #1 White-Green-White-Blue 14-bit unpacked */
#define PFNC_SCF1WGWB16 0x01100074 /* Sparse Color Filter #1 White-Green-White-Blue 16-bit */
#define PFNC_SCF1WGWR8 0x01080075 /* Sparse Color Filter #1 White-Green-White-Red 8-bit */
#define PFNC_SCF1WGWR10 0x01100076 /* Sparse Color Filter #1 White-Green-White-Red 10-bit unpacked */
#define PFNC_SCF1WGWR10p 0x010A0077 /* Sparse Color Filter #1 White-Green-White-Red 10-bit packed */
#define PFNC_SCF1WGWR12 0x01100078 /* Sparse Color Filter #1 White-Green-White-Red 12-bit unpacked */
#define PFNC_SCF1WGWR12p 0x010C0079 /* Sparse Color Filter #1 White-Green-White-Red 12-bit packed */
#define PFNC_SCF1WGWR14 0x0110007A /* Sparse Color Filter #1 White-Green-White-Red 14-bit unpacked */
#define PFNC_SCF1WGWR16 0x0110007B /* Sparse Color Filter #1 White-Green-White-Red 16-bit */
#define PFNC_SCF1WRWG8 0x0108007C /* Sparse Color Filter #1 White-Red-White-Green 8-bit */
#define PFNC_SCF1WRWG10 0x0110007D /* Sparse Color Filter #1 White-Red-White-Green 10-bit unpacked */
#define PFNC_SCF1WRWG10p 0x010A007E /* Sparse Color Filter #1 White-Red-White-Green 10-bit packed */
#define PFNC_SCF1WRWG12 0x0110007F /* Sparse Color Filter #1 White-Red-White-Green 12-bit unpacked */
#define PFNC_SCF1WRWG12p 0x010C0080 /* Sparse Color Filter #1 White-Red-White-Green 12-bit packed */
#define PFNC_SCF1WRWG14 0x01100081 /* Sparse Color Filter #1 White-Red-White-Green 14-bit unpacked */
#define PFNC_SCF1WRWG16 0x01100082 /* Sparse Color Filter #1 White-Red-White-Green 16-bit */
#define PFNC_YCbCr8 0x0218005B /* YCbCr 4:4:4 8-bit */
#define PFNC_YCbCr8_CbYCr 0x0218003A /* YCbCr 4:4:4 8-bit */
#define PFNC_YCbCr10_CbYCr 0x02300083 /* YCbCr 4:4:4 10-bit unpacked */
#define PFNC_YCbCr10p_CbYCr 0x021E0084 /* YCbCr 4:4:4 10-bit packed */
#define PFNC_YCbCr12_CbYCr 0x02300085 /* YCbCr 4:4:4 12-bit unpacked */
#define PFNC_YCbCr12p_CbYCr 0x02240086 /* YCbCr 4:4:4 12-bit packed */
#define PFNC_YCbCr411_8 0x020C005A /* YCbCr 4:1:1 8-bit */
#define PFNC_YCbCr411_8_CbYYCrYY 0x020C003C /* YCbCr 4:1:1 8-bit */
#define PFNC_YCbCr422_8 0x0210003B /* YCbCr 4:2:2 8-bit */
#define PFNC_YCbCr422_8_CbYCrY 0x02100043 /* YCbCr 4:2:2 8-bit */
#define PFNC_YCbCr422_10 0x02200065 /* YCbCr 4:2:2 10-bit unpacked */
#define PFNC_YCbCr422_10_CbYCrY 0x02200099 /* YCbCr 4:2:2 10-bit unpacked */
#define PFNC_YCbCr422_10p 0x02140087 /* YCbCr 4:2:2 10-bit packed */
#define PFNC_YCbCr422_10p_CbYCrY 0x0214009A /* YCbCr 4:2:2 10-bit packed */
#define PFNC_YCbCr422_12 0x02200066 /* YCbCr 4:2:2 12-bit unpacked */
#define PFNC_YCbCr422_12_CbYCrY 0x0220009B /* YCbCr 4:2:2 12-bit unpacked */
#define PFNC_YCbCr422_12p 0x02180088 /* YCbCr 4:2:2 12-bit packed */
#define PFNC_YCbCr422_12p_CbYCrY 0x0218009C /* YCbCr 4:2:2 12-bit packed */
#define PFNC_YCbCr601_8_CbYCr 0x0218003D /* YCbCr 4:4:4 8-bit BT.601 */
#define PFNC_YCbCr601_10_CbYCr 0x02300089 /* YCbCr 4:4:4 10-bit unpacked BT.601 */
#define PFNC_YCbCr601_10p_CbYCr 0x021E008A /* YCbCr 4:4:4 10-bit packed BT.601 */
#define PFNC_YCbCr601_12_CbYCr 0x0230008B /* YCbCr 4:4:4 12-bit unpacked BT.601 */
#define PFNC_YCbCr601_12p_CbYCr 0x0224008C /* YCbCr 4:4:4 12-bit packed BT.601 */
#define PFNC_YCbCr601_411_8_CbYYCrYY 0x020C003F /* YCbCr 4:1:1 8-bit BT.601 */
#define PFNC_YCbCr601_422_8 0x0210003E /* YCbCr 4:2:2 8-bit BT.601 */
#define PFNC_YCbCr601_422_8_CbYCrY 0x02100044 /* YCbCr 4:2:2 8-bit BT.601 */
#define PFNC_YCbCr601_422_10 0x0220008D /* YCbCr 4:2:2 10-bit unpacked BT.601 */
#define PFNC_YCbCr601_422_10_CbYCrY 0x0220009D /* YCbCr 4:2:2 10-bit unpacked BT.601 */
#define PFNC_YCbCr601_422_10p 0x0214008E /* YCbCr 4:2:2 10-bit packed BT.601 */
#define PFNC_YCbCr601_422_10p_CbYCrY 0x0214009E /* YCbCr 4:2:2 10-bit packed BT.601 */
#define PFNC_YCbCr601_422_12 0x0220008F /* YCbCr 4:2:2 12-bit unpacked BT.601 */
#define PFNC_YCbCr601_422_12_CbYCrY 0x0220009F /* YCbCr 4:2:2 12-bit unpacked BT.601 */
#define PFNC_YCbCr601_422_12p 0x02180090 /* YCbCr 4:2:2 12-bit packed BT.601 */
#define PFNC_YCbCr601_422_12p_CbYCrY 0x021800A0 /* YCbCr 4:2:2 12-bit packed BT.601 */
#define PFNC_YCbCr709_8_CbYCr 0x02180040 /* YCbCr 4:4:4 8-bit BT.709 */
#define PFNC_YCbCr709_10_CbYCr 0x02300091 /* YCbCr 4:4:4 10-bit unpacked BT.709 */
#define PFNC_YCbCr709_10p_CbYCr 0x021E0092 /* YCbCr 4:4:4 10-bit packed BT.709 */
#define PFNC_YCbCr709_12_CbYCr 0x02300093 /* YCbCr 4:4:4 12-bit unpacked BT.709 */
#define PFNC_YCbCr709_12p_CbYCr 0x02240094 /* YCbCr 4:4:4 12-bit packed BT.709 */
#define PFNC_YCbCr709_411_8_CbYYCrYY 0x020C0042 /* YCbCr 4:1:1 8-bit BT.709 */
#define PFNC_YCbCr709_422_8 0x02100041 /* YCbCr 4:2:2 8-bit BT.709 */
#define PFNC_YCbCr709_422_8_CbYCrY 0x02100045 /* YCbCr 4:2:2 8-bit BT.709 */
#define PFNC_YCbCr709_422_10 0x02200095 /* YCbCr 4:2:2 10-bit unpacked BT.709 */
#define PFNC_YCbCr709_422_10_CbYCrY 0x022000A1 /* YCbCr 4:2:2 10-bit unpacked BT.709 */
#define PFNC_YCbCr709_422_10p 0x02140096 /* YCbCr 4:2:2 10-bit packed BT.709 */
#define PFNC_YCbCr709_422_10p_CbYCrY 0x021400A2 /* YCbCr 4:2:2 10-bit packed BT.709 */
#define PFNC_YCbCr709_422_12 0x02200097 /* YCbCr 4:2:2 12-bit unpacked BT.709 */
#define PFNC_YCbCr709_422_12_CbYCrY 0x022000A3 /* YCbCr 4:2:2 12-bit unpacked BT.709 */
#define PFNC_YCbCr709_422_12p 0x02180098 /* YCbCr 4:2:2 12-bit packed BT.709 */
#define PFNC_YCbCr709_422_12p_CbYCrY 0x021800A4 /* YCbCr 4:2:2 12-bit packed BT.709 */
#define PFNC_YCbCr2020_8_CbYCr 0x021800F4 /* YCbCr 4:4:4 8-bit BT.2020 */
#define PFNC_YCbCr2020_10_CbYCr 0x023000F5 /* YCbCr 4:4:4 10-bit unpacked BT.2020 */
#define PFNC_YCbCr2020_10p_CbYCr 0x021E00F6 /* YCbCr 4:4:4 10-bit packed BT.2020 */
#define PFNC_YCbCr2020_12_CbYCr 0x023000F7 /* YCbCr 4:4:4 12-bit unpacked BT.2020  */
#define PFNC_YCbCr2020_12p_CbYCr 0x022400F8 /* YCbCr 4:4:4 12-bit packed BT.2020 */
#define PFNC_YCbCr2020_411_8_CbYYCrYY 0x020C00F9 /* YCbCr 4:1:1 8-bit BT.2020 */
#define PFNC_YCbCr2020_422_8 0x021000FA /* YCbCr 4:2:2 8-bit BT.2020  */
#define PFNC_YCbCr2020_422_8_CbYCrY 0x021000FB /* YCbCr 4:2:2 8-bit BT.2020 */
#define PFNC_YCbCr2020_422_10 0x022000FC /* YCbCr 4:2:2 10-bit unpacked BT.2020 */
#define PFNC_YCbCr2020_422_10_CbYCrY 0x022000FD /* YCbCr 4:2:2 10-bit unpacked BT.2020 */
#define PFNC_YCbCr2020_422_10p 0x021400FE /* YCbCr 4:2:2 10-bit packed BT.2020 */
#define PFNC_YCbCr2020_422_10p_CbYCrY 0x021400FF /* YCbCr 4:2:2 10-bit packed BT.2020 */
#define PFNC_YCbCr2020_422_12 0x02200100 /* YCbCr 4:2:2 12-bit unpacked BT.2020 */
#define PFNC_YCbCr2020_422_12_CbYCrY 0x02200101 /* YCbCr 4:2:2 12-bit unpacked BT.2020 */
#define PFNC_YCbCr2020_422_12p 0x02180102 /* YCbCr 4:2:2 12-bit packed BT.2020 */
#define PFNC_YCbCr2020_422_12p_CbYCrY 0x02180103 /* YCbCr 4:2:2 12-bit packed BT.2020 */
#define PFNC_YUV8_UYV 0x02180020 /* YUV 4:4:4 8-bit */
#define PFNC_YUV411_8_UYYVYY 0x020C001E /* YUV 4:1:1 8-bit */
#define PFNC_YUV422_8 0x02100032 /* YUV 4:2:2 8-bit */
#define PFNC_YUV422_8_UYVY 0x0210001F /* YUV 4:2:2 8-bit */
#define GVSP_Mono10Packed 0x010C0004 /* GigE Vision specific format, Monochrome 10-bit packed */
#define GVSP_Mono12Packed 0x010C0006 /* GigE Vision specific format, Monochrome 12-bit packed */
#define GVSP_BayerBG10Packed 0x010C0029 /* GigE Vision specific format, Bayer Blue-Green 10-bit packed */
#define GVSP_BayerBG12Packed 0x010C002D /* GigE Vision specific format, Bayer Blue-Green 12-bit packed */
#define GVSP_BayerGB10Packed 0x010C0028 /* GigE Vision specific format, Bayer Green-Blue 10-bit packed */
#define GVSP_BayerGB12Packed 0x010C002C /* GigE Vision specific format, Bayer Green-Blue 12-bit packed */
#define GVSP_BayerGR10Packed 0x010C0026 /* GigE Vision specific format, Bayer Green-Red 10-bit packed */
#define GVSP_BayerGR12Packed 0x010C002A /* GigE Vision specific format, Bayer Green-Red 12-bit packed */
#define GVSP_BayerRG10Packed 0x010C0027 /* GigE Vision specific format, Bayer Red-Green 10-bit packed */
#define GVSP_BayerRG12Packed 0x010C002B /* GigE Vision specific format, Bayer Red-Green 12-bit packed */
#define GVSP_RGB10V1Packed 0x0220001C /* GigE Vision specific format, Red-Green-Blue 10-bit packed - variant 1 */
#define GVSP_RGB12V1Packed 0x02240034 /* GigE Vision specific format, Red-Green-Blue 12-bit packed - variant 1 */
#define LUCID_PolarizeMono8 0x81080001 /* */
#define LUCID_PolarizeMono12p 0x810C0047 /* */
#define LUCID_PolarizeMono12Packed 0x810C0006 /* */
#define LUCID_PolarizeMono12 0x81100008 /* */
#define LUCID_PolarizeMono16 0x81100007 /* */
#define LUCID_PolarizedAngles_0d_45d_90d_135d_Mono8 0x8220000F /* */
#define LUCID_PolarizedAngles_0d_45d_90d_135d_Mono12p 0x8230001F /* */
#define LUCID_PolarizedAngles_0d_45d_90d_135d_Mono16 0x8240002F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_Mono8 0x8218003F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_Mono12p 0x8224004F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_Mono16 0x8230005F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_S3_Mono8 0x8220006F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_S3_Mono12p 0x8230007F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_S3_Mono16 0x8240008F /* */
#define LUCID_PolarizedAolp_Mono8 0x810800DF /* */
#define LUCID_PolarizedAolp_Mono12p 0x810C00EF /* */
#define LUCID_PolarizedDolp_Mono8 0x810800BF /* */
#define LUCID_PolarizedDolp_Mono12p 0x810C00CF /* */
#define LUCID_PolarizedDolpAolp_Mono8 0x8210009F /* */
#define LUCID_PolarizedDolpAolp_Mono12p 0x821800AF /* */
#define LUCID_Coord3D_ABCY16s 0x82400400 /* */
#define LUCID_Coord3D_ABC16s 0x82300401 /* */
#define LUCID_Coord3D_C16s 0x81100402 /* */
#define LUCID_Coord3D_ABCY16 0x82400403 /* */
#define LUCID_PolarizedAngles_0d_45d_90d_135d_BayerRG8 0x8220020F /* */
#define LUCID_PolarizedAngles_0d_45d_90d_135d_BayerRG12p 0x8230021F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_BayerRG8 0x8218023F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_BayerRG12p 0x8224024F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_S3_BayerRG8 0x8220026F /* */
#define LUCID_PolarizedStokes_S0_S1_S2_S3_BayerRG12p 0x8230027F /* */
#define LUCID_PolarizedAolp_BayerRG8 0x810802DF /* */
#define LUCID_PolarizedAolp_BayerRG12p 0x810C02EF /* */
#define LUCID_PolarizedDolp_BayerRG8 0x810802BF /* */
#define LUCID_PolarizedDolp_BayerRG12p 0x810C02CF /* */
#define LUCID_PolarizedDolpAolp_BayerRG8 0x8210029F /* */
#define LUCID_PolarizedDolpAolp_BayerRG12p 0x821802AF /* */
#ifdef __cplusplus
} // extern "C"
#endif
