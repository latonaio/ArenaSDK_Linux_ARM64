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
 * @fn AC_ERROR AC_API acStringSetValue(acNode hNode, const char* pValue)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts string nodes
 *
 * @param pValue
 *  - Type: const char*
 *  - [In] parameter
 *  - Value to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acStringSetValue </B> sets a new value. New value must be shorter than the
 * maximum value length (acStringGetMaxLength).
 *
 * @see 
 *  - acStringGetMaxLength
 */
AC_ERROR AC_API acStringSetValue(acNode hNode, const char* pValue);

/**
 * @fn AC_ERROR AC_API acStringGetValue(acNode hNode, char* pValue, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts string nodes
 *
 * @param pValue
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
 * <B> acStringGetValue </B> retrieves the current value of the node.
 */
AC_ERROR AC_API acStringGetValue(acNode hNode, char* pValue, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acStringGetMaxLength(acNode hNode, int64_t* pMaxLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts string nodes
 *
 * @param pMaxLen
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Maximum length
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acStringGetMaxLength </B> retrieves the maximum length of the value
 * string.
 */
AC_ERROR AC_API acStringGetMaxLength(acNode hNode, int64_t* pMaxLen);

#ifdef __cplusplus
} // extern "C"
#endif
