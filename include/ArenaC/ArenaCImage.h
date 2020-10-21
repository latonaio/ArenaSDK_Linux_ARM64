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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn AC_ERROR AC_API acImageGetWidth(acBuffer hBuffer, size_t* pWidth)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pWidth
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Width of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetWidth </B> gets the width of the image in pixels. Images are
 * self-describing, so the device does not need to be queried to get this
 * information.
 */
AC_ERROR AC_API acImageGetWidth(acBuffer hBuffer, size_t* pWidth);

/**
 * @fn AC_ERROR AC_API acImageGetHeight(acBuffer hBuffer, size_t* pHeight)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pHeight
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Height of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetHeight </B> gets the height of the image. Images are
 * self-describing, so the device does not need to be queried to get this
 * information.
 */
AC_ERROR AC_API acImageGetHeight(acBuffer hBuffer, size_t* pHeight);

/**
 * @fn AC_ERROR AC_API acImageGetOffsetX(acBuffer hBuffer, size_t* pOffsetX)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pOffsetX
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Offset X of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetOffsetX </B> gets the offset of the image along the X-axis.
 * Images are self-describing, so the device does not need to be queried to get
 * this information.
 */
AC_ERROR AC_API acImageGetOffsetX(acBuffer hBuffer, size_t* pOffsetX);

/**
 * @fn AC_ERROR AC_API acImageGetOffsetY(acBuffer hBuffer, size_t* pOffsetY)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pOffsetY
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Offset Y of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetOffsetY </B> gets the offset of the image along the Y-axis.
 * Images are self-describing, so the device does not need to be queried to get
 * this information.
 */
AC_ERROR AC_API acImageGetOffsetY(acBuffer hBuffer, size_t* pOffsetY);

/**
 * @fn AC_ERROR AC_API acImageGetPaddingX(acBuffer hBuffer, size_t* pPaddingX)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pPaddingX
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Padding X of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetPaddingX </B> gets the padding of the image along the X-axis.
 * Images are self-describing, so the device does not need to be queried to get
 * this information.
 */
AC_ERROR AC_API acImageGetPaddingX(acBuffer hBuffer, size_t* pPaddingX);

/**
 * @fn AC_ERROR AC_API acImageGetPaddingY(acBuffer hBuffer, size_t* pPaddingY)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pPaddingY
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Padding Y of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetPaddingY </B> gets the padding of the image along the Y-axis.
 * Images are self-describing, so the device does not need to be queried to get
 * this information.
 */
AC_ERROR AC_API acImageGetPaddingY(acBuffer hBuffer, size_t* pPaddingY);

/**
 * @fn AC_ERROR AC_API acImageGetPixelFormat(acBuffer hBuffer, uint64_t* pPixelFormat)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pPixelFormat
 *  - Type: uint64_t*
 *  - [Out] parameter
 *  - Pixel format of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetPixelFormat </B> gets the pixel format (PfncFormat) of the
 * image, as defined by the PFNC (Pixel Format Naming Convention). Images are
 * self-describing, so the device does not need to be queried to get this
 * information.
 */
AC_ERROR AC_API acImageGetPixelFormat(acBuffer hBuffer, uint64_t* pPixelFormat);

/**
 * @fn AC_ERROR AC_API acImageGetBitsPerPixel(acBuffer hBuffer, size_t* pBitsPerPixel)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pBitsPerPixel
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Bits per pixel of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetBitsPerPixel </B> gets the number of bits per pixel of the image
 * from the integer value of the pixel format (PfncFormat). Internally, a public
 * helper funciton is called (acGetBitsPerPixel).
 *
 * @see 
 *  - acGetBitsPerPixel
 */
AC_ERROR AC_API acImageGetBitsPerPixel(acBuffer hBuffer, size_t* pBitsPerPixel);

/**
 * @fn AC_ERROR AC_API acImageGetPixelEndianness(acBuffer hBuffer, AC_PIXEL_ENDIANNESS* pPixelEndianness)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pPixelEndianness
 *  - Type: AC_PIXEL_ENDIANNESS*
 *  - [Out] parameter
 *  - Endianness of the pixels of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetPixelEndianness </B> gets the pixel endianness
 * (AC_PIXEL_ENDIANNESS) of the image. Images are self-describing, so the
 * device does not need to be queried to get this information.
 */
AC_ERROR AC_API acImageGetPixelEndianness(acBuffer hBuffer, AC_PIXEL_ENDIANNESS* pPixelEndianness);

/**
 * @fn AC_ERROR AC_API acImageGetTimestamp(acBuffer hBuffer, uint64_t* pTimestamp)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pTimestamp
 *  - Type: uint64_t*
 *  - Unit: nanoseconds
 *  - [Out] parameter
 *  - Timestamp of the image in nanoseconds
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetTimestamp </B> gets the timestamp of the image in nanoseconds.
 * Images are self-describing, so the device does not need to be queried to get
 * this information.
 */
AC_ERROR AC_API acImageGetTimestamp(acBuffer hBuffer, uint64_t* pTimestamp);

/**
 * @fn AC_ERROR AC_API acImageGetTimestampNs(acBuffer hBuffer, uint64_t* pTimestampNs)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pTimestampNs
 *  - Type: uint64_t*
 *  - Unit: nanoseconds
 *  - [Out] parameter
 *  - Timestamp of the image in nanoseconds
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetTimestampNs </B> gets the timestamp of the image in nanoseconds.
 * Images are self-describing, so the device does not need to be queried to get
 * this information.
 */
AC_ERROR AC_API acImageGetTimestampNs(acBuffer hBuffer, uint64_t* pTimestampNs);

/**
 * @fn AC_ERROR AC_API acImageGetData(acBuffer hBuffer, uint8_t** ppData)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param ppData
 *  - Type: uint8_t**
 *  - [Out] parameter
 *  - Point to the payload data
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageGetData </B> returns a pointer to the beginning of the image's
 * payload data. The payload may include chunk data.
 */
AC_ERROR AC_API acImageGetData(acBuffer hBuffer, uint8_t** ppData);

#ifdef __cplusplus
} // extern "C"
#endif
