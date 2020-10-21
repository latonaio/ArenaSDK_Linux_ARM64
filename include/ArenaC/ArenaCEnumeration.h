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
 * @fn AC_ERROR AC_API acEnumerationGetNumEntries(acNode hNode, size_t* pNumEntries)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param pNumEntries
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of entries
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetNumEntries </B> retrieves the number of entries.
 */
AC_ERROR AC_API acEnumerationGetNumEntries(acNode hNode, size_t* pNumEntries);

/**
 * @fn AC_ERROR AC_API acEnumerationGetEntryByIndex(acNode hNode, size_t index, acNode* phEntryNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the entry
 *
 * @param phEntryNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Entry node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetEntryByIndex </B> retrieves an entry by its index.
 */
AC_ERROR AC_API acEnumerationGetEntryByIndex(acNode hNode, size_t index, acNode* phEntryNode);

/**
 * @fn AC_ERROR AC_API acEnumerationGetEntryAndAccessModeByIndex(acNode hNode, size_t index, acNode* phEntryNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the entry
 *
 * @param phEntryNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Entry node
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Entry node's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetEntryAndAccessModeByIndex </B> retrieves an entry and its
 * access mode by index.
 */
AC_ERROR AC_API acEnumerationGetEntryAndAccessModeByIndex(acNode hNode, size_t index, acNode* phEntryNode, AC_ACCESS_MODE* pAccessMode);


/**
 * @fn AC_ERROR AC_API acEnumerationGetNumSymbolics(acNode hNode, size_t* pNumSymbolics)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param pNumSymbolics
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of symbolics
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetNumSymbolics </B> retrieves the number of symbolics.
 */
AC_ERROR AC_API acEnumerationGetNumSymbolics(acNode hNode, size_t* pNumSymbolics);

/**
 * @fn AC_ERROR AC_API acEnumerationGetSymbolicByIndex(acNode hNode, size_t index, char* pSymbolicBuf, size_t* pBufLen)
 *
 * )
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Entry of the node
 *
 * @param pSymbolicBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Symbolic of the entry
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
 * <B> acEnumerationGetSymbolicByIndex </B> retrieves a symbolic by its entry's
 * index.
 */
AC_ERROR AC_API acEnumerationGetSymbolicByIndex(acNode hNode, size_t index, char* pSymbolicBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acEnumerationSetByIntValue(acNode hNode, int64_t value)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param value
 *  - Type: int64_t
 *  - [In] parameter
 *  - Integer value to set (from entry node)
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationSetByIntValue </B> sets the entry by its integer value.
 */
AC_ERROR AC_API acEnumerationSetByIntValue(acNode hNode, int64_t value);

/**
 * @fn AC_ERROR AC_API acEnumerationSetBySymbolic(acNode hNode, const char* pSymbolic)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param pSymbolic
 *  - Type: const char*
 *  - [In] parameter
 *  - Symbolic to set (from entry node)
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationSetBySymbolic </B> sets the entry of by its symbolic.
 */
AC_ERROR AC_API acEnumerationSetBySymbolic(acNode hNode, const char* pSymbolic);

/**
 * @fn AC_ERROR AC_API acEnumerationGetEntryByName(acNode hNode, const char* pEntryName, acNode* phEntryNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param pEntryName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the entry
 *
 * @param phEntryNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Entry node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetEntryByName </B> retrieves an entry by its name.
 */
AC_ERROR AC_API acEnumerationGetEntryByName(acNode hNode, const char* pEntryName, acNode* phEntryNode);

/**
 * @fn AC_ERROR AC_API acEnumerationGetEntryAndAccessModeByName(acNode hNode, const char* pEntryName, acNode* phEntryNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param pEntryName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the entry
 *
 * @param phEntryNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Entry node
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Entry node's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetEntryAndAccessModeByName </B> retrieves an entry and its
 * access mode by its name.
 */
AC_ERROR AC_API acEnumerationGetEntryAndAccessModeByName(acNode hNode, const char* pEntryName, acNode* phEntryNode, AC_ACCESS_MODE* pAccessMode);

/**
 * @fn AC_ERROR AC_API acEnumerationGetCurrentEntry(acNode hNode, acNode* phEntryNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param phEntryNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Current entry node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetCurrentEntry </B> retrieves the current entry.
 */
AC_ERROR AC_API acEnumerationGetCurrentEntry(acNode hNode, acNode* phEntryNode);

/**
 * @fn AC_ERROR AC_API acEnumerationGetCurrentEntryAndAccessMode(acNode hNode, acNode* phEntryNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
 *
 * @param phEntryNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Current entry node
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Current entry node's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetCurrentEntryAndAccessMode </B> retrieves the current entry
 * and its access mode.
 */
AC_ERROR AC_API acEnumerationGetCurrentEntryAndAccessMode(acNode hNode, acNode* phEntryNode, AC_ACCESS_MODE* pAccessMode);

/**
 * @fn AC_ERROR AC_API acEnumerationGetCurrentSymbolic(acNode hNode, char* pSymbolicBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enumeration nodes
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
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumerationGetCurrentSymbolic </B> retrieves the current entry's
 * symbolic.
 */
AC_ERROR AC_API acEnumerationGetCurrentSymbolic(acNode hNode, char* pSymbolicBuf, size_t* pBufLen);

#ifdef __cplusplus
} // extern "C"
#endif
