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
 * @fn AC_ERROR AC_API acNodeMapGetNode(acNodeMap hNodeMap, const char* pNodeName, acNode* phNode)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node to retrieve
 *
 * @param phNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - The node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetNode </B> retrieves a node.
 */
AC_ERROR AC_API acNodeMapGetNode(acNodeMap hNodeMap, const char* pNodeName, acNode* phNode);

/**
 * @fn AC_ERROR AC_API acNodeMapGetNodeAndAccessMode(acNodeMap hNodeMap, const char* pNodeName, acNode* phNode, AC_ACCESS_MODE* pNodeAccess)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node to retrieve
 *
 * @param phNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - The node
 *
 * @param pNodeAccess
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - The node's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetNodeAndAccessMode </B> retrieves a node with its access mode.
 */
AC_ERROR AC_API acNodeMapGetNodeAndAccessMode(acNodeMap hNodeMap, const char* pNodeName, acNode* phNode, AC_ACCESS_MODE* pNodeAccess);

/**
 * @fn AC_ERROR AC_API acNodeMapInvalidateNodes(acNodeMap hNodeMap)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapInvalidateNodes </B> invalidates all of its nodes.
 */
AC_ERROR AC_API acNodeMapInvalidateNodes(acNodeMap hNodeMap);

/**
 * @fn AC_ERROR AC_API acNodeMapGetDeviceName(acNodeMap hNodeMap, char* pDeviceNameBuf, size_t* pBufLen)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pDeviceNameBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Name of the device
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
 * <B> acNodeMapGetDeviceName </B> retrieves the node map's device name.
 */
AC_ERROR AC_API acNodeMapGetDeviceName(acNodeMap hNodeMap, char* pDeviceNameBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeMapPoll(acNodeMap hNodeMap, int64_t elapsedTime)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param elapsedTime
 *  - Type: int64_t
 *  - Unit: milliseconds
 *  - [In] parameter
 *  - Time since the last poll
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapPoll </B> polls the node map.
 */
AC_ERROR AC_API acNodeMapPoll(acNodeMap hNodeMap, int64_t elapsedTime);

/**
 * @fn AC_ERROR AC_API acNodeMapLock(acNodeMap hNodeMap)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetNode </B> locks the node map.
 */
AC_ERROR AC_API acNodeMapLock(acNodeMap hNodeMap);

/**
 * @fn AC_ERROR AC_API acNodeMapUnlock(acNodeMap hNodeMap)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetNode </B> unlocks the node map.
 */
AC_ERROR AC_API acNodeMapUnlock(acNodeMap hNodeMap);

/**
 * @fn AC_ERROR AC_API acNodeMapTryLock(acNodeMap hNodeMap, bool8_t* pLocked)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pLocked
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if the lock succeeded
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetNode </B> tries to lock the node map, returning whether or not
 * the call succeeded.
 */
AC_ERROR AC_API acNodeMapTryLock(acNodeMap hNodeMap, bool8_t* pLocked);

/**
 * @fn AC_ERROR AC_API acNodeMapGetNumNodes(acNodeMap hNodeMap, uint64_t* pNumNodes)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNumNodes
 *  - Type: uint64_t*
 *  - [Out] parameter
 *  - Total number of nodes in the node map
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetNode </B> retrieves the number of nodes in the node map.
 */
AC_ERROR AC_API acNodeMapGetNumNodes(acNodeMap hNodeMap, uint64_t* pNumNodes);

/**
 * @fn AC_ERROR AC_API acNodeMapGetNodeByIndex(acNodeMap hNodeMap, size_t index, acNode* phNode)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the node
 *
 * @param phNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - The node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetNode </B> retrieves a node by its index.
 */
AC_ERROR AC_API acNodeMapGetNodeByIndex(acNodeMap hNodeMap, size_t index, acNode* phNode);

/**
 * @fn AC_ERROR AC_API acNodeMapGetNodeByIndexAndAccessMode(acNodeMap hNodeMap, size_t index, acNode* phNode, AC_ACCESS_MODE* pNodeAccess)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the node
 *
 * @param phNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - The node
 *
 * @param pNodeAccess
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - The node's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetNode </B> retrieves a node by its index along with its access
 * mode.
 */
AC_ERROR AC_API acNodeMapGetNodeByIndexAndAccessMode(acNodeMap hNodeMap, size_t index, acNode* phNode, AC_ACCESS_MODE* pNodeAccess);

/**
 * @fn AC_ERROR AC_API acNodeMapGetStringValue(acNodeMap hNodeMap, const char* pNodeName, char* pValueBuf, size_t* pBufLen)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param pValueBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Value of the node
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
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetStringValue </B> acts as a shortcut for retrieving the value
 * of a string node.
 */
AC_ERROR AC_API acNodeMapGetStringValue(acNodeMap hNodeMap, const char* pNodeName, char* pValueBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeMapGetIntegerValue(acNodeMap hNodeMap, const char* pNodeName, int64_t* pValue)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param pValue
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Value of the node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetIntegerValue </B> acts as a shortcut for retrieving the value
 * of an integer node.
 */
AC_ERROR AC_API acNodeMapGetIntegerValue(acNodeMap hNodeMap, const char* pNodeName, int64_t* pValue);

/**
 * @fn AC_ERROR AC_API acNodeMapGetFloatValue(acNodeMap hNodeMap, const char* pNodeName, double* pValue)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param pValue
 *  - Type: double*
 *  - [Out] parameter
 *  - Value of the node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetFloatValue </B> acts as a shortcut for retrieving the value of
 * a float node.
 */
AC_ERROR AC_API acNodeMapGetFloatValue(acNodeMap hNodeMap, const char* pNodeName, double* pValue);

/**
 * @fn AC_ERROR AC_API acNodeMapGetBooleanValue(acNodeMap hNodeMap, const char* pNodeName, bool8_t* pValue)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param pValue
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - Value of the node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetBooleanValue </B> acts as a shortcut for retrieving the value
 * of a boolean node.
 */
AC_ERROR AC_API acNodeMapGetBooleanValue(acNodeMap hNodeMap, const char* pNodeName, bool8_t* pValue);

/**
 * @fn AC_ERROR AC_API acNodeMapGetEnumerationValue(acNodeMap hNodeMap, const char* pNodeName, char* pSymbolicBuf, size_t* pBufLen)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param pSymbolicBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Symbolic of the current entry
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
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapGetEnumerationValue </B> acts as a shortcut for retrieving the
 * entry of an enumeration node.
 */
AC_ERROR AC_API acNodeMapGetEnumerationValue(acNodeMap hNodeMap, const char* pNodeName, char* pSymbolicBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeMapSetStringValue(acNodeMap hNodeMap, const char* pNodeName, const char* pValue)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param pValue
 *  - Type: const char*
 *  - [In] parameter
 *  - String value to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapSetStringValue </B> acts as a shortcut for setting the value of a
 * string node.
 */
AC_ERROR AC_API acNodeMapSetStringValue(acNodeMap hNodeMap, const char* pNodeName, const char* pValue);

/**
 * @fn AC_ERROR AC_API acNodeMapSetIntegerValue(acNodeMap hNodeMap, const char* pNodeName, int64_t value)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param value
 *  - Type: int64_t
 *  - [In] parameter
 *  - Integer value to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapSetIntegerValue </B> acts as a shortcut for setting the value of
 * an integer node.
 */
AC_ERROR AC_API acNodeMapSetIntegerValue(acNodeMap hNodeMap, const char* pNodeName, int64_t value);

/**
 * @fn AC_ERROR AC_API acNodeMapSetFloatValue(acNodeMap hNodeMap, const char* pNodeName, double value)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param value
 *  - Type: double
 *  - [In] parameter
 *  - Float value to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapSetFloatValue </B> acts as a shortcut for setting the value of a
 * float node.
 */
AC_ERROR AC_API acNodeMapSetFloatValue(acNodeMap hNodeMap, const char* pNodeName, double value);

/**
 * @fn AC_ERROR AC_API acNodeMapSetBooleanValue(acNodeMap hNodeMap, const char* pNodeName, bool8_t value)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param value
 *  - Type: bool8_t
 *  - [In] parameter
 *  - Boolean value to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapSetBooleanValue </B> acts as a shortcut for setting the value of
 * a boolean node.
 */
AC_ERROR AC_API acNodeMapSetBooleanValue(acNodeMap hNodeMap, const char* pNodeName, bool8_t value);

/**
 * @fn AC_ERROR AC_API acNodeMapSetEnumerationValue(acNodeMap hNodeMap, const char* pNodeName, const char* pSymbolic)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @param pSymbolic
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the entry node to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapSetEnumerationValue </B> acts as a shortcut for setting the value
 * of an enumeration node.
 */
AC_ERROR AC_API acNodeMapSetEnumerationValue(acNodeMap hNodeMap, const char* pNodeName, const char* pSymbolic);

/**
 * @fn AC_ERROR AC_API acNodeMapExecute(acNodeMap hNodeMap, const char* pNodeName)
 *
 * @param hNodeMap
 *  - Type: acNodeMap
 *  - [In] parameter
 *  - A node map
 *
 * @param pNodeName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeMapExecute </B> acts as a shortcut for executing a command node.
 */
AC_ERROR AC_API acNodeMapExecute(acNodeMap hNodeMap, const char* pNodeName);

#ifdef __cplusplus
} // extern "C"
#endif
