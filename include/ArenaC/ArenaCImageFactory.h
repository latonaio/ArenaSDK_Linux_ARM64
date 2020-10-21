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
 * @fn AC_ERROR AC_API acImageFactoryCreate(uint8_t* pData, size_t dataSize, size_t width, size_t height, uint64_t pixelFormat, acBuffer* phDst)
 *
 * FactoryCreate(uint8_t pData, size_t dataSize, size_t width, size_t height,
 * uint64_t pixelFormat, acBuffer* phDst)
 *
 * @param pData
 *  - Type: uint8_t*
 *  - [Out] parameter
 *  - Pointer to the beginning of the payload data
 *
 * @param dataSize
 *  - Type: size_t
 *  - [In] parameter
 *  - Size of the data
 *
 * @param width
 *  - Type: size_t
 *  - [In] parameter
 *  - Width of the image to create
 *
 * @param height
 *  - Type: size_t
 *  - [In] parameter
 *  - Height of the image to create
 *
 * @param pixelFormat
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Pixel format of the image to create
 *
 * @param phDst
 *  - Type: acBuffer*
 *  - [Out] parameter
 *  - Image created from the parameters
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageFactoryCreate </B> creates an image (acBuffer) from a minimal set
 * of parameters. Images created with the image factory must be destroyed
 * (acImageFactoryDestroy) when no longer needed.
 *
 * @see 
 *  - acBuffer
 *  - acImageFactoryDestroy
 */
AC_ERROR AC_API acImageFactoryCreate(uint8_t* pData, size_t dataSize, size_t width, size_t height, uint64_t pixelFormat, acBuffer* phDst);

/**
 * @fn AC_ERROR AC_API acImageFactoryCopy(acBuffer hSrc, acBuffer* phDst)
 *
 * @param hSrc
 *  - Type: acBuffer
 *  - [In] parameter
 *  - Image to copy
 *
 * @param phDst
 *  - Type: acBuffer*
 *  - [Out] parameter
 *  - Deep copy of the image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageFactoryCopy </B> creates a deep copy of an image (acBuffer) from
 * another image. Images created with the image factory must be destroyed
 * (acImageFactoryDestroy) when no longer needed.
 *
 * @see 
 *  - acBuffer
 *  - acImageFactoryDestroy
 */
AC_ERROR AC_API acImageFactoryCopy(acBuffer hSrc, acBuffer* phDst);

/**
 * @fn AC_ERROR AC_API acImageFactoryConvert(acBuffer hSrc, uint64_t pixelFormat, acBuffer* phDst)
 *
 * @param hSrc
 *  - Type: acBuffer
 *  - [In] parameter
 *  - Image to convert
 *
 * @param pixelFormat
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Pixel format to convert to
 *
 * @param phDst
 *  - Type: acBuffer*
 *  - [Out] parameter
 *  - Convert image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageFactoryConvert </B> converts an image (acBuffer) to a select pixel
 * format. In doing so, it creates a completely new image, similar to a deep copy
 * but with a different pixel format. Images created with the image factory must
 * be destroyed (acImageFactoryDestroy) when no longer needed; otherwise, memory
 * will leak.
 *
 * @see 
 *  - acBuffer
 *  - acImageFactoryDestroy
 */
AC_ERROR AC_API acImageFactoryConvert(acBuffer hSrc, uint64_t pixelFormat, acBuffer* phDst);

/**
 * @fn AC_ERROR AC_API acImageFactoryConvertBayerAlgorithm(acBuffer hSrc, uint64_t pixelFormat, AC_BAYER_ALGORITHM bayerAlgo, acBuffer* phDst)
 *
 * @param hSrc
 *  - Type: acBuffer
 *  - [In] parameter
 *  - Image to convert
 *
 * @param pixelFormat
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Pixel format to convert to
 *
 * @param bayerAlgo
 *  - Type: AC_BAYER_ALGORITHM
 *  - [In] parameter
 *  - Bayer conversion algorithm to use
 *  - Only applicable when converting from bayer
 *
 * @param phDst
 *  - Type: acBuffer*
 *  - [Out] parameter
 *  - Converted image
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageFactoryConvert </B> converts an image (acBuffer) to a select pixel
 * format. In doing so, it creates a completely new image, similar to a deep copy
 * but with a different pixel format. Images created with the image factory must
 * be destroyed (acImageFactoryDestroy) when no longer needed; otherwise, memory
 * will leak.
 *
 * @see 
 *  - acBuffer
 *  - acImageFactoryDestroy
 */
AC_ERROR AC_API acImageFactoryConvertBayerAlgorithm(acBuffer hSrc, uint64_t pixelFormat, AC_BAYER_ALGORITHM bayerAlgo, acBuffer* phDst);

/**
 * @fn AC_ERROR AC_API acImageFactoryDestroy(acBuffer hBuffer)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - Image to destroy
 *  - Image must be from image factory
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acImageFactoryDestroy </B> cleans up an image (acBuffer) and deallocates
 * its memory. It must be called on any image created by the image factory
 * (acImageFactoryCreate, acImageFactoryCopy, acImageFactoryConvert).
 *
 * @see 
 *  - acBuffer
 *  - acImageFactoryCreate
 */
AC_ERROR AC_API acImageFactoryDestroy(acBuffer hBuffer);

#ifdef __cplusplus
} // extern "C"
#endif
