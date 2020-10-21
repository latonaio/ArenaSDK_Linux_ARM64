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
 * @fn AC_ERROR AC_API acEnumEntryGetIntValue(acNode hNode, int64_t* pValue)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enum entry nodes
 *
 * @param pValue
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Integer value of the entry
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumEntryGetIntValue </B> retrieves the integer value of the enum entry.
 */
AC_ERROR AC_API acEnumEntryGetIntValue(acNode hNode, int64_t* pValue);

/**
 * @fn AC_ERROR AC_API acEnumEntryGetNumericValue(acNode hNode, double* pValue)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enum entry nodes
 *
 * @param pValue
 *  - Type: double*
 *  - [Out] parameter
 *  - Numeric value of the entry (if applicable)
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumEntryGetNumericValue </B> retrieves the numeric value of the enum
 * entry (if applicable).
 */
AC_ERROR AC_API acEnumEntryGetNumericValue(acNode hNode, double* pValue);

/**
 * @fn AC_ERROR AC_API acEnumEntryGetSymbolic(acNode hNode, char* pSymbolicBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enum entry nodes
 *
 * @param pSymbolicBuf
 *  - Type: char*
 *  - [Out] parameter
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
 * <B> acEnumEntryGetSymbolic </B> retrieves the symbolic of the enum entry.
 */
AC_ERROR AC_API acEnumEntryGetSymbolic(acNode hNode, char* pSymbolicBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acEnumEntryIsSelfClearing(acNode hNode, bool8_t* pIsSelfClearing)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts enum entry nodes
 *
 * @param pIsSelfClearing
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if the entry is self clearing
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acEnumEntryIsSelfClearing </B> retrieves whether or not the enum entry is
 * self clearing.
 */
AC_ERROR AC_API acEnumEntryIsSelfClearing(acNode hNode, bool8_t* pIsSelfClearing);

#ifdef __cplusplus
} // extern "C"
#endif
