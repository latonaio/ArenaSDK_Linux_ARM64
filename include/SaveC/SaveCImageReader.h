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
 * @fn SC_ERROR SC_API saveReaderCreate(saveReader* phReader, const char* pFileName)
 *
 * @param phReader
 *  - Type: saveReader*
 *  - [Out] parameter
 *  - Created image reader
 *
 * @param pFileName
 *  - Type: const char*
 *  - [In] parameter
 *  - Relative path to a file to load
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveReaderCreate </B> initializes an image reader and loads an image. The
 * image reader must be destroyed or memory will leak. This image reader loads
 * images starting from the upper leftmost pixel.
 */
SC_ERROR SC_API saveReaderCreate(saveReader* phReader, const char* pFileName);
#ifdef _WIN32
SC_ERROR SC_API saveReaderCreateU(saveReader* phReader, const wchar_t* pFileName);
#endif

/**
 * @fn SC_ERROR SC_API saveReaderCreateBottomToTop(saveReader* phReader, const char* pFileName)
 *
 * @param phReader
 *  - Type: saveReader*
 *  - [Out] parameter
 *  - Created image reader
 *
 * @param pFileName
 *  - Type: const char*
 *  - [In] parameter
 *  - Relative path to a file to load
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveReaderCreateBottomToTop </B> initializes an image reader and loads an
 * image. The image reader must be destroyed or memory will leak. This image
 * reader loads images starting from the lower leftmost pixel.
 */
SC_ERROR SC_API saveReaderCreateBottomToTop(saveReader* phReader, const char* pFileName);
#ifdef _WIN32
SC_ERROR SC_API saveReaderCreateBottomToTopU(saveReader* phReader, const wchar_t* pFileName);
#endif

/**
 * @fn SC_ERROR SC_API saveReaderDestroy(saveReader hReader)
 *
 * @param hReader
 *  - Type: saveReader
 *  - [In] parameter
 *  - An image reader
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveReaderDestroy </B> cleans up an image reader (saveReader),
 * deallocating its memory.
 *
 * @warning 
 *  - Invalidates pointers to image data
 *
 * @see 
 *  - saveReader
 */
SC_ERROR SC_API saveReaderDestroy(saveReader hReader);

/**
 * @fn SC_ERROR SC_API saveReaderGetParams(saveReader hReader, size_t* pWidth, size_t* pHeight, size_t* pBitsPerPixel)
 *
 * @param hReader
 *  - Type: saveReader
 *  - [In] parameter
 *  - An image reader
 *
 * @param pWidth
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Width of the loaded image
 *
 * @param pHeight
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Height of the loaded image
 *
 * @param pBitsPerPixel
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of bits per pixel of the loaded image
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveReaderGetParams </B> retrieves the width, height, and number of bits
 * per pixel of the loaded image.
 */
SC_ERROR SC_API saveReaderGetParams(saveReader hReader, size_t* pWidth, size_t* pHeight, size_t* pBitsPerPixel);

/**
 * @fn SC_ERROR SC_API saveReaderGetData(saveReader hReader, uint8_t** ppData)
 *
 * @param hReader
 *  - Type: saveReader
 *  - [In] parameter
 *  - An image reader
 *
 * @param ppData
 *  - Type: uint8_t**
 *  - [Out] parameter
 *  - Pointer to the image data
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveReaderGetData </B> retrieves a pointer to the beginning of the loaded
 * image data. Data must be copied manually in order to retain data beyond beyond
 * the life cycle of the image reader.
 *
 * @warning 
 *  - Pointer invalidated on destruction of image reader
 */
SC_ERROR SC_API saveReaderGetData(saveReader hReader, uint8_t** ppData);

/**
 * @fn SC_ERROR SC_API saveReaderLoadRawData(const char* pFileName, uint8_t* pImageData, const size_t size)
 *
 * @param pFileName
 *  - Type: const char*
 *  - [In] parameter
 *  - Relative path to the file to load
 *
 * @param pImageData
 *  - Type: uint8_t*
 *  - [Out] parameter
 *  - Retrieved data
 *  - Buffer must be preallocated
 *
 * @param size
 *  - Type: const size_t
 *  - [In] parameter
 *  - Size of the buffer
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveReaderLoadRawData </B> loads raw data into a buffer. It does not
 * require an image reader context (saveReader).
 *
 * @warning 
 *  - does not require context
 *  - image data buffer must be preallocated
 *
 * @see 
 *  - saveReader
 */
SC_ERROR SC_API saveReaderLoadRawData(const char* pFileName, uint8_t* pImageData, const size_t size);
#ifdef _WIN32
SC_ERROR SC_API saveReaderLoadRawDataU(const wchar_t* pFileName, uint8_t* pImageData, const size_t size);
#endif

#ifdef __cplusplus
}
#endif
