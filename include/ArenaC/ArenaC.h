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
 * @fn AC_ERROR AC_API acOpenSystem(acSystem* phSystem)
 *
 * @param phSystem
 *  - Type: acSystem*
 *  - [Out] parameter
 *  - The system object
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acOpenSystem </B> initializes the Arena SDK and retrieves the system
 * object (acSystem). The system must be closed or memory will leak.
 *
 * @see 
 *  - acSystem
 */
AC_ERROR AC_API acOpenSystem(acSystem* phSystem);

/**
 * @fn AC_ERROR AC_API acCloseSystem(acSystem hSystem)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acCloseSystem </B> cleans up the system (acSystem) and deinitializes the
 * Arena SDK, deallocating all memory.
 *
 * @see 
 *  - acSystem
 */
AC_ERROR AC_API acCloseSystem(acSystem hSystem);

/**
 * AC_ERROR acGetLastErrorMessage(char* pMessageBuf, size_t* pBufLen)
 *
 * @param pMessageBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Current value
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer
 *  - (Out) Length of the value
 *
 * @return
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acGetLastError </B> gets the most recent error from the current thread. If
 * no errors have occurred in the thread, a "No Error" message is returned.
 * 
 */
AC_ERROR AC_API acGetLastErrorMessage(char* pMessageBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acGetBitsPerPixel(uint64_t pixelFormat, size_t* pBitsPerPixel)
 *
 * @param pixelFormat
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Pixel format to get bits per pixel
 *
 * @param pBitsPerPixel
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of bits per pixel for given pixel format
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acGetBitsPerPixel </B> get the number of bits per pixel of the image from
 * the integer value of the pixel format (PfncFormat).
 */
AC_ERROR AC_API acGetBitsPerPixel(uint64_t pixelFormat, size_t* pBitsPerPixel);

/**
 * @fn AC_ERROR AC_API acCalculateCRC32(const uint8_t* pData, size_t pDataLen, size_t* pCRCValue)
 *
 * @param pData
 *  - Type: const uint8_t*
 *  - [In] parameter
 *  - A pointer to the data to use to calculate the CRC
 *
 * @param pDataLen
 *  - Type: size_t
 *  - [In] parameter
 *  - The size of the data
 *
 * @param pCRCValue
 *  - Type: size_t*
 *  - [Out] parameter
 *  - The calculated CRC value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acCalculatedCRC32 </B> calculates a CRC value (cyclical redundancy check)
 * on a dataset. This is used to check whether the dataset has been sent in its
 * entirety.
 */
AC_ERROR AC_API acCalculateCRC32(const uint8_t* pData, size_t pDataLen, size_t* pCRCValue);

/**
 * @fn AC_ERROR AC_API acIsReadable(acNode hNode, bool8_t* pIsReadable)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *
 * @param pIsReadable
 *  - Type: bool8_t*
 *  - [Out] parameter
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIsReadable </B> checks whether a node is readable.
 */
AC_ERROR AC_API acIsReadable(acNode hNode, bool8_t* pIsReadable);

/**
 * @fn AC_ERROR AC_API acIsWritable(acNode hNode, bool8_t* pIsWritable)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *
 * @param pIsWritable
 *  - Type: bool8_t*
 *  - [Out] parameter
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIsWritable </B> checks whether a node is writable.
 */
AC_ERROR AC_API acIsWritable(acNode hNode, bool8_t* pIsWritable);

/**
 * @fn AC_ERROR AC_API acCalculateMaximumNumberOfBuffers(size_t payloadSize, size_t* pMaxBufs);
 *
 * @param payloadSize
 *	- Type: size_t
 *	- Unit: bytes
 *	- [In] parameter
 *	- Payload size of an image
 *
 * @param pMaxBufs
 *	- Type: size_t*
 *	- [Out] parameter
 *	- Maximum number of buffers
 *
 * @return
 *	- Type: AC_ERROR
 *	- Error code for the function
 *	- Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acCalculateMaximumNumberOfBuffers </B> calculates the number of buffers it
 * would take to fill 80% of the available memory.
 */
AC_ERROR AC_API acCalculateMaximumNumberOfBuffers(size_t payloadSize, size_t* pMaxBufs);

#ifdef __cplusplus
} // extern "C"
#endif
