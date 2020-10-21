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
 * @fn AC_ERROR AC_API acBufferGetSizeFilled(acBuffer hBuffer, size_t* pSizeFilled)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pSizeFilled
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Size of the payload
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferGetSizeFilled </B> retrieves the size of the payload data,
 * excluding transport layer protocol leaders. The payload data may include image
 * data, chunk data, or both.
 */
AC_ERROR AC_API acBufferGetSizeFilled(acBuffer hBuffer, size_t* pSizeFilled);

/**
 * @fn AC_ERROR AC_API acBufferGetPayloadSize(acBuffer hBuffer, size_t* pPayloadSize)
 *
 * @return 
 *  - Type: size_t
 *  - Unit: bytes
 *  - Size of the intended payload
 *
 * <B> GetPayloadSize </B> retrieves the intended size of the payload. This is
 * similar to the retrieved payload size (acBufferGetSizeFilled), but different
 * in that missed data is included.
 *
 * @warning 
 *  - Causes undefined behavior if buffer
 *
 * @see 
 *  - acBufferGetSizeFilled
 */
AC_ERROR AC_API acBufferGetPayloadSize(acBuffer hBuffer, size_t* pPayloadSize);

/**
 * @fn AC_ERROR AC_API acBufferGetSizeOfBuffer(acBuffer hBuffer, size_t* pSizeOfBuffer)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pSizeOfBuffer
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Size of the buffer
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferGetSizeOfBuffer </B> retrieves the size of a buffer.
 */
AC_ERROR AC_API acBufferGetSizeOfBuffer(acBuffer hBuffer, size_t* pSizeOfBuffer);

/**
 * @fn AC_ERROR AC_API acBufferGetSizeFilled(acBuffer hBuffer, size_t* pSizeFilled)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - In parameter
 *  - An image
 *
 * @param pSizeFilled
 *  - Type: size_t*
 *  - Out parameter
 *  - Size of the payload data
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferGetSizeFilled </B> retrieves the size of the data of a buffer,
 * excluding transport layer protocol leaders.
 */
AC_ERROR AC_API acBufferGetSizeFilled(acBuffer hBuffer, size_t* pSizeFilled);

/**
 * @fn AC_ERROR AC_API acBufferGetFrameId(acBuffer hBuffer, uint64_t* pFrameId)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pFrameId
 *  - Type: uint64_t*
 *  - [Out] parameter
 *  - Frame ID
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferGetFrameId </B> gets the frame ID, a sequential identifier for
 * buffers.
 */
AC_ERROR AC_API acBufferGetFrameId(acBuffer hBuffer, uint64_t* pFrameId);

/**
 * @fn AC_ERROR AC_API acBufferGetPayloadType(acBuffer hBuffer, AC_PAYLOAD_TYPE* pPayloadType)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pPayloadType
 *  - Type: AC_PAYLOAD_TYPE*
 *  - [Out] parameter
 *  - Type of payload data
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferGetPayloadType </B> returns a buffer's payload type
 * (AC_PAYLOAD_TYPE), as defined by the GigE Vision specification.
 */
AC_ERROR AC_API acBufferGetPayloadType(acBuffer hBuffer, AC_PAYLOAD_TYPE* pPayloadType);

/**
 * @fn AC_ERROR AC_API acBufferHasChunkData(acBuffer hBuffer, bool8_t* pHasChunkData)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pHasChunkData
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if the payload has chunk data
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferHasChunkData </B> returns whether or not a buffer's payload that
 * may be interpreted as chunk data.
 */
AC_ERROR AC_API acBufferHasChunkData(acBuffer hBuffer, bool8_t* pHasChunkData);

/**
 * @fn AC_ERROR AC_API acBufferHasImageData(acBuffer hBuffer, bool8_t* pHasImageData)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pHasImageData
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if the payload has image data
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferHasChunkData </B> returns whether or not a buffer's payload that
 * may be interpreted as chunk data.
 */
AC_ERROR AC_API acBufferHasImageData(acBuffer hBuffer, bool8_t* pHasImageData);

/**
 * @fn AC_ERROR AC_API acBufferIsIncomplete(acBuffer hBuffer, bool8_t* pIsIncomplete)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pIsIncomplete
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if the data is incomplete
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferIsIncomplete </B> returns whether or not the payload is complete.
 */
AC_ERROR AC_API acBufferIsIncomplete(acBuffer hBuffer, bool8_t* pIsIncomplete);

/**
 * @fn AC_ERROR AC_API acBufferDataLargerThanBuffer(acBuffer hBuffer, bool8_t* pDataLargerThanBuffer)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pDataLargerThanBuffer
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if the payload is larger than the buffer
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferDataLargerThanBuffer </B> returns whether or not a buffer's
 * payload data is larger than the buffer.
 */
AC_ERROR AC_API acBufferDataLargerThanBuffer(acBuffer hBuffer, bool8_t* pDataLargerThanBuffer);

/**
 * @fn AC_ERROR AC_API acBufferVerifyCRC(acBuffer hBuffer, bool8_t* pVerifyCRC)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A buffer
 *
 * @param pVerifyCRC
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if the calculated CRC value equals the one sent from the device
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBufferVerifyCRC </B> calculates the CRC of a buffer's data and verifies
 * it against the CRC value sent from the device via chunk data. This helps
 * verify that no data has been changed or missed during a transmission. This
 * function calls a global helper function to calculate the CRC
 * (acCalculateCRC32).
 *
 * @see 
 *  - acCalculateCRC
 */
AC_ERROR AC_API acBufferVerifyCRC(acBuffer hBuffer, bool8_t* pVerifyCRC);

#ifdef __cplusplus
} // extern "C"
#endif
