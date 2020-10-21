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

#if defined(_WIN32)
#ifdef SAVEC_EXPORTS
#define SC_API __declspec(dllexport)
#else
#define SC_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) && (__GNUC__ >= 4) && defined(__linux__)
#define SC_API __attribute__((visibility("default")))
#else
#define SC_API
#endif

#ifndef GC_TLI_CLIENT_H_
#if defined(__cplusplus)
typedef bool bool8_t;
#else
typedef uint8_t bool8_t;
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef saveReader;
 *
 * Handle to an image reader, used to read images from disk into Save C.
 */
typedef void* saveReader;

/**
 * @typedef saveWriter;
 *
 * Handle to an image writer, used to save images to disk.
 */
typedef void* saveWriter;

typedef void* saveRecorder;

typedef struct savePlyParams_t
{
	bool8_t filterPoints; /*!< Default: true. Filter NaN points (A = B = C = -32,678) */
	bool8_t isSigned;	  /*!< Default: false. If true, interpret data as signed signed pixel format .Otherwise, interpret as unsigned signed pixel format  */
	float scale;		  /*!< Default: 0.25f. Data scaling */
	float offsetA;		  /*!< Default: 0.0f. X-axis (A) offset */
	float offsetB;		  /*!< Default: 0.0f. Y-axis (B) offset */
	float offsetC;		  /*!< Default: 0.0f. Z-axis (C) offset */
} savePlyParams;

/**
 * @enum SC_ERROR_LIST
 *
 * This enum represents the different errors that a function might return.
 *
 * @warning 
 *  - Use SC_ERROR integer values in place of SC_ERROR_LIST enum values
 */
enum SC_ERROR_LIST
{
	SC_ERR_SUCCESS = 0,				   /*!< Success, no error */
	SC_ERR_ERROR = -1001,			   /*!< Generic error */
	SC_ERR_NOT_INITIALIZED = -1002,	   /*!< Arena SDK not initialized */
	SC_ERR_NOT_IMPLEMENTED = -1003,	   /*!< Function not implemented */
	SC_ERR_RESOURCE_IN_USE = -1004,	   /*!< Resource already in use */
	SC_ERR_ACCESS_DENIED = -1005,	   /*!< Incorrect access */
	SC_ERR_INVALID_HANDLE = -1006,	   /*!< Null/incorrect handle */
	SC_ERR_INVALID_ID = -1007,		   /*!< Incorrect ID */
	SC_ERR_NO_DATA = -1008,			   /*!< No data available */
	SC_ERR_INVALID_PARAMETER = -1009,  /*!< Null/incorrect parameter */
	SC_ERR_IO = -1010,				   /*!< Input/output error */
	SC_ERR_TIMEOUT = -1011,			   /*!< Timed out */
	SC_ERR_ABORT = -1012,			   /*!< Function aborted */
	SC_ERR_INVALID_BUFFER = -1013,	   /*!< Invalid buffer */
	SC_ERR_NOT_AVAILABLE = -1014,	   /*!< Function not available */
	SC_ERR_INVALID_ADDRESS = -1015,	   /*!< Invalid register address */
	SC_ERR_BUFFER_TOO_SMALL = -1016,   /*!< Buffer too small */
	SC_ERR_INVALID_INDEX = -1017,	   /*!< Invalid index */
	SC_ERR_PARSING_CHUNK_DATA = -1018, /*!< Error parsing chunk data */
	SC_ERR_INVALID_VALUE = -1019,	   /*!< Invalid value */
	SC_ERR_RESOURCE_EXHAUSTED = -1020, /*!< Resource cannot perform more actions */
	SC_ERR_OUT_OF_MEMORY = -1021,	   /*!< Not enough memory */
	SC_ERR_BUSY = -1022,			   /*!< Busy on anothe process */
	SC_ERR_CUSTOM_ID = -10000		   /*!< Start adding custom error LIST here */
};

/**
 * @typedef SC_ERROR;
 *
 * Integer representation of the error enum (SC_ERROR_LIST).
 */
typedef int32_t SC_ERROR;

/**
 * @enum SC_FORMAT_LIST
 *
 * This enum represents the different file formats that Save C is able to
 * read/write.
 *
 * @warning 
 *  - Use SC_FORMAT integer values in place of SC_FORMAT_LIST enum values
 */
enum SC_FORMAT_LIST
{
	SC_FORMAT_UNKNOWN = 0, /*!< Unknown file format */
	SC_FORMAT_JPEG = 1,	   /*!< Jpeg image file format */
	SC_FORMAT_PNG = 2,	   /*!< Png image file format */
	SC_FORMAT_TIFF = 3,	   /*!< Tiff image file format */
	SC_FORMAT_BMP = 4,	   /*!< Bmp image file format */
	SC_FORMAT_RAW = 6,	   /*!< Raw image or video file format */
	SC_FORMAT_AVI = 7,	   /*!< Avi video file format */
	SC_FORMAT_MOV = 8,	   /*!< Mov video file format */
	SC_FORMAT_MP4 = 9,	   /*!< Mpeg-4 video file format */
	SC_FORMAT_PLY = 10	   /*!< Ply 3D pointcloud file format */
};

/**
 * @typedef SC_FORMAT;
 *
 * Integer representation of the file format enum (SC_FORMAT_LIST).
 */
typedef int32_t SC_FORMAT;

/**
 * @enum SC_JPEG_SUBSAMPLING_LIST
 *
 * This enum represents the different subsampling options when saving JPEG images.
 *
 * @warning
 *  - Use SC_JPEG_SUBSAMPLING integer values in place of SC_JPEG_SUBSAMPLING_LIST enum values
 */
enum SC_JPEG_SUBSAMPLING_LIST
{
	SC_NO_JPEG_SUBSAMPLING = 0,	 /*!< Save with no chroma subsampling (4:4:4) */
	SC_JPEG_SUBSAMPLING_411 = 1, /*!< Save with high 4x1 chroma subsampling (4:1:1) */
	SC_JPEG_SUBSAMPLING_420 = 2, /*!< Save with medium 2x2 chroma subsampling (4:2:0) */
	SC_JPEG_SUBSAMPLING_422 = 3	 /*!< Save with low 2x1 chroma subsampling (4:2:2) */
};

/**
* @typedef SC_JPEG_SUBSAMPLING;
*
* Integer representation of the file format enum (SC_JPEG_SUBSAMPLING_LIST).
*/
typedef int32_t SC_JPEG_SUBSAMPLING;

/**
* @enum SC_TIFF_COMPRESSION_LIST
*
* This enum represents the different compression options when saving TIFF images.
*
* @warning
*  - Use SC_TIFF_COMPRESSION integer values in place of SC_TIFF_COMPRESSION_LIST enum values
*/
enum SC_TIFF_COMPRESSION_LIST
{
	SC_NO_TIFF_COMPRESSION = 0,			   /*!< Save without any compression */
	SC_TIFF_COMPRESSION_PACKBITS = 1,	   /*!< Save using PACKBITS compression */
	SC_TIFF_COMPRESSION_DEFLATE = 2,	   /*!< Save using DEFLATE compression (also known as ZLIP compression */
	SC_TIFF_COMPRESSION_ADOBE_DEFLATE = 3, /*!< Save using ADOBE DEFLATE compression */
	SC_TIFF_COMPRESSION_CCITTFAX3 = 4,	   /*!< Save using CCITT Group 3 fax encoding */
	SC_TIFF_COMPRESSION_CCITTFAX4 = 5,	   /*!< Save using CCITT Group 4 fax encoding */
	SC_TIFF_COMPRESSION_LZW = 6,		   /*!< Save using LZW compression */
	SC_TIFF_COMPRESSION_JPEG_TIFF = 7,	   /*!< Save using JPEG compression (8-bit greyscale and 24-bit only. Default to LZW for other bitdepths) */
	SC_TIFF_COMPRESSION_LOGLUV = 8		   /*!< Save using LogLuv compression (only available with RGBF images. Default to LZW compression) */
};

/**
* @typedef SC_TIFF_COMPRESSION;
*
* Integer representation of the file format enum (SC_TIFF_COMPRESSION_LIST).
*/
typedef int32_t SC_TIFF_COMPRESSION;

#ifdef __cplusplus
} // extern "C"
#endif
