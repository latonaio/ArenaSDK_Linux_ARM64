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
 * @fn AC_ERROR AC_API acFeatureStreamCreate(acNodeMap hNodeMap, acFeatureStream* phFeatureStream)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - Node map to stream
 *
 * @param phFeatureStream
 *  - Type: acFeatureStream*
 *  - [Out] parameter
 *  - Feature stream object to create
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFeatureStream </B> builds a feature stream from a device's node map
 * (acNodeMap). This node map cannot be changed later; another feature stream
 * must be created to stream another node map.
 *
 * @see 
 *  - acNodeMap
 */
AC_ERROR AC_API acFeatureStreamCreate(acNodeMap hNodeMap, acFeatureStream* phFeatureStream);

/**
 * @fn AC_ERROR AC_API acFeatureStreamDestroy(acFeatureStream hFeatureStream)
 *
 * @param hFeatureStream
 *  - Type: acFeatureStream
 *  - [In] parameter
 *  - Feature stream object to destroy
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFeatureStreamDestroy </B> destroys a feature stream object, cleaning up
 * allocated memory.
 */
AC_ERROR AC_API acFeatureStreamDestroy(acFeatureStream hFeatureStream);

/**
 * @fn AC_ERROR AC_API acFeatureStreamWrite(acFeatureStream hFeatureStream)
 *
 * @param hFeatureStream
 *  - Type: acFeatureStream
 *  - [In] parameter
 *  - The feature stream object
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFeatureStreamWrite </B> streams all selected features from the node map
 * (acNodeMap) to a file.
 *
 * @see 
 *  - acNodeMap
 */
AC_ERROR AC_API acFeatureStreamWrite(acFeatureStream hFeatureStream);

/**
 * @fn AC_ERROR AC_API acFeatureStreamWriteFileName(acFeatureStream hFeatureStream, const char* pFileName)
 *
 * @param hFeatureStream
 *  - Type: acFeatureStream
 *  - [In] parameter
 *  - The feature stream object
 *
 * @param pFileName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the file to stream to
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFeatureStreamWriteFileName </B> streams all selected features from the
 * node map (acNodeMap) to a file.
 *
 * @see 
 *  - acNodeMap
 */
AC_ERROR AC_API acFeatureStreamWriteFileName(acFeatureStream hFeatureStream, const char* pFileName);

/**
 * @fn AC_ERROR AC_API acFeatureStreamRead(acFeatureStream hFeatureStream)
 *
 * @param hFeatureStream
 *  - Type: acFeatureStream
 *  - [In] parameter
 *  - The feature stream object
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFeatureStreamRead </B> streams all selected features from a file to the
 * node map (acNodeMap).
 *
 * @see 
 *  - acNodeMap
 */
AC_ERROR AC_API acFeatureStreamRead(acFeatureStream hFeatureStream);

/**
 * @fn AC_ERROR AC_API acFeatureStreamReadFileName(acFeatureStream hFeatureStream, const char* pFileName)
 *
 * @param hFeatureStream
 *  - Type: acFeatureStream
 *  - [In] parameter
 *  - The feature stream object
 *
 * @param pFileName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the file to stream from
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFeatureStreamReadFileName </B> streams all selected features from a file
 * to the node map (acNodeMap).
 *
 * @see 
 *  - acNodeMap
 */
AC_ERROR AC_API acFeatureStreamReadFileName(acFeatureStream hFeatureStream, const char* pFileName);

/**
 * @fn AC_ERROR AC_API acFeatureStreamSelect(acFeatureStream hFeatureStream, const char* pFeatureName)
 *
 * @param hFeatureStream
 *  - Type: acFeatureStream
 *  - [In] parameter
 *  - The feature stream object
 *
 * @param pFeatureName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the feature to select
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFeatureStreamSelect </B> adds a single streamable feature to the list of
 * selected features to stream. If called for the first time, <B>
 * acFeatureStreamSelect </B> also sets an internal 'select-all' flag to false.
 */
AC_ERROR AC_API acFeatureStreamSelect(acFeatureStream hFeatureStream, const char* pFeatureName);

#ifdef __cplusplus
} // extern "C"
#endif
