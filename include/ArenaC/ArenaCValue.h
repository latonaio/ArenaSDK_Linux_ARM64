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
 * @fn AC_ERROR AC_API acValueToString(acNode hNode, char* pValueBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts any node type
 *
 * @param pValueBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Value as a string
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
 * <B> acValueToString </B> gets a node's value as a string.
 */
AC_ERROR AC_API acValueToString(acNode hNode, char* pValueBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acValueFromString(acNode hNode, const char* pValue)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts any node type
 *
 * @param pValue
 *  - Type: const char*
 *  - [In] parameter
 *  - Value as a string
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acValueFromString </B> sets a node's value from a string.
 */
AC_ERROR AC_API acValueFromString(acNode hNode, const char* pValue);

/**
 * @fn AC_ERROR AC_API acValueIsValueCacheValid(acNode hNode, bool8_t* pIsValid)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts any node type
 *
 * @param pIsValid
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if cache value is valid
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acValueIsValueCacheValid </B> retrieves whether or not a node's value
 * cache is valid.
 */
AC_ERROR AC_API acValueIsValueCacheValid(acNode hNode, bool8_t* pIsValid);

#ifdef __cplusplus
} // extern "C"
#endif
